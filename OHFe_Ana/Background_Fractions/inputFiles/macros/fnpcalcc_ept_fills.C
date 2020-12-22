#include <iostream>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TString.h"

#include "../../../../Asymmetry_Ana/Constants.h"

void fnpcalcc_ept_fills()
{
  TString outFileName = "../forfnp.root";
  TString treeName = "e_svx_tree";
  cout << "Saving " << treeName << " in " << outFileName << endl;

  TFile *fillFile = TFile::Open( "../../../../Asymmetry_Ana/fill.root" );

  TTree *fillTree = (TTree*)fillFile->Get( "fill_tree" );
  int fillNumberFillTree;
  Long64_t triggerCounts[ NUM_XINGS ];

  fillTree->SetBranchAddress( "triggerCounts", triggerCounts );
  fillTree->SetBranchAddress( "fillNumber",    &fillNumberFillTree );

  //TFile *dataFile = TFile::Open( "ohfe.root" );

  TFile *dataFile = TFile::Open("../../../../AllRuns_725_ana644.root");

  // May need to add sector and energy content.. look into this. Will arm suffice instead of sector? //
  int fillnumber, run, event, xing, spinpattern, sector, arm, charge; 
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
  inputTree->SetBranchAddress("charge", &charge); 

  int eventsRun;
  TTree *eventsTree = (TTree*)dataFile->Get( "events_tree" );
  //TTree *eventsTree = (TTree*)eventFile->Get( "events_tree" );
  eventsTree->SetBranchAddress( "run",           &eventsRun );

  TFile *outFile = new TFile( outFileName,  "RECREATE" );

  TH1F* h_ept_conveto = new TH1F("h_ept_conveto",";pT;",NUM_VALUE_BINS,VALUE_BINS);
  h_ept_conveto->Sumw2();

  TH1F* h_ept_noconveto = new TH1F("h_ept_nv",";pT;",NUM_VALUE_BINS,VALUE_BINS);
  h_ept_noconveto->Sumw2();

  // + charge //
  TH1F* h_ept_conveto_plus = new TH1F("h_ept_conveto_plus",";pT;",NUM_VALUE_BINS,VALUE_BINS);
  h_ept_conveto_plus->Sumw2();

  TH1F* h_ept_noconveto_plus = new TH1F("h_ept_nv_plus",";pT;",NUM_VALUE_BINS,VALUE_BINS);
  h_ept_noconveto_plus->Sumw2();

  // - charge //
  TH1F* h_ept_conveto_minus = new TH1F("h_ept_conveto_minus",";pT;",NUM_VALUE_BINS,VALUE_BINS);
  h_ept_conveto_minus->Sumw2();

  TH1F* h_ept_noconveto_minus = new TH1F("h_ept_nv_minus",";pT;",NUM_VALUE_BINS,VALUE_BINS);
  h_ept_noconveto_minus->Sumw2();
 
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
      
      if(fillnumber == 0) 
      {
          if (run != lastRunNumber ) { cout << run << endl; }
	  lastRunNumber = run; 
          continue;  
      }
      
      if(fillnumber==18777 || fillnumber==18758 || fillnumber==18778) 
      {
	if (run != lastRunNumber ) { cout << run << endl; }
	lastRunNumber = run;	  
	continue;
      }

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
      if (pt < 1.0 || pt > 5)                       {continue;}
      if (abs(dep)>=2.)                             {continue;}
      if (abs(sigemcdphi)>=3.||abs(sigemcdz)>=3.)   {continue;}
      if (abs(zed)>= 75)                            {continue;}
      if (disp>=5.)                                 {continue;}
      if (ndf != 0) {
	// 3 in note, 2 in ppg
       if (chisq/ndf >= 3.)                         {continue;}      
      } 
      if (nhit <= 2 )                               {continue;}
      if (quality != 63 && quality !=31)            {continue;}
      if (!((hitpattern&3)==3))                     {continue;}
      if (pt<5.) {
	if (n0 <= 1 )                               {continue;}
	if (prob <= 0.01)                            {continue;}
       }
      else {
	if (n0 <=3 )                                {cout << " a test... " <<endl; continue;}
	if (prob <= 0.2)                            {continue;}
      }
      if( triggerCounts[ xing ] < 10000 )           {continue;}

      // This cut has no effect //
      //if (abs(dcat) >= 0.2)                         {continue;}

      h_ept_noconveto->Fill(pt);

      if (conversionveto2x==1)
	h_ept_conveto->Fill(pt);
      
      // + charge //
      if (charge == 1)
      {
        h_ept_noconveto_plus->Fill(pt);

        if (conversionveto2x==1)
	  h_ept_conveto_plus->Fill(pt);
      }

      // - charge //
      if (charge == -1)
      {
        h_ept_noconveto_minus->Fill(pt);

        if (conversionveto2x==1)
	  h_ept_conveto_minus->Fill(pt);
      }
             

    }

  outFile->cd();
  h_ept_noconveto->Write();
  h_ept_conveto->Write();
  h_ept_noconveto_plus->Write();
  h_ept_conveto_plus->Write();
  h_ept_noconveto_minus->Write();
  h_ept_conveto_minus->Write();

  h_ept_noconveto->Delete();
  h_ept_conveto->Delete();
  h_ept_noconveto_plus->Delete();
  h_ept_conveto_plus->Delete();
  h_ept_noconveto_minus->Delete();
  h_ept_conveto_minus->Delete();

  outFile->Close();
  outFile->Delete();
}
