#include <iostream>
#include <algorithm>
#include "TFile.h"
#include "TH1.h"

void backgroundFractions_wSys(const char* particle = "eta", const char* collisionSystem = "pAu")
{

  if (particle == "pi0")
  {
    const int NUM_PT_BINS = 10;
    const double PT_BINS[ NUM_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 20 };    
    
  }
  else if (particle == "eta")
  {
    const int NUM_PT_BINS = 8;
    const double PT_BINS[ NUM_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 10, 20 };    
  }
  TString infileName = "../dataFiles/";
  infileName += particle;
  infileName += "_bgfracs_";
  infileName += collisionSystem;
  infileName += ".root";
  TFile *infile = TFile::Open(infileName);
  
  infileName = "../dataFiles/";
  infileName += particle;
  infileName += "_bgfracs_lowerFitRange_";
  infileName += collisionSystem;
  infileName += ".root";
  TFile *infile_lower = TFile::Open(infileName); 
  
  infileName = "../dataFiles/";
  infileName += particle;
  infileName += "_bgfracs_upperFitRange_";
  infileName += collisionSystem;
  infileName += ".root";
  TFile *infile_upper = TFile::Open(infileName);   
  
	TH1F* h_rSqrt = (TH1F*)infile->Get("rSqrt");  
	TH1F* h_rWest = (TH1F*)infile->Get("rWest");  
	TH1F* h_rEast = (TH1F*)infile->Get("rEast");  		
	TH1F* h_rSqrt_lower = (TH1F*)infile_lower->Get("rSqrt");  
	TH1F* h_rSqrt_upper = (TH1F*)infile_upper->Get("rSqrt");  
	
	TString outfileName = "../dataFiles/";
  outfileName += particle;
  outfileName += "_bgfracs_wSys_";
  outfileName += collisionSystem;
  outfileName += ".root";
	TFile *outfile = new TFile(outfileName, "RECREATE");
	outfile->cd();
	
	TH1F* h_rRange = new TH1F("rRange",";#sigma_{r};", NUM_PT_BINS, PT_BINS);
	for (int i=0; i<NUM_PT_BINS; ++i)
	{
	  h_rRange->SetBinContent(i+1, std::max(fabs(h_rSqrt->GetBinContent(i+1)-h_rSqrt_lower->GetBinContent(i+1)), fabs(h_rSqrt->GetBinContent(i+1)-h_rSqrt_upper->GetBinContent(i+1))));
	  cout << "r_range(" << i+1 << ") " << h_rRange->GetBinContent(i+1) << endl;
	}
	
	h_rRange->Draw();
	
	h_rWest->Write();
	h_rEast->Write();
	h_rSqrt->Write();
	h_rRange->Write();
	
  h_rWest->Delete();
  h_rEast->Delete();
	h_rSqrt->Delete();
	//h_rRange->Delete();
	
}
