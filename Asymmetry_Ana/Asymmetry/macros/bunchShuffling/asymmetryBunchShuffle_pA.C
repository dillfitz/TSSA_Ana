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

const int NUM_FILLS = NUM_FILL_BINS * NUM_FILLS_IN_GROUP;
const int NUM_POLARIZED = 111;
const int histoNumBins = 100;
const float histoMin = -5;
const float histoMax = +5;
//float SQRT_ACC_CORR[NUM_VALUE_BINS];   

const int multiply = 111;

// Use this for the open heavy flavor electron measurement //
int asymmetryBunchShuffle_pA( float seed = 2, const int NUM_SHUFFLES = 10000, const char* particle = "pi0", const char* collisionSystem = "pAl")
{
  TString outputFileName = "shuffled_";
  outputFileName += particle;
  outputFileName += "_";
  outputFileName += collisionSystem;
  //outputFileName += "_";
  //outputFileName += seed;
  outputFileName += ".root";
  
  TString inputTreeNameInFile = particle;
  inputTreeNameInFile += "_tree";
   
  if (particle == "pi0" && collisionSystem == "pAu")
  {
    float SQRT_ACC_CORR[NUM_VALUE_BINS] = {0.841251, 0.867366, 0.870088, 0.875006, 0.878855, 0.880475, 0.882042, 0.881463, 0.88339, 0.890346};
  }  
  else if (particle == "pi0" && collisionSystem == "pAl")
  {
    float SQRT_ACC_CORR[NUM_VALUE_BINS] = {0.830103, 0.852208, 0.864715, 0.870581, 0.874193, 0.875093, 0.87718, 0.877952, 0.879643, 0.885372};  
  }  
  else if (particle == "eta" && collisionSystem == "pAu")
  {
    float SQRT_ACC_CORR[NUM_VALUE_BINS] = {0.877714, 0.888074, 0.890006, 0.889938, 0.889817, 0.889482, 0.888538, 0.888311};    
  }    
  else if (particle == "eta" && collisionSystem == "pAl")
  {
    float SQRT_ACC_CORR[NUM_VALUE_BINS] = {0.87136, 0.883857, 0.887107, 0.885778, 0.885864, 0.884948, 0.883384, 0.883265};      
  }
    
  TString inputDataFileName = "../../../curated_";
  inputDataFileName += particle;
  inputDataFileName += "_";
  inputDataFileName += collisionSystem;
  inputDataFileName += ".root";
  
  gSystem->Load( "libppAsymmetry.so" );
  seed = seed * multiply;

  //gSystem->ListLibraries();

  cout << "Doing " << NUM_SHUFFLES << " bunch shuffles at a time to file " << outputFileName << endl;
  TFile *outFile = new TFile( outputFileName, "RECREATE");
  TH1F *bunchShuffled[ NUM_VALUE_BINS ];
  for( int i = 0; i < NUM_VALUE_BINS; i++ )
  {
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
    bunchShuffled[i] = new TH1F( name.str().c_str(), title.str().c_str(), histoNumBins, histoMin, histoMax );
  }
  
  TString fillFileName = "../../../fill_";
  fillFileName += collisionSystem;
  fillFileName += ".root";
  TFile *fillFile = TFile::Open(fillFileName);
  
  TTree* fillTree = (TTree*)fillFile->Get("fill_tree");
  int fillNumberFillTree;
  float polB, polErrB, polY, polErrY;
  Long64_t countsUpB, countsDownB, countsUpY, countsDownY;
  fillTree->SetBranchAddress( "fillNumber",       &fillNumberFillTree );
  fillTree->SetBranchAddress( "polarizationB",    &polB );
  fillTree->SetBranchAddress( "polarizationErrB", &polErrB );
  fillTree->SetBranchAddress( "countsUpB",        &countsUpB );
  fillTree->SetBranchAddress( "countsDownB",      &countsDownB );

  
  int numFillEntries = fillTree->GetEntries();
  cout << "numFillEntries : " << numFillEntries << endl;
  cout << "num fill bins : " << NUM_FILL_BINS << endl;
  cout <<"num fills in group : " << NUM_FILLS_IN_GROUP << endl;
  if( numFillEntries != NUM_FILL_BINS * NUM_FILLS_IN_GROUP )
    cout << "Panic! There are the wrong amonut of entries in fill tree " << endl;

  int numFills = fillTree->GetEntries();
  Polarization polarizationB;
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
    double d_upB   = (double)countsUpB;
    double d_downB = (double)countsDownB;
    polarizationB.updatePolarization( fillBinInit, fillIndexInit, polB, polErrB, d_upB + d_downB );
    fillIndexInit++;
  }

  //load all the polarization and relative luminosity info
  Asymmetry asymmetry[ NUM_SHUFFLES ];
  for( int f = 0; f < NUM_FILL_BINS; f++ )
  {
    float avePolB, avePolErrB;
    polarizationB.averagePolarization( f, avePolB, avePolErrB );

    for( int s = 0; s < NUM_SHUFFLES; s++ )
	  {
	    asymmetry[s].setPolarization( BLUE, f, polB, polErrB );
	    asymmetry[s].setAccCorrection( SQRT_ACC_CORR );
 	  }
  }

  TFile *dataFile = TFile::Open( inputDataFileName );
  TTree *dataTree = (TTree*)dataFile->Get( inputTreeNameInFile );
  cout << "Loading tree " << inputTreeNameInFile << " from file " << inputDataFileName << endl;
  int fillNumber, arm, xing, ptBin;
  dataTree->SetBranchAddress( "fillNumber", &fillNumber );
  dataTree->SetBranchAddress( "arm",        &arm );
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
	        shuffledSpin[f][ xing ][ sh ] = getRandom(0,1);
	        if (shuffledSpin[f][ xing ][ sh ] == 0)
	          shuffledSpin[f][ xing ][ sh ] = -1;
	}

  int lastFillNumber = 0;
  int fillBin = 0;
  int fillIndex = 0;
  for( int treeEntry = 0; treeEntry < numEntries; treeEntry++ )
  {
    dataTree->GetEntry( treeEntry );
    if( treeEntry % 100000 == 0 ) 
	    cout << "Processed " << treeEntry << " entries in data tree" << endl;

    if( fillNumber != lastFillNumber )
	  {
	    fillBin = fillIndex / NUM_FILLS_IN_GROUP;
	    //cout << "Fill " << fillNumber << " fill index " << fillIndex << " fill bin " << fillBin << endl;
	    fillIndex++;
	  }
    lastFillNumber = fillNumber;

    if( xing >= 0 && xing <= NUM_POLARIZED )
	  //only doing poliarizated xings
	  for( int s = 0; s < NUM_SHUFFLES; s++ )
	  {
	    int spinPatt = shuffledSpin[ fillBin ][ xing ][s];
	    asymmetry[s].incrementCounts_pA( fillBin, ptBin, arm, spinPatt );
	  }
      
  }//end looping through particles

  for( int s = 0; s < NUM_SHUFFLES; s++ )
  {
    for( int beam = 0; beam < NUM_BEAMS; beam++ )
    {
      if (beam == YELLOW) {continue;}
	    for( int f = 0; f < NUM_FILL_BINS; f++ )
	      asymmetry[s].calculateAsymmetry( beam, f, SQUARE_ROOT );
	  }

    float asArray[ NUM_VALUE_BINS ], erArray[ NUM_VALUE_BINS ];
	  
    asymmetry[s].averageOverFills( BLUE, asArray, erArray );
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
  asymmetry[0].printCounts(true);

  cout << endl;
  asymmetry[2].printCounts(true);

  return 0;
}

int getRandom( const int min, const int max )
{
  int uniRand;
  uniRand = rand() % ((max + 1) - min) + min;
  return (uniRand);
}
