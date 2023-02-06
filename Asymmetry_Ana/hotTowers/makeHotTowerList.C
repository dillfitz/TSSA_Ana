#include <iostream>
#include <fstream>
using namespace std;

#include "TFile.h"
#include "TH2.h"
#include "TString.h"

//const char* inputFileName =  "low0.txt";//high0.txt";

const int NUM_SECTORS = 8;
const int NUM_PBSC = 6;
const int NUM_PBGL = 2;
const int PBSC_WIDTH  = 36;
const int PBSC_HEIGHT = 72;
const int PBGL_WIDTH  = 48;
const int PBGL_HEIGHT = 96;

const bool doingLowEnergy = false;
 
void makeHotTowerList(const char* system = "pAl")
{
  int hotMapPBSC[ NUM_PBSC ][ PBSC_WIDTH ][ PBSC_HEIGHT ] = {0};
  int hotMapPBGL[ NUM_PBGL ][ PBGL_WIDTH ][ PBGL_HEIGHT ] = {0};

  //TString inputFileName =  "low4_";
  //TString inputFileName =  "high2_"; 
  // for a check... //
  //TString inputFileName = "lowHotTowers_";
  TString inputFileName = "highHotTowers_";  
  inputFileName += system;
  inputFileName += ".txt";  
  //TString outputFileName = "low5_";
  TString outputFileName = "temp_";  
  //TString outputFileName = "high3_";
  outputFileName += system;
  outputFileName += ".txt";
  //load old files
  ifstream inputFile;
  int sector, ypos, zpos;
  cout << "Oldies: " << endl;
  inputFile.open( inputFileName );
  if( inputFile.is_open() )
  {
    while( !inputFile.eof() )
    {
	    inputFile >> sector >> ypos >> zpos;
	    if( sector < 6 && ypos > -1 && zpos > -1 && ypos < PBSC_WIDTH && zpos < PBSC_HEIGHT )
	      hotMapPBSC[ sector ][ ypos ][ zpos ] = 1;
	    else if( sector > 5 && ypos > -1 && zpos > -1 && ypos < PBGL_WIDTH && zpos < PBGL_HEIGHT )
	      hotMapPBGL[ sector - 6 ][ ypos ][ zpos ] = 1;
	    
	    cout << sector << " " << ypos << " " << zpos << endl;
    }
  }  
  else
    cout << "Couldn't find file " << inputFileName << endl;

  TString dataFileName = system;
  dataFileName += "_goodruns_ana664.root";
  //load histos
  //TFile *dataFile = TFile::Open( "/phenix/spin/phnxsp01/nialewis/taxi/Run15pp200CAnoVTXERTPro104/14712/data/AllRuns.root" );
  TFile *dataFile = TFile::Open( dataFileName );
  TH2I *histos[ NUM_SECTORS ];
  for( int sec = 0; sec < NUM_SECTORS; sec++ )
  { 
    TString name = "hotSector";
    name += sec;
    /*
    if( doingLowEnergy )
	    name += "LowAfter";
    else
	    name += "HighAfter";
    */
    if( doingLowEnergy )
	    name += "LowBefore";
    else
	    name += "HighBefore";
      
    histos[ sec ] = (TH2I*)dataFile->Get( name );
  }

  //calculate RMS
  double rms[ NUM_SECTORS ];
  for( int sec = 0; sec < NUM_SECTORS; sec++ )
  {
    int numY, numZ;
    if( sec < 6 )
	  {
	    numY = PBSC_WIDTH;
	    numZ = PBSC_HEIGHT;
 	  }
    else
	  {
	    numY = PBGL_WIDTH;
	    numZ = PBGL_HEIGHT;
	  }

    int numTowersCounted = 0;
    double sumCountsSquared = 0;

    for( int y = 0; y < numY; y++ )
    {
	    for( int z = 0; z < numZ; z++ )
	    {
	      //making sure not already flagged tower
	      if( ( sec < 6 && hotMapPBSC[ sec ][y][z] == 0 ) || ( sec > 5 && hotMapPBGL[ sec - 6 ][y][z] == 0 ) )
	      { 
	        double counts = histos[ sec ]->GetBinContent( y + 1, z + 1 );
	        sumCountsSquared += counts*counts;
	        if( counts != 0 )
		      numTowersCounted++;
	      } 
	    }
	  }

    rms[ sec ] = sqrt( sumCountsSquared / (double)(numTowersCounted) );
    cout << sec << ": " << rms[ sec ] << " = sqrt ( " << sumCountsSquared << " / " << numTowersCounted << " ) " << endl;
  }

  ofstream outputFile( outputFileName );
  cout << "Newbies: " << endl;
  for( int sec = 0; sec < NUM_SECTORS; sec++ )
  { 
    int numY, numZ;
    if( sec < 6 )
	  {
	    numY = PBSC_WIDTH;
	    numZ = PBSC_HEIGHT;
	  }
    else
	  {
	    numY = PBGL_WIDTH;
	    numZ = PBGL_HEIGHT;
	  }
	  
    for( int y = 0; y < numY; y++ )
    {
	    for( int z = 0; z < numZ; z++ )
	    {
	      double counts = histos[ sec ]->GetBinContent( y + 1, z + 1 );
	      if( counts >= (6 * rms[ sec ]) )
	      {
		      outputFile << sec << " " << y << " " << z << endl;
		      /*
		      for( int i = -1; i <= 1; i++ )
		      {
		        for( int j = -1; j <= 1; j++ )
		        {
		          outputFile << sec << " " << y  + i << " " << z  + j << endl;
		        }
		          outputFile << endl;
		      }
		      */
		      if( ( sec < 6 && hotMapPBSC[ sec ][y][z] == 0 ) || ( sec > 5 && hotMapPBGL[ sec - 6 ][y][z] == 0 ) )
		        cout << sec << " " << y << " " << z << endl;
	      }
	    }
    }
  }
}
