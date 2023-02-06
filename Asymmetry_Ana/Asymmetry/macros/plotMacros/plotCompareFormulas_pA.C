#include <iostream>
#include <cmath>
using namespace std;

#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TF1.h"

#include "ttest.h"
#include "ttestNew.h"

const int SIZE = 2;
//const float MIN = -0.06;
//const float MAX =  0.06;

// Use this for the open heavy flavor electron measurement //
const float MIN = -0.07;
const float MAX =  0.07;

const bool SAVE_IMAGES = true;

// Use this for the open heavy flavor electron measurement //
void plotCompareFormulas_pA( const char* particle = "eta", const char* collisionSystem = "pAu", const bool background = false, const bool bgCorrected = true )
{
  TString inputFileName = "../dataFiles/";
  inputFileName += particle;
  inputFileName += "_";
  if (background && !bgCorrected)
    inputFileName += "background_";
  else if (bgCorrected && !background)
    inputFileName += "bgCorrected_"; 
  else if (bgCorrected && background)
    inputFileName += "this is a fail safe";   
  inputFileName += collisionSystem;
  inputFileName += "_AN";
  inputFileName += ".root";

  cout << "Inputting file " << inputFileName << endl;
  TFile *file = TFile::Open( inputFileName );
  TGraphAsymmErrors* lumi;
  if (bgCorrected)
  {
    lumi = (TGraphAsymmErrors*)file->Get( "lumi" );
  }
  else
  {
    lumi = (TGraphAsymmErrors*)file->Get( "lumiB" );
  }
 
  TGraphAsymmErrors* sqrt;
  if (bgCorrected)
  {
    sqrt = (TGraphAsymmErrors*)file->Get( "sqrt" );
  }
  else 
  {
    sqrt = (TGraphAsymmErrors*)file->Get( "sqrtB" );
  }

  TLegend *legend = new TLegend( 0.15, 0.7, 0.5, 0.9);
  //TLegend *legend = new TLegend( 0.15,0.9, 0.53, 0.72 );
  legend->AddEntry( sqrt, "Square Root Formula", "lep" );
  legend->AddEntry( lumi, "Relative Luminosity Formula", "lep" );

  TF1 *line = new TF1( "line", "0", 0, 20 );
  char* marph ="Blue";
  line->SetLineColor( kBlack );
  line->SetLineStyle( 2 );
  TCanvas *canvases[2];
  TGraph *ttests; 

  lumi->SetMarkerStyle( kFullDiamond );
  lumi->SetMarkerColor( kViolet - 6 );
  lumi->SetMarkerSize( SIZE );

  sqrt->SetMarkerStyle( kFullSquare );
  sqrt->SetMarkerColor( kRed );
  sqrt->SetMarkerSize( 0.7* SIZE );
  if (bgCorrected)
    sqrt->GetYaxis()->SetRangeUser(-0.07, 0.15);
  else
    sqrt->GetYaxis()->SetRangeUser( MIN, MAX );
  
  if( !SAVE_IMAGES )
	{
	    sqrt->SetTitle( "Blue Beam; p_{T} [GeV];" ); 
	}
  else
	sqrt->SetTitle( "; p_{T} [GeV];" ); 

  TString label = "c";
  label += 100;
  canvases[0] = new TCanvas( label, marph );
  sqrt->Draw("AP");
  lumi->Draw("P");
  legend->Draw();
  line->Draw( "same" );

  label += 1;
  canvases[1] = new TCanvas( label, marph, 800, 0, 700, 500 );
  canvases[1]->SetTicky();
  ttests = ttest( sqrt, lumi, 1 );
  ttests->SetMarkerColor( kBlack );
  ttests->SetMarkerStyle( kFullCircle );
  ttests->SetTitle( "; p_{T}[GeV];T" );
  ttests->GetYaxis()->SetRangeUser( -8, 8 );
  ttests->GetXaxis()->SetLimits( 0, 21 );
  ttests->Draw( "AP" );
  line->Draw( "same" );

  if( SAVE_IMAGES )
	{
	  TString name = "./images/";
	  name += particle;
	  name += "/";
	  if (background && !bgCorrected)
	    name += "bg_";
	  else if (bgCorrected && !background)
	    name += "bgCorrected_";
	  name += collisionSystem; 
	  name += "CompareSqrtLumi";
	  name += marph;
	  TString title = name;
	  title += ".png";
	  canvases[0]->SaveAs( title );

	  TString ttestTitle = name;
	  ttestTitle += "TTest.png";
	  canvases[1]->SaveAs( ttestTitle );
	}
}
