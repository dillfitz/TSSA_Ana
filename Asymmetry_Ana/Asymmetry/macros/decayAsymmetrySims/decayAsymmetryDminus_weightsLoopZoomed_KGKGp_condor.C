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
void decayAsymmetryDminus_weightsLoopZoomed_KGKGp_condor(int process=0)
{
  TString infileName = "dataFiles/condor/dminusdecay_";
  infileName += process;
  infileName += ".root";
  TString outfileName = "dataFiles/condor/dminusDecayAsymmetry_weightsLoopZoomed_KGKGp_";
  outfileName += process;
  outfileName += ".root";

  gStyle->SetOptFit();
  const int ndptbins = 41;
  const int neptbins = 6;
  double eptbins[neptbins+1] = {1.0, 1.3, 1.5, 1.8, 2.1, 2.7, 5.0};
  double eptcenters[neptbins] = {1.161, 1.398, 1.639, 1.936, 2.349, 3.290};
  double dptbins[ndptbins+1] = {0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95, 1.05, 1.15, 1.25, 1.35, 1.45, 1.55, 1.65, 1.75, 1.85, 1.95, 2.05, 2.15, 2.25, 2.35, 2.45, 2.55, 2.65, 2.75, 2.85, 2.95, 3.05, 3.15, 3.25, 3.35, 3.45, 3.55, 3.65, 3.75, 3.85, 3.95, 4.05, 4.15, 4.25, 4.35, 4.45};
  double dptcenters[ndptbins] = {0.4, 0.5, 0.6,, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4};

  double ANDminus_noKG[ndptbins] = {-2.00386888568295, -2.4634624262480265, -2.895429032940172, -3.296657333151198, -3.657456154433885, -3.987912342799291, -4.2781812051213155, -4.5355308478884355, -4.753132780716358, -4.938094538532421, -5.0858629453546556, -5.208565486468753, -5.30774413709565, -5.377173944790739, -5.427814877022022, -5.452156863309067, -5.470232304171338, -5.472710395104902, -5.457658529773524, -5.436716316217272, -5.4072875026139595, -5.371992770875464, -5.330675046564382, -5.281203721076877, -5.233091893645654, -5.185056146745793, -5.131602251901805, -5.076321269121237, -5.016624590647145, -4.959253168218867, -4.904534273807386, -4.851026242177821, -4.790385863386832, -4.734115123803418, -4.680153347077257, -4.629178563965116, -4.5777808958949375, -4.523699255338875, -4.473014519371925, -4.422400095909339, -4.370365692136383};

  double ANDminus_noKGp[ndptbins] = {-16.226317983340564, -19.803061067412788, -23.08265217905688, -26.03064941089843, -28.583883729933063, -30.809631979661503, -32.65682390603426, -34.173614798244174, -35.341199636415666, -36.217887451153125, -36.79314395377731, -37.14857265278113, -37.308646496878104, -37.25834896674968, -37.06144015819155, -36.700291594561754, -36.287638248217455, -35.78805212268783, -35.186207289953195, -34.555557220276846, -33.8842696745963, -33.194818132553586, -32.49136037232465, -31.76051856848113, -31.051915683484317, -30.35963343810752, -29.65659495091731, -28.959171753788787, -28.25870655103152, -27.587740155712602, -26.944668839747006, -26.324904952274288, -25.688894583926114, -25.093231904955143, -24.519887042307957, -23.972049136568423, -23.438440527394313, -22.90937073508341, -22.4046091785157, -21.91401670541035, -21.427133981094762};

  // parameters to tune to data! //
  double K_G = 0.002;
  double K_Gp = 0.0005; 
  const int Npar = 41;
  double KGs[Npar] = {-0.002, -0.0019, -0.0018, -0.0017, -0.0016, -0.0015, -0.0014, -0.0013, -0.0012, -0.0011, -0.0010, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0004, -0.0003, -0.0002, -0.0001, 0.0, 0.0001, 0.0002, 0.0003, 0.0004, 0.0005, 0.0006, 0.0007, 0.0008, 0.0009, 0.0010, 0.0011, 0.0012, 0.0013, 0.0014, 0.0015 , 0.0016, 0.0017, 0.0018, 0.0019, 0.002};
  double KGps[Npar] = {-0.002, -0.0019, -0.0018, -0.0017, -0.0016, -0.0015, -0.0014, -0.0013, -0.0012, -0.0011, -0.0010, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0004, -0.0003, -0.0002, -0.0001, 0.0, 0.0001, 0.0002, 0.0003, 0.0004, 0.0005, 0.0006, 0.0007, 0.0008, 0.0009, 0.0010, 0.0011, 0.0012, 0.0013, 0.0014, 0.0015 , 0.0016, 0.0017, 0.0018, 0.0019, 0.002};

  double ANDminus_model1[Npar][ndptbins]; 
  double ANDminus_model2[Npar][ndptbins]; 
  for (int j=0; j<Npar; ++j)
    {
      for (int i=0; i<ndptbins; ++i)
	{
	  ANDminus_model1[j][i] = KGs[j]*ANDminus_noKG[i];
	  ANDminus_model2[j][i] = KGps[j]*ANDminus_noKGp[i];
	} 
    }
  float Pol = 1;
  TFile *inFile = TFile::Open(infileName);
  TTree *decayTree = (TTree*)inFile->Get( "dminus" );
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
  cout << "There are  " << nEntries << " in the original dminus decay tree " << endl;
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
	  double weight_model1 = 1.0 + ANDminus_model1[l][dPtIndex]*std::cos(phi);
	  double weight_model2 = 1.0 + ANDminus_model2[l][dPtIndex]*std::cos(phi);
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
