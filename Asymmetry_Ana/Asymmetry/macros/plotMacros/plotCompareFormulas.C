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

const int SIZE = 2;
const int numGraphs = 3;
const char *names[6] = { "lumiY", "lumiB", "lumi", "sqrtY", "sqrtB",  "sqrt0" };
//const float MIN = -0.06;
//const float MAX =  0.06;
const float MIN = -0.07;
const float MAX =  0.07;

const bool SAVE_IMAGES = true;

void plotCompareFormulas( const char* particle = "ohfe" )
{
  TString inputFileName = "../dataFiles/";
  inputFileName += particle;
  inputFileName += "_AN";
  inputFileName += ".root";

  cout << "Inputting file " << inputFileName << endl;
  TFile *file = TFile::Open( inputFileName );
  TGraphAsymmErrors* lumi[ numGraphs ];
  for( int i = 0; i < numGraphs; i++ )
    lumi[i] = (TGraphAsymmErrors*)file->Get( names[i] );
 
  TGraphAsymmErrors* sqrt[ numGraphs ];
  for( int i = 3; i < 6; i++ )
    sqrt[i - 3] = (TGraphAsymmErrors*)file->Get( names[i] );

  TLegend *legend = new TLegend( 0.55, 0.9, 0.95, 0.7 );
  //TLegend *legend = new TLegend( 0.15,0.9, 0.53, 0.72 );
  legend->AddEntry( sqrt[0], "Square Root Formula", "lep" );
  legend->AddEntry( lumi[0], "Relative Luminosity Formula", "lep" );

  TF1 *line = new TF1( "line", "0", 0, 20 );
  char* marph[ numGraphs ] = { "Yellow", "Blue", "Averaged" };
  line->SetLineColor( kBlack );
  line->SetLineStyle( 2 );
  TCanvas *canvases[2][ numGraphs ];
  TGraph *ttests[ numGraphs ]; 
  for( int i = 0; i < numGraphs; i++ )
    {
      lumi[i]->SetMarkerStyle( kFullDiamond );
      lumi[i]->SetMarkerColor( kViolet - 6 );
      lumi[i]->SetMarkerSize( SIZE );

      sqrt[i]->SetMarkerStyle( kFullSquare );
      sqrt[i]->SetMarkerColor( kRed );
      sqrt[i]->SetMarkerSize( 0.7* SIZE );
      sqrt[i]->GetYaxis()->SetRangeUser( MIN, MAX );
      if( !SAVE_IMAGES )
	{
	  if( i == 0 )
	    sqrt[i]->SetTitle( "Yellow Beam; p_{T} [GeV];" ); 
	  else if( i == 1 )
	    sqrt[i]->SetTitle( "Blue Beam; p_{T} [GeV];" ); 
	  else if( i == 2 )
	    sqrt[i]->SetTitle( "Weighted Average Asymmetry; p_{T} [GeV];" ); 
	}
      else
	    sqrt[i]->SetTitle( "; p_{T} [GeV];" ); 

      TString label = "c";
      label += i + 100;
      canvases[0][i] = new TCanvas( label, marph[i] );
      sqrt[i]->Draw("AP");
      lumi[i]->Draw("P");
      legend->Draw();
      line->Draw( "same" );

      label += i;
      canvases[1][i] = new TCanvas( label, marph[i], 800, 0, 700, 500 );
      canvases[1][i]->SetTicky();
      ttests[i] = ttest( sqrt[i], lumi[i], 1 );
      ttests[i]->SetMarkerColor( kBlack );
      ttests[i]->SetMarkerStyle( kFullCircle );
      ttests[i]->SetTitle( "; p_{T}[GeV];T" );
      ttests[i]->GetYaxis()->SetRangeUser( -3, 3 );
      ttests[i]->Draw( "AP" );
      line->Draw( "same" );

      if( SAVE_IMAGES )
	{
	  TString name = "./images/";
	  name += particle; 
	  name += "/CompareFormulas/";
	  name += "compareSqrtLumi";
	  name += marph[i];

	  TString title = name;
	  title += ".png";
	  canvases[0][i]->SaveAs( title );

	  TString ttestTitle = name;
	  ttestTitle += "TTest.png";
	  canvases[1][i]->SaveAs( ttestTitle );
	}

    }
}
