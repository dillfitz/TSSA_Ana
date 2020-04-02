#include <iostream>
using namespace std;

#include "TFile.h"
#include "TGraphAsymmErrors.h"

#include "../Constants.h"
#include "weightedAverage.h"

//alpha < 0.8
const double rWest[ NUM_VALUE_BINS ] = 
  { 0.780, 0.658, 0.604, 0.567, 0.545, 0.511 };
const double rEast[ NUM_VALUE_BINS ] = 
  { 0.773, 0.651, 0.600, 0.565, 0.547, 0.532 };

const double rRange[ NUM_VALUE_BINS ] = 
  { 0.007, 0.007, 0.005, 0.005, 0.009, 0.001 };

//just to keep track of things in an arryay
const int NUM_THINGS = 3;
const int MINUS = 0;
const int ORIG  = 1;
const int PLUS  = 2;

const int NUM_GRAPHS = 4;
const char* names[ NUM_GRAPHS ] = 
  { "lumiYL","lumiYR","lumiBL","lumiBR"};

void correctBackgroundSystError()
{
  cout << "Subtracting off background asymmetry" << endl;
  TFile *sgFile = 
    TFile::Open( "dataFiles/etaGraphs.root" );
  TGraphAsymmErrors* peakGraphs[ NUM_GRAPHS ];
  double *peak[ NUM_GRAPHS ], *peakErr[ NUM_GRAPHS ];
  for( int i = 0; i < NUM_GRAPHS; i++ )
    {
      peakGraphs[i] = (TGraphAsymmErrors*)sgFile->Get( names[i] );
      peak[i]    = peakGraphs[i]->GetY();
      peakErr[i] = peakGraphs[i]->GetEYhigh();
    }

  TFile *bgFile = 
    TFile::Open( "dataFiles/backgroundGraphs.root" );
  TGraphAsymmErrors* backGraphs[ NUM_GRAPHS ];
  double *back[ NUM_GRAPHS ], *backErr[ NUM_GRAPHS ];
  for( int i = 0; i < NUM_GRAPHS; i++ )
    {
      backGraphs[i] = (TGraphAsymmErrors*)bgFile->Get( names[i] );
      back[i]    = backGraphs[i]->GetY();
      backErr[i] = backGraphs[i]->GetEYhigh();
    }
 
  double r[ NUM_VALUE_BINS ];
  //3: minus range, orig, plus range
  double corrected[ NUM_THINGS ][ NUM_GRAPHS ][ NUM_VALUE_BINS ];
  double correctedErr[ NUM_THINGS ][ NUM_GRAPHS ][ NUM_VALUE_BINS ];
  for( int i = 0; i < NUM_THINGS; i++ )
    for( int graph = 0; graph < NUM_GRAPHS; graph++ )
      for( int ptBin = 0; ptBin < NUM_VALUE_BINS; ptBin++ )
	{
	  //     0        1        2        3
	  //  { "lumiYL","lumiYR","lumiBL","lumiBR" };
	  if( graph == 0 || graph == 3 )//yellow left, blue right
	    r[ ptBin ] = rEast[ ptBin ];
	  else if(  graph == 1 || graph == 2 )//yellow right, blue left
	    r[ ptBin ] = rWest[ ptBin ];

	  if( i == MINUS )
	    r[ ptBin ] = r[ ptBin ] - rRange[ ptBin ];
	  else if( i == PLUS )
	    r[ ptBin ] = r[ ptBin ] + rRange[ ptBin ];

	  corrected[i][ graph ][ ptBin ] = 
	    ( peak[ graph ][ ptBin ] - r[ ptBin ]*back[graph ][ ptBin ] ) 
	    / ( 1 - r[ ptBin ] );
	  
	  correctedErr[i][ graph ][ ptBin ] = 
	    sqrt( peakErr[ graph ][ptBin ]*peakErr[ graph ][ ptBin ]
		  + r[ ptBin ]*r[ ptBin ]
		       *backErr[ graph ][ ptBin ]*backErr[ graph ][ ptBin ] );

	  correctedErr[i][ graph ][ ptBin ] /= 1 - r[ ptBin ];

	  //cout << corrected[i][ graph ][ ptBin ] << endl;

	}

  double asymmetry[ NUM_THINGS ][ NUM_VALUE_BINS ];
  double asymError[ NUM_THINGS ][ NUM_VALUE_BINS ];
  double maxDiff[ NUM_VALUE_BINS ];
  for( int i = 0; i < NUM_THINGS; i++ )
    for( int ptBin = 0; ptBin < NUM_VALUE_BINS; ptBin++ )
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


  for( int ptBin = 0; ptBin < NUM_VALUE_BINS; ptBin++ )
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
  cout << "const double rError[ numPTBin ] = { ";
  for( int i = 0; i < NUM_VALUE_BINS; i++ )
    {
      cout << maxDiff[ i ];
      if( i < NUM_VALUE_BINS - 1 )
	cout << ", ";
    }
  cout << " };" << endl;

}
