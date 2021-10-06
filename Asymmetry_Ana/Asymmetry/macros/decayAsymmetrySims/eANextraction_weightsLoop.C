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

const float PI =  3.14159265358979323846;
bool verbosity = 1;
bool D0flag = 1;

void eANextraction_weightsLoop()
{
  gStyle->SetOptFit();
  const int nd0ptbins = 21;
  const int neptbins = 6;
  double eptbins[neptbins+1] = {1.0, 1.3, 1.5, 1.8, 2.1, 2.7, 5.0};
  double eptcenters[neptbins] = {1.161, 1.398, 1.639, 1.936, 2.349, 3.290};
  double d0ptbins[nd0ptbins+1] = {0.3, 0.5, 0.7, 0.9, 1.1, 1.3, 1.5, 1.7, 1.8, 2.1, 2.3, 2.5, 2.7, 2.9, 3.1, 3.3, 3.5, 3.7, 3.9, 4.1, 4.3, 4.5};
  double d0ptcenters[nd0ptbins] = {0.40, 0.60, 0.80, 1.00, 1.20, 1.40, 1.60, 1.80, 2.00, 2.20, 2.40, 2.60, 2.80, 3.00, 3.20, 3.40, 3.60, 3.80, 4.00, 4.20, 4.40};
  // Paramarization for AN D0 //
  double a0[nd0ptbins] = {-0.00027, -0.000307, -0.000322, -0.000334, -0.000342, -0.000366, -0.000381, -0.000400, -0.000422, -0.000457, -0.000481, -0.000510, -0.000541, -0.000572, -0.000614, -0.000652, -0.000677, -0.000719, -0.000760, -0.000809, -0.000853};
  double b0[nd0ptbins] = {-0.0000926, -0.000106, -0.000110, -0.000116, -0.000118, -0.000123, -0.000131, -0.000139, -0.000141, -0.000155, -0.000165, -0.000175, -0.000187, -0.000198, -0.000209, -0.000222, -0.000237, -0.000247, -0.000264, -0.000274, -0.000291};
  double a1[nd0ptbins] = {-0.155, -0.216, -0.252, -0.265, -0.267, -0.258, -0.246, -0.231, -0.220, -0.207, -0.194, -0.189, -0.174, -0.166, -0.158, -0.147, -0.144, -0.137, -0.129, -0.126, -0.119};
  double a2[nd0ptbins] = {0.0200, 0.0530, 0.0775, 0.0901, 0.0949, 0.0933, 0.0899, 0.0860, 0.0805, 0.0757, 0.0721, 0.0668, 0.0633, 0.0614, 0.0585, 0.0543, 0.0518, 0.0503, 0.0480, 0.0458, 0.0451};
  // parameters to tune to data! //
  double lambda_f = 0.07;
  double lambda_d = 0.07;
  const int Npar = 43;
  double fs[Npar] = {-1., -0.95, -0.9, -0.85, -0.8, -0.75, -0.7, -0.65, -0.6, -0.55, -0.5, -0.45, -0.4, -0.35, -0.3, -0.25, -0.2, -0.15, -0.1, -0.07, -0.05, 0.0, 0.05, 0.07, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1};
  double ds[Npar] = {-1., -0.95, -0.9, -0.85, -0.8, -0.75, -0.7, -0.65, -0.6, -0.55, -0.5, -0.45, -0.4, -0.35, -0.3, -0.25, -0.2, -0.15, -0.1, -0.07, -0.05, 0.0, 0.05, 0.07, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1};
  TString par_labels[Npar] = {"m1", "m0p95", "m0p9", "m0p85", "m0p8", "m0p75", "m0p7", "m0p65", "m0p6", "m0p55", "m0p5", "m0p45", "m0p4", "m0p35", "m0p3", "m0p25", "m0p2", "m0p15", "m0p1", "m0p07", "m0p05", "0", "0p05", "0p07", "0p1", "0p15", "0p2", "0p25", "0p3", "0p35", "0p4", "0p45", "0p5", "0p55", "0p6", "0p65", "0p7", "0p75", "0p8", "0p85", "0p9", "0p95", "1"}

  double AN[Npar][Npar][nd0ptbins]; 
  for (int j=0; j<Npar; ++j)
  {
    for (int k=0; k<Npar; ++k)
	  {
	    for (int i=0; i<nd0ptbins; ++i)
	    {
	      if (D0flag)
	        AN[j][k][i] = a0[i] + fs[j]*a1[i] + ds[k]*a2[i];
	      else
	      	AN[j][k][i] = b0[i] + fs[j]*a1[i] - ds[k]*a2[i];
	    } 
	  }
  }
  float Pol = 1; 
  if (D0flag)
  {
    TFile *inFile = TFile::Open("dataFiles/d0DecayAsymmetry_10B_weightsLoop.root");  
	  TFile *inFileData = TFile::Open("../dataFiles/bgCorrected_ohfe_AN_plus.root");    
	  TFile *outFile = new TFile("dataFiles/d0DecayAsymmetry_fitted_10B_weightsLoop.root", "RECREATE"); 
  }
  else
  {
    TFile *inFile = TFile::Open("dataFiles/antid0DecayAsymmetry_10B_weightsLoop.root");   
	  TFile *inFileData = TFile::Open("../dataFiles/bgCorrected_ohfe_AN_minus.root");    
	  TFile *outFile = new TFile("dataFiles/antid0DecayAsymmetry_fitted_10B_weightsLoop.root", "RECREATE"); 
  }

  cout << "Get histos" << endl;
  string hist_name = ""; 
  TString histname = "";
  TH1F *ephi[Npar][Npar][neptbins];
  for (int j=0; j<Npar; ++j)
  {
    for (int k=0; k<Npar; ++k)
	  {
	    for (int i=0; i<neptbins; ++i)
	    {
	      histname = "ephi";
	      histname += i;
	      histname += "_";
        histname += par_labels[j];
        histname += "_";
        histname += par_labels[k];
	      ephi[j][k][i] = (TH1F*)inFile->Get(histname);
	    }
	  } 
  } 

  cout << "Fit histos" << endl;
  TF1 *eAN[Npar][Npar][neptbins];
  double eANfit[Npar][Npar][neptbins];
  double eANfitUC[Npar][Npar][neptbins];  
  double eANfitUCx[Npar][Npar][neptbins];  
  TString efitname = "";
  string efit_name = "";
  for (int j=0; j<Npar; ++j)
  {
    cout << " j : " << j << " of 43 " << endl;
    for (int k=0; k<Npar; ++k)
	  {
	    for (int i=0; i<neptbins; ++i)
	    {
	      efitname =  "efit";
	      efitname += i;
	      efitname += "_";
        efitname += par_labels[j];
        efitname += "_";
        efitname += par_labels[k];  
	      efitname = efit_name;    
	      // got rid of floating phase // 
	      eAN[j][k][i] = new TF1(efitname,"[0]*(1+[1]*cos(x))", 0, 2*PI);
	      //eAN[j][k][i]->SetParameter(1,0.);
	      ephi[j][k][i]->Fit(eAN[j][k][i], "q");	  
	      if ( eAN[j][k][i]->GetChisquare()/eAN[j][k][i]->GetNDF() > 8. )
	        {
	          cout << "take 1 : chisq/ndf : " << eAN[j][k][i]->GetChisquare()/eAN[j][k][i]->GetNDF() << endl;  
	          ephi[j][k][i]->Fit(eAN[j][k][i],"q");	  
	          cout << "take 2 : chisq/ndf : " << eAN[j][k][i]->GetChisquare()/eAN[j][k][i]->GetNDF() << endl;  
	        } 
	      eANfit[j][k][i] = eAN[j][k][i]->GetParameter(1);
	      eANfitUC[j][k][i] = eAN[j][k][i]->GetParError(1);	
	      eANfitUCx[j][k][i] = 0;	      	            
	      ephi[j][k][i]->Write();
	      ephi[j][k][i]->Delete();
	    }
	  }
  }
  
  cout << "Make graphs" << endl;
  TGraph *d0ANGraph[Npar][Npar]; 
  TGraphErrors *eANGraph[Npar][Npar];  
  for (int j=0; j<Npar; ++j)
  {
    for (int k=0; k<Npar; ++k)
	  {
	    eANGraph[j][k] = new TGraphErrors(neptbins, eptcenters, eANfit[j][k], eANfitUCx[j][k], eANfitUC[j][k]);
	    TString graphname = "eAN";
	    graphname += "_";
      graphname += par_labels[j];
      graphname += "_";
      graphname += par_labels[k];
	    eANGraph[j][k]->SetName(graphname);
	    eANGraph[j][k]->SetTitle("");
	    eANGraph[j][k]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	    eANGraph[j][k]->GetXaxis()->SetTitleOffset(1.4);
	    eANGraph[j][k]->GetYaxis()->SetTitle("A_{N}");
	    eANGraph[j][k]->GetYaxis()->SetTitleOffset(1.4);
	    eANGraph[j][k]->SetMarkerStyle(23);
	    eANGraph[j][k]->SetMarkerColor(kRed);
	    eANGraph[j][k]->SetLineColor(kRed);
	    //eANGraph[j][k]->GetYaxis()->SetRangeUser(-0.5, 0.5);
	    eANGraph[j][k]->GetXaxis()->SetLimits(0.0, 5.1);
	    eANGraph[j][k]->Write();
	    eANGraph[j][k]->Delete();
	  
	    d0ANGraph[j][k] = new TGraph(nd0ptbins,d0ptcenters, AN[j][k]);
	    graphname = "d0AN";
	    graphname += "_";
      graphname += par_labels[j];
      graphname += "_";
      graphname += par_labels[k];
	    d0ANGraph[j][k]->SetName(graphname);
	    d0ANGraph[j][k]->SetTitle("");
	    d0ANGraph[j][k]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	    d0ANGraph[j][k]->GetXaxis()->SetTitleOffset(1.4);
	    d0ANGraph[j][k]->GetYaxis()->SetTitle("A_{N}");
	    d0ANGraph[j][k]->GetYaxis()->SetTitleOffset(1.4);
	    d0ANGraph[j][k]->SetMarkerStyle(23);
	    d0ANGraph[j][k]->SetMarkerColor(kBlue);
	    d0ANGraph[j][k]->SetLineColor(kBlue);
  	  //d0ANGraph[j][k]->GetYaxis()->SetRangeUser(-0.5, 0.5);
	    d0ANGraph[j][k]->GetXaxis()->SetLimits(0.0, 5.1);
	    d0ANGraph[j][k]->Write();
	    d0ANGraph[j][k]->Delete();
	    
	  }
  }
  

  outFile->cd();
  outFile->Write();
  outFile->Close();
}
