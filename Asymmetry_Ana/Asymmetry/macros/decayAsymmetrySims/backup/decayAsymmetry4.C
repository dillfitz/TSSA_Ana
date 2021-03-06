#include <iostream>
#include <string>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TString.h"

const float PI =  3.14159265358979323846;
void decayAsymmetry4()
{

  const int neptbins = 4;
  const int njptbins = 10;
  double eptbins[neptbins+1] = {1.5, 1.8, 2.1, 2.7, 5.0};
  double jptbins[njptbins+1] = {0,0.5,1,2,3,4,6,8,10,14,20};
  float AN = -0.06;
  float Pol = 1;

  TFile *inFile = TFile::Open("jpsidecay.root");
  TTree *decayTree = (TTree*)inFile->Get( "particle" );
  float pt, pt1, pt2, phi, phi1, phi2;
  decayTree->SetBranchAddress("pt", &pt);
  decayTree->SetBranchAddress("pt1", &pt1);  
  decayTree->SetBranchAddress("pt2", &pt2);
  decayTree->SetBranchAddress("phi", &phi);  
  decayTree->SetBranchAddress("phi1", &phi1);
  decayTree->SetBranchAddress("phi2", &phi2);

  //double Nphivals[6] = {104358,133280,162068,162068,133280,104358};

  TFile *outFile = new TFile("jpsiDecayAsymmetry_ptbin4.root", "RECREATE");
  TH1F *jphi_presel = new TH1F("jphi_presel",";#phi;", 6, 0, 2*PI);
  TH1F *jphi = new TH1F("jphi",";#phi;", 6, 0, 2*PI);
  TH1F *ephi1 = new TH1F("ephi1",";#phi_{e^{+}};", 6, 0, 2*PI);
  TH1F *ephi2 = new TH1F("ephi2",";#phi_{e^{-}};", 6, 0, 2*PI);
  TH1F *ephi =  new TH1F("ephi" ,";#phi_{e};",6,0,2*PI);
  TH1F *jpt = new TH1F("jpt",";p_{T} [GeV];", 12, 0., 12.);
  TH2F *epjpsipt = new TH2F("epjpsipt",";p_{T}^{e^{+}};p_{T}^{J/#psi}",neptbins,eptbins,njptbins,jptbins);
  TH2F *emjpsipt = new TH2F("emjpsipt",";p_{T}^{e^{-}};p_{T}^{J/#psi}",neptbins,eptbins,njptbins,jptbins);
  TH2F *ejpsipt = new TH2F("ejpsipt",";p_{T}^{e};p_{T}^{J/#psi}",neptbins,eptbins,njptbins,jptbins);
  jphi_presel->Sumw2();
  jphi->Sumw2();
  ephi1->Sumw2();
  ephi2->Sumw2();
  ephi->Sumw2();
  jpt->Sumw2();
      
  decayTree->Draw("phi>>jphi_presel","pt1<1.5");
  TF1 *phiHeight = new TF1("con","[0]",0,2*PI);
  jphi_presel->Fit("con");

  float norm = 0;
  if (AN > 0)
    norm = phiHeight->GetParameter(0)/(1+Pol*AN);
  if (AN < 0)
    norm = phiHeight->GetParameter(0)/(1-Pol*AN);

  double Nphivals[6] = { norm*(1+AN*Pol*cos(PI/6)),
			 norm*(1+AN*Pol*cos(3*PI/6)),
			 norm*(1+AN*Pol*cos(5*PI/6)),
			 norm*(1+AN*Pol*cos(7*PI/6)),
			 norm*(1+AN*Pol*cos(9*PI/6)),
			 norm*(1+AN*Pol*cos(11*PI/6)) };

  int Nphi0=0; int Nphi1=0; int Nphi2=0; int Nphi3=0; int Nphi4=0; int Nphi5=0; 
  int nEntries = decayTree->GetEntries();
  cout << "There are  " << nEntries << " in the original jpsi decay tree " << endl;
  for (int i=0; i<nEntries; ++i)
  {
    decayTree->GetEntry(i);
    if ( pt1 > 1.5) {continue;}

    jpt->Fill(pt);
    epjpsipt->Fill(pt1,pt);
    emjpsipt->Fill(pt2,pt);
    ejpsipt->Fill(pt1,pt);
    ejpsipt->Fill(pt2,pt);

    if ( phi > 0 && phi < PI/3)
    {
      Nphi0++;
      if (Nphi0 > Nphivals[0]) {continue;}
      jphi->Fill(phi);
      ephi1->Fill(phi1);
      ephi2->Fill(phi2);
      ephi->Fill(phi1);
      ephi->Fill(phi2);
    }
    if ( phi > PI/3 && phi < 2*PI/3)
    {
      Nphi1++;
      if (Nphi1 > Nphivals[1]) {continue;}
      jphi->Fill(phi);
      ephi1->Fill(phi1);
      ephi2->Fill(phi2);
      ephi->Fill(phi1);
      ephi->Fill(phi2);         
    }
    if ( phi > 2*PI/3 && phi < PI)
    {
      Nphi2++; 
      if (Nphi2 > Nphivals[2]) {continue;}
      jphi->Fill(phi);
      ephi1->Fill(phi1);
      ephi2->Fill(phi2); 
      ephi->Fill(phi1);
      ephi->Fill(phi2);        
    }
    if ( phi > PI && phi < 4*PI/3)
    {
      Nphi3++;  
      if (Nphi3 > Nphivals[3]) {continue;} 
      jphi->Fill(phi);
      ephi1->Fill(phi1);
      ephi2->Fill(phi2);  
      ephi->Fill(phi1);
      ephi->Fill(phi2);  
    }
    if ( phi > 4*PI/3 && phi < 5*PI/3)
    {
      Nphi4++; 
      if (Nphi4 > Nphivals[4]) {continue;} 
      jphi->Fill(phi);
      ephi1->Fill(phi1);
      ephi2->Fill(phi2); 
      ephi->Fill(phi1);
      ephi->Fill(phi2);       
    }
    if ( phi > 5*PI/3 && phi < 2*PI)
    {
      Nphi5++;  
      if (Nphi5 > Nphivals[5]) {continue;}
      jphi->Fill(phi);
      ephi1->Fill(phi1);
      ephi2->Fill(phi2);  
      ephi->Fill(phi1);
      ephi->Fill(phi2);     
    }
  }

  TF1 *jAN = new TF1("jpsiAN","[0]*(1+[1]*cos(x))", 0, 2*PI);
  jphi->Fit(jpsiAN);

  TF1 *eAN = new TF1("eAN","[0]*(1+[1]*cos(x))", 0, 2*PI);
  ephi->Fit(eAN);

  TF1 *epAN = new TF1("epAN","[0]*(1+[1]*cos(x))", 0, 2*PI);
  ephi1->Fit(epAN);

  TF1 *emAN = new TF1("emAN","[0]*(1+[1]*cos(x))", 0, 2*PI);
  ephi2->Fit(emAN);


  float asymmetryDilution = epAN->GetParameter(1)/jAN->GetParameter(1);
  cout << "dilution factor : " << asymmetryDilution << endl;

  float asymmetryDilution_other = emAN->GetParameter(1)/jAN->GetParameter(1);
  cout << "dilution factor em : " << asymmetryDilution_other << endl;

  float asymmetryDilution_both = eAN->GetParameter(1)/jAN->GetParameter(1);
  cout << "dilution factor e : " << asymmetryDilution_both  << endl;

  cout << "input AN : " << AN << endl;
 
  outFile->cd();
  jphi_presel->Write();
  jphi->Write();
  ephi1->Write();
  ephi2->Write();
  ephi->Write();
  epjpsipt->Write();
  emjpsipt->Write();
  ejpsipt->Write();
  jpt->Write();
  jphi_presel->Delete();
  jphi->Delete();
  ephi1->Delete();
  ephi2->Delete();
  ephi->Delete();
  epjpsipt->Delete();
  emjpsipt->Delete();
  jpt->Delete();
  ejpsipt->Delete();
  outFile->Write();
  outFile->Close();
}
