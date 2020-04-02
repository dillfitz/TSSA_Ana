//This macro calculates the systematic error due to setting the background 
//asymmetry to zero.  It calculates how much the relative luminosity formula 
//result changes when we plug in a nonzero background asymmetry.

//Ranges for plugged in the the background pi0 and eta asymmetries are the 
//statisical uncertainty from the inclusive (no isolation cut) pi0 and eta 
//asymmetryies integrated over photon p_T.  i.e. find the statisical uncertainty
//for all photons with 5 < p_T < 18 GeV that were tagged as coming from either 
//a pi0 or an eta decay

#include <iostream>
using namespace std;

#include "TFile.h"
#include "TGraphAsymmErrors.h"

#include "../../Constants.h"
#include "../plotMacros/weightedAverage.h"
const int NUM_PT_BINS = 4;
const double PT_BINS[ NUM_PT_BINS + 1 ] = { 5, 6, 8, 10, 18 }; 
const float r_PI0[ NUM_ARMS ][ NUM_PT_BINS ] = 
  {
    { 0.384, 0.320, 0.225, 0.124 },//West Arm
    { 0.409, 0.343, 0.257, 0.137 },//East Arm
  };
const float r_ETA[ NUM_ARMS ][ NUM_PT_BINS ] = 
  {
    { 0.0853, 0.0729, 0.0548, 0.0346 },//West Arm
    { 0.0873, 0.0781, 0.0632, 0.0458 } //East Arm
  };

const int NUM_GRAPHS = 4;
const char* names[ NUM_GRAPHS ] = 
  { "lumiYL","lumiYR","lumiBL","lumiBR"};


