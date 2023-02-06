#include <iostream>
using namespace std;

#include "TF1.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TLatex.h"
#include "TH1.h"
#include "TH2.h"


void finalPlots_pA( const char* particle = "eta", const char* collisionSystem = "pAl" )
{

  if (particle == "pi0")
  {
    const int NUM_PT_BINS = 10;
    const float PT_BINS[ NUM_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 20 };
  }
  
  if (particle == "eta")
  {
    const int NUM_PT_BINS = 8;
    const float PT_BINS[ NUM_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 10, 20 };
  }  

  TString infileName = "../dataFiles/";
  infileName += particle;
  infileName += "_bgCorrected_";
  infileName += collisionSystem;
  infileName += "_AN.root";
  TFile *file = TFile::Open( infileName );
  TGraphAsymmErrors *sqrtGraph = (TGraphAsymmErrors*)file->Get( "sqrt" );
  TGraphAsymmErrors *lumiGraph = (TGraphAsymmErrors*)file->Get( "lumi" );
	TH1F* h_rError = (TH1F*)file->Get("rError");	  
  double *ptCenters = lumiGraph->GetX();
  double *lumi      = lumiGraph->GetY();
  double *lumiErr   = lumiGraph->GetEYhigh();
  double *sqrtAs    = sqrtGraph->GetY();
  double *sqrtErr   = sqrtGraph->GetEYhigh();
  
  TString shuffleSysString = "../dataFiles/";
  shuffleSysString += particle;
  shuffleSysString += "_";
  shuffleSysString += collisionSystem;
  shuffleSysString += "_shufflingSys.root";
  TFile *shufflingSysFile = TFile::Open(shuffleSysString);	
  TH1F* h_shufflingSys = (TH1F*)shufflingSysFile->Get("sys");  

  double ptError[ NUM_PT_BINS ];
  double boxError[ NUM_PT_BINS ], formulaDiff[ NUM_PT_BINS ];
  for( int b = 0; b < NUM_PT_BINS; b++ )
  {
    ptError[b]  = 0.15;
    formulaDiff[b] =  lumi[b] - sqrtAs[b];
    boxError[b] = sqrt( formulaDiff[b]*formulaDiff[b] + h_rError->GetBinContent(b+1)*h_rError->GetBinContent(b+1) + h_shufflingSys->GetBinContent(b+1)*h_shufflingSys->GetBinContent(b+1) );
  }

  TCanvas *c1 = new TCanvas( "c3", "", 200, 200, 800, 500 );
  gPad->SetBottomMargin( 0.15 );
  gPad->SetLeftMargin( 0.15 );
  if (particle == "pi0")
    TH1 *frame = new TH2F( "frame", " ", 1, 1, 15, 1, -0.07, 0.1 );
  if (particle == "eta")
    TH1 *frame = new TH2F( "frame", " ", 1, 1, 15, 1, -0.07, 0.12 );    
 
  frame->SetStats(0);
  frame->SetTitle( " ; p_{T} [GeV/c]; A_{N} " );

  float labelSize   = 0.055;
  TAxis *xAxis = frame->GetXaxis();
  xAxis->SetTitleSize( 0.05 );
  xAxis->SetLabelSize( labelSize );
  xAxis->SetTitleOffset( 1.2 );
  TAxis *yAxis = frame->GetYaxis();
  yAxis->CenterTitle( true );
  yAxis->SetTitleSize( 0.07 );
  yAxis->SetLabelSize( labelSize );
  yAxis->SetNdivisions( 504 );//310 );
  yAxis->SetTitleOffset( 0.7 );
  frame->Draw( " " );

  TGraphErrors *boxes = new TGraphErrors( NUM_PT_BINS, ptCenters, lumi, ptError, boxError );
  if (particle == "pi0")
  {
    boxes->SetLineColor( 38 );
    boxes->SetFillColor( 38 );
  }
  else if (particle == "eta")
  {
    boxes->SetLineColor( kRed - 9 );
    boxes->SetFillColor( kRed - 9 );   
  }
  boxes->Draw( "5" );

  TGraphErrors *lines = new TGraphErrors( NUM_PT_BINS, ptCenters, lumi, 0, lumiErr );
  if (particle == "pi0")
  {
    lines->SetLineColor( kBlue );
    lines->SetMarkerColor( kBlue );
    lines->SetMarkerStyle( kFullCircle );    
  }
  else if (particle == "eta" )
  {
    lines->SetLineColor( kRed );
    lines->SetMarkerColor( kRed );  
    lines->SetMarkerStyle( kFullSquare );    
  }

  lines->Draw( "P" );

  TF1 *zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlack );
  zeroLine->SetLineStyle( 2 );
  zeroLine->Draw( "same" );
  if (particle == "pi0")
  {
    if (collisionSystem == "pAu")
    {
      TString label = "p^{#uparrow} + Au #rightarrow #pi^{0} + X, #sqrt{s_{NN}} = 200 GeV, |#eta|<0.35";
    }
    else if (collisionSystem == "pAl")
    {
      TString label = "p^{#uparrow} + Al #rightarrow #pi^{0} + X, #sqrt{s_{NN}} = 200 GeV, |#eta|<0.35";    
    }
  }
  if (particle == "eta")
  {
    if (collisionSystem == "pAu")
    {
      TString label = "p^{#uparrow} + Au #rightarrow #eta + X, #sqrt{s_{NN}} = 200 GeV, |#eta|<0.35";
    }
    else if (collisionSystem == "pAl")
    {
      TString label = "p^{#uparrow} + Al #rightarrow #eta + X, #sqrt{s_{NN}} = 200 GeV, |#eta|<0.35";    
    }
  }  

  if (particle == "pi0")
    TLegend *legend = new TLegend( 0.55, 0.65, 0.835, 0.85 );
  else if (particle == "eta")
    TLegend *legend = new TLegend( 0.165, 0.6, 0.45, 0.8 );    
  //legend->AddEntry( PPG135Lines, "Phys. Rev. D 90, 012006", "p" );
  legend->AddEntry( lines,       "This Result",             "p" );
  legend->SetBorderSize( 0 );
  legend->Draw();
  
  TLatex text;
  text.SetTextSize( 0.065 );
  text.SetTextAlign( 13 );
  if (particle == "pi0")
    text.DrawLatex( 1.5, 0.097, label );
  else if (particle == "eta")
    text.DrawLatex( 1.5, 0.115, label );  
    
  //	     "p + p #rightarrow #pi^{0} + X, #sqrt{s} = 200 GeV, |#eta|<0.35" );
  //text.DrawLatex( 1.55, 0.09, "3.4% polarization uncertainty not included" );
  TLatex bottomText;
  bottomText.SetTextSize( 0.1 );
  bottomText.SetTextAlign( 13 );
  bottomText.DrawLatex( 2.5, -0.035, "PHENIX" );    
      
  if (particle == "pi0")
  {
    TPad *subPad = new TPad( "subPad", "", 0.18, 0.57, 0.57, 0.81 ); 
    subPad->SetLeftMargin( 0.17 );
    subPad->SetBottomMargin( 0.15 );
    subPad->Draw(); 
    subPad->cd(); 

    TH1 *smallFrame = new TH2F( "small", " ", 1, 2, 6, 1, -0.004, 0.004 );
  
    smallFrame->SetStats(0);
    float labelSizeSmall = 0.15;
    TAxis *xAxisSmall = smallFrame->GetXaxis();
    xAxisSmall->SetLabelSize( labelSizeSmall );
    xAxisSmall->SetNdivisions( 703 );
    //  xAxisSmall->SetTickLength( 0.1 );
    xAxisSmall->SetTickSize( 0.1 );
    TAxis *yAxisSmall = smallFrame->GetYaxis();
    yAxisSmall->SetLabelSize( labelSizeSmall );
    yAxisSmall->SetNdivisions( 503 );
    smallFrame->Draw( " " );

    boxes->Draw( "5" );
    lines->Draw( "P" );
    zeroLine->Draw( "same" );
  }

  TString imgname = particle;
  imgname += "_";
  imgname += collisionSystem;
  imgname += "_finalAN.png";
  c1->SaveAs(imgname);

  cout << "summary of systematic errors" << endl;
  cout << "p_T & Stat. unc. & "
       << "Sys unc. (rel lumi vs. sqrt) & "
       << "Sys unc. (from bg fraction)  & "
       << "Sys unc. (from bunch shuffling) & "
       << "Sys unc. (total). " << endl;

  for( int b = 0; b < NUM_PT_BINS; b++ )
  {
    //cout << PT_BINS[b] << " - " << PT_BINS[b + 1] << " & " << lumi[b] << " & " 
    cout <<   setprecision(3) << PT_BINS[b] << " - " << PT_BINS[b + 1] << " & " << fabs(formulaDiff[b]) << " & " << h_rError->GetBinContent(b+1)  
	       << " & " << h_shufflingSys->GetBinContent(b+1) << " & " << boxError[b] << " \\\\" << endl;
	             
   }

 cout << endl;
 cout << "pt then <pt> then asymmetry then stat err then the total syst" << endl;
 cout << endl;
 for( int b = 0; b < NUM_PT_BINS; b++ )
   {
     cout << PT_BINS[b] << "-" << PT_BINS[b + 1] << " & " << ptCenters[b] << " & " << lumi[b] << " & " << lumiErr[b] << " & " 
          << boxError[b] << " \\\\" << endl;
   }
   
}
