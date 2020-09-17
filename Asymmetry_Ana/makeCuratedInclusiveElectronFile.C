#include <iostream>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TString.h"

#include "Constants.h"
#include "findBin.h"

const int nbins = NUM_VALUE_BINS;
const float bins[ nbins + 1 ] = { 1.5, 1.8, 2.1, 2.7, 5.0 };

void makeCuratedInclusiveElectronFile()
{
  TString outFileName = "curated_e.root";
  TString treeName = "e_svx_tree";
  cout << "Saving " << treeName << " in " << outFileName << endl;

  TFile *fillFile = TFile::Open( "fill.root" );
  TTree *fillTree = (TTree*)fillFile->Get( "fill_tree" );
  int fillNumberFillTree;
  Long64_t triggerCounts[ NUM_XINGS ];
  fillTree->SetBranchAddress( "triggerCounts", triggerCounts );
  fillTree->SetBranchAddress( "fillNumber",    &fillNumberFillTree );
  TString phinames[NUM_ARMS] = {"EAST", "WEST"};

  TH1F *phihistos[NUM_ARMS];
  phihistos[0] = new TH1F(phinames[0], phinames[0], 80, 2.0, 4.0);
  phihistos[0]->Sumw2();
  phihistos[1] = new TH1F(phinames[1], phinames[1], 80, -1.0, 1.0);
  phihistos[1]->Sumw2();
  TFile *dataFile = TFile::Open("../AllRuns_725_ana644.root");

  // May need to add sector and energy content.. look into this. Will arm suffice instead of sector? //
  int fillnumber, run, event, xing, spinpattern, sector, arm; 
  int charge, triginfo, quality, nhit, hitpattern, n0, n1, ndf;
  float pt, pz, phi, phi0, mom, dcat, dcal, chisq, phi0, phi, disp, dep, zed;
  float emcdphi, emcdz, emce, ecore, sigemcdphi, sigemcdz, npe0, prob;
  bool conversionveto2x, conversionveto10x;
  TTree *inputTree = (TTree*)dataFile->Get( treeName );
  inputTree->SetBranchAddress("fillnumber", &fillnumber );
  inputTree->SetBranchAddress("run", &run);
  inputTree->SetBranchAddress("event", &event);
  inputTree->SetBranchAddress("spinpattern", &spinpattern);
  inputTree->SetBranchAddress("xing", &xing);
  inputTree->SetBranchAddress("charge", &charge);
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
  newTree->Branch( "charge",         &charge,         "charge/I" );
  newTree->Branch( "triginfo",       &triginfo,       "triginfo/I" );
  newTree->Branch( "dcat",           &dcat,           "dcal/F" );
  newTree->Branch( "dcal",           &dcal,           "dcal/F" );
  newTree->Branch( "mom",            &mom,            "mom/F" );
  newTree->Branch( "ptBin",          &ptBin,          "ptBin/I" );

  TH1F *e_pt_noscaler = new TH1F("pt_noscaler", ";p_{T} [GeV];", nbins, bins);
  e_pt_noscaler->Sumw2();
  TH1F *e_pt = new TH1F("pt", ";p_{T} [GeV];", nbins, bins);
  e_pt->Sumw2();
 
  int numEntries = inputTree->GetEntries();
  cout << "There are " << numEntries << " in the original electron tree " << endl;
  int fillIndex = 0;
  int lastFillNumber = -99;
  int eventsTreeIndex = 0;
  int lastRunNumber = -99;
  int cutval = 0;
  int bin4 = 0; int bin5_8 = 0; int bin5_10 = 0;
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
      if (pt <= 1.5 || pt >= 10.0)                   {continue;}
      if (conversionveto2x==0)                      {continue;}
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

       if (n0 <= 1 )                              {continue;}
       if (prob <= 0.01)                          {continue;}


      if (ndf == 0 )                                {continue;}

      e_pt_noscaler->Fill(pt);
      if( triggerCounts[ xing ] < 10000 )           {continue;}
      e_pt->Fill(pt);

      if (pt > 2.7 && pt < 5.0 ) {bin4++;} 
      if (pt > 5.0 && pt < 8.0 ) {bin5_8++;}
      if (pt > 5.0 && pt < 8.0 ) {bin5_10++;}

      if (arm==0)
      	phihistos[0]->Fill(phi);
      if (arm==1)
      	phihistos[1]->Fill(phi);
      ptBin = findBin( NUM_VALUE_BINS, VALUE_BINS, pt ) ;
      if( ptBin >= 0 )
	newTree->Fill();
      

    }
  cout << "Candidates passing cuts in the 2.7 < pT [GeV] < 5.0 range : " << bin4 << endl;
  cout << "Candidates passing cuts in the 5.0 < pT [GeV] < 8.0 range : " << bin5_8 << endl;
  cout << "Candidates passing cuts in the 5.0 < pT [GeV] < 10.0 range : " << bin5_10 << endl;

  outFile->cd();
  newTree->Write();
  e_pt_noscaler->Write();
  e_pt->Write();
  phihistos[0]->Write();
  phihistos[1]->Write();

  newTree->Delete();
  e_pt_noscaler->Delete();
  e_pt->Delete();
  phihistos[0]->Delete();
  phihistos[1]->Delete();

 
  outFile->Close();
  outFile->Delete();
}
