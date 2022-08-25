#include <iostream>
#include <fstream>
using namespace std;

#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"

#include "../../../Constants.h"

const bool SAVE_IMAGES = true;
/**/
const char* particle = "eta";
const int NUM_PARA = 7;
const int NUM_HISTO_BINS = 70;
const double HISTO_MIN = 0.2;
const double HISTO_MAX = 0.9;
const double PEAK_MIN = 0.480;
const double PEAK_MAX = 0.620;
const double FIT_RANGE_MIN = 0.4;
const double FIT_RANGE_MAX = 0.7;
const int NUM_PT_BINS = 8;
const float PT_BINS[ NUM_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 10, 20 };
//guas + pol3;
const double GUESS[ NUM_PT_BINS ][ NUM_PARA ] = 
{
  {  25000, 0.548, 0.03, -9000, 3000000, -7000000, 4000000 },//2-3
  {  60000, 0.548, 0.03, 10000,   10000,   -10000,  -10000 },//3-4
  {  60000, 0.548, 0.03, 10000,   10000,   -10000,  -10000 },//4-5
  {  45000, 0.548, 0.03, 10000,   10000,   -10000,  -10000 },//5-6
  {   8000, 0.548, 0.03, 10000,    4000,   -10000,  -10000 },//6-7
  {   3000, 0.548, 0.03,  1000,    4000,    -5000,   10000 },//7-8
  {   2000, 0.548, 0.03,  1000,    2000,    -3000,   20000 },//8-10
  {   2000, 0.548, 0.03,  1000,    2000,    -3000,   20000 },//10-20
};
/*
const char* particle = "pi0";
const int NUM_HISTO_BINS = 300;
const float HISTO_MIN = 0;
const float HISTO_MAX = 0.300;
const double PEAK_MIN = 0.112;
const double PEAK_MAX = 0.162;
const double FIT_RANGE_MIN = 0.07;// 0.05;//
const double FIT_RANGE_MAX = 0.27;//0.25;
const int NUM_PT_BINS = 10;
const float PT_BINS[ NUM_PT_BINS + 1 ] = 
  { 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 20 };
const int NUM_PARA = 7;
const double GUESS[ NUM_PT_BINS ][ NUM_PARA ] = 
{
  { 25000, 0.135, 0.02, -900, 30000, -70000, 40000 },//2-3
  { 60000, 0.135, 0.02, 1000,   100,   -100,  -100 },//3-4
  { 60000, 0.135, 0.02, 1000,   100,   -100,  -100 },//4-5
  { 45000, 0.135, 0.02, 1000,   100,   -100,  -100 },//5-6
  {  8000, 0.135, 0.02, 1000,    40,   -100,  -100 },//6-7
  {  5000, 0.135, 0.02,  100,    40,   -100,  -100 },//7-8
  {  2000, 0.135, 0.02,  100,    40,   -100,  -100 },//8-9
  {  1300, 0.135, 0.02,  100,    40,   -100,  -100 },//9-10
  {  1000, 0.135, 0.02,  100,    40,   -100,  -100 },//10-12
  {   500, 0.135, 0.02,  100,    20,    -30,   200 },//12-20
};
/**/

