#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TString.h"
#include "TGraph.h"


const float PI =  3.14159265358979323846;
bool verbosity = 1;
void decayAsymmetryDplus_weightsLoopZoomed_KGKGp_condor(int process=0)
{
  TString infileName = "dataFiles/condor/dplusdecay_";
  infileName += process;
  infileName += ".root";
  TString outfileName = "dataFiles/condor/dplusDecayAsymmetry_weightsLoopZoomed_KGKGp_";
  outfileName += process;
  outfileName += ".root";

  gStyle->SetOptFit();
  const int ndptbins = 41;
  const int neptbins = 6;
  double eptbins[neptbins+1] = {1.0, 1.3, 1.5, 1.8, 2.1, 2.7, 5.0};
  double eptcenters[neptbins] = {1.161, 1.398, 1.639, 1.936, 2.349, 3.290};
  double dptbins[ndptbins+1] = {0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95, 1.05, 1.15, 1.25, 1.35, 1.45, 1.55, 1.65, 1.75, 1.85, 1.95, 2.05, 2.15, 2.25, 2.35, 2.45, 2.55, 2.65, 2.75, 2.85, 2.95, 3.05, 3.15, 3.25, 3.35, 3.45, 3.55, 3.65, 3.75, 3.85, 3.95, 4.05, 4.15, 4.25, 4.35, 4.45};
  double dptcenters[ndptbins] = {0.4, 0.5, 0.6,, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4};

  double ANDplus_noKG[ndptbins] = {-0.633445002301727, -0.788215184257882, -0.9258910109106822, -1.0480413689834749, -1.1722366571770184, -1.284220882397701, -1.3794201818081158, -1.4766399041215414, -1.5553021778851412, -1.6345332747310417, -1.6923920554386296, -1.7702169692102037, -1.8286181681908154, -1.8564579691776852, -1.9062718132655048, -1.927888234350968, -1.9734657530499577, -1.9758848930859, -1.9908219851162507, -2.0075767837171172, -2.0231349757486137, -2.0302326803537647, -2.033168538334925, -2.034065443806923, -2.0350589492604865, -2.0220845239403817, -2.0248224211241643, -2.016601895772738, -2.019548950100019, -2.003422037528232, -1.9901700232581092, -1.977453389006957, -1.9864285529093502, -1.9551467632900097, -1.9454099314825954, -1.9573976978429328, -1.9395960527439347, -1.924716999528432, -1.9154153438562604, -1.8968475558242242, -1.905226413502139};

  double ANDplus_noKGp[ndptbins] = {15.046465836260676, 18.204299512736497, 21.07011383056006, 23.594168472138676, 25.696918159412927, 27.428727945824587, 28.829492080961117, 29.799557052745808, 30.5003089324194, 30.896709457816446, 31.08774446410004, 30.948253304019808, 30.647461210445915, 30.305161761033247, 29.719349048213108, 29.09742734282163, 28.31070349523236, 27.61340281350978, 26.809428875889065, 25.964262372768594, 25.06381303650736, 24.196792910181678, 23.365594416227253, 22.5122134108699, 21.686849458713425, 20.905285433860236, 20.098796814160778, 19.33787686058397, 18.569097178149896, 17.86159834979566, 17.168175591018016, 16.507227013535772, 15.804544592168687, 15.256639224931615, 14.663720936118763, 14.035353379626917, 13.499598735054747, 13.000675583250567, 12.493267991874577, 12.024414403450193, 11.510697192786505};

  // parameters to tune to data! //
  double K_G = 0.002;
  double K_Gp = 0.0005; 
  const int Npar = 41;
  double KGs[Npar] = {-0.002, -0.0019, -0.0018, -0.0017, -0.0016, -0.0015, -0.0014, -0.0013, -0.0012, -0.0011, -0.0010, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0004, -0.0003, -0.0002, -0.0001, 0.0, 0.0001, 0.0002, 0.0003, 0.0004, 0.0005, 0.0006, 0.0007, 0.0008, 0.0009, 0.0010, 0.0011, 0.0012, 0.0013, 0.0014, 0.0015 , 0.0016, 0.0017, 0.0018, 0.0019, 0.002};
  double KGps[Npar] = {-0.002, -0.0019, -0.0018, -0.0017, -0.0016, -0.0015, -0.0014, -0.0013, -0.0012, -0.0011, -0.0010, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0004, -0.0003, -0.0002, -0.0001, 0.0, 0.0001, 0.0002, 0.0003, 0.0004, 0.0005, 0.0006, 0.0007, 0.0008, 0.0009, 0.0010, 0.0011, 0.0012, 0.0013, 0.0014, 0.0015 , 0.0016, 0.0017, 0.0018, 0.0019, 0.002};

  double ANDplus_model1[Npar][ndptbins]; 
  double ANDplus_model2[Npar][ndptbins]; 
  for (int j=0; j<Npar; ++j)
    {
      for (int i=0; i<ndptbins; ++i)
	{
	  ANDplus_model1[j][i] = KGs[j]*ANDplus_noKG[i];
	  ANDplus_model2[j][i] = KGps[j]*ANDplus_noKGp[i];
	} 
    }
  float Pol = 1;
  TFile *inFile = TFile::Open(infileName);
  TTree *decayTree = (TTree*)inFile->Get( "dplus" );
  float pt, pt1, phi, phi1;
  decayTree->SetBranchAddress("pt", &pt);
  decayTree->SetBranchAddress("pt1", &pt1);  
  decayTree->SetBranchAddress("phi", &phi);  
  decayTree->SetBranchAddress("phi1", &phi1);

  TFile *outFile = new TFile(outfileName, "RECREATE");

  TString histname = "";
  string hist_name_model1 = ""; 
  string hist_name_model2 = ""; 
  TString histname_model1 = "";
  TString histname_model2 = "";

  TH1F *ephi_model1[Npar][neptbins];
  TH1F *ephi_model2[Npar][neptbins];
  for (int j=0; j<Npar; ++j)
    {
      for (int i=0; i<neptbins; ++i)
	{
	  histname = "ephi";
	  histname += i;
	  ostringstream ss1;
	  ostringstream ss2;
	  ss1<<KGs[j];
	  ss2<<KGps[j];
	  histname_model1 = histname;
	  histname_model1 += "_KG_";
	  histname_model1 += ss1.str();
	  histname_model2 = histname; 
	  histname_model2 += "_KGp_";
	  histname_model2 += ss2.str();

	  string hist_name_model1 = histname_model1;
	  replace(hist_name_model1.begin(), hist_name_model1.end(), '.', 'p');
	  replace(hist_name_model1.begin(), hist_name_model1.end(), '-', 'm');

	  string hist_name_model2 = histname_model2;
	  replace(hist_name_model2.begin(), hist_name_model2.end(), '.', 'p');
	  replace(hist_name_model2.begin(), hist_name_model2.end(), '-', 'm');

	  histname_model1 = hist_name_model1;
	  histname_model2 = hist_name_model2;

	  ephi_model1[j][i] = new TH1F(histname_model1, ";#phi;", 12, 0, 2*PI);
	  ephi_model1[j][i]->Sumw2();

	  ephi_model2[j][i] = new TH1F(histname_model2, ";#phi;", 12, 0, 2*PI);
	  ephi_model2[j][i]->Sumw2();
	}
    } 
  
  int nEntries = decayTree->GetEntries();
  cout << "There are  " << nEntries << " in the original dplus decay tree " << endl;
  for (int i=0; i<nEntries; ++i)
    {
      if (i%1000000==0) {cout << "Entry " << i << endl; }
      decayTree->GetEntry(i);
      if (pt < 0.35 || pt > 4.45) {continue;}
      if (pt1 < 1.0 || pt1 > 5.0) {continue;}
      for (int l=0; l<Npar; ++l)
	{
	  int dPtIndex = 0;
	  for (int j=0; j<ndptbins; ++j)
	    {
	      if ( pt > dptbins[j] && pt < dptbins[j+1] )
		{
		  dPtIndex = j;
		}
	    }
	  double weight_model1 = 1.0 + ANDplus_model1[l][dPtIndex]*std::cos(phi);
	  double weight_model2 = 1.0 + ANDplus_model2[l][dPtIndex]*std::cos(phi);
	  //cout << "weight : " << weight << endl;
	  for (int k=0; k<neptbins; ++k)
	    {
	      if (pt1 > eptbins[k] && pt1 < eptbins[k+1] )
		{
		  ephi_model1[l][k]->Fill(phi1, weight_model1);
		  ephi_model2[l][k]->Fill(phi1, weight_model2);
		}
	    }
	}
    }
  
  outFile->cd();
  for (int j=0; j<Npar; ++j)
    {
      for (int i=0; i<neptbins; ++i)
	{
	  ephi_model1[j][i]->Write();
	  ephi_model1[j][i]->Delete();

	  ephi_model2[j][i]->Write();
	  ephi_model2[j][i]->Delete();
	}
    }
  outFile->Write();
  outFile->Close();
}
