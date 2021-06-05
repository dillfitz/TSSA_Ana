#include <iostream>
using namespace std;

#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TAxis.h"
#include "TGraph.h" 
#include "TCanvas.h"
#include "TF1.h"

#include "ttest.h"

const int SIZE = 2;
//const float MIN = -0.025;
//const float MAX =  0.03;
const float MIN = -0.06;
const float MAX =  0.06;

const bool SAVE_IMAGES = true;//false;

// Use this for the open heavy flavor electron measurement //
void plotYellowBlueMinus( const char* particle = "ohfe" )
{
  TString inputFileName = "../dataFiles/";
  inputFileName += particle;
  inputFileName += "_AN_minus";
  inputFileName += ".root";

  TF1* zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlack );
  zeroLine->SetLineStyle( 2 );
  cout << "Opening file " << inputFileName << endl;
  TFile *file = TFile::Open( inputFileName );
  TGraphAsymmErrors* lumiY = (TGraphAsymmErrors*)file->Get("lumiY_minus");
  TGraphAsymmErrors* lumiB = (TGraphAsymmErrors*)file->Get("lumiB_minus");
  TGraphAsymmErrors* sqrtY = (TGraphAsymmErrors*)file->Get("sqrtY_minus");
  TGraphAsymmErrors* sqrtB = (TGraphAsymmErrors*)file->Get("sqrtB_minus");
 
  TCanvas *c29 = new TCanvas( "c2", "Lumi Blue Yellow, charge (-)" );
  lumiY->SetMarkerStyle( kFullSquare );
  lumiY->SetMarkerColor( kOrange );
  lumiY->SetMarkerSize( 0.7*SIZE );
  lumiY->GetYaxis()->SetRangeUser( MIN, MAX );
  if( !SAVE_IMAGES )
    lumiY->SetTitle( "Relative Luminosity Blue and Yellow, charge (-); p_{T} [GeV]; " );
  else
    lumiY->SetTitle( "; p_{T} [GeV]; " );

  lumiY->Draw( "AP" );
  lumiB->SetMarkerStyle( kFullDiamond );
  lumiB->SetMarkerColor( kBlue );
  lumiB->SetMarkerSize( SIZE );
  lumiB->Draw("P");
  zeroLine->Draw( "same" );
  if( SAVE_IMAGES )
    {
      TString name = "./images/";
      name += particle;
      name += "/YellowBlue/";
      name += "lumiYellowBlueMinus.png";
      c29->SaveAs( name );
    }

  TCanvas *c30 = new TCanvas( "c3", "Lumi compare yellow blue, charge (-)", 
			      800, 0, 700, 500 );
  c30->SetTicky();
  TGraph *ttestLumi = ttest( lumiY, lumiB, 0 );
  ttestLumi->SetTitle( "; p_{T}[GeV];T" );
  ttestLumi->SetMarkerStyle( kFullCircle );
  ttestLumi->GetYaxis()->SetRangeUser( -3, 3 );
  ttestLumi->GetXaxis()->SetLimits( 0, 6 );
  ttestLumi->Draw( "AP" );
  zeroLine->Draw( "same" );
  if( SAVE_IMAGES )
    {
      TString name = "./images/";
      name += particle;
      name += "/YellowBlue/";
      name += "lumiYellowBlueMinusTTest.png";
      c30->SaveAs( name );
    }

  TCanvas *c41 = new TCanvas( "c4", "Sqrt Blue Yellow, charge (-) " );
  sqrtY->SetMarkerStyle( kFullSquare );
  sqrtY->SetMarkerColor( kOrange );
  sqrtY->SetMarkerSize( 0.7 * SIZE );
  sqrtY->GetYaxis()->SetRangeUser( MIN, MAX );
  if( !SAVE_IMAGES )
    sqrtY->SetTitle( "Square Root Blue and Yellow, charge (-); p_{T} [GeV]; " );
  else
    sqrtY->SetTitle( "; p_{T} [GeV]; " );

  sqrtY->Draw("AP");
  sqrtB->SetMarkerStyle(kFullDiamond);
  sqrtB->SetMarkerColor(kBlue);
  sqrtB->SetMarkerSize(SIZE);
  sqrtB->Draw("P");
  zeroLine->Draw( "same" );
  if( SAVE_IMAGES )
    {
      TString name = "./images/";
      name += particle;
      name += "/YellowBlue/";
      name += "sqrtYellowBlueMinus.png";
      c41->SaveAs( name );
    }

  TCanvas *c40 = new TCanvas( "c5", "Sqrt compare yellow blue, charge (-) ", 
			      800, 0, 700, 500 );
  c40->SetTicky();
  TGraph *ttestSqrt = ttest( sqrtY, sqrtB, 0 );
  ttestSqrt->SetTitle( "; p_{T}[GeV];T" );
  ttestSqrt->SetMarkerStyle( kFullCircle );
  ttestSqrt->GetYaxis()->SetRangeUser( -3, 3 );
  ttestSqrt->GetXaxis()->SetLimits( 0, 6 );
  ttestSqrt->Draw( "AP" );
  zeroLine->Draw( "same" );
  if( SAVE_IMAGES )
    {
      TString name = "./images/";
      name += particle;
      name += "/YellowBlue/";
      name += "sqrtYellowBlueMinusTTest.png";
      c40->SaveAs( name );
    }
}