void findBackgroundFraction(const char* collisionSystem = "pAu" )
{
  gStyle->SetOptFit();
  TString inputFileName = "../../../curated_";
  inputFileName += particle;
  inputFileName += "_";
  inputFileName += collisionSystem;
  inputFileName += ".root";
  cout << "opening file " << inputFileName << endl;
  TFile *histoFile = TFile::Open( inputFileName );
  TH1F* invariantMass[ NUM_ARMS ][ NUM_PT_BINS ];
  for( int a = 0; a < NUM_ARMS; a  ++ )
    for( int b = 0; b < NUM_PT_BINS; b++ )
      {
 	TString histoName = "invmass";
	if( a == WEST ) 
	  histoName += "West";
	else if( a == EAST )
	  histoName += "East";
	histoName += PT_BINS[b];
	histoName += "to";
	histoName += PT_BINS[b + 1];
	invariantMass[a][b] = (TH1F*)histoFile->Get( histoName );
      }

  double r[ NUM_ARMS ][ NUM_PT_BINS ];
  double peakCounts[ NUM_ARMS ][ NUM_PT_BINS ];
  TCanvas *canvases[ NUM_ARMS ][ NUM_PT_BINS ];

  TH1F *chiSquaredHisto = new TH1F( "chiSquaredHisto", "Reduced #chi^{2}",
				    1000, 0, 500 );

  //make arrays for recording difference in fits and histo value
  TCanvas *compareCans[ NUM_ARMS ][ NUM_PT_BINS ];
  double binMid[ NUM_HISTO_BINS ], binRange[ NUM_HISTO_BINS ];
  double binWidth = ( HISTO_MAX - HISTO_MIN ) / NUM_HISTO_BINS; 
  for( int i = 0; i < NUM_HISTO_BINS; i++ )
    {
      binRange[i] = binWidth / 2;
      binMid[i] = HISTO_MIN  + binWidth * i + binWidth / 2;
    }
  int firstBin = invariantMass[0][0]->FindBin( FIT_RANGE_MIN );
  int lastBin  = invariantMass[0][0]->FindBin( FIT_RANGE_MAX );
  double compare[ NUM_HISTO_BINS ];
  for( int i = 0; i < firstBin; i++ )
    compare[i] = 0;
  for( int i = lastBin; i < NUM_HISTO_BINS; i++ )
    compare[i] = 0;
  for( int a = 0; a < NUM_ARMS; a++ )
    for( int b = 0; b < NUM_PT_BINS; b++ )
      {
	TString fName = "f";
	fName += a * 10 + b;
	if( a == WEST )
	  canvases[a][b] = new TCanvas( );
	else if( a == EAST )
	  canvases[a][b] = new TCanvas( fName, fName, 800, 0, 700, 500 );
	invariantMass[a][b]->Draw();
	/**/
	TF1 *fit = new TF1( fName, "gaus+pol3(3)", 
			    FIT_RANGE_MIN, FIT_RANGE_MAX );
	fit->SetParNames( "amplitude", "mean", "sigma", "A", "B", "C", "D" );
	/*
	TF1 *fit = new TF1( fName, "gaus+pol2(3)", 
			    FIT_RANGE_MIN, FIT_RANGE_MAX );
	fit->SetParNames( "amplitude", "mean", "sigma", "A", "B", "C" );*/
	for( int para = 0; para < NUM_PARA; para++ )
	  fit->SetParameter( para, GUESS[b][ para ] );

	invariantMass[a][b]->Fit( fName, "QR" );
	double chi = fit->GetChisquare();
	int ndf    = fit->GetNDF();
	chiSquaredHisto->Fill( chi / ndf );

	double A = fit->GetParameter(3);
	double B = fit->GetParameter(4);
	double C = fit->GetParameter(5);
	double D = fit->GetParameter(6);
	//TF1 *line = new TF1( "line", "pol3", FIT_RANGE_MIN, FIT_RANGE_MAX );
	TF1 *line = new TF1( "line", "pol3", PEAK_MIN, PEAK_MAX );
	//TF1 *line = new TF1( "line", "pol2", PEAK_MIN, PEAK_MAX );
	line->SetParameter( 0, A );
	line->SetParameter( 1, B );
	line->SetParameter( 2, C );
	line->SetParameter( 3, D );
	line->SetLineColor( kMagenta );
	line->Draw( "same" );

	double factor = ( HISTO_MAX - HISTO_MIN )/ NUM_HISTO_BINS;
	double backgroundCounts = line->Integral( PEAK_MIN, PEAK_MAX );
	backgroundCounts /= factor;
	//peakCounts[a][b]        = fit ->Integral( PEAK_MIN, PEAK_MAX );
	int peakMinBin = invariantMass[a][b]->FindBin( PEAK_MIN );
	int peakMaxBin = invariantMass[a][b]->FindBin( PEAK_MAX );
	peakCounts[a][b] = invariantMass[a][b]->Integral( peakMinBin, 
							  peakMaxBin );

	r[a][b] = backgroundCounts / peakCounts[a][b];
	cout << ARM_NAMES[a] << " " << PT_BINS [b] << "-" << PT_BINS[b + 1] 
	     << ": " << r[a][b] << " = " << backgroundCounts << " / " 
	     << peakCounts[a][b] << endl;

	for( int j = firstBin; j < lastBin; j++ )
	  {
	    double dataValue = invariantMass[a][b]->GetBinContent(j);
	    double fitValue  = fit->Eval( binMid[ j - 1 ] );
	    if( dataValue )
	      compare[j] = ( fitValue - dataValue ) / sqrt( dataValue );
	    else
	      compare[j] = 0;
	    /*if( i == NUM_PT_BINS - 1 )
	      cout << binMid[ j - 1 ] << ": " << compare[j] << " = ( " 
		   << fitValue << " - " << dataValue << " ) / sqrt( " 
		   << dataValue << " )" << endl;*/
	  }
     
	TString name = "d";
	name += a*20 + b;
	TString title = "Comparison plot for r ";
	if( a == 0 ) 
	  title += "West";
	else if( a == 1 )
	  title += "East";
	title += " Arm ";
	title += PT_BINS[b];
	title += " < p_{T} < ";
	title += PT_BINS[b + 1];
	title += " [GeV]; M_{#gamma#gamma} [GeV]; ";
	if( a == WEST )
	  compareCans[a][b] = new TCanvas( name, name, 0, 500, 700, 400 );
	else if( a == EAST )
	  compareCans[a][b] = new TCanvas( name, name, 800, 500, 700, 400 );
	TGraphErrors *graph = new TGraphErrors( NUM_HISTO_BINS, binMid, compare,
					       binRange, 0 );
	graph->SetTitle( title );
	graph->GetXaxis()->SetRangeUser( HISTO_MIN, HISTO_MAX );
	graph->SetFillColor( kBlue - 3 );
	graph->Draw( "AB" );

	if( SAVE_IMAGES )
	  {
	    //invariantMass[a][b]->SetTitle( ";M_{#gamma#gamma};" );

	    TString name = "images/";
	    name += particle;
	    name += "/r";
	    name += ARM_NAMES[a];
	    name += PT_BINS[b];
	    name += "to";
	    name += PT_BINS[b + 1];
	    name += "_";
	    name += collisionSystem;

	    TString invName = name;
	    invName += ".png";
	    //invName += ".pdf";
	    canvases[a][b]->SaveAs( invName );

	    TString compareName = name;
	    compareName += "Compare.png";
	    //compareCans[a][b]->SaveAs( compareName );
	  }
      }

  TCanvas *chi2 = new TCanvas( "chi2", "Reduced Chi Squared" );
  chiSquaredHisto->Draw();

  //cout << setprecision(3);

  cout << endl << "For fit ranges " << FIT_RANGE_MIN << " to " << FIT_RANGE_MAX 
       << endl;
  for( int a = 0; a < NUM_ARMS; a++ )
    {
      if( a == WEST )
	cout << "const double R_WEST[ NUM_PT_BINS ] = { ";
      else if( a == EAST )
	cout << "const double R_EAST[ NUM_PT_BINS ] = { ";
      for( int b = 0; b < NUM_PT_BINS; b++ )
	{
	  cout << r[a][b];
	  if( b < NUM_PT_BINS - 1 )
	    cout << ", ";
	}
      cout << " };" << endl;
    }

  float rSqrt[ NUM_PT_BINS ];
  cout << "const double R_SQRT[ NUM_PT_BINS ] = { ";
  for( int b = 0; b < NUM_PT_BINS; b++ )
    {
      //average weighted by peak counts!
      rSqrt[b] = peakCounts[0][b]*r[0][b] + peakCounts[1][b]*r[1][b];
      rSqrt[b] /= peakCounts[0][b] + peakCounts[1][b];
      cout << rSqrt[b];
      if( b < NUM_PT_BINS - 1 )
	cout << ", ";
    }
  cout << " };" << endl;

  TString outfileName = "tables/";
  outfileName += particle;
  outfileName += "_backgroundFractions_";
  outfileName += collisionSystem;
  outfileName += ".txt";
  ofstream backgroundFracsFile( outfileName );

  backgroundFracsFile << setprecision(3);
  backgroundFracsFile << " (p_T)[GeV] & West Arm & East Arm & Both Arms\\ " 
       << FIT_RANGE_MIN << " to " << FIT_RANGE_MAX << endl;
  for( int b = 0; b < NUM_PT_BINS; b++ )
    backgroundFracsFile << PT_BINS[b] << " - " << PT_BINS[b + 1] << " & "
	 << r[0][b] << " & " << r[1][b] << " & "<< rSqrt[b] << " \\" << endl;
	   
  cout << setprecision(3);
  cout << " (p_T)[GeV] & West Arm & East Arm & Both Arms\\ " 
       << FIT_RANGE_MIN << " to " << FIT_RANGE_MAX << endl;
  for( int b = 0; b < NUM_PT_BINS; b++ )
    cout << PT_BINS[b] << " - " << PT_BINS[b + 1] << " & "
	 << r[0][b] << " & " << r[1][b] << " & "<< rSqrt[b] << " \\" << endl;

}
