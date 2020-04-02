#include <iostream>
using namespace std;

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TLatex.h"
#include "TF1.h"

const int kosterColor = kBlack;;
const int myColor = kRed;

const int numPTBins = 6;
const float ptBins[ numPTBins + 1 ] = { 2, 3, 4, 5, 6, 8, 15 };
const double rError[ numPTBins ] = { 0.0000341248, 0.0000776153, 0.0000515655, 
				     0.0000334533, 0.0000761264, 0.0000458296 };
//Table X of PPG135 (PhysRevD.90.012006)
const int numKosterBins = 7;
const float ptKoster[ numKosterBins ] = { 2.4, 3.4, 4.4, 5.4, 6.4, 7.4, 9.1};
const float kosterAs[ numKosterBins ] = 
  { -0.0069, -0.0024, 0.0019, 0.0292, -0.0458, 0.0035, 0.0842 };
const float kosterStatErr[ numKosterBins ] =
  {  0.0049,  0.0057, 0.0099, 0.0171,  0.0285, 0.0431, 0.0550 };
const float kosterSystErr[ numKosterBins ] =
  { 0.0010,   0.0012, 0.0020, 0.0034,  0.0057, 0.0086, 0.0110 };

void prettyPrettyComparePPG135()
{
  TCanvas *c1 = new TCanvas( "c3", "Pretty Compare" );
  gPad->SetBottomMargin( 0.15 );
  gPad->SetLeftMargin( 0.15 );
  TH1 *frame = new TH2F( "frame", " ", 1, 1.5, 10.7, 1, -0.085, 0.145 );
  frame->SetStats(0);
  frame->SetTitle( " ; p_{T} [GeV/c]; A_{N} " );

  float labelSize = 0.05;
  float offSet = 1.2;
  TAxis *xAxis = frame->GetXaxis();
  xAxis->SetTitleOffset( offSet );
  xAxis->SetLabelSize( labelSize );
  xAxis->SetTitleSize( 0.05 );
  TAxis *yAxis = frame->GetYaxis();
  //yAxis->SetTitleOffset( offSet  - 0.17 );
  yAxis->SetLabelSize( labelSize );
  yAxis->CenterTitle( true );
  yAxis->SetNdivisions( 509 );
  yAxis->SetTitleSize( 0.07 );
  frame->Draw( " " );

  TLatex text;
  text.SetTextSize( 0.04 );
  text.SetTextAlign( 13 );
  text.DrawLatex( 2, 0.135, 
		"p + p #rightarrow #eta + X, #sqrt{s} = 200 GeV, |#eta|<0.35" );
  text.DrawLatex( 2, 0.12, "3.4% polarization uncertainty not included" );

  float kosterPTErrors[ numKosterBins ];
  for( int i = 0; i < numKosterBins; i++ )
    kosterPTErrors[i] = ptKoster[i]*0.02;
  TGraphErrors *kosterBoxes = 
    new TGraphErrors( numKosterBins, ptKoster, kosterAs, 
		      kosterPTErrors, kosterSystErr );
  kosterBoxes->SetLineColor( kosterColor );
  kosterBoxes->SetFillColor( kWhite );
  kosterBoxes->Draw( "5" );

  TGraphErrors *kosterLines = 
    new TGraphErrors( numKosterBins, ptKoster, kosterAs, 0, kosterStatErr );
  kosterLines->SetLineColor( kosterColor );
  kosterLines->SetMarkerColor( kosterColor );
  kosterLines->SetMarkerStyle( kFullSquare );
  kosterLines->SetMarkerSize( 0.7 );
  kosterLines->Draw( "P" );

  TFile *file = TFile::Open( "../dataFiles/correctedGraphs.root" );
  TGraphAsymmErrors *sqrtGraph = (TGraphAsymmErrors*)file->Get( "sqrt" );
  TGraphAsymmErrors *lumiGraph = (TGraphAsymmErrors*)file->Get( "lumi" );
  double *ptCenters = lumiGraph->GetX();
  double *lumi      = lumiGraph->GetY();
  double *lumiErr   = lumiGraph->GetEYhigh();
  double *sqrt      = sqrtGraph->GetY();
  double *sqrtErr   = sqrtGraph->GetEYhigh();
  double ptError[ numPTBins ], boxError[ numPTBins ], formulaDiff[ numPTBins ];
  for( int i = 0; i < numPTBins; i++ )
    {
      ptError[i]  = 0.02*ptCenters[i];
      formulaDiff[i] =  lumi[i] - sqrt[i];
      boxError[i] = sqrt( formulaDiff[i]*formulaDiff[i] + rError[i]*rError[i] );
   }
  //extra syst error for 2 to 3
  double factor = 1.078;
  double systError2to3 = sqrt( factor*factor - 1 )*lumiErr[0];
  boxError[0] = sqrt( formulaDiff[0]*formulaDiff[0] + rError[0]*rError[0] 
		       + systError2to3*systError2to3 );
  cout << endl << "2 to 3 bin is " <<  formulaDiff[0] << ", " << rError[0] 
       << ", " << systError2to3 << " = " << boxError[0] << endl;
  //double systError2to3 = 1.135*lumiErr[0];
  //boxError[0] = sqrt( boxError[0]*boxError[0] + systError2to3*systError2to3 );

  TGraphErrors *boxes = 
    new TGraphErrors( numPTBins, ptCenters, lumi, ptError, boxError );
  boxes->SetLineColor( myColor );
  boxes->SetFillColor( kWhite );
  boxes->Draw( "5" );

  TGraphErrors *lines = 
    new TGraphErrors( numPTBins, ptCenters, lumi, 0, lumiErr );
  lines->SetLineColor( myColor );
  lines->SetMarkerColor( myColor );
  lines->SetMarkerStyle( kFullCircle );
  lines->Draw( "P" );

  TF1 *zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlue );
  zeroLine->SetLineStyle( 2 );
  zeroLine->Draw( "same" );

  //  TLegend *legend = new TLegend( 0.17, 0.17, 0.535, 0.38 );
  TLegend *legend = new TLegend( 0.175, 0.58, 0.65, 0.77);
  legend->AddEntry( kosterLines, "Phys. Rev. D 90, 012006", "lep" );
  legend->AddEntry( lines, "This Result", "lep" );
  legend->SetBorderSize( 0 );
  legend->Draw();

  c1->Print("PRE_LOGO_A_N_eta_compare.pdf");
}

