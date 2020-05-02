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

const float YMIN = -0.1;
const float YMAX =  0.1;
//const float YMIN = -0.05;
//const float YMAX =  0.06;

const int SIZE = 2;
const int numGraphs = 4;
const char *names[ numGraphs ] = { "lumiYL", "lumiYR", "lumiBL", "lumiBR"};
const int YELLOW_EAST = 0;
const int YELLOW_WEST = 1;
const int BLUE_WEST = 2;
const int BLUE_EAST = 3;

void plotRelLumi( const char* particle = "ohfe" )//"eta_iso_dp_pt" )
{
  TString inputFileName = "../dataFiles/";
  inputFileName += particle;
  inputFileName += "_AN";
  inputFileName += ".root";
  TF1* zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlack );
  zeroLine->SetLineStyle( 2 );

  cout << "Opening file " << inputFileName << endl;
  TGraphAsymmErrors *lumi[ numGraphs ]; 
  TFile *file = TFile::Open( inputFileName );
  for( int i = 0; i < numGraphs; i++ )
    {
      lumi[i] = (TGraphAsymmErrors*)file->Get( names[i] );
      if( i == YELLOW_EAST || i == BLUE_EAST )//east
	{
	  lumi[i]->SetMarkerStyle( kFullSquare );
	  lumi[i]->SetMarkerColor( kGreen + 3 );
	  lumi[i]->SetMarkerSize( 0.7 * SIZE );
	}
      else if( i == YELLOW_WEST || i == BLUE_WEST )
	{
	  lumi[i]->SetMarkerStyle( kFullDiamond );
	  lumi[i]->SetMarkerColor( kCyan + 1 );
	  lumi[i]->SetMarkerSize( SIZE );
	}
      lumi[i]->GetYaxis()->SetRangeUser( YMIN, YMAX );
    }

  //  { "lumiYL", "lumiYR", "lumiBL", "lumiBR"};
  TCanvas *c20 = new TCanvas( "c20", "Yellow Beam Left Right" );
  if( !SAVE_IMAGES )
    lumi[ YELLOW_EAST ]
      ->SetTitle( "Yellow Beam Left Right Asymmetry; p_{T} [GeV]; " );
  else
    lumi[ YELLOW_EAST ]->SetTitle( "; p_{T} [GeV]; " );

  lumi[ YELLOW_EAST ]->Draw("AP");
  lumi[ YELLOW_WEST ]->Draw("P");
  zeroLine->Draw( "same" );
  TLegend *legend2 = new TLegend( 0.52,0.9, 0.9, 0.7 );
  //TLegend *legend2 = new TLegend( 0.58,0.7, 0.96, 0.5 );
  legend2->AddEntry( lumi[ YELLOW_EAST ], "East Arm - Left Asymmetry", "lep" );
  legend2->AddEntry( lumi[ YELLOW_WEST ], "West Arm - Right Asymmetry", "lep" );
  legend2->Draw();

  TString directoryName = "./images/";

  if( SAVE_IMAGES )
    {
      TString name = directoryName;
      name += particle;
      name += "/RelLumi/";
      name += "lumiLeftRightYellow.png";
      c20->SaveAs( name );
    }

  TCanvas *c41 = new TCanvas( "c41", "Blue Beam Left Right Asymmetry " );
  if( !SAVE_IMAGES )
    lumi[ BLUE_EAST ]
      ->SetTitle( "Blue Beam Left Right Asymmetry; p_{T} [GeV];" );
  else
    lumi[ BLUE_EAST ]->SetTitle( "; p_{T} [GeV];" );

  lumi[ BLUE_EAST ]->Draw("AP");
  lumi[ BLUE_WEST ]->Draw("P");
  zeroLine->Draw( "same" );
  TLegend *legend4 = new TLegend( 0.52,0.9, 0.9, 0.7 );
  //TLegend *legend4 = new TLegend( 0.58,0.4, 0.96, 0.2 );
  legend4->AddEntry( lumi[ BLUE_EAST ], "East Arm - Right Asymmetry", "lep" );
  legend4->AddEntry( lumi[ BLUE_WEST ], "West Arm - Left Asymmetry", "lep" );
  legend4->Draw();

  if( SAVE_IMAGES )
    {
      TString name = directoryName;
      name += particle;
      name += "/RelLumi/";
      name += "lumiLeftRightBlue.png";
      c41->SaveAs( name );
    }

  //  char *names[ numGraphs ] = { "lumiYL", "lumiYR", "lumiBL", "lumiBR"};
  TCanvas *c35 = new TCanvas( "c35", "Yellow:left vs right", 800, 0, 700, 500 );
  c35->SetTicky();
  TGraph *ttestYLR = ttest( lumi[ YELLOW_EAST ], lumi[ YELLOW_WEST ], 0 );
  ttestYLR->SetMarkerColor( kBlack );
  ttestYLR->SetMarkerStyle( kFullCircle );
  ttestYLR->SetTitle( "; p_{T}[GeV];T" );
  ttestYLR->GetYaxis()->SetRangeUser( -4, 4 );
  ttestYLR->Draw( "AP" );
  zeroLine->Draw( "same" );

  if( SAVE_IMAGES )
    {
      TString name = directoryName;
      name += particle;
      name += "/RelLumi/";
      name += "lumiLeftRightYellowTTest.png";
      c35->SaveAs( name );
    }

  TCanvas *c45 = new TCanvas( "c45", "Blue:left vs right", 800, 0, 700, 500 );
  c45->SetTicky();
  TGraph *ttestBLR = ttest( lumi[ BLUE_WEST ], lumi[ BLUE_EAST ], 0 );
  ttestBLR->SetMarkerColor( kBlack );
  ttestBLR->SetMarkerStyle( kFullCircle );
  ttestBLR->SetTitle( "; p_{T}[GeV];T" );
  ttestBLR->GetYaxis()->SetRangeUser( -4, 4 );
  ttestBLR->Draw( "AP" );
  zeroLine->Draw( "same" );

  if( SAVE_IMAGES )
    {
      TString name = directoryName;
      name += particle;
      name += "/RelLumi/";
      name += "lumiLeftRightBlueTTest.png";
      c45->SaveAs( name );
    }


}
