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
void decayAsymmetryD0_weightsLoopZoomed_KGKGp_condor(int process=0)
{
  TString infileName = "dataFiles/condor/d0decay_";
  infileName += process;
  infileName += ".root";
  TString outfileName = "dataFiles/condor/d0DecayAsymmetry_weightsLoopZoomed_KGKGp_";
  outfileName += process;
  outfileName += ".root";

  gStyle->SetOptFit();
  const int ndptbins = 41;
  const int neptbins = 6;
  double eptbins[neptbins+1] = {1.0, 1.3, 1.5, 1.8, 2.1, 2.7, 5.0};
  double eptcenters[neptbins] = {1.161, 1.398, 1.639, 1.936, 2.349, 3.290};
  double dptbins[ndptbins+1] = {0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95, 1.05, 1.15, 1.25, 1.35, 1.45, 1.55, 1.65, 1.75, 1.85, 1.95, 2.05, 2.15, 2.25, 2.35, 2.45, 2.55, 2.65, 2.75, 2.85, 2.95, 3.05, 3.15, 3.25, 3.35, 3.45, 3.55, 3.65, 3.75, 3.85, 3.95, 4.05, 4.15, 4.25, 4.35, 4.45};
  double dptcenters[ndptbins] = {0.4, 0.5, 0.6,, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4};

  double AND0_noKG[ndptbins] = {-0.6274824000929276, -0.7817068208160842, -0.9176233774342479, -1.0383295182343715, -1.1614377365415864, -1.2719632267368592, -1.365848090415206, -1.4635185651217544, -1.5423645757732245, -1.621185301977502, -1.6781225866338163, -1.755490389864286, -1.8134261731389314, -1.842423989618348, -1.8910393232078682, -1.909882461534582, -1.9551374737742815, -1.956217187978704, -1.970901348571342, -1.9894661943412897, -2.0055426338105775, -2.01310324426068, -2.0154468980340785, -2.0140606493179507, -2.0140015684657904, -2.0006098801460004, -2.0055031688081333, -1.9986565149191142, -2.001215045903091, -1.9852186692649718, -1.9722096713547168, -1.958407448557239, -1.966900737185024, -1.9365534703477105, -1.9267895045891439, -1.9401859855271426, -1.921165628605535, -1.9058310906517422, -1.8975633922943356, -1.8799557813286518, -1.887982344283107};

  double AND0_noKGp[ndptbins] = {14.664000517139224, 17.731979902254313, 20.516193408406657, 22.967092215467076, 25.001432262376348, 26.677202792121467, 28.028213070458897, 28.951191615297773, 29.61248256174749, 29.982225265006146, 30.154998155994832, 30.00234174710597, 29.69732259410898, 29.35141531720642, 28.777203373850906, 28.174772819837365, 27.40163750549663, 26.723818770303897, 25.938149440621277, 25.10622182663265, 24.221540006795266, 23.37373064261309, 22.567466978322354, 21.745046270865217, 20.94561125929737, 20.186709173886836, 19.39393342069707, 18.64942885961422, 17.903161369594734, 17.216353550053046, 16.54581396045734, 15.908251427864064, 15.225655999372847, 14.693419178817408, 14.118147293496879, 13.504589876951393, 12.989662218495138, 12.50771876208015, 12.013844431782507, 11.556677529229606, 11.059718890923591};

  // parameters to tune to data! //
  double K_G = 0.002;
  double K_Gp = 0.0005; 
  const int Npar = 41;
  double KGs[Npar] = {-0.002, -0.0019, -0.0018, -0.0017, -0.0016, -0.0015, -0.0014, -0.0013, -0.0012, -0.0011, -0.0010, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0004, -0.0003, -0.0002, -0.0001, 0.0, 0.0001, 0.0002, 0.0003, 0.0004, 0.0005, 0.0006, 0.0007, 0.0008, 0.0009, 0.0010, 0.0011, 0.0012, 0.0013, 0.0014, 0.0015 , 0.0016, 0.0017, 0.0018, 0.0019, 0.002};
  double KGps[Npar] = {-0.002, -0.0019, -0.0018, -0.0017, -0.0016, -0.0015, -0.0014, -0.0013, -0.0012, -0.0011, -0.0010, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0004, -0.0003, -0.0002, -0.0001, 0.0, 0.0001, 0.0002, 0.0003, 0.0004, 0.0005, 0.0006, 0.0007, 0.0008, 0.0009, 0.0010, 0.0011, 0.0012, 0.0013, 0.0014, 0.0015 , 0.0016, 0.0017, 0.0018, 0.0019, 0.002};

  double AND0_model1[Npar][ndptbins]; 
  double AND0_model2[Npar][ndptbins]; 
  for (int j=0; j<Npar; ++j)
    {
      for (int i=0; i<ndptbins; ++i)
	{
	  AND0_model1[j][i] = KGs[j]*AND0_noKG[i];
	  AND0_model2[j][i] = KGps[j]*AND0_noKGp[i];
	} 
    }
  float Pol = 1;
  TFile *inFile = TFile::Open(infileName);
  TTree *decayTree = (TTree*)inFile->Get( "d0" );
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
  cout << "There are  " << nEntries << " in the original d0 decay tree " << endl;
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
	  double weight_model1 = 1.0 + AND0_model1[l][dPtIndex]*std::cos(phi);
	  double weight_model2 = 1.0 + AND0_model2[l][dPtIndex]*std::cos(phi);
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
