#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TString.h"
#include "TGraph.h"

using namespace std;

const float PI =  3.14159265358979323846;
bool verbosity = 1;
void decayAsymmetryD0_weights()
{
	gStyle->SetOptFit();
  const int nd0ptbins = 21;
  const int neptbins = 6;
  double eptbins[neptbins+1] = {1.0, 1.3, 1.5, 1.8, 2.1, 2.7, 5.0};
  double eptcenters[neptbins] = {1.161, 1.398, 1.639, 1.936, 2.349, 3.290};
	double d0ptbins[nd0ptbins+1] = {0.3, 0.5, 0.7, 0.9, 1.1, 1.3, 1.5, 1.7, 1.8, 2.1, 2.3, 2.5, 2.7, 2.9, 3.1, 3.3, 3.5, 3.7, 3.9, 4.1, 4.3, 4.5};
  // Paramarization for AN D0 //
  double a0[nd0ptbins] = {-0.00027, -0.000307, -0.000322, -0.000334, -0.000342, -0.000366, -0.000381, -0.000400, -0.000422, -0.000457, -0.000481, -0.000510, -0.000541, -0.000572, -0.000614, -0.000652, -0.000677, -0.000719, -0.000760, -0.000809, -0.000853};
  double b0[nd0ptbins] = {-0.0000926, -0.000106, -0.000110, -0.000116, -0.000118, -0.000123, -0.000131, -0.000139, -0.000141, -0.000155, -0.000165, -0.000175, -0.000187, -0.000198, -0.000209, -0.000222, -0.000237, -0.000247, -0.000264, -0.000274, -0.000291};
  double a1[nd0ptbins] = {-0.155, -0.216, -0.252, -0.265, -0.267, -0.258, -0.246, -0.231, -0.220, -0.207, -0.194, -0.189, -0.174, -0.166, -0.158, -0.147, -0.144, -0.137, -0.129, -0.126, -0.119};
  double a2[nd0ptbins] = {0.0200, 0.0530, 0.0775, 0.0901, 0.0949, 0.0933, 0.0899, 0.0860, 0.0805, 0.0757, 0.0721, 0.0668, 0.0633, 0.0614, 0.0585, 0.0543, 0.0518, 0.0503, 0.0480, 0.0458, 0.0451};
  // parameters to tune to data! //
  double lambda_f = 0.07;
  double lambda_d = 0.07;

  double AN[nd0ptbins]; 
  for (int i=0; i<nd0ptbins; ++i)
  {
    AN[i] = a0[i] + lambda_f*a1[i] + lambda_d*a2[i];
    if (verbosity)
	  {
	  	cout << "d0ptbin : " << i+1  << " of 21" <<  endl;
	  	cout << "a0 : " << a0[i] << endl;
	  	cout << "a1 : " << a1[i] << endl;
	  	cout << "a2 : " << a2[i] << endl;
	  	cout << "lambda_f : " << lambda_f << " lambda_d : " << lambda_d << endl;
	  	cout << "AN(D0) : " << AN[i] << endl;
	  }
  } 
  float Pol = 1;

  TFile *inFile = TFile::Open("dataFiles/d0decay_100M.root");
  TTree *decayTree = (TTree*)inFile->Get( "d0" );
  float pt, pt1, phi, phi1;
  decayTree->SetBranchAddress("pt", &pt);
  decayTree->SetBranchAddress("pt1", &pt1);  
  decayTree->SetBranchAddress("phi", &phi);  
  decayTree->SetBranchAddress("phi1", &phi1);

  TFile *outFile = new TFile("dataFiles/d0DecayAsymmetry_weights.root", "RECREATE");

  TString hist_name = ""; 

  TH1F *ephi[neptbins];
  for (int i=0; i<neptbins; ++i)
  {
    hist_name = "ephi";
		hist_name += i;
    ephi[i] = new TH1F(hist_name, ";#phi;", 12, 0, 2*PI);
    ephi[i]->Sumw2(); 
  } 

  int nEntries = decayTree->GetEntries();
  cout << "There are  " << nEntries << " in the original d0 decay tree " << endl;
  for (int i=0; i<nEntries; ++i)
  {
		if (i%1000000==0) {cout << "Entry " << i << endl; }
    decayTree->GetEntry(i);
		if (pt < 0.4 || pt > 4.4) {continue;}
		if (pt1 < 1.0 || pt1 > 5.0) {continue;}
		int d0PtIndex = 0;
		for (int j=0; j<nd0ptbins; ++j)
		{
			if ( pt > d0ptbins[j] && pt < d0ptbins[j+1] )
			{
				d0PtIndex = j;
			}
		}
		double weight = 1.0 + AN[d0PtIndex]*std::cos(phi);
		//cout << "weight : " << weight << endl;
		for (int k=0; k<neptbins; ++k)
		{
			if (pt1 > eptbins[k] && pt1 < eptbins[k+1] )
			{
					ephi[k]->Fill(phi1, weight);
			}
		}
  }
  
  TF1 *eAN[neptbins];
  double eANfit[neptbins];
  for (int i=0; i<neptbins; ++i)
  {
    TString efitName =  "efit";
		efitName += i;
    eAN[i] = new TF1(efitName,"[0]*(1+[1]*cos(x+[2]))", 0, 2*PI);
    ephi[i]->Fit(eAN[i],"q");

    eANfit[i] = eAN[i]->GetParameter(1);
    cout << " e asymmetry :" <<  eAN[i]->GetParameter(1) << endl;
  }
  
  TGraph eANGraph(neptbins, eptcenters, eANfit);
  eANGraph.SetName("eANfit");
  eANGraph.SetTitle("");
  eANGraph.GetXaxis()->SetTitle("p_{T} [GeV/c]");
  eANGraph.GetXaxis()->SetTitleOffset(1.4);
  eANGraph.GetYaxis()->SetTitle("A_{N}");
  eANGraph.GetYaxis()->SetTitleOffset(1.4);
  eANGraph.SetMarkerStyle(23);
  eANGraph.SetMarkerColor(kRed);
  eANGraph.GetYaxis()->SetRangeUser(-.05, 0.05);
  eANGraph.GetXaxis()->SetLimits(0.0, 5.1);


  outFile->cd();
  for (int i=0; i<neptbins; ++i)
    {
      ephi[i]->Write();
      ephi[i]->Delete();
    }

  eANGraph.Write();
  eANGraph.Delete();

  outFile->Write();
  outFile->Close();
}
