#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TTree.h"

#include "Constants.h"
#include "findBin.h"

/**/
//etas
const float MIN_MASS = 0.2;
const float MAX_MASS = 0.9;
const int NUM_BINS = 70;
const float MIN_SIGNAL_MASS = 0.480;
const float MAX_SIGNAL_MASS = 0.620;
const float MIN_BG_MASS_1 = 0.300;
const float MAX_BG_MASS_1 = 0.400;
const float MIN_BG_MASS_2 = 0.700;
const float MAX_BG_MASS_2 = 0.800;
const int NUM_PT_BINS = 8;
const float PT_BINS[ NUM_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 10, 20 };
const int NUM_BG_PT_BINS = 7;
const float BG_PT_BINS[ NUM_BG_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 20 };
//const int NUM_PT_BINS = 6;
//const float PT_BINS[ NUM_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 8, 15 };
const char* particle = "eta";
/*
//pi0s
const float MIN_MASS = 0;
const float MAX_MASS = 0.300;
const int NUM_BINS = 300;
const float MIN_SIGNAL_MASS = 0.112;
const float MAX_SIGNAL_MASS = 0.162;
const float MIN_BG_MASS_1 = 0.047;
const float MAX_BG_MASS_1 = 0.097;
const float MIN_BG_MASS_2 = 0.177;
const float MAX_BG_MASS_2 = 0.227;
const int NUM_PT_BINS = 10;
const float PT_BINS[ NUM_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 20 };
const int NUM_BG_PT_BINS = 8;
const float BG_PT_BINS[ NUM_BG_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 9, 20 };
const char* particle = "pi0";
/**/

