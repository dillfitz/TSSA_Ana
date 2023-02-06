#include <iostream>
using namespace std;

#include "TFile.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TLatex.h"

const int NUM_PARA = 7;
double GUESS[ NUM_PARA ] = {  60000, 0.135, 0.02, 1000, 100, -100, -100 };

//pi0//
/*
const double FIT_RANGE_MIN = 0.07;
const double FIT_RANGE_MAX = 0.25;
const float MIN_MASS = 0.0;
const float MAX_MASS = 0.3;
//const int NUM_BINS = 300;
const float MIN_SIGNAL_MASS = 0.112;
const float MAX_SIGNAL_MASS = 0.162;
const float MIN_BG_MASS_1 = 0.047;
const float MAX_BG_MASS_1 = 0.097;
const float MIN_BG_MASS_2 = 0.177;
const float MAX_BG_MASS_2 = 0.227;
/**/
/**/
//eta//
/**/
const double FIT_RANGE_MIN = 0.4;
const double FIT_RANGE_MAX = 0.7;
const float MIN_MASS = 0.2;
const float MAX_MASS = 0.9;
//const int NUM_BINS = 70;
const float MIN_SIGNAL_MASS = 0.480;
const float MAX_SIGNAL_MASS = 0.620;
const float MIN_BG_MASS_1 = 0.300;
const float MAX_BG_MASS_1 = 0.400;
const float MIN_BG_MASS_2 = 0.700;
const float MAX_BG_MASS_2 = 0.800;
/**/

