#include <iostream>
#include <sstream>
using namespace std;

#include "TStyle.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TGraph.h" 
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "../../../Constants.h"
#include "../plotMacros/weightedAverage.h"
#include "ConstantsSinPhi.h"
#include "../plotMacros/ttest.h"


const bool SAVE_PICTURES = true;

// Use this for the open heavy flavor electron measurement //
const float YMIN[ NUM_PT_BINS ] = 
  { -0.25, -0.25, -0.25, -0.25};
const float YMAX[ NUM_PT_BINS ] = 
  {  0.25,  0.25,  0.25,  0.25};

const float AVE_YMIN = -0.1;
const float AVE_YMAX =  0.1;

// Use this for the open heavy flavor electron measurement //
void plotSinPhi( const char* particle = "ohfe" )
{
  gStyle->SetOptFit( true );

  TF1* zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlack );
  zeroLine->SetLineStyle( 2 );

  TString phiFileName = particle;
  phiFileName += "_phi.root";
  TFile *file = TFile::Open( phiFileName );
  cout << "opening phi file " << phiFileName << endl;
  TGraphAsymmErrors *asGraphs[ NUM_BEAMS ][ NUM_OPTIONS_PHI ][ NUM_PT_BINS ];
  for( int b = 0; b < NUM_BEAMS; b++ )
    for( int op = 0; op < NUM_OPTIONS_PHI; op++ )
      for( int ptBin = 0; ptBin < NUM_PT_BINS; ptBin++ )
	{

	  ostringstream name;
	  name << BEAM_NAMES[ b ] << OPTION_NAMES[ op ] << PT_BINS[ ptBin ]
	       << "to" << PT_BINS[ ptBin + 1 ];

	  asGraphs[ b ][ op ][ ptBin ] = (TGraphAsymmErrors*)file->Get( name.str().c_str() );
	}

  TCanvas *canvases[ NUM_BEAMS ][ NUM_PT_BINS ];
  TGraphAsymmErrors *newAsGraphs[ NUM_BEAMS ][ NUM_PT_BINS ];
  double amp[ NUM_BEAMS ][ NUM_PT_BINS ];
  double ampErr[ NUM_BEAMS ][ NUM_PT_BINS ];
  for( int b = 0; b < NUM_BEAMS; b++ )
    for( int ptBin = 0; ptBin < NUM_PT_BINS; ptBin++ )
      {
	//declare new things
	double newPhiCent[ NUM_PHI_BINS*2 ];
	double newPhiLow[ NUM_PHI_BINS*2 ], newPhiHigh[ NUM_PHI_BINS*2 ];
	double newAs[ NUM_PHI_BINS*2 ], newAsErr[ NUM_PHI_BINS*2 ];
 
	//left first: going backwards
	double *phiLeftCent = asGraphs[b][ LEFT ][ ptBin ]->GetX();
	double *phiLeftLow  = asGraphs[b][ LEFT ][ ptBin ]->GetEXlow();
	double *phiLeftHigh = asGraphs[b][ LEFT ][ ptBin ]->GetEXhigh();
	double *leftAs      = asGraphs[b][ LEFT ][ ptBin ]->GetY();
	double *leftAsErr   = asGraphs[b][ LEFT ][ ptBin ]->GetEYhigh();
	for( int newBin = 0; newBin < NUM_PHI_BINS; newBin++ )
	  {
	    int oldBin = newBin;

	    newPhiCent[ newBin ] = phiLeftCent[ oldBin ];
	    newPhiLow[ newBin ]  = phiLeftLow[ oldBin ];
	    newPhiHigh[ newBin ] = phiLeftHigh[ oldBin ];

	    //cout << ptBin << ": " << newPhiCent[ newBin ] << endl;

	    newAs[ newBin ] = leftAs[ oldBin ];
	    newAsErr[ newBin ] = leftAsErr[ oldBin ];
	  }

	//then right: shifting by pi
	double *phiRightCent = asGraphs[b][ RIGHT ][ ptBin ]->GetX();
	double *phiRightLow  = asGraphs[b][ RIGHT ][ ptBin ]->GetEXlow();
	double *phiRightHigh = asGraphs[b][ RIGHT ][ ptBin ]->GetEXhigh();
	double *rightAs      = asGraphs[b][ RIGHT ][ ptBin ]->GetY();
	double *rightAsErr   = asGraphs[b][ RIGHT ][ ptBin ]->GetEYhigh();
	for( int newBin = NUM_PHI_BINS; newBin < NUM_PHI_BINS*2; newBin++ )
	  {
	    int oldBin = NUM_PHI_BINS*2 -1 - newBin;

	    newPhiCent[ newBin ] = 2*PI - phiRightCent[ oldBin ];
	    newPhiLow[ newBin ]  = phiRightHigh[ oldBin ];
	    newPhiHigh[ newBin ] = phiRightLow[ oldBin ];

	    //cout << ptBin << ": " << newPhiCent[ newBin ] << endl;

	    newAs[ newBin ]    =  - rightAs[ oldBin ];
	    newAsErr[ newBin ] = rightAsErr[ oldBin ];
	  }


	//TString name = BEAM_NAMES[b];
	//name += PT_BINS[ ptBin ];
	//name += "to";
	//name += PT_BINS[ ptBin + 1 ];

	ostringstream name;
	name << BEAM_NAMES[ b ] << PT_BINS[ ptBin ]
	     << "to" << PT_BINS[ ptBin + 1 ];

	if( b == 0 )
	  canvases[b][ ptBin ] = new TCanvas( name.str().c_str(), name.str().c_str() );
	else if( b == 1 )
	  canvases[b][ ptBin ] = new TCanvas( name.str().c_str(), name.str().c_str(), 800, 0, 700, 500 );

	TString frameName = "f";
	frameName += b*10 + ptBin;
	TH1 *frame = new TH2F( frameName, " ", 1, 0.0, 2*PI, 1, 
			       YMIN[ ptBin ], YMAX[ ptBin ] );
	frame->SetStats(0);
	//TString title = BEAM_NAMES[b];
	//title += " Asymmetry for ";
	//title += PT_BINS[ ptBin ];
	//title += " < p_{T} <  ";
	//title += PT_BINS[ ptBin + 1 ];
	//title += " GeV ;#phi_{S};";


	ostringstream title;
	title << BEAM_NAMES[ b ] <<  " Asymmetry for " << PT_BINS[ ptBin ]
	     << " < p_{T} <  " << PT_BINS[ ptBin + 1 ] << " GeV ;#phi_{S};";

	frame->SetTitle( title.str().c_str() );

	if( SAVE_PICTURES  ) frame->SetTitle( ";#phi_{S};" );
	frame->Draw( " " );
  
	newAsGraphs[b][ ptBin ] = 
	  new TGraphAsymmErrors( NUM_PHI_BINS*2, newPhiCent, newAs, 
				 newPhiLow, newPhiHigh, newAsErr, newAsErr );
	newAsGraphs[b][ ptBin ]->Draw( "P" );
	zeroLine->Draw( "same" );
	TF1 *fit = new TF1( "fit", "[0]*sin(x)", 0, 2*PI );
	fit->SetParameter( 0, 0.005 );
	newAsGraphs[b][ ptBin ]->Fit( fit, "QR" );
	amp[b][ ptBin ] = fit->GetParameter( 0 );
	ampErr[b][ ptBin ] = fit->GetParError( 0 );

	if( SAVE_PICTURES )
	  {
	    TString pictureName = "images/";
	    pictureName += particle;
	    pictureName += "/sinPhi";
	    pictureName += name.str().c_str();
	    pictureName += ".png";
	    canvases[b][ ptBin ]->SaveAs( pictureName );
	  }
      }

  double ptLow[ NUM_PT_BINS ], ptHigh[ NUM_PT_BINS ];
  double finalAmp[ NUM_PT_BINS ], finalAmpErr[ NUM_PT_BINS ];
  for( int ptBin = 0; ptBin < NUM_PT_BINS; ptBin++ )
    {
      ptLow[ ptBin ] = PT_BIN_CENTERS[ ptBin ] - PT_BINS[ ptBin ];
      ptHigh[ ptBin ] = PT_BINS[ ptBin + 1 ] - PT_BIN_CENTERS[ ptBin ];

      weightedAverage( amp[ YELLOW ][ ptBin ], ampErr[ YELLOW ][ ptBin ],
		       amp[ BLUE   ][ ptBin ], ampErr[ BLUE   ][ ptBin ],
		       finalAmp[ ptBin ], finalAmpErr[ ptBin ] );
    }
  double *asymmetry = amp[ YELLOW ];
  double *asymmetryErr = ampErr[ YELLOW ];
  cout << " asymmetryErr " << asymmetryErr << endl;
  TGraphAsymmErrors *yellow = 
    new TGraphAsymmErrors( NUM_PT_BINS, PT_BIN_CENTERS, asymmetry, 
                           ptLow, ptHigh, asymmetryErr, asymmetryErr );
  yellow->SetMarkerColor( kOrange );
  yellow->SetMarkerStyle( kFullDiamond );
  asymmetry = amp[ BLUE ];
  asymmetryErr = ampErr[ BLUE ];
  TGraphAsymmErrors *blue = 
    new TGraphAsymmErrors( NUM_PT_BINS, PT_BIN_CENTERS, asymmetry,
			   ptLow, ptHigh, asymmetryErr, asymmetryErr );
  blue->SetMarkerColor( kBlue );
  blue->SetMarkerStyle( kFullDiamond );
  
  blue->SetMarkerStyle( kFullSquare );
  blue->SetTitle( "Asymmetry Values From Fits; p_{T} [GeV];" );
  blue->GetYaxis()->SetRangeUser( AVE_YMIN, AVE_YMAX );
  TCanvas *c30 = new TCanvas( "c30", "fit parameters" );
  blue->Draw( "AP" );
  yellow->Draw( "P" );
  zeroLine->Draw( "same" );
  if( SAVE_PICTURES )
    {
      blue->SetTitle( " ;p_{T} [GeV]; " );
      TString pictureName = "images/";
      pictureName += particle;
      pictureName += "/blueYellowSinPhi";
      TString ttestSinName = pictureName;
      pictureName += ".png";
      c30->SaveAs( pictureName );
    }

  TCanvas *tcanSin = new TCanvas( "tcanSin", "SinPhi A_{N} Yellow Blue tTest" );
  TGraph *tTestSin; 
  tTestSin = ttest(yellow, blue, 0 );
  tTestSin->SetMarkerColor( kBlack );
  tTestSin->SetMarkerStyle( kFullCircle );
  tTestSin->SetTitle( "; p_{T}[GeV];T" );
  tTestSin->GetYaxis()->SetRangeUser( -6, 6 );
  tTestSin->Draw( "AP" );
  zeroLine->Draw( "same" );

  if( SAVE_PICTURES )
  {
      ttestSinName += "TTest.png";
      tcanSin->SaveAs( ttestSinName );
  }
 
  TString graphFileName = "../dataFiles/";
  graphFileName += particle;
  graphFileName += "_AN.root";
  TFile *otherFile = TFile::Open( graphFileName );
  cout << "Opening original asymmetry graph file " << graphFileName << endl;
  TGraphAsymmErrors* otherYellow = (TGraphAsymmErrors*)otherFile->Get("lumiY");
  TGraphAsymmErrors* otherBlue   = (TGraphAsymmErrors*)otherFile->Get("lumiB");
  TGraphAsymmErrors* otherLumi   = (TGraphAsymmErrors*)otherFile->Get("lumi");
  /*
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  otherYellow->SetMarkerColor( kOrange + 7 );
  otherYellow->SetMarkerStyle( kFullSquare );
  otherYellow->GetYaxis()->SetRangeUser( AVE_YMIN, AVE_YMAX );
  otherYellow->SetTitle( "Yellow Beam Asymmtries; p_{T} [GeV];" );
  TCanvas *c45 = new TCanvas( "Yellow ones" );
  otherYellow->Draw( "AP" );
  yellow->Draw( "P" );
  zeroLine->Draw( "same" );
  TLegend *yLegend = new TLegend( 0.15, 0.9, 0.53, 0.68 );
  yLegend->AddEntry( otherYellow, "From integrating over the arms", "lep" );
  yLegend->AddEntry( yellow, "From fits", "lep" );
  yLegend->Draw();
  if( SAVE_PICTURES )
    c45->SaveAs( "images/pi0_iso/yellowComepareSinPhi.png" );

  otherBlue->SetMarkerColor( kAzure + 6 );
  otherBlue->SetMarkerStyle( kFullSquare );
  otherBlue->GetYaxis()->SetRangeUser( AVE_YMIN, AVE_YMAX );
  otherBlue->SetTitle( "Blue Beam Asymmetries; p_{T} [GeV];" );
  TCanvas *c46 = new TCanvas( "Blue ones" );
  otherBlue->Draw( "AP" );
  blue->Draw( "P" );
  zeroLine->Draw( "same" );
  TLegend *bLegend = new TLegend( 0.15, 0.9, 0.53, 0.68 );
  bLegend->AddEntry( otherBlue, "From integrating over the arms", "lep" );
  bLegend->AddEntry( blue, "From fits", "lep" );
  bLegend->Draw();
  if( SAVE_PICTURES )
    c46->SaveAs( "images/pi0_iso/blueComepareSinPhi.png" );
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  */
    
  TCanvas *c85 = new TCanvas( "c85", "From intergrating over arm" );
  otherYellow->SetMarkerColor( kOrange );
  otherYellow->SetMarkerStyle( kFullDiamond );
  otherBlue->SetMarkerColor( kBlue );
  otherBlue->SetMarkerStyle( kFullSquare );
  otherBlue->
    SetTitle("Yellow Blue Asymmetry From Integrating Over Arms; p_{T} [GeV];" );
  otherBlue->GetYaxis()->SetRangeUser( AVE_YMIN, AVE_YMAX );
  otherBlue->Draw( "AP" );
  otherYellow->Draw( "P" );
  zeroLine->Draw( "same" );
  //*/
  TCanvas *c52 = new TCanvas( "c52", "the finals" );
  TGraphAsymmErrors *lumi = 
    new TGraphAsymmErrors( NUM_PT_BINS, PT_BIN_CENTERS, finalAmp,
			   ptLow, ptHigh, finalAmpErr, finalAmpErr );
  lumi->SetMarkerColor( kRed );
  lumi->SetMarkerStyle( kFullSquare );
  lumi->SetTitle( "Final Relative Luminosity Asymmetries; p_{T} [GeV];" );
  lumi->GetYaxis()->SetRangeUser( AVE_YMIN, AVE_YMAX );
  lumi->Draw( "AP" );
  otherLumi->SetMarkerColor( kViolet - 6 );
  otherLumi->SetMarkerStyle( kFullCircle );
  otherLumi->Draw( "P" );
  zeroLine->Draw( "same" );
  TLegend *legend = new TLegend( 0.55, 0.9, 0.95, 0.7 );
  //TLegend *legend = new TLegend( 0.15, 0.9, 0.53, 0.68 );
  legend->AddEntry( lumi, "From fits", "lep" );
  legend->AddEntry( otherLumi, "From Integrating Over Arms", "lep" );
  legend->Draw();
  if( SAVE_PICTURES )
    {
      lumi->SetTitle( "; p_{T} [GeV];" );
      TString pictureName = "images/";
      pictureName += particle;
      pictureName += "/finalCompareSinPhi";
      TString ttestName = pictureName;
      pictureName += ".png";
      c52->SaveAs( pictureName );
    }

    TCanvas *tcan = new TCanvas( "tcan", "SinPhi A_{N} Comparison tTest" );
    TGraph *tTest; 
    tTest = ttest(lumi, otherLumi, 1 );
    tTest->SetMarkerColor( kBlack );
    tTest->SetMarkerStyle( kFullCircle );
    tTest->SetTitle( "; p_{T}[GeV];T" );
    tTest->GetYaxis()->SetRangeUser( -6, 6 );
    tTest->Draw( "AP" );
    zeroLine->Draw( "same" );

    if( SAVE_PICTURES )
    {
        ttestName += "TTest.png";
        tcan->SaveAs( ttestName );
    }

}
