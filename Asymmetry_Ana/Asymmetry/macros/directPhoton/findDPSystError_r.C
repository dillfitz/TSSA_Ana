//Calculates the systematic uncertainty on the asymmetry associated with the 
//uncertainty on the background fraction.  Calculculates how much the relative 
//luminosity asymmetry changes when the background fraction is changed. 
//This macro just spits out numbers, no output graphs here

//The uncertainty on the background fractions came from (1)statisical 
//uncertainty in how the background fraction was calculated and (2) a VERY small
//uncertainty applied from pi0 merging

#include <iostream>
using namespace std;

#include "TFile.h"
#include "TGraphAsymmErrors.h"

#include "../../Constants.h"
#include "../plotMacros/weightedAverage.h"
const int NUM_PT_BINS = 4;

const float r_PI0[ NUM_ARMS ][ NUM_PT_BINS ] = 
  {
    { 0.384, 0.320, 0.225, 0.124 },//West Arm
    { 0.409, 0.343, 0.257, 0.137 },//East Arm
  };
const float r_PI0_ERR[ NUM_ARMS ][ NUM_PT_BINS ] = 
  {
    { 0.00296, 0.0042, 0.00853, 0.0099 },//West Arm
    { 0.00354, 0.00498, 0.0111, 0.0124 } //East Arm
  };
const float r_ETA[ NUM_ARMS ][ NUM_PT_BINS ] = 
  {
    { 0.0853, 0.0729, 0.0548, 0.0346 },//West Arm
    { 0.0873, 0.0781, 0.0632, 0.0458 } //East Arm
  };
const float r_ETA_ERR[ NUM_ARMS ][ NUM_PT_BINS ] = 
  {
    { 0.00124, 0.00176, 0.00367, 0.00457 },//West Arm
    { 0.00146, 0.00213, 0.00465, 0.00666 } //East Arm
  };

const float r_MERGE_ERR[ NUM_ARMS ][ NUM_PT_BINS ] = 
  {
    { 0.000210335, 9.84012e-05, 0.000631089, 0.00119244 }, //West Arm
    { 0.000130292, 0.000186516, 0.000540118, 0.00161044 }  //East Arm
  };

//indices to keep track of things in arrays
const int NUM_THINGS = 3;
const int MINUS = 0;
const int ORIG  = 1;
const int PLUS  = 2;

const int NUM_GRAPHS = 4;
const char* names[ NUM_GRAPHS ] = 
  { "lumiYL","lumiYR","lumiBL","lumiBR"};

void findDPSystError_r()
{
  TFile *isoFile = TFile::Open( "../dataFiles/graphs_dp.root" );
  double *iso[ NUM_GRAPHS ];
  double *isoErr[ NUM_GRAPHS ];
  for( int i = 0; i < NUM_GRAPHS; i++ )
    {
      TGraphAsymmErrors* isoGraph 
	= (TGraphAsymmErrors*)isoFile->Get( names[i] );
      iso[i]    = isoGraph->GetY();
      isoErr[i] = isoGraph->GetEYlow();
    }

  //3: minus range, orig, plus range
  float corrected[ NUM_THINGS ][ NUM_GRAPHS ][ NUM_PT_BINS ];
  float correctedErr[ NUM_THINGS ][ NUM_GRAPHS ][ NUM_PT_BINS ];
  for( int i = 0; i < NUM_THINGS; i++ )  //3: minus range, orig, plus range
    for( int graph = 0; graph < NUM_GRAPHS; graph++ )
      for( int ptBin = 0; ptBin < NUM_PT_BINS; ptBin++ )
	{
	  //     0        1        2        3
	  //  { "lumiYL","lumiYR","lumiBL","lumiBR" };
	  int arm;
	  if( graph == 0 || graph == 3 )//yellow left, blue right
	    arm = EAST;
	  else if(  graph == 1 || graph == 2 )//yellow right, blue left
	    arm = WEST;

	  float r = r_PI0[ arm ][ ptBin ] + r_ETA[ arm ][ ptBin ];
	  float sigma_r = r_PI0_ERR[ arm ][ ptBin ] + r_ETA_ERR[ arm ][ ptBin ]
	                    + r_MERGE_ERR[ arm ][ ptBin ];
	  if( i == MINUS )
	    r = r - sigma_r;
	  else if( i == PLUS )
	    r = r + sigma_r;

	  //setting bacgkround asymmetry to zero so in practice just rescaling
	  //the asymmetry by the bacgkround fraction
	  corrected[i][ graph ][ ptBin ] =  iso[ graph ][ ptBin ] / ( 1 - r );
	  
	  correctedErr[i][ graph ][ ptBin ] = 
	    isoErr[ graph ][ ptBin ] / ( 1 - r );

	}

  double asymmetry[ NUM_THINGS ][ NUM_PT_BINS ];
  double asymError[ NUM_THINGS ][ NUM_PT_BINS ];
  double maxDiff[ NUM_PT_BINS ];
  for( int i = 0; i < NUM_THINGS; i++ )
    for( int ptBin = 0; ptBin < NUM_PT_BINS; ptBin++ )
      {
	//     0        1        2        3
	//  { "lumiYL","lumiYR","lumiBL","lumiBR" };

	//first need to average left and right and then average blue and yellow
	double lumiY, lumiYErr, lumiB, lumiBErr;
	weightedAverage( corrected[i][0][ ptBin ], correctedErr[i][0][ ptBin ],
			 corrected[i][1][ ptBin ], correctedErr[i][1][ ptBin ],
			 lumiY, lumiYErr );

	weightedAverage( corrected[i][2][ ptBin ], correctedErr[i][2][ ptBin ],
			 corrected[i][3][ ptBin ], correctedErr[i][3][ ptBin ],
			 lumiB, lumiBErr );
 
	weightedAverage( lumiY, lumiYErr, lumiB, lumiBErr, 
			 asymmetry[i][ ptBin ], asymError[i][ ptBin ] );

	if( i == ORIG )
	  cout << lumiY << " +/- " << lumiYErr << ", " 
	       << lumiB << " +/- " << lumiBErr << " : " 
	       << asymmetry[i][ ptBin ] << " +/ - " << asymError[i][ ptBin ]
	       << endl;

      }


  for( int ptBin = 0; ptBin < NUM_PT_BINS; ptBin++ )
    {
      double minusDiff = fabs( asymmetry[ MINUS ][ ptBin ] 
			       - asymmetry[ ORIG ][ ptBin ] );
      double plusDiff  = fabs( asymmetry[ PLUS ][ ptBin ] 
			       - asymmetry[ ORIG ][ ptBin ] );

      if( minusDiff > plusDiff )
	maxDiff[ ptBin ] = minusDiff;
      else
	maxDiff[ ptBin ] = plusDiff;

      cout << ptBin << ": " << "max diff " << maxDiff[ ptBin ] << " for: " 
	   << asymmetry[ MINUS ][ ptBin ] << " " << asymmetry[ ORIG ][ ptBin ] 
	   << " " << asymmetry[ PLUS ][ ptBin ] << endl;
    }

  cout << endl;
  cout << "Or to print it more prettiful: " << endl;
  cout << "const float r_SYST_ERROR[ NUM_PT_BINS ] = { ";
  for( int i = 0; i < NUM_PT_BINS; i++ )
    {
      cout << maxDiff[ i ];
      if( i < NUM_PT_BINS - 1 )
	cout << ", ";
    }
  cout << " };" << endl;

}
