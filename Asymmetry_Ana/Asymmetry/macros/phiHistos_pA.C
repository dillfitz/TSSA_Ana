#include "../../Constants.h"

#include "TGraphErrors.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

#include <math.h>

void phiHistos_pA(const char* particle = "eta", const char* collisionSystem = "pAu")
{

  const float pi = 2*acos(0.0);
  TH1F *phiHistsWest[NUM_VALUE_BINS], *phiHistsEast[NUM_VALUE_BINS];
  TString hist_name = "";
    
  for (int j=0; j<NUM_VALUE_BINS; ++j)
  {
    hist_name = "phiWest_";
    hist_name += VALUE_BINS[j];
    hist_name += "_";
    hist_name += VALUE_BINS[j+1];
    phiHistsWest[j] = new TH1F(hist_name, ";#phi;", 100, -0.65, 1.05);
    hist_name = "phiEast_";
    hist_name += VALUE_BINS[j];
    hist_name += "_";
    hist_name += VALUE_BINS[j+1];    
    phiHistsEast[j] = new TH1F(hist_name, ";#phi;", 100, 2.1, 3.8);    
  }
  

  TString inputDataFileName = "../../curated_";
  inputDataFileName += particle;
  inputDataFileName += "_";
  inputDataFileName += collisionSystem;
  inputDataFileName += ".root";
  
  TString inputTreeNameInFile = "";
  inputTreeNameInFile += particle;
  inputTreeNameInFile += "_tree";  
  
  TFile *dataFile = TFile::Open( inputDataFileName );
  TTree* dataTree = (TTree*)dataFile->Get( inputTreeNameInFile );
  cout << "~~~~~~~~~~~Loading " << inputTreeNameInFile << " from " 
       << inputDataFileName << "~~~~~~~~~~" << endl;
  int ptBin, arm;
  float px1, px2, py1, py2;
  float px, py, phi;
  dataTree->SetBranchAddress( "ptBin",  &ptBin );
  dataTree->SetBranchAddress("arm",  &arm);
  dataTree->SetBranchAddress( "px1", &px1 );
  dataTree->SetBranchAddress( "py1", &py1 );
  dataTree->SetBranchAddress( "px2", &px2 );
  dataTree->SetBranchAddress( "py2", &py2 );  
  
  TString outfilename = "dataFiles/phiHists_";
  outfilename += particle;
  outfilename += "_";
  outfilename += collisionSystem; 
  outfilename += ".root";
  TFile *outfile = new TFile(outfilename, "recreate");
  outfile->cd();
  int nentries = dataTree->GetEntries();
  for(int i=0; i<nentries; ++i)
  {
    dataTree->GetEntry(i);
	  px = px1 + px2;
	  py = py1 + py2;

    phi = atan2( py , px );	    
/*
	  if( arm == 0 )  
	    phi = PI/2 - phi;
    else if( arm == 1 )
      phi = PI/2 + phi;
*/      
    for (int j=0; j<NUM_VALUE_BINS; ++j)
    {
      if (ptBin == j)
      {
        if (arm == 0)
        {
          //cout << "WEST : " << phi << endl;
          phiHistsWest[j]->Fill(phi);
        }  
        else if (arm == 1)
        {   
          //cout << "EAST : " << phi << endl;
          if (phi < 0) {phi = phi + 2*pi;}
          phiHistsEast[j]->Fill(phi);
        }
      }  
    }      
    	            
  }
  
  for (int j=0; j<NUM_VALUE_BINS; ++j)
  {
    phiHistsEast[j]->Draw();
    phiHistsEast[j]->Write();
    phiHistsEast[j]->Delete();
    phiHistsWest[j]->Draw();
    phiHistsWest[j]->Write();
    phiHistsWest[j]->Delete();
  }
  
  outfile->Write();
  outfile->Close();
  
}