void plotPrettyInvMass_pA(const char* particle = "eta", const char* collisionSystem = "pAl")
{

  if (particle == "eta" && collisionSystem == "pAl")
  {
    GUESS[0] = 30000;
  }

  TString inputFileName = "../../../curated_";
  inputFileName += particle;
  inputFileName += "_";
  inputFileName += collisionSystem;
  inputFileName += ".root";
  
  TFile *file = TFile::Open( inputFileName );
  TH1F *histo = (TH1F*)file->Get( "invmassWest4to5" );
  //if (particle == "pi0") {histo->Rebin(3);} 
    
  int NUM_BINS = histo->GetNbinsX();  

  TH1F *sig = new TH1F( "sig", "", NUM_BINS, MIN_MASS, MAX_MASS );
  TH1F *lilBG = new TH1F( "lilBG", "", NUM_BINS, MIN_MASS, MAX_MASS );
  TH1F *bigBG = new TH1F( "bigBG", "", NUM_BINS, MIN_MASS, MAX_MASS );
  cout << histo->GetNbinsX() << endl;
  for( int i = 1; i <= NUM_BINS; i++ )
    {
      double counts = histo->GetBinContent( i );
      //cout << counts << endl;

      double invmass = (i - 1 )* (MAX_MASS - MIN_MASS)/NUM_BINS + MIN_MASS + (MAX_MASS - MIN_MASS)/(2*NUM_BINS);
 
   
      if( invmass > MIN_SIGNAL_MASS  && invmass < MAX_SIGNAL_MASS  )
	      sig->Fill( invmass, counts );
      else if( invmass > MIN_BG_MASS_1  && invmass < MAX_BG_MASS_1 ) 
	      lilBG->Fill( invmass, counts );
      else if( invmass > MIN_BG_MASS_2  && invmass < MAX_BG_MASS_2 )
	      bigBG->Fill( invmass, counts );
    }

  if (particle == "pi0")
  {
    histo->GetXaxis()->SetLimits(0., 0.25);
    sig->GetXaxis()->SetLimits(0., 0.25);
    lilBG->GetXaxis()->SetLimits(0., 0.25);
    bigBG->GetXaxis()->SetLimits(0., 0.25);
  }
  
  TCanvas *can = new TCanvas();
  gStyle->SetHatchesSpacing(0.8);
  gStyle->SetHatchesLineWidth(2);
  TGaxis::SetMaxDigits(4);
  sig->SetStats( false );
  if (particle == "pi0")
    sig->SetTitle( "; M_{#gamma#gamma} [GeV/c^{2}]; counts/(0.001 GeV)/c^{2}" );
  else if (particle == "eta")
    sig->SetTitle( "; M_{#gamma#gamma} [GeV/c^{2}]; counts/(0.01 GeV/c^{2})" );  
  float titleOffset = 0.775;
  TAxis *xAxis = sig->GetXaxis();
  //xAxis->CenterTitle( true );
  xAxis->SetTitleSize( 0.055 );
  xAxis->SetLabelSize(0.04);
  xAxis->SetTitleOffset( titleOffset );
  //xAxis->CenterTitle( true );  
  TAxis *yAxis = sig->GetYaxis();
  yAxis->CenterTitle( true );
  yAxis->SetTitleSize( 0.055 );
  yAxis->SetLabelSize(0.04);
  yAxis->SetTitleOffset( titleOffset);

  sig->SetFillColor( kBlue );
  sig->SetFillStyle(3353);
  //sig->GetXaxis()->SetLimits(0., 0.25);
  sig->Draw( "B"  );

  lilBG->SetFillColor( kRed );
  lilBG->SetFillStyle(3335);
  lilBG->Draw( "BSAME" );
  bigBG->SetFillColor( kRed );
  bigBG->SetFillStyle(3335);
  bigBG->Draw( "BSAME" );

  histo->Draw("SAME");
  TF1 *fit = new TF1( "fit", "gaus+pol3(3)",  FIT_RANGE_MIN, FIT_RANGE_MAX );
  for( int para = 0; para < NUM_PARA; para++ )
    fit->SetParameter( para, GUESS[ para ] );
    

  TF1 *bgfit = new TF1("bgfit", "[0] + [1]*x + [2]*x*x + [3]*x*x*x", FIT_RANGE_MIN, FIT_RANGE_MAX);
  histo->Fit( "fit", "0" );
  cout << fit->GetParameter(3) << endl;
  bgfit->SetParameters(fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5), fit->GetParameter(6));
  bgfit->SetLineColor(kGreen+1);
  bgfit->SetLineWidth(4);
  bgfit->Draw("same");
  
  TLegend *leg = new TLegend(0.6, 0.6, 0.9, 0.9);
  leg->AddEntry(sig, "Peak Region", "f");
  leg->AddEntry(lilBG, "Sideband Regions", "f");
  leg->AddEntry(bgfit, "Fit to Background");
  leg->Draw();
 
  if (particle == "pi0" && collisionSystem == "pAu")
  {
    TLatex decay;
    decay.SetTextSize( 0.08 );
    decay.SetTextAlign( 13 ); 
    decay.DrawLatex( 0.025, 120000, "#pi^{0} #rightarrow #gamma#gamma" );  

    TLatex system;
    system.SetTextSize(0.075);
    system.SetTextAlign( 13 );
    system.DrawLatex( 0.025, 100000, "p^{#uparrow} + Au");
 
    TLatex PHENIX;
    PHENIX.SetTextSize(0.09);
    PHENIX.SetTextAlign( 13 );
    PHENIX.DrawLatex( 0.155, 55000, "PHENIX");        
  }
  else if (particle == "pi0" && collisionSystem == "pAl")
  {
    TLatex decay;
    decay.SetTextSize( 0.08 );
    decay.SetTextAlign( 13 ); 
    decay.DrawLatex( 0.025, 50000, "#pi^{0} #rightarrow #gamma#gamma" );    
  
    TLatex system;
    system.SetTextSize(0.075);
    system.SetTextAlign( 13 );
    system.DrawLatex( 0.025, 40000, "p^{#uparrow} + Al"); 
    
    TLatex PHENIX;
    PHENIX.SetTextSize(0.09);
    PHENIX.SetTextAlign( 13 );
    PHENIX.DrawLatex( 0.155, 20000, "PHENIX");       
  }
  else if (particle == "eta" && collisionSystem == "pAu")
  {
    TLatex decay;
    decay.SetTextSize( 0.08 );
    decay.SetTextAlign( 13 ); 
    decay.DrawLatex( 0.25, 85000, "#eta #rightarrow #gamma#gamma" );  

    TLatex system;
    system.SetTextSize(0.075);
    system.SetTextAlign( 13 );
    system.DrawLatex( 0.25, 75000, "p^{#uparrow} + Au");
    
    TLatex PHENIX;
    PHENIX.SetTextSize(0.09);
    PHENIX.SetTextAlign( 13 );
    PHENIX.DrawLatex( 0.675, 50000, "PHENIX");       
  }
  else if (particle == "eta" && collisionSystem == "pAl")
  {
    TLatex decay;
    decay.SetTextSize( 0.08 );
    decay.SetTextAlign( 13 ); 
    decay.DrawLatex( 0.25, 35000, "#eta #rightarrow #gamma#gamma" );    
  
    TLatex system;
    system.SetTextSize(0.075);
    system.SetTextAlign( 13 );
    system.DrawLatex( 0.25, 30000, "p^{#uparrow} + Al");  
    
    TLatex PHENIX;
    PHENIX.SetTextSize(0.09);
    PHENIX.SetTextAlign( 13 );
    PHENIX.DrawLatex( 0.675, 22000, "PHENIX");       
  }  
  TString imgname = "images/invmass_figs/";
  imgname += particle;
  imgname += "_";
  imgname += collisionSystem;
  imgname += "_invMass_4to5W.png";
  can->SaveAs(imgname);
}
