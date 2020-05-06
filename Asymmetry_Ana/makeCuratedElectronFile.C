#include <iostream>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TString.h"

#include "Constants.h"
#include "findBin.h"

//const int NUM_PT_BINS = NUM_VALUE_BINS;
//const float PT_BINS[ NUM_PT_BINS + 1 ] = { 1.5, 1.8, 2.1, 2.7, 6.0 };

void makeCuratedElectronFile()
{
  TString outFileName = "curated_ohfe.root";
  TString treeName = "e_svx_tree";
  cout << "Saving " << treeName << " in " << outFileName << endl;

  TFile *fillFile = TFile::Open( "../fill.root" );
  TTree *fillTree = (TTree*)fillFile->Get( "fill_tree" );
  int fillNumberFillTree;
  Long64_t triggerCounts[ NUM_XINGS ];
  fillTree->SetBranchAddress( "triggerCounts", triggerCounts );
  fillTree->SetBranchAddress( "fillNumber",    &fillNumberFillTree );

  //TFile *dataFile = TFile::Open( "ohfe.root" );
  TFile *dataFile = TFile::Open("../AllRuns_754_ana640.root");

  // May need to add sector and energy content.. look into this. Will arm suffice instead of sector? //
  int fillnumber, run, event, xing, spinpattern, sector, arm; 
  int triginfo, quality, nhit, hitpattern, n0, n1, ndf;
  float pt, pz, phi, phi0, mom, dcat, dcal, chisq, phi0, phi, disp, dep, zed;
  float emcdphi, emcdz, emce, ecore, sigemcdphi, sigemcdz, npe0, prob;
  bool conversionveto2x, conversionveto10x;
  TTree *inputTree = (TTree*)dataFile->Get( treeName );
  inputTree->SetBranchAddress("fillnumber", &fillnumber );
  inputTree->SetBranchAddress("run", &run);
  inputTree->SetBranchAddress("event", &event);
  inputTree->SetBranchAddress("spinpattern", &spinpattern);
  inputTree->SetBranchAddress("xing", &xing);
  inputTree->SetBranchAddress("triginfo", &triginfo);
  inputTree->SetBranchAddress("arm", &arm);
  inputTree->SetBranchAddress("dcat", &dcat); 
  inputTree->SetBranchAddress("dcal", &dcal); 
  inputTree->SetBranchAddress("mom", &mom); 
  inputTree->SetBranchAddress("chisq", &chisq); 
  inputTree->SetBranchAddress("ndf", &ndf); 
  inputTree->SetBranchAddress("phi0", &phi0); 
  inputTree->SetBranchAddress("phi", &phi);  
  inputTree->SetBranchAddress("nhit", &nhit); 
  inputTree->SetBranchAddress("hitpattern", &hitpattern); 
  inputTree->SetBranchAddress("pt", &pt); 
  inputTree->SetBranchAddress("pz", &pz); 
  inputTree->SetBranchAddress("quality", &quality); 
  inputTree->SetBranchAddress("n0", &n0); 
  inputTree->SetBranchAddress("disp", &disp); 
  inputTree->SetBranchAddress("dep", &dep); 
  inputTree->SetBranchAddress("zed", &zed); 
  inputTree->SetBranchAddress("emcdphi", &emcdphi); 
  inputTree->SetBranchAddress("emcdz", &emcdz); 
  inputTree->SetBranchAddress("emce", &emce); 
  inputTree->SetBranchAddress("ecore", &ecore); 
  inputTree->SetBranchAddress("sigemcdphi", &sigemcdphi); 
  inputTree->SetBranchAddress("sigemcdz", &sigemcdz); 
  inputTree->SetBranchAddress("n1", &n1); 
  inputTree->SetBranchAddress("npe0", &npe0); 
  inputTree->SetBranchAddress("prob", &prob); 
  inputTree->SetBranchAddress("conversionveto2x", &conversionveto2x); 
  inputTree->SetBranchAddress("conversionveto10x", &conversionveto10x); 

  int eventsRun;
  TTree *eventsTree = (TTree*)dataFile->Get( "events_tree" );
  //TTree *eventsTree = (TTree*)eventFile->Get( "events_tree" );
  eventsTree->SetBranchAddress( "run",           &eventsRun );

  TFile *outFile = new TFile( outFileName,  "RECREATE" );
  TTree *newTree = new TTree( treeName, "Curated list of electrons" );
  int ptBin;
  newTree->Branch( "fillNumber",     &fillnumber,     "fillnumber/I"  );
  newTree->Branch( "runNumber",      &run,            "run/I" );
  newTree->Branch( "eventNumber",    &event,          "event/I" );
  newTree->Branch( "spinPattern",    &spinpattern,    "spinpattern/I" );
  newTree->Branch( "xing",           &xing,           "xing/I" );
  newTree->Branch( "arm",            &arm,            "arm/I" );
  newTree->Branch( "pt",             &pt,             "pt/F" );
  newTree->Branch( "pz",             &pz,             "pz/F" );
  newTree->Branch( "phi",            &phi,            "phi/F" );
  newTree->Branch( "triginfo",       &triginfo,       "triginfo/I" );
  newTree->Branch( "dcat",           &dcat,           "dcal/F" );
  newTree->Branch( "dcal",           &dcal,           "dcal/F" );
  newTree->Branch( "mom",            &mom,            "mom/F" );
  newTree->Branch( "ptBin",          &ptBin,          "ptBin/I" );
 
  int numEntries = inputTree->GetEntries();
  cout << "There are " << numEntries << " in the original electron tree " << endl;
  int fillIndex = 0;
  int lastFillNumber = -99;
  int eventsTreeIndex = 0;
  int lastRunNumber = -99;
  int cutval = 0;
  for( int i = 0; i < numEntries; i++ )
  {
      inputTree->GetEntry( i );

/*
      //loading scale down info for cross section cross check
      if( run!= lastRunNumber )
	{
	  eventsTree->GetEntry( eventsTreeIndex );
	  while( run != eventsRun )
	    {
	      cout << "Entry " << eventsTreeIndex
		   << " was an events tree dupliate so we'll try the next one"
		   << endl;
	      eventsTreeIndex++;
	      eventsTree->GetEntry( eventsTreeIndex );
	  }
          eventsTreeIndex++;
      }
      lastRunNumber = run;
*/

      if(fillnumber == 0) 
      {
          if (run != lastRunNumber ) { cout << run << endl; }
	  lastRunNumber = run; 
          continue;  
      }
      if(fillnumber==18777 || fillnumber==18758 || fillnumber==18777 || fillnumber==18778) continue;

      if( fillnumber != lastFillNumber )
      {
	  fillTree->GetEntry( fillIndex );

	  if( fillnumber != fillNumberFillTree )
	    cout << "Panic: Entry " << i << " Loading info about fill " 
		 << fillNumberFillTree << " but looking at data from " 
		 << fillnumber << endl;

	  fillIndex++;
      }
      lastFillNumber = fillnumber;

      // Analysis Quality Cuts
      if (pt <= 1.0 || pt >= 6.0)                     {continue;}
      if (conversionveto10x==0)                      {continue;}
      if (abs(dep)>=2.)                             {continue;}
      if (abs(sigemcdphi)>=3.||abs(sigemcdz)>=3.)   {continue;}
      if (abs(zed)>= 75)                            {continue;}
      if (disp>=5.)                                 {continue;}
      if (ndf != 0) {
      // 3 in note, 2 in ppg
         if (chisq/ndf >= 3.)                       {continue;}      
      } 
      if (nhit <= 2 )                               {continue;}
      if (quality != 63 && quality !=31)            {continue;}
      if (!((hitpattern&3)==3))                     {continue;}
      if (pt<5.) {
         if (n0 <= 1 )                              {continue;}
         if (prob <= 0.01)                          {continue;}
      }
      else {
         if (n0 <=3 )                               {continue;}
         if (prob <= 0.2)                           {continue;}
      }
      if (ndf == 0 )                                {continue;}
      if( triggerCounts[ xing ] < 10000 )           {continue;}

      ptBin = findBin( NUM_VALUE_BINS, VALUE_BINS, pt ) ;
      if( ptBin >= 0 )
	newTree->Fill();
      

    }

  outFile->cd();
  newTree->Write();
  newTree->Delete();
 
  outFile->Close();
  outFile->Delete();
}
