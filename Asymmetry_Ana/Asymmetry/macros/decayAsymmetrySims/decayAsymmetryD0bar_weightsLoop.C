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
void decayAsymmetryD0bar_weightsLoop()
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
  
  double AN[Npar][Npar][nd0ptbins]; 
  for (int j=0; j<Npar; ++j)
    {
      for (int k=0; k<Npar; ++k)
	{
	  for (int i=0; i<nd0ptbins; ++i)
	    {
	      AN[j][k][i] = b0[i] + fs[j]*a1[i] - ds[k]*a2[i];
	    } 
	}
    }
  float Pol = 1;
  
  TFile *inFile = TFile::Open("dataFiles/antid0decay_100M.root");
  TTree *decayTree = (TTree*)inFile->Get( "d0bar" );
  float pt, pt1, phi, phi1;
  decayTree->SetBranchAddress("pt", &pt);
  decayTree->SetBranchAddress("pt1", &pt1);  
  decayTree->SetBranchAddress("phi", &phi);  
  decayTree->SetBranchAddress("phi1", &phi1);
  
  TFile *outFile = new TFile("dataFiles/antid0DecayAsymmetry_weightsLoop.root", "RECREATE");
  
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
	      ostringstream ss;
	      ss<<"_"<<fs[j]<<"_"<<ds[k];
	      histname += ss.str();
	      string hist_name = histname;
	      replace(hist_name.begin(), hist_name.end(), '.', 'p');
	      replace(hist_name.begin(), hist_name.end(), '-', 'm');
	      histname = hist_name;
	      ephi[j][k][i] = new TH1F(histname, ";#phi;", 12, 0, 2*PI);
	      ephi[j][k][i]->Sumw2();
	    }
	} 
    } 
  
  int nEntries = decayTree->GetEntries();
  cout << "There are  " << nEntries << " in the original d0 decay tree " << endl;
  for (int i=0; i<nEntries; ++i)
    {
      if (i%1000000==0) {cout << "Entry " << i << endl; }
      decayTree->GetEntry(i);
      if (pt < 0.3 || pt > 4.5) {continue;}
      if (pt1 < 1.0 || pt1 > 5.0) {continue;}
      for (int l=0; l<Npar; ++l)
	{
	  for(int m=0; m<Npar; ++m)
	    {
	      int d0PtIndex = 0;
	      for (int k=0; j<nd0ptbins; ++j)
		{
		  if ( pt > d0ptbins[j] && pt < d0ptbins[j+1] )
		    {
		      d0PtIndex = j;
		    }
		}
	      double weight = 1.0 + AN[l][m][d0PtIndex]*std::cos(phi);
	      //cout << "weight : " << weight << endl;
	      for (int k=0; k<neptbins; ++k)
		{
		  if (pt1 > eptbins[k] && pt1 < eptbins[k+1] )
		    {
		      ephi[l][m][k]->Fill(phi1, weight);
		    }
		}
	    }
	}
    }
  
  TF1 *eAN[neptbins];
  double eANfit[Npar][Npar][neptbins];
  for (int j=0; j<Npar; ++j)
    {
      for (int k=0; k<Npar; ++k)
	{
	  for (int i=0; i<neptbins; ++i)
	    {
	      TString efitName =  "efit";
	      efitName += i;
	      eAN[i] = new TF1(efitName,"[0]*(1+[1]*cos(x+[2]))", 0, 2*PI);
	      ephi[j][k][i]->Fit(eAN[i],"q");
	      
	      eANfit[j][k][i] = eAN[i]->GetParameter(1);
	    }
	}
    }
  
  TGraph *d0ANGraph[Npar][Npar]; 
  TGraph *eANGraph[Npar][Npar];
  for (int j=0; j<Npar; ++j)
    {
      for (int k=0; k<Npar; ++k)
	{
	  eANGraph[j][k] = new TGraph(neptbins, eptcenters, eANfit[j][k]);
	  ostringstream ss;
	  ss<<"_"<<fs[j]<<"_"<<ds[k];
	  
	  TString graphname = "eAN";
	  graphname += ss.str();
	  string graph_name = graphname;
	  replace(graph_name.begin(), graph_name.end(), '.', 'p');
	  replace(graph_name.begin(), graph_name.end(), '-', 'm');
	  graphname = graph_name;
	  eANGraph[j][k]->SetName(graphname);
	  eANGraph[j][k]->SetTitle("");
	  eANGraph[j][k]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	  eANGraph[j][k]->GetXaxis()->SetTitleOffset(1.4);
	  eANGraph[j][k]->GetYaxis()->SetTitle("A_{N}");
	  eANGraph[j][k]->GetYaxis()->SetTitleOffset(1.4);
	  eANGraph[j][k]->SetMarkerStyle(23);
	  eANGraph[j][k]->SetMarkerColor(kRed);
	  eANGraph[j][k]->SetLineColor(kRed);
	  eANGraph[j][k]->GetYaxis()->SetRangeUser(-0.5, 0.5);
	  eANGraph[j][k]->GetXaxis()->SetLimits(0.0, 5.1);
	  eANGraph[j][k]->Write();
	  eANGraph[j][k]->Delete();
	  
	  d0ANGraph[j][k] = new TGraph(nd0ptbins,d0ptcenters, AN[j][k]);
	  graphname = "antid0AN";
	  graphname += ss.str();
	  string graph_name = graphname;
	  replace(graph_name.begin(), graph_name.end(), '.', 'p');
	  replace(graph_name.begin(), graph_name.end(), '-', 'm');
	  graphname = graph_name;
	  d0ANGraph[j][k]->SetName(graphname);
	  d0ANGraph[j][k]->SetTitle("");
	  d0ANGraph[j][k]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	  d0ANGraph[j][k]->GetXaxis()->SetTitleOffset(1.4);
	  d0ANGraph[j][k]->GetYaxis()->SetTitle("A_{N}");
	  d0ANGraph[j][k]->GetYaxis()->SetTitleOffset(1.4);
	  d0ANGraph[j][k]->SetMarkerStyle(23);
	  d0ANGraph[j][k]->SetMarkerColor(kBlue);
	  d0ANGraph[j][k]->SetLineColor(kBlue);
  	  d0ANGraph[j][k]->GetYaxis()->SetRangeUser(-0.5, 0.5);
	  d0ANGraph[j][k]->GetXaxis()->SetLimits(0.0, 5.1);
	  d0ANGraph[j][k]->Write();
	  d0ANGraph[j][k]->Delete();
	}
    }
  outFile->cd();
  for (int j=0; j<Npar; ++j)
    {
      for (int k=0; k<Npar; ++k)
	{
	  for (int i=0; i<neptbins; ++i)
	    {
	      ephi[j][k][i]->Write();
	      ephi[j][k][i]->Delete();
	    }
	}
    }
  outFile->Write();
  outFile->Close();
}
