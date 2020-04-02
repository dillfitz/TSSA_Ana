//Makes a pretty pretty plot of the direct photon asymmetry with statisical AND
//systematic errors and prints out a nice table that can be adjusted in latex

#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TF1.h"

const int NUM_PT_BINS = 4;
const double BIN_CENTERS[ NUM_PT_BINS ] = { 5.39, 6.69, 8.77, 11.9 };
const double PT_BINS[ NUM_PT_BINS + 1 ] = { 5, 6, 8, 10, 18 }; 

//systematic error from the uncertainty on the background fraction
const double r_SYST_ERROR[ NUM_PT_BINS ] = 
  { 1.56765e-05, 2.30807e-05, 0.000192872, 7.6042e-05 };

//systematic error from setting the background asymmetry to zero
const double bg_SYST_ERROR[ NUM_PT_BINS ] = 
  { 0.00336531, 0.00248428, 0.00156061, 0.000767137 };
//integrating background asymmetry over photon p_T 

void prettyPretty()
{
  TFile *inFile = TFile::Open( "../dataFiles/graphs_dp_corrected.root" );
  TGraphAsymmErrors* lumiGraph = (TGraphAsymmErrors*)inFile->Get( "lumi" );
  TGraphAsymmErrors* sqrtGraph = (TGraphAsymmErrors*)inFile->Get( "sqrt" );

  double *lumiAs  = lumiGraph->GetY();
  double *lumiErr = lumiGraph->GetEYhigh();
  double *sqrtAs  = sqrtGraph->GetY();

  double ptError[ NUM_PT_BINS ], formulaDiff[ NUM_PT_BINS ];
  double systError[ NUM_PT_BINS ];
  for( int i = 0; i < NUM_PT_BINS; i++ )
    {
      ptError[i] = 0.02 * BIN_CENTERS[i];
      formulaDiff[i] = fabs( lumiAs[i] - sqrtAs[i] );

      systError[i] = sqrt( r_SYST_ERROR[i]*r_SYST_ERROR[i] 
			   + bg_SYST_ERROR[i]*bg_SYST_ERROR[i]
			   + formulaDiff[i]*formulaDiff[i] );
    }

  TCanvas *c1 = new TCanvas( "c1", "Direct Photon" );
  gPad->SetBottomMargin( 0.15 );
  gPad->SetLeftMargin( 0.15 );

  float labelSize   = 0.05;
  float titleOffset = 1.2;
  TGraphErrors *boxes = 
    new TGraphErrors( NUM_PT_BINS, BIN_CENTERS, lumiAs, ptError, systError );
  boxes->SetTitle( "; p_{T} [GeV/c]; A_{N}" );
  boxes->SetFillColor( kWhite );
  TAxis *xAxis = boxes->GetXaxis();
  xAxis->SetTitleSize( 0.05 );
  xAxis->SetLabelSize( labelSize );
  xAxis->SetTitleOffset( titleOffset );
  TAxis *yAxis = boxes->GetYaxis();
  yAxis->SetRangeUser( -0.01, 0.03 );
  yAxis->SetTitleSize( 0.07 );
  yAxis->SetLabelSize( labelSize );
  yAxis->CenterTitle( true );
  yAxis->SetNdivisions( 310 );
  boxes->Draw( "A5" );

  TGraphErrors *lines = 
    new TGraphErrors( NUM_PT_BINS, BIN_CENTERS, lumiAs, 0, lumiErr );
  lines->SetMarkerStyle( kFullCircle );
  //lines->SetMarkerSize( 0.07 );
  lines->Draw( "P" );

  TF1 *zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlue );
  zeroLine->SetLineStyle( 2 );
  zeroLine->Draw( "same" );

  TLatex text;
  text.SetTextSize( 0.04 );
  text.SetTextAlign( 13 );
  text.DrawLatex( 5, 0.029, 
	    "Isolated Direct Photons, p + p #sqrt{s} = 200 GeV, |#eta|<0.35" );
  text.DrawLatex( 5, 0.026, "3.4% polarization uncertainty not included" );

  c1->Print( "PRE_LOGO_A_N_dp.pdf" );

  cout << "p_T [GeV] & Asymmetry & Stat. unc. & " << endl;
  cout << "Sys unc. (rel lumi vs. sqrt) & " << endl;
  cout << "Sys unc. (from bg fraction)  & " << endl;
  cout << "Sys unc. (from bg asymmetry) & " << endl;
  cout << "Sys unc. (total). " << endl;
 for( int ptBin = 0; ptBin < NUM_PT_BINS; ptBin++ )
   {
     cout << PT_BINS[ ptBin ] << " to " << PT_BINS[ ptBin + 1 ] << " & "
	  << lumiAs[ ptBin ] << " & " 
	  << lumiErr[ ptBin ] << " & " 
	  << formulaDiff[ ptBin ] << " & "
	  << r_SYST_ERROR[ ptBin ]  << " & "
	  << bg_SYST_ERROR[ ptBin ] << " & " 
	  << systError[ ptBin ] << " \\" << endl;
   }


}
