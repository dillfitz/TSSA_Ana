#include <iostream>
using namespace std;

#include "TFile.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"
#include "TAxis.h"

#include "ttest.h"
#include "../../Constants.h"

const bool SAVE_PICTURES = true;//false;

void plotWestVsEast( const char* particle = "pi0_iso_no_pbgl" )
//_isolation_cut_commented_out" )
{
  TF1* zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlack );
  zeroLine->SetLineStyle( 2 );

  TString inputFileName = "/direct/phenix+u/workarea/nialewis/Run15ppPhotons/Asymmetry/macros/dataFiles/graphs_";
  inputFileName += particle;
  inputFileName += ".root";
  cout << "Opening file " << inputFileName << endl;
  TFile *file = TFile::Open( inputFileName );

  TGraphAsymmErrors *y[ NUM_ARMS ], *b[ NUM_ARMS ];
  TGraph *ttests[ NUM_ARMS ];
  TCanvas *canvases[2][ NUM_ARMS ];
  y[ WEST ] = (TGraphAsymmErrors*)file->Get( "lumiYR" );
  y[ EAST ] = (TGraphAsymmErrors*)file->Get( "lumiYL" );
  b[ WEST ] = (TGraphAsymmErrors*)file->Get( "lumiBL" );
  b[ EAST ] = (TGraphAsymmErrors*)file->Get( "lumiBR" );
  for( int arm = 0; arm < NUM_ARMS; arm++ )
    {
      y[ arm ]->SetMarkerColor( kOrange );
      y[ arm ]->SetMarkerStyle( kFullSquare );
      TString title = ARM_NAMES[ arm ];
      title += " Arm Asymmetries; p_{T} [GeV]; ";
      y[ arm ]->SetTitle( title );
      y[ arm ]->GetYaxis()->SetRangeUser( -0.15, 0.25 );
      b[ arm ]->SetMarkerColor( kBlue );
      b[ arm ]->SetMarkerStyle( kFullDiamond );
      TString name = ARM_NAMES[ arm ];
      name += arm;
      canvases[0][ arm ]  = new TCanvas( name, name );
      y[ arm ]->Draw( "AP" );
      b[ arm ]->Draw( "P" );
      zeroLine->Draw( "same" );
      if( SAVE_PICTURES )
	{
	  TString pictureName = "images/WestVsEast/";
	  pictureName += particle;
	  pictureName += ARM_NAMES[ arm ];
	  pictureName += ".png";
	  canvases[0][ arm ]->SaveAs( pictureName );
	}

      TString name1 = ARM_NAMES[ arm ];
      name1 += 10 + arm ;
      canvases[1][ arm ] = new TCanvas( name1, name1, 
					800, 0, 700, 500 );
      canvases[1][ arm ]->SetTicky();
      ttests[ arm ] = ttest( y[ arm ], b[ arm ], 0 );
      ttests[ arm ]->SetMarkerColor( kBlack );
      ttests[ arm ]->SetMarkerStyle( kFullCircle );
      TString ttestTitle = ARM_NAMES[ arm ];
      ttestTitle += ": Yellow - Blue; p_{T} [GeV]; T";
      ttests[ arm ]->SetTitle( ttestTitle );
      ttests[ arm ]->GetYaxis()->SetRangeUser( -4, 4 );
      ttests[ arm ]->Draw( "AP" );
      zeroLine->Draw( "same" );
      if( SAVE_PICTURES )
	{
	  TString pictureName = "images/WestVsEast/";
	  pictureName += particle;
	  pictureName += ARM_NAMES[ arm ];
	  pictureName += "TTest.png";
	  canvases[1][ arm ]->SaveAs( pictureName );
	}


    }

}
