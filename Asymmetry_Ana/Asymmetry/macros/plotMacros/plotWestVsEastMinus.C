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

// Use this for the open heavy flavor electron measurement //
void plotWestVsEastMinus( const char* particle = "ohfe" )
{
  TF1* zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlack );
  zeroLine->SetLineStyle( 2 );

  TString inputFileName = "../dataFiles/";
  inputFileName += particle;
  inputFileName += "_AN_minus";
  inputFileName += ".root";

  cout << "Opening file " << inputFileName << endl;
  TFile *file = TFile::Open( inputFileName );

  TGraphAsymmErrors *y[ NUM_ARMS ], *b[ NUM_ARMS ];
  TGraph *ttests[ NUM_ARMS ];
  TCanvas *canvases[2][ NUM_ARMS ];
  y[ WEST ] = (TGraphAsymmErrors*)file->Get( "lumiYR_minus" );
  y[ EAST ] = (TGraphAsymmErrors*)file->Get( "lumiYL_minus" );
  b[ WEST ] = (TGraphAsymmErrors*)file->Get( "lumiBL_minus" );
  b[ EAST ] = (TGraphAsymmErrors*)file->Get( "lumiBR_minus" );
  for( int arm = 0; arm < NUM_ARMS; arm++ )
    {
      y[ arm ]->SetMarkerColor( kOrange );
      y[ arm ]->SetMarkerStyle( kFullSquare );
      TString title = ARM_NAMES[ arm ];
      title += " Arm Asymmetries; p_{T} [GeV]; ";
      y[ arm ]->SetTitle( title );
      y[ arm ]->GetYaxis()->SetRangeUser( -0.1, 0.1 );
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
	  TString pictureName = "./images/";
	  pictureName += particle;
	  pictureName += "/WestVsEast/";
	  pictureName += ARM_NAMES[ arm ];
	  pictureName += "Minus.png";
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
      ttests[ arm ]->GetXaxis()->SetLimits( 0, 6 );
      ttests[ arm ]->Draw( "AP" );
      zeroLine->Draw( "same" );
      if( SAVE_PICTURES )
	{
	  TString pictureName = "./images/";
	  pictureName += particle;
	  pictureName += "/WestVsEast/";
	  pictureName += ARM_NAMES[ arm ];
	  pictureName += "MinusTTest.png";
	  canvases[1][ arm ]->SaveAs( pictureName );
	}


    }

}
