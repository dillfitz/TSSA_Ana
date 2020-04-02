//Corrects the direct photon asymmetry for background of photons coming from pi0
//and eta decays.  In reality these background asymmetries are being set to zero
//so this macro just rescales the asymmetry values and the statistical 
//uncertainty
//This macro output the graphs with background corrected values to 
//"../dataFiles/graphs_dp_corrected.root"

#include <iostream>
#include <fstream>
using namespace std;

#include "TFile.h"
#include "TGraphAsymmErrors.h"

#include "../../Constants.h"
#include "../plotMacros/weightedAverage.h"

const int NUM_PT_BINS = 4;
const float PT_BIN_CENTERS[ NUM_PT_BINS ] = { 5.39, 6.69, 8.77, 11.9 }; 
const double PT_BINS[ NUM_PT_BINS + 1 ] = { 5, 6, 8, 10, 18 };

//Starting with many sig figs for the bacgkround fraction and then rounding at 
//the end.  Initially when I rounded the background fraction before the 
//correction it made the T values from compairing the sqrt and rel lumi 
//asymmettries VERY large
const float r_PI0[ NUM_OPTIONS ][ NUM_PT_BINS ] = 
  {
    { 0.38369,  0.319536, 0.225098, 0.124452 },//West Arm
    { 0.408852, 0.343064, 0.257425, 0.136619 },//East Arm
    { 0.396062, 0.331128, 0.241072, 0.130449 } //Sqrt
  }; 

const float r_ETA[ NUM_OPTIONS ][ NUM_PT_BINS ] = 
 {
    { 0.08534,   0.072936,  0.0547602, 0.0345592 },//West Arm
    { 0.08732,   0.07808,   0.06324,   0.045766  },//East Arm
    { 0.0863135, 0.0754704, 0.0589505, 0.0400829 } //Sqrt
  };

const int NUM_GRAPHS = 6;
const char* names[ NUM_GRAPHS ] = 
  { "lumiYL","lumiYR","lumiBL","lumiBR", "sqrtY","sqrtB" };

void correctBackgroundDP()
{
  float ptLow[ NUM_PT_BINS ], ptHigh[ NUM_PT_BINS ];
  for( int b = 0; b < NUM_PT_BINS; b++ )
    {
      ptLow[b]  = fabs( PT_BINS[b] - PT_BIN_CENTERS[b] );
      ptHigh[b] = fabs( PT_BINS[b + 1] - PT_BIN_CENTERS[b] );
    }

  TFile *isoFile = TFile::Open( "../dataFiles/graphs_dp.root" );
  TGraphAsymmErrors* isoGraphs[ NUM_GRAPHS ];
  double *iso[ NUM_GRAPHS ], *isoErr[ NUM_GRAPHS ];
  for( int i = 0; i < NUM_GRAPHS; i++ )
    {
      isoGraphs[i] = (TGraphAsymmErrors*)isoFile->Get( names[i] );
      iso[i]    = isoGraphs[i]->GetY();
      isoErr[i] = isoGraphs[i]->GetEYhigh();
    }

  float corrected[ NUM_GRAPHS ][ NUM_PT_BINS ];
  float correctedErr[ NUM_GRAPHS ][ NUM_PT_BINS ];
  for( int g = 0; g < NUM_GRAPHS; g++ )
    for( int b = 0; b < NUM_PT_BINS; b++ )
      {
	float r = -99999;
	//     0        1        2        3        4        5
	//  { "lumiYL","lumiYR","lumiBL","lumiBR", "sqrtY","sqrtB" };
	if( g == 0 || g == 3 )//yellow left, blue right
	  r = r_PI0[ EAST ][b] + r_ETA[ EAST ][b];

	else if(  g == 1 || g == 2 )//yellow right, blue left
	  r = r_PI0[ WEST ][b] + r_ETA[ WEST ][b];

	else if(  g == 4 || g == 5 )//sqrt
	  r = r_PI0[ SQRT ][b] + r_ETA[ SQRT ][b];

	corrected[g][b] = ( iso[g][b] ) / ( 1 - r );

	correctedErr[g][b] = isoErr[g][b] /( 1 - r );
      }

  TFile *outFile = 
    new TFile( "../dataFiles/graphs_dp_corrected.root","RECREATE" );
  outFile->cd();
  for( int g = 0; g < NUM_GRAPHS; g++ )
    {
      TGraphAsymmErrors* graph = 
	new TGraphAsymmErrors( NUM_PT_BINS, PT_BIN_CENTERS, corrected[g], 
			      ptLow, ptHigh, correctedErr[g], correctedErr[g] );

      graph->SetMarkerStyle( kFullCircle );
      graph->Write( names[g] );
    }

  float lumiY[ NUM_PT_BINS ], lumiYErr[ NUM_PT_BINS ];
  float lumiB[ NUM_PT_BINS ], lumiBErr[ NUM_PT_BINS ];
  float asymmetrySqrt[ NUM_PT_BINS ], asymmetrySqrtErr[ NUM_PT_BINS ];
  float asymmetryLumi[ NUM_PT_BINS ], asymmetryLumiErr[ NUM_PT_BINS ];
  for( int i = 0; i < NUM_PT_BINS; i++ )
    {
	//     0        1        2        3        4        5
	//  { "lumiYL","lumiYR","lumiBL","lumiBR", "sqrtY","sqrtB" };
      weightedAverage(  corrected[0][i], correctedErr[0][i],
			corrected[1][i], correctedErr[1][i],
			lumiY[i], lumiYErr[i] );

      weightedAverage( corrected[2][i], correctedErr[2][i],
		       corrected[3][i], correctedErr[3][i],
		       lumiB[i], lumiBErr[i] );

      weightedAverage( lumiY[i], lumiYErr[i],
		       lumiB[i], lumiBErr[i],
		       asymmetryLumi[i], asymmetryLumiErr[i] );

      weightedAverage( corrected[4][i], correctedErr[4][i],
		       corrected[5][i], correctedErr[5][i],
		       asymmetrySqrt[i], asymmetrySqrtErr[i] );
    }

  TGraphAsymmErrors *graphLumiY = 
    new TGraphAsymmErrors( NUM_PT_BINS, PT_BIN_CENTERS, lumiY,  
			   ptLow, ptHigh, lumiYErr, lumiYErr );
  graphLumiY->Write( "lumiY" );
  graphLumiY->SetMarkerStyle( kFullCircle );
  graphLumiY->Delete();

  TGraphAsymmErrors *graphLumiB =  
    new TGraphAsymmErrors( NUM_PT_BINS, PT_BIN_CENTERS, lumiB, 
			   ptLow, ptHigh, lumiBErr, lumiBErr );
  graphLumiB->Write( "lumiB" );
  graphLumiB->SetMarkerStyle( kFullCircle );
  graphLumiB->Delete();

  TGraphAsymmErrors *graphLumi = 
    new TGraphAsymmErrors( NUM_PT_BINS, PT_BIN_CENTERS, asymmetryLumi,
			   ptLow, ptHigh, asymmetryLumiErr, asymmetryLumiErr );
  graphLumi->Write( "lumi" );
  graphLumi->SetMarkerStyle( kFullCircle );
  graphLumi->Delete();

  TGraphAsymmErrors *graphSqrt = 
    new TGraphAsymmErrors( NUM_PT_BINS, PT_BIN_CENTERS,asymmetrySqrt, 
			   ptLow, ptHigh, asymmetrySqrtErr, asymmetrySqrtErr );
  graphSqrt->Write( "sqrt" );
  graphSqrt->SetMarkerStyle( kFullCircle );
  graphSqrt->Delete();

}