void findDPSystError_BGAsymmetry()
{
  TFile *dpFile = TFile::Open( "../dataFiles/graphs_dp.root" );
  //using uncorrected to plug into formula
  TGraphAsymmErrors *dpGraphs[ NUM_GRAPHS ];
  for( int g = 0; g < NUM_GRAPHS; g++ )
    dpGraphs[g] = (TGraphAsymmErrors*)dpFile->Get( names[g] );
  double isoAsymm[ NUM_GRAPHS ][ NUM_PT_BINS ];
  for( int g = 0; g < NUM_GRAPHS; g++ )
    {
     double* asymm = dpGraphs[g]->GetY();
     for( int b = 0; b < NUM_PT_BINS; b++ )//root can be stupid sometimes
       isoAsymm[g][b] = asymm[b];
    }

  TFile *pi0File = 
    TFile::Open( "../dataFiles/graphs_pi0_dp_pt_one_pt_bin.root" );
  //using inclusive pi0 asymetry integrated over photon p_T 
  //... can include both photons
  TGraphAsymmErrors *pi0Graphs[ NUM_GRAPHS ];
  for( int g = 0; g < NUM_GRAPHS; g++ )
    pi0Graphs[g] = (TGraphAsymmErrors*)pi0File->Get( names[g] );
  double pi0AsymmErr[ NUM_GRAPHS ];
  for( int g = 0; g < NUM_GRAPHS; g++ )
      pi0AsymmErr[g] = pi0Graphs[g]->GetErrorYhigh( 0 );
  //has extra pt bins

  TFile *etaFile = 
    TFile::Open( "../dataFiles/graphs_eta_dp_pt_one_pt_bin.root" );
  //using inclusive eta asymmetry integrated over photon p_T
  //... can include both photons
  TGraphAsymmErrors *etaGraphs[ NUM_GRAPHS ];
  for( int g = 0; g < NUM_GRAPHS; g++ )
    etaGraphs[g] = (TGraphAsymmErrors*)etaFile->Get( names[g] );
  double etaAsymmErr[ NUM_GRAPHS ];
  for( int g = 0; g < NUM_GRAPHS; g++ )
      etaAsymmErr[g] = etaGraphs[g]->GetErrorYhigh( 0 );
  //has extra pt bins

  //for copying and pasting to a latex table
  cout << " & pi0 & eta \\" << endl;
  for( int g = 0; g < NUM_GRAPHS; g++ )
    cout << names[g] << " & " << pi0AsymmErr[g] << " & " << etaAsymmErr[g] 
	 << endl;


  TFile *corrFile = TFile::Open( "../dataFiles/graphs_dp_corrected.root" );
  //used for weights when calculated ave asymm with nonzero background
  TGraphAsymmErrors *corrGraphs[ NUM_GRAPHS ];
  for( int g = 0; g < NUM_GRAPHS; g++ )
    corrGraphs[g] = (TGraphAsymmErrors*)corrFile->Get( names[g] );
  double corrAsymmErr[ NUM_GRAPHS ][ NUM_PT_BINS ];
  for( int g = 0; g < NUM_GRAPHS; g++ )
    for( int b = 0; b < NUM_PT_BINS; b++ )
      corrAsymmErr[g][b] = corrGraphs[g]->GetErrorYhigh( b );
  //just need the final lumi to calculate the difference
  TGraphAsymmErrors* corrLumiGraph 
    = (TGraphAsymmErrors*)corrFile->Get( "lumi" );
  double *corrLumi = corrLumiGraph->GetY();

  double dirAsymm[ NUM_GRAPHS ][ NUM_PT_BINS ];
  for( int g = 0; g < NUM_GRAPHS; g++ )
    for( int b = 0; b < NUM_PT_BINS; b++ )
      {
	//     0        1        2        3
	//  { "lumiYL","lumiYR","lumiBL","lumiBR" };
	int arm;
	if( g == 0 || g == 3 )//yellow left, blue right
	  arm = EAST;
	else if(  g == 1 || g == 2 )//yellow right, blue left
	  arm = WEST;

	dirAsymm[g][b] = isoAsymm[g][b];
	dirAsymm[g][b] -= r_PI0[ arm ][b] * pi0AsymmErr[g];
      	dirAsymm[g][b] -= r_ETA[ arm ][b] * etaAsymmErr[g];
	dirAsymm[g][b] /= 1 - r_ETA[ arm ][b] - r_PI0[ arm ][b];

	//not bothing with adding and subracting the asymmetry because it is
	//just a linear function, i.e. adding background asymmetry will change 
	//the asymmetry exactly as much as subtraction it 

	cout << names[g] << ": " << PT_BINS[b] << "-" << PT_BINS[b + 1] 
	     << " : " << dirAsymm[g][b] << " = ( " << isoAsymm[g][b] << " - "
	     << r_PI0[ arm ][b] << "*" << pi0AsymmErr[g] << " - "
	     << r_ETA[ arm ][b] << "*" << etaAsymmErr[g] 
	     << " ) / (  1 - " << r_PI0[ arm ][b] << " - " << r_ETA[ arm ][b] 
	     << " ); " << endl;
	cout << endl;

      }
  cout << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
       << endl;
  cout << endl;

  //compare averaged asymmetry to averaged asymmetry
  double aveAsymm[ NUM_PT_BINS ], aveAsymmErr[ NUM_PT_BINS ];
  for( int b = 0; b < NUM_PT_BINS; b++ )
    {
      //     0        1        2        3
      //  { "lumiYL","lumiYR","lumiBL","lumiBR" };
      //first need to average left and right and then average blue and yellow
      double lumiY, lumiYErr, lumiB, lumiBErr;
      weightedAverage( dirAsymm[0][b], corrAsymmErr[0][b],
		       dirAsymm[1][b], corrAsymmErr[1][b],
		       lumiY, lumiYErr );

      weightedAverage( dirAsymm[2][b], corrAsymmErr[2][b],
		       dirAsymm[3][b], corrAsymmErr[3][b],
		       lumiB, lumiBErr );

      weightedAverage( lumiY, lumiYErr, lumiB, lumiBErr, 
		       aveAsymm[b], aveAsymmErr[b] );
      cout << lumiY << " +/- " << lumiYErr << ", " 
	   << lumiB << " +/- " << lumiBErr << " : " 
	   << aveAsymm[b]<< " +/- " <<  aveAsymmErr[b] << endl;

    }

  cout << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
       << endl;
  cout << endl;

  //systematic undertainty is how much the overall asymmetry changes when I 
  //plug in a non zero background asymmetry
  cout << " const float bg_SYST_ERROR[ NUM_PT_BINS ] = { ";
  for( int b = 0; b < NUM_PT_BINS; b++ )
    {
      cout << fabs( corrLumi[b] - aveAsymm[b] );
      if( b < NUM_PT_BINS - 1 )
	cout << ", ";
    }
  cout << " }; " << endl;
  //the systematic uncertainty is larger at lower p_T because the background
  //fraction is larger at lower p_T

}
