#include "TFile.h"
#include "TString.h"
#include "TH1.h"

void bunchShufflingSystematics_pA(const char* particle = "eta", const char* collisionSystem = "pAl")
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
  TString infileString = "dataFiles/";
  infileString += particle;
  infileString += "_";
  infileString += collisionSystem;
  infileString += "_shufflingMeanSigma.root";
  
  TFile *infile = TFile::Open(infileString);
  
  TH1F *h_sigma = (TH1F*)infile->Get("sigma");
  TH1F *h_sys = new TH1F("sys",";p_{T} [GeV/c];#sigma",NUM_PT_BINS, PT_BINS);
  //h_sigma->Draw();

  infileString = "../dataFiles/";
  infileString += particle;
  infileString += "_bgCorrected_";
  infileString += collisionSystem;
  infileString += "_AN.root";
  
  TFile *ANFile = TFile::Open(infileString);
  
  TGraphAsymmErrors* bgCorrected;
  bgCorrected = (TGraphAsymmErrors*)ANFile->Get( "lumi" );
  double *error;
  error = bgCorrected->GetEYhigh();
  
  //int nPtBins = h_sigma->GetNbinsX();
  for (int i=0; i<NUM_PT_BINS; ++i)
  {
    //cout << fabs(h_sigma->GetBinContent(i+1) - 1) << " > " << 3*h_sigma->GetBinError(i+1) << endl;
    float diff = h_sigma->GetBinContent(i+1) - 1;
    float histerr = h_sigma->GetBinError(i+1);
    //if (fabs(h_sigma->GetBinContent(i+1) - 1) > 3*h_sigma->GetBinError(i+1));    
    if (diff > 3 *histerr)
    {

      h_sys->SetBinContent(i+1, sqrt(pow(h_sigma->GetBinContent(i+1),2) - 1) * error[i]);
      cout << h_sys->GetBinContent(i+1) << endl;
      
    }
    else 
    {
      cout << " made it here" << endl;
      h_sys->SetBinContent(i+1, 0.);
    }
  }
  TCanvas can;
  h_sys->Draw();
  
  TString outfileName = "../dataFiles/";
  outfileName += particle;
  outfileName += "_";
  outfileName += collisionSystem;
  outfileName += "_shufflingSys.root";
  TFile *outfile = new TFile(outfileName, "RECREATE");
  h_sys->Draw();
  h_sys->Write();

  
}
