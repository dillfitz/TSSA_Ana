#include <iostream>
using namespace std;

#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TF1.h"
#include "TAxis.h"

#include "ttest.h"

const float YMIN = -0.15;
const float YMAX =  0.2;

const int SIZE = 2;
const int NUM_GRAPHS = 4;
const char *names[ NUM_GRAPHS ] = { "lumiYL", "lumiYR", "lumiBL", "lumiBR"};
const int YELLOW_EAST = 0;
const int YELLOW_WEST = 1;
const int BLUE_WEST = 2;
const int BLUE_EAST = 3;

bool SAVE_IMAGES = true;//false;

//Curret type options:
//             "" - regular
// "_no_sec4and5" - just PbGl in the East Arm
//     "_no_pbgl" - just PbSc in the West Arm

void plotPi0IsoWithInclRelLumi( char* type = "_no_sec4and5" )
{
  TF1* zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlack );
  zeroLine->SetLineStyle( 2 );

  char *startOfFileName = "/direct/phenix+u/workarea/nialewis/Run15ppPhotons/Asymmetry/macros/dataFiles/graphs_pi0_iso";
  TString isoFileName = startOfFileName;
  isoFileName += type;
  isoFileName += ".root";
  TFile *isoFile = TFile::Open( isoFileName );
  cout << isoFileName << endl;
  TString inclFileName = startOfFileName;
  inclFileName += "_isolation_cut_commented_out";
  inclFileName += type;
  inclFileName += ".root";
  TFile *inclFile = TFile::Open( inclFileName );
  cout << inclFileName << endl;

  for( int i = 0; i < NUM_GRAPHS; i++ )
    {
      TGraphAsymmErrors* iso  = (TGraphAsymmErrors*)isoFile->Get( names[i] );
      TGraphAsymmErrors* incl = (TGraphAsymmErrors*)inclFile->Get( names[i] );

      if( i == YELLOW_EAST || i == YELLOW_WEST )
	{
	  iso->SetMarkerStyle( kFullSquare );
	  iso->SetMarkerColor( kOrange );
	  iso->SetMarkerSize( 0.7 * SIZE );

	  incl->SetMarkerStyle( kOpenSquare );
	  incl->SetMarkerColor( kOrange );
	  incl->SetMarkerSize( 0.7 * SIZE );
	}
      else if( i == BLUE_EAST || i == BLUE_WEST )
	{
	  iso->SetMarkerStyle( kFullDiamond );
	  iso->SetMarkerColor( kBlue );
	  iso->SetMarkerSize( SIZE );

	  incl->SetMarkerStyle( kOpenDiamond );
	  incl->SetMarkerColor( kBlue );
	  incl->SetMarkerSize( SIZE );
	}

      iso->GetYaxis()->SetRangeUser( YMIN, YMAX );
      if( i == YELLOW_EAST )
	iso->SetTitle( "Yellow East; p_{T}^{#gamma} [GeV];" );
      else if( i == YELLOW_WEST )
	iso->SetTitle( "Yellow West; p_{T}^{#gamma} [GeV];" );
      else if( i == BLUE_WEST )
	iso->SetTitle( "Blue West; p_{T}^{#gamma} [GeV];" );
      else if( i == BLUE_EAST )
	iso->SetTitle( "Blue East; p_{T}^{#gamma} [GeV];" );


      TCanvas *c1 = new TCanvas( names[i], names[i], 0, 0, 700, 500 );
      iso->Draw( "AP" );
      incl->Draw( "P" );
      zeroLine->Draw( "same" );
      TLegend *legend = new TLegend( 0.6, 0.9, 0.9, 0.7 );
      legend->AddEntry( iso, "Isolated #pi^{0}s", "lep" );
      legend->AddEntry( incl, "Inclusive #pi^{0}s", "lep" );
      legend->Draw();
      if( SAVE_IMAGES )
	{
	  TString pictureName = "images/isoWithIncl/";
	  if( i == YELLOW_EAST || i == BLUE_EAST )
	    pictureName += type;
	  pictureName += names[i];
	  pictureName += ".png";
	  c1->SaveAs( pictureName );
	}

      TString canvasName = "c";
      canvasName += names[i];
      TCanvas *c2 = new TCanvas( canvasName, canvasName, 800, 0, 700, 500 );
      c2->SetTicky();
      TGraph *ttestGraph = ttest( iso, incl, 1 );
      ttestGraph->GetYaxis()->SetRangeUser( -3, 3 );
      ttestGraph->Draw( "AP" );
      zeroLine->Draw( "same" );
      if( SAVE_IMAGES )
	{
	  TString pictureName = "images/isoWithIncl/";
	  if( i == YELLOW_EAST || i == BLUE_EAST )
	    pictureName += type;
	  pictureName += names[i];
	  pictureName += "TTest.png";
	  c2->SaveAs( pictureName );
	}

    }
}
