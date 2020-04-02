#include <iostream>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TString.h"

#include "Constants.h"
#include "findBin.h"

const int NUM_PT_BINS = 4;
const float PT_BINS[ NUM_PT_BINS + 1 ] = { 5, 6, 8, 10, 18 };

const char* PHOTON_TYPE = "incl";//"dp";//

void makeCuratedPhotonFile()
{
  TString treeName = PHOTON_TYPE;
  treeName += "_tree";
  TString outFileName = "curated_";
  outFileName += PHOTON_TYPE;
  outFileName += "_file.root";
  cout << "Saving " << treeName << " in " << outFileName << endl;
  int printNumber = 2;
  if( PHOTON_TYPE == "dp" )        printNumber = 100000;
  else if( PHOTON_TYPE == "incl" ) printNumber = 50000000;

  TFile *fillFile = TFile::Open( "fill.root" );
  TTree *fillTree = (TTree*)fillFile->Get( "fill_tree" );
  int fillNumberFillTree;
  Long64_t triggerCounts[ NUM_XINGS ];
  fillTree->SetBranchAddress( "triggerCounts", triggerCounts );
  fillTree->SetBranchAddress( "fillNumber",    &fillNumberFillTree );

  TFile *dataFile = TFile::Open( "AllRuns.root" );

  int fillNumber, runNumber, arm, eventNumber, xing, spinPattern, sector;
  int scaledERT_4x4c, scaledERT_4x4a, scaledERT_4x4b, ERT[3];
  float energy, px, py, pz;
  TTree *inputTree = (TTree*)dataFile->Get( treeName );
  inputTree->SetBranchAddress( "fillnumber",     &fillNumber );
  inputTree->SetBranchAddress( "run",            &runNumber );
  inputTree->SetBranchAddress( "arm",            &arm );
  inputTree->SetBranchAddress( "event",          &eventNumber );
  inputTree->SetBranchAddress( "xing",           &xing );
  inputTree->SetBranchAddress( "spinpattern",    &spinPattern );
  inputTree->SetBranchAddress( "sector",         &sector );
  inputTree->SetBranchAddress( "energy",         &energy );
  inputTree->SetBranchAddress( "px",             &px );
  inputTree->SetBranchAddress( "py",             &py );
  inputTree->SetBranchAddress( "pz",             &pz );
  inputTree->SetBranchAddress( "ERT",            ERT );
  inputTree->SetBranchAddress( "scaledERT_4x4c", &scaledERT_4x4c );
  inputTree->SetBranchAddress( "scaledERT_4x4a", &scaledERT_4x4a );
  inputTree->SetBranchAddress( "scaledERT_4x4b", &scaledERT_4x4b );

  int eventsRun, ERTBScaleDown, ERTAScaleDown, ERTCScaleDown;
  TTree *eventsTree = (TTree*)dataFile->Get( "events_tree" );
  eventsTree->SetBranchAddress( "run",           &eventsRun );
  eventsTree->SetBranchAddress( "ERTBScaleDown", &ERTBScaleDown );
  eventsTree->SetBranchAddress( "ERTAScaleDown", &ERTAScaleDown );
  eventsTree->SetBranchAddress( "ERTCScaleDown", &ERTCScaleDown );

  TFile *outFile = new TFile( outFileName,  "RECREATE" );
  TTree *newTree = new TTree( treeName, "Curated list of photons " );
  int ptBin;
  newTree->Branch( "fillNumber",     &fillNumber,     "fillNumber/I"  );
  newTree->Branch( "runNumber",      &runNumber,      "runNumber/I" );
  newTree->Branch( "eventNumber",    &eventNumber,    "eventNumber/I" );
  newTree->Branch( "ERT",            ERT,             "ERT[3]/I" );
  newTree->Branch( "scaledERT_4x4c", &scaledERT_4x4c, "scaledERT_4x4c/I" );
  newTree->Branch( "scaledERT_4x4a", &scaledERT_4x4a, "scaledERT_4x4a/I" );
  newTree->Branch( "scaledERT_4x4b", &scaledERT_4x4b, "scaledERT_4x4b/I" );
  newTree->Branch( "ERTBScaleDown",  &ERTBScaleDown,  "ERTBScaleDown/I" );
  newTree->Branch( "ERTAScaleDown",  &ERTAScaleDown,  "ERTAScaleDown/I" );
  newTree->Branch( "ERTCScaleDown",  &ERTCScaleDown,  "ERTCScaleDown/I" );
  newTree->Branch( "xing",           &xing,           "xing/I" );
  newTree->Branch( "arm",            &arm,            "arm/I" );
  newTree->Branch( "sector",         &sector,         "sector/I" );
  newTree->Branch( "spinPattern",    &spinPattern,    "spinPattern/I" );
  newTree->Branch( "energy",         &energy,         "energy/F" );
  newTree->Branch( "px",             &px,             "px/F" );
  newTree->Branch( "py",             &py,             "py/F" );
  newTree->Branch( "pz",             &pz,             "pz/F" );
  newTree->Branch( "ptBin",          &ptBin,          "ptBin/I" );
 
  int numEntries = inputTree->GetEntries();
  cout << "There are " << numEntries << " in the original photon tree " << endl;
  int fillIndex = 0;
  int lastFillNumber = -99;
  int eventsTreeIndex = 0;
  int lastRunNumber = -99;
  for( int i = 0; i < numEntries; i++ )
    {
      inputTree->GetEntry( i );
      if( i%printNumber == 0 )
	cout << "Processed " << i << " photons " << endl;

      //loading scale down info for cross section cross check
      if( runNumber!= lastRunNumber )
	{
	  eventsTree->GetEntry( eventsTreeIndex );
	  while( runNumber != eventsRun )
	    {
	      cout << "Entry " << eventsTreeIndex
		   << " was an events tree dupliate so we'll try the next one"
		   << endl;
	      eventsTreeIndex++;
	      eventsTree->GetEntry( eventsTreeIndex );
	    }
	  eventsTreeIndex++;
	}
      lastRunNumber = runNumber;


      if( fillNumber != lastFillNumber )
	{
	  fillTree->GetEntry( fillIndex );

	  if( fillNumber != fillNumberFillTree )
	    cout << "Panic: Entry " << i << " Loading info about fill " 
		 << fillNumberFillTree << " but looking at data from " 
		 << fillNumber << endl;

	  fillIndex++;
	}
      lastFillNumber = fillNumber;
      if( triggerCounts[ xing ] < 10000 ) 
	continue;

      float pt = sqrt( px*px + py*py );
      ptBin = findBin( NUM_PT_BINS, PT_BINS, pt ) ;
      if( ptBin >= 0 )
	newTree->Fill();

    }

  outFile->cd();
  newTree->Write();
  newTree->Delete();
 
 outFile->Close();
  outFile->Delete();
}