void makeCuratedPairFile_pAl()
{
  TH1F *histos[ NUM_ARMS ][ NUM_PT_BINS ];
  for( int arm = 0; arm < NUM_ARMS; arm++ )
    for( int i = 0; i < NUM_PT_BINS; i++ )
      {
	ostringstream histoName;
	histoName << "invmass";
	if( arm == 0 ) 
	  histoName << "West";
	else if( arm == 1 )
	  histoName << "East";
	histoName << PT_BINS[i] << "to" << PT_BINS[i + 1];

	ostringstream histoTitle;
	histoTitle << "Invariant Mass of Photon Pairs in ";
	if( arm == 0 ) 
	  histoTitle << "West";
	else if( arm == 1 )
	  histoTitle << "East";
	histoTitle << " Arm " << PT_BINS[i] << " < p_{T} < " 
		   << PT_BINS[i + 1] << " [GeV]; M_{#gamma#gamma} [GeV];";

	cout << histoName.str() << ",  "<< histoTitle.str() << endl;
	histos[ arm ][i] = new TH1F( histoName.str().c_str(), 
				     histoTitle.str().c_str(), 
				     NUM_BINS, MIN_MASS, MAX_MASS );
      }
  cout << endl;

  TFile *fillFile = TFile::Open( "fill_pAl.root" );
  TTree *fillTree = (TTree*)fillFile->Get( "fill_tree" );
  int fillNumberFillTree;
  Long64_t triggerCounts[ NUM_XINGS ];

  fillTree->SetBranchAddress( "fillNumber",    &fillNumberFillTree );
  fillTree->SetBranchAddress( "triggerCounts", triggerCounts );

  TFile *inputFile = TFile::Open( "../pAl_goodruns_ana664.root" );
  TTree* pairTree = (TTree*)inputFile->Get( "pair_tree" );
  int fillNumber, runNumber, eventNumber, arm, xing, spin, sector1, sector2;
  float px1, py1, pz1, px2, py2, pz2, energy1, energy2, invmass;
  int ERT1[3], ERT2[3], scaledERT_4x4a, scaledERT_4x4b, scaledERT_4x4c;
  int blueSpinDir;
  pairTree->SetBranchAddress( "fillnumber",     &fillNumber );
  pairTree->SetBranchAddress( "run",            &runNumber );
  pairTree->SetBranchAddress( "event",          &eventNumber );
  pairTree->SetBranchAddress( "sector1",        &sector1 );
  pairTree->SetBranchAddress( "sector2",        &sector2 );
  pairTree->SetBranchAddress( "arm",            &arm );
  pairTree->SetBranchAddress( "xing",           &xing );
  pairTree->SetBranchAddress("blueSpinDir",     &blueSpinDir);
  pairTree->SetBranchAddress( "spinpattern",    &spin );
  pairTree->SetBranchAddress( "px1",            &px1 );
  pairTree->SetBranchAddress( "py1",            &py1 );
  pairTree->SetBranchAddress( "pz1",            &pz1 );
  pairTree->SetBranchAddress( "px2",            &px2 );
  pairTree->SetBranchAddress( "py2",            &py2 );
  pairTree->SetBranchAddress( "pz2",            &pz2  );
  pairTree->SetBranchAddress( "energy1",        &energy1 );
  pairTree->SetBranchAddress( "energy2",        &energy2 );
  pairTree->SetBranchAddress( "invmass",        &invmass );
  pairTree->SetBranchAddress( "ERT1",           ERT1 );
  pairTree->SetBranchAddress( "ERT2",           ERT2 );
  pairTree->SetBranchAddress( "scaledERT_4x4a", &scaledERT_4x4a );
  pairTree->SetBranchAddress( "scaledERT_4x4b", &scaledERT_4x4b );
  pairTree->SetBranchAddress( "scaledERT_4x4c", &scaledERT_4x4c );
  int numEntries = pairTree->GetEntries();
  cout << "There are " << numEntries << " in original tree" << endl;

  //file name, background tree name and title, peak tree name and title
  TString outputFileName = "curated_";
  outputFileName += particle;
  outputFileName += "_pAl.root";
  cout << endl << "Saving everything thing in " << outputFileName << endl;

  TString backgroundTreeName = "background_";
  backgroundTreeName += particle;
  backgroundTreeName += "_tree";
  TString backgroundTreeTitle = "Tree of cleaned up ";
  backgroundTreeTitle += particle;
  backgroundTreeTitle += " background photon pairs";
  cout << "~ ~ ~" << backgroundTreeName << ", " << backgroundTreeTitle 
       << "~ ~ ~" << endl;

  TString peakTreeName = particle;
  peakTreeName += "_tree";
  TString peakTreeTitle = "Tree of cleaned up ";
  peakTreeTitle += particle;
  peakTreeTitle += " peak photon pairs";
  cout << "~ ~ ~" << peakTreeName << ", " << peakTreeTitle << "~ ~ ~" << endl;

  TFile *outputFile = new TFile( outputFileName, "RECREATE" );
  TTree *prettyPeak = new TTree( peakTreeName, peakTreeTitle );
  TTree *prettyBackground = new TTree( backgroundTreeName, 
				       backgroundTreeTitle );
  int ptBin, bgPTBin;
  int blueSpinDir;
  prettyPeak->Branch( "fillNumber",     &fillNumber,  "fillNumber/I"  );
  prettyPeak->Branch( "runNumber",      &runNumber,   "runNumber/I" );
  prettyPeak->Branch( "eventNumber",    &eventNumber, "eventNumber/I" );
  prettyPeak->Branch( "arm",            &arm,         "arm/I" );
  prettyPeak->Branch( "sector1",        &sector1,     "sector1/I" );
  prettyPeak->Branch( "sector2",        &sector2,     "sector2/I" );
  prettyPeak->Branch( "xing",           &xing,        "xing/I" );
  prettyPeak->Branch( "blueSpinDir",    &blueSpinDir, "blueSpinDir/I" );
  prettyPeak->Branch( "spinPattern",    &spin,        "spinPattern/I" );
  prettyPeak->Branch( "energy1",        &energy1,     "energy1/F" );
  prettyPeak->Branch( "energy2",        &energy2,     "energy2/F" );
  prettyPeak->Branch( "px1",            &px1,         "px1/F" );
  prettyPeak->Branch( "py1",            &py1,         "py1/F" );
  prettyPeak->Branch( "pz1",            &pz1,         "pz1/F" );
  prettyPeak->Branch( "px2",            &px2,         "px2/F" );
  prettyPeak->Branch( "py2",            &py2,         "py2/F" );
  prettyPeak->Branch( "pz2",            &pz2,         "pz2/F" );
  prettyPeak->Branch( "ptBin",          &ptBin,       "ptBin/I" );
  prettyPeak->Branch( "ERT1",           ERT1,         "ERT1[3]/I" );
  prettyPeak->Branch( "ERT2",           ERT2,         "ERT2[3]/I" );
  prettyPeak->Branch( "scaledERT_4x4a", &scaledERT_4x4a, "scaledERT_4x4a/I" );
  prettyPeak->Branch( "scaledERT_4x4b", &scaledERT_4x4b, "scaledERT_4x4b/I"  );
  prettyPeak->Branch( "scaledERT_4x4c", &scaledERT_4x4c, "scaledERT_4x4c/I"  );

  prettyBackground->Branch( "fillNumber",     &fillNumber,  "fillNumber/I"  );
  prettyBackground->Branch( "runNumber",      &runNumber,   "runNumber/I" );
  prettyBackground->Branch( "eventNumber",    &eventNumber, "eventNumber/I" );
  prettyBackground->Branch( "arm",            &arm,         "arm/I" );
  prettyBackground->Branch( "sector1",        &sector1,     "sector1/I" );
  prettyBackground->Branch( "sector2",        &sector2,     "sector2/I" );
  prettyBackground->Branch( "xing",           &xing,        "xing/I" );
  prettyBackground->Branch( "blueSpinDir",    &blueSpinDir, "blueSpinDir/I" );
  prettyBackground->Branch( "spinPattern",    &spin,        "spinPattern/I" );
  prettyBackground->Branch( "energy1",        &energy1,     "energy1/F" );
  prettyBackground->Branch( "energy2",        &energy2,     "energy2/F" );
  prettyBackground->Branch( "px1",            &px1,         "px1/F" );
  prettyBackground->Branch( "py1",            &py1,         "py1/F" );
  prettyBackground->Branch( "pz1",            &pz1,         "pz1/F" );
  prettyBackground->Branch( "px2",            &px2,         "px2/F" );
  prettyBackground->Branch( "py2",            &py2,         "py2/F" );
  prettyBackground->Branch( "pz2",            &pz2,         "pz2/F" );
  prettyBackground->Branch( "ptBin",          &bgPTBin,     "ptBin/I" );
  prettyBackground->Branch( "ERT1",           ERT1,         "ERT1[3]/I" );
  prettyBackground->Branch( "ERT2",           ERT2,         "ERT2[3]/I" );
  prettyBackground->Branch( "scaledERT_4x4a", &scaledERT_4x4a, 
				                          "scaledERT_4x4a/I" );
  prettyBackground->Branch( "scaledERT_4x4b", &scaledERT_4x4b, 
				                          "scaledERT_4x4b/I" );
  prettyBackground->Branch( "scaledERT_4x4c", &scaledERT_4x4c, 
			                                  "scaledERT_4x4c/I" );

  int fillIndex = 0;
  int lastFillNumber = -99;

  ifstream skipEntriesFile;
  skipEntriesFile.open( "multiesPair_pAl.txt" );

  if (!skipEntriesFile.is_open()) 
  {
    cout << "!!!!! File multiesPair_pAl.txt was not found... Abort mission!!!!!" << endl;
    break;
  }
    
  int skipIndex;
  skipEntriesFile >> skipIndex;
  cout << "First skip index is " << skipIndex << endl;

  for( int i = 0; i < numEntries; i++ )
    {
      pairTree->GetEntry(i);
      
 
     if( i%10000000 == 0 ) 
	cout << "Processed " << i << " photon pairs " << endl;

     if( !skipEntriesFile.eof() && i == skipIndex )
       {
	 //this is an index that I should skip, and now I need to find the next
	 //skip index
	 skipEntriesFile >> skipIndex;
	 continue;
       }
       
     // this is just a test... // 
     //if (fillNumber==19237 || fillNumber==19238) {continue;}

     // These fills should only correspond to p+p runs, double check...
     /*
     if( fillNumber == 18777 )//somehow never eliminated 426319... oops
       continue;

     //weird GL1P scalers...like >100 times bigger than other fills GL1P scalers
      if( fillNumber == 18948 || fillNumber == 18949 || fillNumber == 18950 ) 
	continue;
     */
      if( fillNumber != lastFillNumber )
	{
	  fillTree->GetEntry( fillIndex );
	  if( fillNumber != fillNumberFillTree )
	    cout << "Loading info about fill " << fillNumberFillTree 
		 << " but looking at data from " << fillNumber << endl;

	  fillIndex++;
	}

      lastFillNumber = fillNumber;
      
      if( triggerCounts[ xing ] < 10000 ) 
	  continue;

      float alpha = (energy1 - energy2) / (energy1 + energy2);
      if( alpha > 0.8 ) continue;

      if( invmass < MIN_MASS || invmass > MAX_MASS ) continue;
 
      float pt = sqrt( (px1 + px2)*(px1 + px2) + (py1 + py2)*(py1 + py2) );
      ptBin = findBin( NUM_PT_BINS, PT_BINS, pt );
      bgPTBin = findBin( NUM_BG_PT_BINS, BG_PT_BINS, pt );
      if( ptBin >= 0 )
	{
	  histos[ arm ][ ptBin ]->Fill( invmass );
	  
	  if( ( invmass > MIN_BG_MASS_1 && invmass < MAX_BG_MASS_1 ) 
	      || ( invmass > MIN_BG_MASS_2 && invmass < MAX_BG_MASS_2 ) )
	    prettyBackground->Fill();
	  
	  else if( invmass > MIN_SIGNAL_MASS && invmass < MAX_SIGNAL_MASS )
	    prettyPeak->Fill();
	}

    }//end looping over original tree
  cout << "Got to the end of the loop!" << endl;

  outputFile->cd();
  prettyBackground->Write();
  prettyBackground->Delete();

  prettyPeak->Write();
  prettyPeak->Delete();

  for( int arm = 0; arm < NUM_ARMS; arm++ )
    for( int i = 0; i < NUM_PT_BINS; i++ )
      {
	histos[ arm ][i]->Write();
	histos[ arm ][i]->Delete();
      }

  outputFile->Close();
  outputFile->Delete();
}
