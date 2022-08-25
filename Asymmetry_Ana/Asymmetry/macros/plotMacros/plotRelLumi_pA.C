#include <iostream>
using namespace std;

#include "TFile.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"
#include "TF1.h"
#include "TAxis.h"

#include "ttest.h"

const bool SAVE_IMAGES = true;

const float YMIN = -0.18;
const float YMAX =  0.18;
//const float YMIN = -0.05;
//const float YMAX =  0.06;

const int SIZE = 2;

// Use this for the open heavy flavor electron measurement //
void plotRelLumi_pA( const char* particle = "pi0", const char* collisionSystem = "pAl" )
{
  TString inputFileName = "../dataFiles/";
  inputFileName += particle;
  inputFileName += "_";
  inputFileName += collisionSystem;
  inputFileName += "_AN";
  inputFileName += ".root";
  
  TF1* zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlack );
  zeroLine->SetLineStyle( 2 );

  cout << "Opening file " << inputFileName << endl;
  TGraphAsymmErrors *lumiBL, *lumiBR; 
  TFile *file = TFile::Open( inputFileName );

  lumiBL = (TGraphAsymmErrors*)file->Get( "lumiBL" );
  lumiBR = (TGraphAsymmErrors*)file->Get( "lumiBR" );     
   
  //east	 
	lumiBR->SetMarkerStyle( kFullSquare );
	lumiBR->SetMarkerColor( kGreen + 3 );
	lumiBR->SetMarkerSize( 0.7 * SIZE );
	
  //west
  lumiBL->SetMarkerStyle( kFullDiamond );
	lumiBL->SetMarkerColor( kCyan + 1 );
	lumiBL->SetMarkerSize( SIZE );

  lumiBL->GetYaxis()->SetRangeUser( YMIN, YMAX );
  lumiBR->GetYaxis()->SetRangeUser( YMIN, YMAX );   
    
  //  { "lumiYL", "lumiYR", "lumiBL", "lumiBR"};
  TCanvas *c20 = new TCanvas( "c20", "Yellow Beam Left Right" );


  TString directoryName = "./images/";

  if( SAVE_IMAGES )
  {
    TString name = directoryName;
    name += particle;
    name += "/";
    name += collisionSystem;
    name += "lumiLeftRightYellow.png";
    c20->SaveAs( name );
  }

  TCanvas *c41 = new TCanvas( "c41", "Blue Beam Left Right Asymmetry " );
  if( !SAVE_IMAGES )
    lumiBR->SetTitle( "Blue Beam Left Right Asymmetry; p_{T} [GeV];" );
  else
    lumiBR->SetTitle( "; p_{T} [GeV];" );

  lumiBR->Draw("AP");
  lumiBL->Draw("P");
  zeroLine->Draw( "same" );
  TLegend *legend4 = new TLegend( 0.15,0.7, 0.5, 0.9 );
  //TLegend *legend4 = new TLegend( 0.58,0.4, 0.96, 0.2 );
  legend4->AddEntry( lumiBR, "East Arm - Right Asymmetry", "lep" );
  legend4->AddEntry( lumiBL, "West Arm - Left Asymmetry", "lep" );
  legend4->Draw();

  if( SAVE_IMAGES )
  {
    TString name = directoryName;
    name += particle;
    name += "/RelLumi/";
    name += "lumiLeftRightBlue.png";
    c41->SaveAs( name );
  }

  TCanvas *c45 = new TCanvas( "c45", "Blue:left vs right", 800, 0, 700, 500 );
  c45->SetTicky();
  TGraph *ttestBLR = ttest( lumiBL, lumiBR, 0 );
  ttestBLR->SetMarkerColor( kBlack );
  ttestBLR->SetMarkerStyle( kFullCircle );
  ttestBLR->SetTitle( "; p_{T}[GeV];T" );
  ttestBLR->GetYaxis()->SetRangeUser( -8, 8 );
  ttestBLR->GetXaxis()->SetLimits(0, 21);
  ttestBLR->Draw( "AP" );
  zeroLine->Draw( "same" );

  if( SAVE_IMAGES )
  {
    TString name = directoryName;
    name += particle;
    name += "/";
    name += collisionSystem;
    name += "lumiLeftRightBlueTTest.png";
    c45->SaveAs( name );
  }
}
