#include <iostream>
using namespace std;

#include "TF1.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TLegend.h"

const int numPTBins = 6;
const float ptBins[ numPTBins + 1 ] = { 2, 3, 4, 5, 6, 8, 15 };
const double rError[ numPTBins ] = { 0.0000341248, 0.0000776153, 0.0000515655, 
				     0.0000334533, 0.0000761264, 0.0000458296 };
const double oldAsymmetry[ numPTBins ] = 
  { 0.000670, -0.00307, -0.00252, -0.000480, 0.00463, 0.0137 };
const double oldStatError[ numPTBins ] = 
  { 0.00175,   0.00170,  0.00257,  0.00431,  0.00613, 0.0127 };
const double oldSystError[ numPTBins ] = 
  { 0.00200,   0.000215, 0.000203, 0.000439, 0.00120, 0.000684 };

void prettyPrettyComparePreliminary()
{
  cout << "Plotting new result compared to preliminary result" << endl;

  TFile *file = TFile::Open( "../dataFiles/correctedGraphs.root" );
  TGraphAsymmErrors *sqrtGraph = (TGraphAsymmErrors*)file->Get( "sqrt" );
  TGraphAsymmErrors *lumiGraph = (TGraphAsymmErrors*)file->Get( "lumi" );
  double *ptCenters = lumiGraph->GetX();
  double *lumi      = lumiGraph->GetY();
  double *lumiErr   = lumiGraph->GetEYhigh();
  double *sqrtAs    = sqrtGraph->GetY();
  double *sqrtErr   = sqrtGraph->GetEYhigh();

  double ptError[ numPTBins ], boxError[ numPTBins ], formulaDiff[ numPTBins ];
  double ptPlus[ numPTBins ];
  for( int i = 0; i < numPTBins; i++ )
    {
      ptPlus[i] = ptCenters[i] + 0.25;
      ptError[i]  = 0.02*ptCenters[i];
      formulaDiff[i] =  lumi[i] - sqrtAs[i];
      boxError[i] = sqrt( formulaDiff[i]*formulaDiff[i] + rError[i]*rError[i] );
   }

  //extra syst error for 2 to 3
  double factor = 1.078;
  double systError2to3 = sqrt( factor*factor - 1 )*lumiErr[0];
  boxError[0] = sqrt( formulaDiff[0]*formulaDiff[0] + rError[0]*rError[0] 
		       + systError2to3*systError2to3 );
  cout << endl << "2 to 3 bin is " <<  formulaDiff[0] << ", " << rError[0] 
       << ", " << systError2to3 << " = " << boxError[0] << endl;

  TCanvas *c1 = new TCanvas( "c10", "This result compared to preliminary" );
  gPad->SetBottomMargin( 0.15 );
  gPad->SetLeftMargin( 0.15 );

  TGraphErrors *boxes = 
   new TGraphErrors( numPTBins, ptPlus, lumi, ptError, boxError );
  boxes->SetTitle( " ;p_{T} [GeV/c]; A_{N} " );
  boxes->SetLineColor( kRed );
  boxes->SetFillColor( kWhite );

  float labelSize   = 0.05;
  float titleOffset = 1.2;
  TAxis *xAxis = boxes->GetXaxis();
  xAxis->SetTitleSize( 0.05 );
  xAxis->SetLabelSize( labelSize );
  xAxis->SetTitleOffset( titleOffset );
  TAxis *yAxis = boxes->GetYaxis();
  yAxis->SetRangeUser( -0.008, 0.035 );
  yAxis->CenterTitle( true );
  yAxis->SetTitleSize( 0.07 );
  yAxis->SetLabelSize( labelSize );
  yAxis->SetNdivisions( 310 );
  //yAxis->SetTitleOffset( titleOffset );
  boxes->Draw( "A5" );

  TGraphErrors *lines = 
    new TGraphErrors( numPTBins, ptPlus, lumi, 0, lumiErr );
  lines->SetMarkerStyle( kFullCircle );
  lines->SetMarkerSize( 0.7 );
  lines->SetMarkerColor( kRed );
  lines->SetLineColor( kRed );
  lines->Draw( "P" );

  TGraphErrors *oldBoxes = 
    new TGraphErrors( numPTBins, ptCenters, oldAsymmetry, ptError, oldSystError );
  oldBoxes->SetLineColor( kBlack );
  oldBoxes->SetFillColor( kWhite );
  oldBoxes->Draw( "5" );

  TGraphErrors *oldLines = 
    new TGraphErrors( numPTBins, ptCenters, oldAsymmetry, 0, oldStatError );
  oldLines->SetMarkerStyle( kFullCircle );
  oldLines->SetMarkerSize( 0.7 );
  oldLines->SetMarkerColor( kBlack );
  oldLines->SetLineColor( kBlack );
  oldLines->Draw( "P" );

  TF1 *zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlue );
  zeroLine->SetLineStyle( 2 );
  zeroLine->Draw( "same" );

  TLegend *legend = new TLegend( 0.15,0.9, 0.53, 0.72 );
  legend->AddEntry( oldLines, "Preliminary Result", "lep" );
  legend->AddEntry( lines,    "Updated Result",     "lep" );
  legend->Draw();
  //c1->Print("PRE_LOGO_A_N_eta.pdf");

  cout << "p_T & Asymmetry & Stat. unc. & "
       << "Sys unc. (rel lumi vs. sqrt) & "
       << "Sys unc. (from bg fraction)  & "
       << "Sys unc. (from bunch shuffling) & "
       << "Sys unc. (total). " << endl;
 for( int ptBin = 0; ptBin < numPTBins; ptBin++ )
   {
     cout << ptBins[ ptBin ] << " - " << ptBins[ ptBin + 1 ] << " & "
	  << lumi[ ptBin ] << " & " 
	  << lumiErr[ ptBin ] << " & " 
	  << formulaDiff[ ptBin ] << " & "
	  << rError[ ptBin ]  << " & ";
     if( ptBin == 0 )
       cout << systError2to3 << " & ";
     else
       cout << 0 << " & ";

     cout << boxError[ ptBin ] << " \\" << endl;
   }

}
