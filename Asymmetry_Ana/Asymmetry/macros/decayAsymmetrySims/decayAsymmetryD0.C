#include <iostream>
#include <string>
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
void decayAsymmetryD0()
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
  double lambda_f = 0.07;
  double lambda_d = -0.07;

  double AN[nd0ptbins]; 
  //float AN = 1;
  float Pol = 1;

  TFile *inFile = TFile::Open("dataFiles/d0decay_100M.root");
  TTree *decayTree = (TTree*)inFile->Get( "d0" );
  float pt, pt1, phi, phi1;
  decayTree->SetBranchAddress("pt", &pt);
  decayTree->SetBranchAddress("pt1", &pt1);  
  decayTree->SetBranchAddress("phi", &phi);  
  decayTree->SetBranchAddress("phi1", &phi1);

  TFile *outFile = new TFile("dataFiles/d0DecayAsymmetry.root", "RECREATE");
  TH1F *d0phi_presel[nd0ptbins]; TH1F *d0phi[nd0ptbins]; TH1F *ephi[neptbins];
  TH1F *d0pt = new TH1F("d0pt",";p_{T} [GeV];", 40, 0., 10.);
  TH2F *ed0pt = new TH2F("ed0pt",";p_{T}^{e^{+}};p_{T}^{D^{0}}", 10, 0.0, 5.0, 15, 0.0, 15.0);
  d0pt->Sumw2();

  //float ptBinsLow[neptbins] = {1.5, 1.8, 2.1, 2.7};
  //float ptBinsHigh[neptbins] = {1.8, 2.1, 2.7, 5.0};

  TString presel_hist_name = ""; TString hist_name = ""; TString  heightNames = "";
  TF1 *heights[nd0ptbins];
  float norms[nd0ptbins];
  double Nphivals0[nd0ptbins]; double Nphivals1[nd0ptbins]; double Nphivals2[nd0ptbins]; double Nphivals3[nd0ptbins];  double Nphivals4[nd0ptbins]; double Nphivals5[nd0ptbins];
  int Nphi0[nd0ptbins]; int Nphi1[nd0ptbins]; int Nphi2[nd0ptbins]; int Nphi3[nd0ptbins]; int Nphi4[nd0ptbins]; int Nphi5[nd0ptbins];
  for (int i=0; i<nd0ptbins; ++i)
    {
      Nphi0[i]=0; Nphi1[i]=0; Nphi2[i]=0;
      Nphi3[i]=0; Nphi4[i]=0; Nphi5[i]=0;
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
      presel_hist_name = "d0phi_presel" + d0hist_labels[i];
      d0phi_presel[i] = new TH1F(presel_hist_name, ";#phi;", 6, 0, 2*PI);
      d0phi_presel[i]->Sumw2();

      hist_name = "d0phi" + d0hist_labels[i];
      d0phi[i] = new TH1F(hist_name, ";#phi;", 6, 0, 2*PI);
      d0phi[i]->Sumw2(); 
 
      TString temp = "phi>>" + presel_hist_name;
      TString temp2 = "";
      temp2 += "pt >"; 
      temp2 += d0ptbins[i];
      temp2 += " && pt < ";
      temp2 += d0ptbins[i+1]; 
      cout << " (" << temp << ", " << temp2 << ") " << endl;
      decayTree->Draw(temp, temp2); 

      heightNames = "con" + d0hist_labels[i];
      heights[i] = new TF1(heightNames,"[0]",0,2*PI);
      d0phi_presel[i]->Draw();
      d0phi_presel[i]->Fit(heights[i],"q");
      if (AN[i] > 0)
      	norms[i] = heights[i]->GetParameter(0)/(1+Pol*AN[i]);
      if (AN[i] < 0)
	norms[i] = heights[i]->GetParameter(0)/(1-Pol*AN[i]);	
      

      cout << " normalizations : " << norms[i] << endl;

      Nphivals0[i] = norms[i]*(1+AN[i]*Pol*cos(PI/6));
      Nphivals1[i] = norms[i]*(1+AN[i]*Pol*cos(3*PI/6));
      Nphivals2[i] = norms[i]*(1+AN[i]*Pol*cos(5*PI/6));
      Nphivals3[i] = norms[i]*(1+AN[i]*Pol*cos(7*PI/6));
      Nphivals4[i] = norms[i]*(1+AN[i]*Pol*cos(9*PI/6));
      Nphivals5[i] = norms[i]*(1+AN[i]*Pol*cos(11*PI/6));  
      cout << Nphivals0[i] << ", " << Nphivals1[i] << ", " << Nphivals2[i] << ", " << Nphivals3[i] << ", " << Nphivals4[i] << ", " << Nphivals5[i] << endl;

    } 

  for (int i=0; i<neptbins; ++i)
    {
      hist_name = "ephi" + hist_labels[i];
      ephi[i] = new TH1F(hist_name, ";#phi;", 6, 0, 2*PI);
      ephi[i]->Sumw2(); 
    } 


  // Need to loop over D0 pt bins in order to carve away the asymmetry, but how to correctly implement e pt bins? //
  int nEntries = decayTree->GetEntries();
  cout << "There are  " << nEntries << " in the original d0 decay tree " << endl;
  for (int i=0; i<nEntries; ++i)
    {
      decayTree->GetEntry(i);
      d0pt->Fill(pt);
      ed0pt->Fill(pt1,pt);
      for (int k=0; k<nd0ptbins; ++k)
	{	  	 
	  if (pt > d0ptbins[k] && pt < d0ptbins[k+1])
	    {
	      if ( phi > 0 && phi < PI/3)
		{
		  Nphi0[k]++;
		  if (Nphi0[k] > Nphivals0[k]) {continue;} 	  
		  d0phi[k]->Fill(phi);
		  // I tink we will have to add in a for loop here over all ept bins?? //
		  for (int j=0; j<neptbins; ++j)
		    {
		      if (pt1 > eptbins[j] && pt1 < eptbins[j+1])
			{
			  ephi[j]->Fill(phi1);
			} 
		    }		  
		}
	      if ( phi > PI/3 && phi < 2*PI/3)
		{
		  Nphi1[k]++;
		  if (Nphi1[k] > Nphivals1[k]) {continue;}
		  d0phi[k]->Fill(phi);
		  for (int j=0; j<neptbins; ++j)
		    {
		      if (pt1 > eptbins[j] && pt1 < eptbins[j+1])
			{
			  ephi[j]->Fill(phi1);
			} 
		    }		  
		}
	      if ( phi > 2*PI/3 && phi < PI)
		{
		  Nphi2[k]++; 
		  if (Nphi2[k] > Nphivals2[k]) {continue;}
		  d0phi[k]->Fill(phi);
		  for (int j=0; j<neptbins; ++j)
		    {
		      if (pt1 > eptbins[j] && pt1 < eptbins[j+1])
			{
			  ephi[j]->Fill(phi1);
			} 
		    }		  
		}
	      if ( phi > PI && phi < 4*PI/3)
		{
		  Nphi3[k]++;  
		  if (Nphi3[k] > Nphivals3[k]) {continue;} 
		  d0phi[k]->Fill(phi);
		  for (int j=0; j<neptbins; ++j)
		    {
		      if (pt1 > eptbins[j] && pt1 < eptbins[j+1])
			{
			  ephi[j]->Fill(phi1);
			} 
		    }		  
		}
	      if ( phi > 4*PI/3 && phi < 5*PI/3)
		{
		  Nphi4[k]++; 
		  if (Nphi4[k] > Nphivals4[k]) {continue;} 
		  d0phi[k]->Fill(phi);
		  for (int j=0; j<neptbins; ++j)
		    {
		      if (pt1 > eptbins[j] && pt1 < eptbins[j+1])
			{
			  ephi[j]->Fill(phi1);
			} 
		    }		  
		}
	      if ( phi > 5*PI/3 && phi < 2*PI)
		{
		  Nphi5[k]++;  
		  if (Nphi5[k] > Nphivals5[k]) {continue;}
		  d0phi[k]->Fill(phi);
		  for (int j=0; j<neptbins; ++j)
		    {
		      if (pt1 > eptbins[j] && pt1 < eptbins[j+1])
			{
			  ephi[j]->Fill(phi1);
			} 
		    }		  
		}
	    }
	}
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
  d0pt->Write();
  ed0pt->Write();
  for (int i=0; i<nd0ptbins; ++i)
    {
      d0phi_presel[i]->Write();
      d0phi[i]->Write();
      d0phi_presel[i]->Delete();
      d0phi[i]->Delete();
     
    }

  for (int i=0; i<neptbins; ++i)
    {
      ephi[i]->Write();
      ephi[i]->Delete();
    }
  d0pt->Delete();
  ed0pt->Delete();

  d0ANGraph.Write();
  d0ANfitGraph.Write();
  eANGraph.Write();

  //d0ANGraph.Delete();
  //d0ANfitGraph.Delete();
  //eANGraph.Delete();

  outFile->Write();
  outFile->Close();
}
