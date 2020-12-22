#include <iostream>
#include <string>
#include <vector>
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
void eANextraction()
{
  const int neptbins = 15;
  //const int neptbins = 25;
  const int nd0ptbins = 21;
  double eptbins[neptbins+1] =  {0.00, 0.20, 0.40, 0.60, 0.80, 1.00, 1.20, 1.40, 1.60, 1.80, 2.00, 2.20, 2.40, 2.60, 2.80, 5.00};
  double eptcenters[neptbins] =  {0.10, 0.30, 0.50, 0.70, 0.90, 1.10, 1.30, 1.50, 1.70, 1.90, 2.10, 2.30, 2.50, 2.70, 3.90};

  //double eptbins[neptbins+1] =  {0.00, 0.20, 0.40, 0.60, 0.80, 1.00, 1.20, 1.40, 1.60, 1.80, 2.00, 2.20, 2.40, 2.60, 2.80, 3.00, 3.20, 3.40, 3.60, 3.80, 4.00, 4.20, 4.40, 4.60, 4.80, 5.00};
  //double eptcenters[neptbins] =  {0.10, 0.30, 0.50, 0.70, 0.90, 1.10, 1.30, 1.50, 1.70, 1.90, 2.10, 2.30, 2.50, 2.70, 2.90, 3.10, 3.30, 3.50, 3.70, 3.90, 4.10, 4.30, 4.50, 4.70, 4.90};


  double d0ptbins[nd0ptbins+1] = {0.30, 0.50, 0.70, 0.90, 1.10, 1.30, 1.50, 1.70, 1.90, 2.10, 2.30, 2.50, 2.70, 2.90, 3.10, 3.30, 3.50, 3.70, 3.90, 4.10, 4.30, 4.50};
  double d0ptcenters[nd0ptbins] = {0.40, 0.60, 0.80, 1.00, 1.20, 1.40, 1.60, 1.80, 2.00, 2.20, 2.40, 2.60, 2.80, 3.00, 3.20, 3.40, 3.60, 3.80, 4.00, 4.20, 4.40};


  TString hist_labels[neptbins] = { "100MeV", "300MeV", "500MeV", "700MeV", "900MeV", "1100MeV", "1300MeV", "1500MeV", "1700MeV", "1900MeV", "2100MeV", "2300MeV","2500MeV","2700MeV","3900MeV" };
  //TString hist_labels[neptbins] = { "100MeV", "300MeV", "500MeV", "700MeV", "900MeV", "1100MeV", "1300MeV", "1500MeV", "1700MeV", "1900MeV", "2100MeV", "2300MeV","2500MeV","2700MeV", "2900MeV", "3100MeV", "3300MeV", "3500MeV", "3700MeV","3900MeV", "4100MeV", "4300MeV", "4500MeV", "4700MeV", "4900MeV"};

  TString d0hist_labels[nd0ptbins] = {"400MeV", "600MeV", "800MeV", "1000MeV", "1200MeV", "1400MeV", "1600MeV", "1800MeV", "2000MeV", "2200MeV","2400MeV","2600MeV","2800MeV","3000MeV","3200MeV","3400MeV","3600MeV","3800MeV","4000MeV","4200MeV","4400MeV"};


  // Paramarization for AN D0 //
  double a0[nd0ptbins] = {-0.00027, -0.000307, -0.000322, -0.000334, -0.000342, -0.000366, -0.000381, -0.000400, -0.000422, -0.000457, -0.000481, -0.000510, -0.000541, -0.000572, -0.000614, -0.000652, -0.000677, -0.000719, -0.000760, -0.000809, -0.000853};
  double b0[nd0ptbins] = {-0.0000926, -0.000106, -0.000110, -0.000116, -0.000118, -0.000123, -0.000131, -0.000139, -0.000141, -0.000155, -0.000165, -0.000175, -0.000187, -0.000198, -0.000209, -0.000222, -0.000237, -0.000247, -0.000264, -0.000274, -0.000291};
  double a1[nd0ptbins] = {-0.155, -0.216, -0.252, -0.265, -0.267, -0.258, -0.246, -0.231, -0.220, -0.207, -0.194, -0.189, -0.174, -0.166, -0.158, -0.147, -0.144, -0.137, -0.129, -0.126, -0.119};
  double a2[nd0ptbins] = {0.0200, 0.0530, 0.0775, 0.0901, 0.0949, 0.0933, 0.0899, 0.0860, 0.0805, 0.0757, 0.0721, 0.0668, 0.0633, 0.0614, 0.0585, 0.0543, 0.0518, 0.0503, 0.0480, 0.0458, 0.0451};
  // parameters to tune to data! //
  bool D0flag = 0;
  double lambda_f = 0.07;
  double lambda_d = 0.07;

  double AN[nd0ptbins];
  TH1F *d0phi_presel[nd0ptbins]; TH1F *d0phi[nd0ptbins]; TH1F *ephi[neptbins];
 
  if(D0flag)
    {
      if (lambda_f == 0.07 && lambda_d == 0.07)
	TFile *inFile = TFile::Open("dataFiles/d0DecayAsymmetry_10B_lamfdp07_prefit.root");
      if (lambda_f == 0.07 && lambda_d == -0.07)
	TFile *inFile = TFile::Open("dataFiles/d0DecayAsymmetry_10B_lamfdpm07_prefit.root");
      if (lambda_f == 0.00 && lambda_d == 0.00)
       	TFile *inFile = TFile::Open("dataFiles/d0DecayAsymmetry_10B_lamfd0_prefit.root");
    }
  else
    {
      if (lambda_f == 0.07 && lambda_d == 0.07)
	TFile *inFile = TFile::Open("dataFiles/antid0DecayAsymmetry_10B_lamfdp07_prefit.root");
      if (lambda_f == 0.07 && lambda_d == -0.07)
	TFile *inFile = TFile::Open("dataFiles/antid0DecayAsymmetry_10B_lamfdpm07_prefit.root");
      if (lambda_f == 0.00 && lambda_d == 0.00)
	TFile *inFile = TFile::Open("dataFiles/antid0DecayAsymmetry_10B_lamfd0_prefit.root");
    }

  if(D0flag)
    {
      if (lambda_f == 0.07 && lambda_d == 0.07)
	TFile *outFile = new TFile("dataFiles/d0DecayAsymmetry_10B_lamfdp07.root", "RECREATE");
      if (lambda_f == 0.07 && lambda_d == -0.07)
	TFile *outFile = new TFile("dataFiles/d0DecayAsymmetry_10B_lamfdpm07.root", "RECREATE");
      if (lambda_f == 0.00 && lambda_d == 0.00)
	TFile *outFile = new TFile("dataFiles/d0DecayAsymmetry_10B_lamfd0.root", "RECREATE");
    }
  else   
    {
      if (lambda_f == 0.07 && lambda_d == 0.07)
	TFile *outFile = new TFile("dataFiles/antid0DecayAsymmetry_10B_lamfdp07.root", "RECREATE");
      if (lambda_f == 0.07 && lambda_d == -0.07)
	TFile *outFile = new TFile("dataFiles/antid0DecayAsymmetry_10B_lamfdpm07.root", "RECREATE");
      if (lambda_f == 0.00 && lambda_d == 0.00)
	TFile *outFile = new TFile("dataFiles/antid0DecayAsymmetry_10B_lamfd0.root", "RECREATE");
    }
  for(int i=0; i<nd0ptbins; ++i)
    {
      if (D0flag)
	AN[i] = a0[i] + lambda_f*a1[i] + lambda_d*a2[i];
      else
	AN[i] = b0[i] + lambda_f*a1[i] - lambda_d*a2[i];

      hist_name = "d0phi" + d0hist_labels[i];
      d0phi[i] = (TH1F*)inFile->Get(hist_name);
    }

  for (int i=0; i<neptbins; ++i)
    {
      hist_name = "ephi" + hist_labels[i];
      ephi[i] = (TH1F*)inFile->Get(hist_name);
    } 
  
  TF1 *d0AN[nd0ptbins]; TF1 *eAN[neptbins];
  double d0ANfit[nd0ptbins];
  double eANfit[neptbins];
  for (int i=0; i<nd0ptbins; ++i)
    {
      TString d0fitName =  "d0fit" + d0hist_labels[i];
      d0AN[i] = new TF1(d0fitName,"[0]*(1+[1]*cos(x))", 0, 2*PI);
      d0phi[i]->Fit(d0AN[i],"q");

      d0ANfit[i] = d0AN[i]->GetParameter(1);
      cout << "d0 asymmetry : " << d0AN[i]->GetParameter(1) << endl;
    }

  for (int i=0; i<neptbins; ++i)
    {
      TString efitName =  "efit" + hist_labels[i];
      eAN[i] = new TF1(efitName,"[0]*(1+[1]*cos(x))", 0, 2*PI);
      ephi[i]->Fit(eAN[i],"q");

      eANfit[i] = eAN[i]->GetParameter(1);
      cout << " e asymmetry :" <<  eAN[i]->GetParameter(1) << endl;
    }

  // Name the graphs!
  TGraph d0ANGraph(nd0ptbins, d0ptcenters, AN);
  d0ANGraph.SetName("d0AN");
  d0ANGraph.SetTitle("");
  d0ANGraph.GetXaxis()->SetTitle("p_{T} [GeV/c]");
  d0ANGraph.GetXaxis()->SetTitleOffset(1.4);
  d0ANGraph.GetYaxis()->SetTitle("A_{N}");
  d0ANGraph.GetYaxis()->SetTitleOffset(1.4);
  d0ANGraph.SetMarkerStyle(21);
  d0ANGraph.SetMarkerColor(kBlack);

  d0ANGraph.Draw("AP");
  
  //c1->SaveAs("images/dilutions_ptbin1.png");

  TGraph d0ANfitGraph(nd0ptbins, d0ptcenters, d0ANfit);
  d0ANfitGraph.SetName("d0ANfit");
  d0ANfitGraph.SetTitle("");
  d0ANfitGraph.GetXaxis()->SetTitle("p_{T} [GeV/c]");
  d0ANfitGraph.GetXaxis()->SetTitleOffset(1.4);
  d0ANfitGraph.GetYaxis()->SetTitle("A_{N}");
  d0ANfitGraph.GetYaxis()->SetTitleOffset(1.4);
  d0ANfitGraph.SetMarkerStyle(22);
  d0ANfitGraph.SetMarkerColor(kBlue);


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
  //d0pt->Write();
  //ed0pt->Write();

  //d0pt->Delete();
  //ed0pt->Delete();

  d0ANGraph.Write();
  d0ANfitGraph.Write();
  eANGraph.Write();

  //d0ANGraph.Delete();
  //d0ANfitGraph.Delete();
  //eANGraph.Delete();

  outFile->Write();
  outFile->Close();
}
