#include <iostream>
using namespace std;

#include "TF1.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TLatex.h"

const int numPTBins = 6;
const float ptBins[ numPTBins + 1 ] = { 2, 3, 4, 5, 6, 8, 15 };
const double rError[ numPTBins ] = { 0.0000341248, 0.0000776153, 0.0000515655, 
				     0.0000334533, 0.0000761264, 0.0000458296 };

void prettyPretty()
{
  TFile *file = TFile::Open( "../dataFiles/correctedGraphs.root" );
  TGraphAsymmErrors *sqrtGraph = (TGraphAsymmErrors*)file->Get( "sqrt" );
  TGraphAsymmErrors *lumiGraph = (TGraphAsymmErrors*)file->Get( "lumi" );
  double *ptCenters = lumiGraph->GetX();
  double *lumi      = lumiGraph->GetY();
  double *lumiErr   = lumiGraph->GetEYhigh();
  double *sqrtAs    = sqrtGraph->GetY();
  double *sqrtErr   = sqrtGraph->GetEYhigh();

  double ptError[ numPTBins ], boxError[ numPTBins ], formulaDiff[ numPTBins ];
  for( int i = 0; i < numPTBins; i++ )
    {
      ptError[i]  = 0.02*ptCenters[i];
      formulaDiff[i] =  lumi[i] - sqrtAs[i];
      boxError[i] = sqrt( formulaDiff[i]*formulaDiff[i] + rError[i]*rError[i] );
   }
  //extra syst error for 2 to 3
  //  double systError2to3 = 1.07*lumiErr[0];
  double factor = 1.078;
  double systError2to3 = sqrt( factor*factor - 1 )*lumiErr[0];
  boxError[0] = sqrt( formulaDiff[0]*formulaDiff[0] + rError[0]*rError[0] 
		       + systError2to3*systError2to3 );
  cout << endl << "2 to 3 bin is " <<  formulaDiff[0] << ", " << rError[0] 
       << ", " << systError2to3 << " = " << boxError[0] << endl;

  TCanvas *c1 = new TCanvas( "c3", "This result" );
  TGraphErrors *boxes = 
    new TGraphErrors( numPTBins, ptCenters, lumi, ptError, boxError );
  gPad->SetBottomMargin( 0.15 );
  gPad->SetLeftMargin( 0.15 );

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

  TLatex text;
  text.SetTextSize( 0.04 );
  text.SetTextAlign( 13 );
  text.DrawLatex( 2.3, 0.028, 
		"p + p #rightarrow #eta + X, #sqrt{s} = 200 GeV, |#eta|<0.35" );
  text.DrawLatex( 2.3, 0.025, "3.4% polarization uncertainty not included" );

  TGraphErrors *lines = 
    new TGraphErrors( numPTBins, ptCenters, lumi, 0, lumiErr );
  lines->SetMarkerStyle( kFullCircle );
  lines->SetMarkerSize( 0.7 );
  lines->SetMarkerColor( kRed );
  lines->SetLineColor( kRed );
  lines->Draw( "P" );

  TF1 *zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlue );
  zeroLine->SetLineStyle( 2 );
  zeroLine->Draw( "same" );

  c1->Print("PRE_LOGO_A_N_eta.pdf");

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
