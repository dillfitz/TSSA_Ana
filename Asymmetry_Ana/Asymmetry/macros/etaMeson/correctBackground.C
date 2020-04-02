#include <iostream>
#include <fstream>
using namespace std;

#include "TFile.h"
#include "TGraphAsymmErrors.h"

#include "../Constants.h"

const char* outputFileName = "dataFiles/ResultsCorrected.h";


//alpha < 0.8
const double rWest[ NUM_VALUE_BINS ] = 
  { 0.780, 0.658, 0.604, 0.567, 0.545, 0.511 };
const double rEast[ NUM_VALUE_BINS ] = 
  { 0.773, 0.651, 0.600, 0.565, 0.547, 0.532 };
const double rSqrt[ NUM_VALUE_BINS ] = 
  { 0.777, 0.655, 0.602, 0.566, 0.546, 0.521 };

const int NUM_GRAPHS = 6;
const char* names[ NUM_GRAPHS ] = 
  { "lumiYL","lumiYR","lumiBL","lumiBR", "sqrtY","sqrtB" };

void correctBackground()
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
  double corrected[ NUM_GRAPHS ][ NUM_VALUE_BINS ];
  double correctedErr[ NUM_GRAPHS ][ NUM_VALUE_BINS ];
  for( int i = 0; i < NUM_GRAPHS; i++ )
    for( int j = 0; j < NUM_VALUE_BINS; j++ )
      {
	//     0        1        2        3        4        5
	//  { "lumiYL","lumiYR","lumiBL","lumiBR", "sqrtY","sqrtB" };
	if( i == 0 || i == 3 )//yellow left, blue right
	  r[j] = rEast[j];
	else if(  i == 1 || i == 2 )//yellow right, blue left
	  r[j] = rWest[j];
	else if(  i == 4 || i == 5 )//sqrt
	  r[j] = rSqrt[j];

	corrected[i][j] = ( peak[i][j] - r[j]*back[i][j] ) / ( 1 - r[j] );

	correctedErr[i][j] = sqrt( peakErr[i][j]*peakErr[i][j]
				   + r[j]*r[j]*backErr[i][j]*backErr[i][j] );
	correctedErr[i][j] /= 1 - r[j];
      }

  ofstream outFile;
  outFile.open( outputFileName );
  for( int graph = 0; graph < NUM_GRAPHS; graph++ )
    {
      outFile << "const float " << names[ graph ] << "[ NUM_VALUE_BINS ] = " 
	      << endl << "{ ";
      for( int ptBin = 0; ptBin < NUM_VALUE_BINS; ptBin++ )
	{
	  outFile << corrected[ graph ][ ptBin ];
		if( ptBin < NUM_VALUE_BINS - 1 ) 
		  outFile << ", ";
	}
      outFile << " };" << endl;

      outFile << "const float " << names[ graph ] << "Err[ NUM_VALUE_BINS ] = " 
	      << endl << "{ ";
      for( int ptBin = 0; ptBin < NUM_VALUE_BINS; ptBin++ )
	{
	  outFile << correctedErr[ graph ][ ptBin ];
		if( ptBin < NUM_VALUE_BINS - 1 ) 
		  outFile << ", ";
	}
      outFile << " };" << endl << endl;
    }

 
}

