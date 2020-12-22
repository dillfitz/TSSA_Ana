#include <iostream>
#include <string>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TString.h"

using namespace std;

const float PI =  3.14159265358979323846;
void decayAsymmetryD0()
{
  const int neptbins = 4;
  const int nd0ptbins = 20;
  double eptbins[neptbins+1] = {1.5, 1.8, 2.1, 2.7, 5.0};
  double d0ptbins[nd0ptbins+1] = {0.40, 0.60, 0.80, 1.00, 1.20, 1.40, 1.60, 1.80, 2.00, 2.20, 2.40, 2.60, 2.80, 3.00, 3.20, 3.40, 3.60, 3.80, 4.00, 4.20, 4.40};

  float AN[nd0ptbins]; 
  //float AN = 1;
  float Pol = 1;

  TFile *inFile = TFile::Open("dataFiles/d0decay.root");
  TTree *decayTree = (TTree*)inFile->Get( "d0" );
  float pt, pt1, phi, phi1;
  decayTree->SetBranchAddress("pt", &pt);
  decayTree->SetBranchAddress("pt1", &pt1);  
  decayTree->SetBranchAddress("phi", &phi);  
  decayTree->SetBranchAddress("phi1", &phi1);

  TFile *outFile = new TFile("dataFiles/d0DecayAsymmetry.root", "RECREATE");
TH1F *d0phi_presel[nd0ptbins]; TH1F *d0phi[nd0ptbins]; TH1F *ephi[nd0ptbins];
  TH1F *d0pt = new TH1F("d0pt",";p_{T} [GeV];", 20, 0., 10.);
  TH2F *ed0pt = new TH2F("ed0pt",";p_{T}^{e^{+}};p_{T}^{D^{0}}", 10, 0.0, 5.0, 15, 0.0, 15.0);
  d0pt->Sumw2();

  float ptBinsLow[neptbins] = {1.5, 1.8, 2.1, 2.7};
  float ptBinsHigh[neptbins] = {1.8, 2.1, 2.7, 5.0};
  TString hist_labels[nd0ptbins] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12" };
  TString presel_hist_name = ""; TString hist_name = ""; TString  heightNames = "";
  TF1 *heights[nd0ptbins];
  float norms[nd0ptbins];
  double Nphivals0[nd0ptbins]; double Nphivals1[nd0ptbins]; double Nphivals2[nd0ptbins]; double Nphivals3[nd0ptbins];  double Nphivals4[nd0ptbins]; double Nphivals5[nd0ptbins];
  int Nphi0[nd0ptbins]; int Nphi1[nd0ptbins]; int Nphi2[nd0ptbins]; int Nphi3[nd0ptbins]; int Nphi4[nd0ptbins]; int Nphi5[nd0ptbins];
  for (int i=0; i<nd0ptbins; ++i)
    {
      Nphi0[i]=0; Nphi1[i]=0; Nphi2[i]=0;
      Nphi3[i]=0; Nphi4[i]=0; Nphi5[i]=0;
      AN[i] = -0.05;
      presel_hist_name = "d0phi_presel" + hist_labels[i];
      d0phi_presel[i] = new TH1F(presel_hist_name, ";#phi;", 6, 0, 2*PI);
      d0phi_presel[i]->Sumw2();

      hist_name = "d0phi" + hist_labels[i];
      d0phi[i] = new TH1F(hist_name, ";#phi;", 6, 0, 2*PI);
      d0phi[i]->Sumw2();

      hist_name = "ephi" + hist_labels[i];
      ephi[i] = new TH1F(hist_name, ";#phi;", 6, 0, 2*PI);
      ephi[i]->Sumw2();   
 
      TString temp = "phi>>" + presel_hist_name;
      TString temp2 = "";
      temp2 += "pt >"; 
      temp2 += d0ptbins[i];
      temp2 += " && pt < ";
      temp2 += d0ptbins[i+1]; 
      cout << " (" << temp << ", " << temp2 << ") " << endl;
      decayTree->Draw(temp, temp2); 

      heightNames = "con" + hist_labels[i];
      heights[i] = new TF1(heightNames,"[0]",0,2*PI);
      d0phi_presel[i]->Draw();
      d0phi_presel[i]->Fit(heights[i]);
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

  int nEntries = decayTree->GetEntries();
  cout << "There are  " << nEntries << " in the original d0 decay tree " << endl;
  for (int i=0; i<nEntries; ++i)
    {
      for (int k=0; k<nd0ptbins; ++k)
	{	  

	  decayTree->GetEntry(i);
	  d0pt->Fill(pt);
	  ed0pt->Fill(pt1,pt);
	 
	  if (pt > d0ptbins[k] && pt < d0ptbins[k+1])
	    {
	      if ( phi > 0 && phi < PI/3)
		{
		  Nphi0[k]++;
		  if (Nphi0[k] > Nphivals0[k]) {continue;} 	  
		  d0phi[k]->Fill(phi);
		  ephi[k]->Fill(phi1);
		  
		}
	      if ( phi > PI/3 && phi < 2*PI/3)
		{
		  Nphi1[k]++;
		  if (Nphi1[k] > Nphivals1[k]) {continue;}
		  d0phi[k]->Fill(phi);
		  ephi[k]->Fill(phi1);   
		  
		}
	      if ( phi > 2*PI/3 && phi < PI)
		{
		  Nphi2[k]++; 
		  if (Nphi2[k] > Nphivals2[k]) {continue;}
		  d0phi[k]->Fill(phi);
		  ephi[k]->Fill(phi1);	      
		  
		}
	      if ( phi > PI && phi < 4*PI/3)
		{
		  Nphi3[k]++;  
		  if (Nphi3[k] > Nphivals3[k]) {continue;} 
		  d0phi[k]->Fill(phi);
		  ephi[k]->Fill(phi1);  
		  
		}
	      if ( phi > 4*PI/3 && phi < 5*PI/3)
		{
		  Nphi4[k]++; 
		  if (Nphi4[k] > Nphivals4[k]) {continue;} 
		  d0phi[k]->Fill(phi);
		  ephi[k]->Fill(phi1);
		  
		}
	      if ( phi > 5*PI/3 && phi < 2*PI)
		{
		  Nphi5[k]++;  
		  if (Nphi5[k] > Nphivals5[k]) {continue;}
		  d0phi[k]->Fill(phi);
		  ephi[k]->Fill(phi1);   	      
		  
		}
	    }
	}
    }
  
  TF1 *d0AN[nd0ptbins]; TF1 *eAN[nd0ptbins];
  float asymmetryDilution[nd0ptbins];
  for (int i=0; i<nd0ptbins; ++i)
    {
      TString d0fitName =  "d0fit" + hist_labels[i];
      d0AN[i] = new TF1(d0fitName,"[0]*(1+[1]*cos(x))", 0, 2*PI);
      d0phi[i]->Fit(d0AN[i]);


      TString efitName =  "efit" + hist_labels[i];
      eAN[i] = new TF1(efitName,"[0]*(1+[1]*cos(x))", 0, 2*PI);
      ephi[i]->Fit(eAN[i]);

      asymmetryDilution[i] = eAN[i]->GetParameter(1)/d0AN[i]->GetParameter(1);
      cout << "pT bin " << i << " dilution factor : " <<  asymmetryDilution[i] << endl;
    }
 
  outFile->cd();
  d0pt->Write();
  ed0pt->Write();
  for (int i=0; i<nd0ptbins; ++i)
    {
      d0phi_presel[i]->Write();
      d0phi[i]->Write();
      ephi[i]->Write();
      d0phi_presel[i]->Delete();
      d0phi[i]->Delete();
      ephi[i]->Delete();
     
    }

  d0pt->Delete();
  ed0pt->Delete();

  outFile->Write();
  outFile->Close();
}
