//This code is currently structured to calculated the asymmetry for only one 
//fill at a time
#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;

#include "../../../Constants.h"

#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TString.h"

int getRandom( const int min, const int max );

const int NUM_FILLS = 140;  // should be 142 here right? Find where it is used!
const int NUM_POLARIZED = 111; // what is this number for?
const int histoNumBins = 100;
const float histoMin = -5;
const float histoMax = +5;
//float SQRT_ACC_CORR[NUM_VALUE_BINS] = {0.889585, 0.891166, 0.893838, 0.896951, 0.901238};
float SQRT_ACC_CORR[NUM_VALUE_BINS];   


const char *inputTreeNameInFile = "e_svx_tree";

const int multiply = 111;

// Use this for the open heavy flavor electron measurement //
int asymmetryBunchShuffle( float seed = 2, 
			   const int NUM_SHUFFLES = 10000,
			   const char* outputFileName = "shuffled_ohfe.root")

{


  const char *inputDataFileName = "../../../curated_ohfe.root";
  gSystem->Load( "libppAsymmetry.so" );
  seed = seed * multiply;

  //  gSystem->ListLibraries();

  cout << "Doing " << NUM_SHUFFLES << " bunch shuffles at a time to file " 
       << outputFileName << endl;
  TFile *outFile = new TFile( outputFileName, "RECREATE");
      TH1F *bunchShuffled[ NUM_VALUE_BINS ];
  for( int i = 0; i < NUM_VALUE_BINS; i++ )
    {
	  if (NUM_VALUE_BINS == 6)
      {
      	SQRT_ACC_CORR[0] = 0.884769;
      	SQRT_ACC_CORR[1] = 0.89479;
      	SQRT_ACC_CORR[2] = 0.899258;
      	SQRT_ACC_CORR[3] = 0.901838;
      	SQRT_ACC_CORR[4] = 0.907891;
      	SQRT_ACC_CORR[5] = 0.911921;
      	cout << "acc corr val " << i << " " << SQRT_ACC_CORR[i] << endl;
	  }
	  else if (NUM_VALUE_BINS == 4)
      {
      	SQRT_ACC_CORR[0] = 0.899258;
      	SQRT_ACC_CORR[1] = 0.901838;
      	SQRT_ACC_CORR[2] = 0.907891;
      	SQRT_ACC_CORR[3] = 0.911921;
      	cout << "acc corr val " << i << " " << SQRT_ACC_CORR[i] << endl;
	  }

      ostringstream name;
      name << "shuffled" << i;
      cout << name.str().c_str() << endl;

      ostringstream title;
      title << "Shuffle Seed for " << VALUE_BINS[ i ] << "to" << VALUE_BINS[ i + 1 ] << " GeV; A_{N}/#sigma_{A}; Entries";
      cout << title.str().c_str() << endl;
/*
      TString name = "shuffled";
      name += i;
      TString title = "Shuffle Seed ";
      title += seed;
      title += " for ";
      title += VALUE_BINS[i];
      title += " < p_{T} < ";
      title += VALUE_BINS[i + 1];
      title += " GeV; A_{N}/#sigma_{A}; Entries";
*/
      bunchShuffled[i] = new TH1F( name.str().c_str(), title.str().c_str(), 
				   histoNumBins, histoMin, histoMax );
    }

  TFile *fillFile = TFile::Open("../../../fill.root");
  TTree* fillTree = (TTree*)fillFile->Get("fill_tree");
  int fillNumberFillTree;
  float polB, polErrB, polY, polErrY;
  Long64_t countsUpB, countsDownB, countsUpY, countsDownY;
  fillTree->SetBranchAddress( "fillNumber",       &fillNumberFillTree );
  fillTree->SetBranchAddress( "polarizationB",    &polB );
  fillTree->SetBranchAddress( "polarizationErrB", &polErrB );
  fillTree->SetBranchAddress( "polarizationY",    &polY );
  fillTree->SetBranchAddress( "polarizationErrY", &polErrY );
  fillTree->SetBranchAddress( "countsUpB",        &countsUpB );
  fillTree->SetBranchAddress( "countsDownB",      &countsDownB );
  fillTree->SetBranchAddress( "countsUpY",        &countsUpY );
  fillTree->SetBranchAddress( "countsDownY",      &countsDownY );
  
  int numFillEntries = fillTree->GetEntries();
  if( numFillEntries != NUM_FILL_BINS * NUM_FILLS_IN_GROUP )
    cout << "Panic! There are the wrong amonut of entries in fill tree " 
	 << endl;

  int numFills = fillTree->GetEntries();
  Polarization polarizationY, polarizationB;
  int fillIndexInit = 0;
  int fillBinInit = -1;
  for( int i = 0; i < numFills; i++ )
    {
      fillTree->GetEntry(i);
      if( i%NUM_FILLS_IN_GROUP == 0 )
	{
	  fillBinInit++;
	  fillIndexInit = 0;
	}
      double d_upY   = (double)countsUpY;
      double d_downY = (double)countsDownY;
      double d_upB   = (double)countsUpB;
      double d_downB = (double)countsDownB;
      polarizationY.updatePolarization( fillBinInit, fillIndexInit, 
					polY, polErrY, d_upY + d_downY );
      polarizationB.updatePolarization( fillBinInit, fillIndexInit, 
					polB, polErrB, d_upB + d_downB );
       fillIndexInit++;
    }

  //load all the polarization and relative luminosity info
  Asymmetry asymmetry[ NUM_SHUFFLES ];
  for( int f = 0; f < NUM_FILL_BINS; f++ )
    {
      float avePolY, avePolErrY, avePolB, avePolErrB;
      polarizationY.averagePolarization( f, avePolY, avePolErrY );
      polarizationB.averagePolarization( f, avePolB, avePolErrB );

      for( int s = 0; s < NUM_SHUFFLES; s++ )
	{
	  asymmetry[s].setPolarization( YELLOW, f, polY, polErrY );
	  asymmetry[s].setPolarization( BLUE,   f, polB, polErrB );

	  asymmetry[s].setAccCorrection( SQRT_ACC_CORR );
	}
    }

 
  TFile *dataFile = TFile::Open( inputDataFileName );
  TTree *dataTree = (TTree*)dataFile->Get( inputTreeNameInFile );
  cout << "Loading tree " << inputTreeNameInFile << " from file "
       << inputDataFileName << endl;
  int fillNumber, arm, xing, ptBin, charge;
  dataTree->SetBranchAddress( "fillNumber", &fillNumber );
  dataTree->SetBranchAddress( "arm",        &arm );
  dataTree->SetBranchAddress( "charge",      &charge );
  dataTree->SetBranchAddress( "xing",       &xing );
  dataTree->SetBranchAddress( "ptBin",      &ptBin );
  int numEntries = dataTree->GetEntries();
  cout << "There are " << numEntries << " in tree " << endl;

  cout << "Using seed value " << seed  << endl;
  srand( seed );
  //  TRandom rando( seedValue );
  int shuffledSpin[ NUM_FILLS ][ NUM_XINGS ][ NUM_SHUFFLES ];
  for( int f = 0; f < NUM_FILLS; f++ )
    for( int sh = 0; sh < NUM_SHUFFLES; sh++ )
      for( int xing = 0; xing < NUM_XINGS; xing++ )
	{
	  if( xing > NUM_POLARIZED )
	    shuffledSpin[f][ xing ][ sh ] = -99;
	  else
	    shuffledSpin[f][ xing ][ sh ] = getRandom(0,3);
	  //	      = rando.Integer( 4 );
	}

  int lastFillNumber = 0;
  int fillBin = 0;
  int fillIndex = 0;
  for( int treeEntry = 0; treeEntry < numEntries; treeEntry++ )
    {
      dataTree->GetEntry( treeEntry );
      if( treeEntry % 100000 == 0 ) 
	cout << "Processed " << treeEntry << " electrons in tree" << endl;

      if( fillNumber != lastFillNumber )
	{
	  fillBin = fillIndex / NUM_FILLS_IN_GROUP;
	  //cout << "Fill " << fillNumber << " fill index " << fillIndex
	  //   << " fill bin " << fillBin << endl;
	  fillIndex++;
	}
      lastFillNumber = fillNumber;

      if( xing >= 0 && xing <= NUM_POLARIZED )
	//only doing poliarizated xings
	for( int s = 0; s < NUM_SHUFFLES; s++ )
	  {
	    int spinPatt = shuffledSpin[ fillBin ][ xing ][s];
	    asymmetry[s].incrementCounts( fillBin, ptBin, arm, spinPatt );
	  }
      
    }//end looping through etas

  for( int s = 0; s < NUM_SHUFFLES; s++ )
    {
      for( int beam = 0; beam < NUM_BEAMS; beam++ )
	for( int f = 0; f < NUM_FILL_BINS; f++ )
	  asymmetry[s].calculateAsymmetry( beam, f, SQUARE_ROOT );

      float asArray[ NUM_VALUE_BINS ], erArray[ NUM_VALUE_BINS ];
	  
      asymmetry[s].averageOverBeamsAndFills( asArray, erArray );
      for( int ptBin = 0; ptBin < NUM_VALUE_BINS; ptBin++ )
	{
	  float value = asArray[ ptBin ] / erArray[ ptBin ];
	  bunchShuffled[ ptBin ]->Fill( value );
	}//end looping through pt bins
    }//end looping through shuffles

  outFile->cd();
  for( int i = 0; i < NUM_VALUE_BINS; i++ )
    {
      bunchShuffled[i]->Write();
      bunchShuffled[i]->Delete();
    }

  outFile->Close();
  outFile->Delete();

  asymmetry[0].printCounts();

  cout << endl;
  asymmetry[2].printCounts();

  return 0;
}

int getRandom( const int min, const int max )
{
  int uniRand;
  uniRand = rand() % ((max + 1) - min) + min;
  return (uniRand);
}
