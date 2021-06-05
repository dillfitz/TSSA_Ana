#include <iostream>
#include <string>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TString.h"

const float PI =  3.14159265358979323846;
void decayAsymmetry_pt1p3to1p5()
{
  gStyle->SetOptStat(0);
  gStyle->SetOptFit();

  const int neptbins = 6;
  const int njptbins = 10;
  const int nAN = 12;
  double eptbins[neptbins+1] = {1.0, 1.3, 1.5, 1.8, 2.1, 2.7, 5.0};
  double jptbins[njptbins+1] = {0,0.5,1,2,3,4,6,8,10,14,20};
  //float AN = -0.06;
  double ANs[nAN]  = {-1.0, -0.75, -0.50, -0.25, -0.13, -0.06, 0.06,  0.13, 0.25, 0.50, 0.75, 1.0};
  TString AN_labels[nAN] = {"neg1p0", "neg0p75", "neg0p50", "neg0p25", "neg0p13", "neg0p06", "0p06", "0p13", "0p25", "0p50", "0p75", "1p0"};
  float Pol = 1;

  TFile *inFile = TFile::Open("dataFiles/jpsidecay.root");
  TTree *decayTree = (TTree*)inFile->Get( "particle" );
  float pt, pt1, pt2, phi, phi1, phi2;
  decayTree->SetBranchAddress("pt", &pt);
  decayTree->SetBranchAddress("pt1", &pt1);  
  decayTree->SetBranchAddress("pt2", &pt2);
  decayTree->SetBranchAddress("phi", &phi);  
  decayTree->SetBranchAddress("phi1", &phi1);
  decayTree->SetBranchAddress("phi2", &phi2);

  TFile *outFile = new TFile("dataFiles/jpsiDecayAsymmetry_ptbin1p3to1p5.root", "RECREATE");
  TH1F *jphi_presel = new TH1F("jphi_presel",";#phi;", 6, 0, 2*PI);
  TH1F *jpt = new TH1F("jpt",";p_{T} [GeV];", 12, 0., 12.);
  TH2F *epjpsipt = new TH2F("epjpsipt",";p_{T}^{e^{+}};p_{T}^{J/#psi}",neptbins,eptbins,njptbins,jptbins);
  TH2F *emjpsipt = new TH2F("emjpsipt",";p_{T}^{e^{-}};p_{T}^{J/#psi}",neptbins,eptbins,njptbins,jptbins);
  TH2F *ejpsipt = new TH2F("ejpsipt",";p_{T}^{e};p_{T}^{J/#psi}",neptbins,eptbins,njptbins,jptbins);
  TH2F *ejpsiphi = new TH2F("ejpsiphi",";#phi^{e};#phi^{J/#psi}",60,0.,2*PI,60,0.,2*PI);
  jphi_presel->Sumw2();
  jpt->Sumw2();
      
  decayTree->Draw("phi>>jphi_presel","(pt1>1.3&&pt1<1.5)||(pt2>1.3&&pt2<1.5)");
  TF1 *phiHeight = new TF1("con","[0]",0,2*PI);
  jphi_presel->Fit("con");

  TH1F *jphis[nAN];
  TH1F *epphis[nAN];
  TH1F *emphis[nAN];
  TH1F *ephis[nAN];
  double norms[nAN];
  double Nphivals2[6][nAN];

  // gStyle->SetOptFit(11111);
  // gStyle->SetOptStat(0);


  TString histname = "";
  for (int i=0; i<nAN; ++i)
    {
      histname = "jpsi_phi_pt1p3to1p5_AN_" + AN_labels[i];
      jphis[i] = new TH1F(histname, ";#phi;" ,6,0,2*PI);
      jphis[i]->Sumw2();

      histname = "ep_phi_pt1p3to1p5_AN_" +  AN_labels[i];
      epphis[i] = new TH1F(histname,";#phi;",6,0,2*PI);
      epphis[i]->Sumw2();

      histname = "em_phi_pt1p3to1p5_AN_" +  AN_labels[i];
      emphis[i] = new TH1F(histname,";#phi;",6,0,2*PI);
      emphis[i]->Sumw2();

      histname = "e_phi_pt1p3to1p5_AN_" +  AN_labels[i];
      ephis[i] = new TH1F(histname,";#phi;",6,0,2*PI);
      ephis[i]->Sumw2();

      if (ANs[i] > 0)
	norms[i] = phiHeight->GetParameter(0)/(1+Pol*ANs[i]);
      if (ANs[i] < 0)
	norms[i] = phiHeight->GetParameter(0)/(1-Pol*ANs[i]);

      cout << " normalizations : " << norms[i] << endl;


      Nphivals2[0][i] = norms[i]*(1+ANs[i]*Pol*cos(PI/6));
      Nphivals2[1][i] = norms[i]*(1+ANs[i]*Pol*cos(3*PI/6));
      Nphivals2[2][i] = norms[i]*(1+ANs[i]*Pol*cos(5*PI/6));
      Nphivals2[3][i] = norms[i]*(1+ANs[i]*Pol*cos(7*PI/6));
      Nphivals2[4][i] = norms[i]*(1+ANs[i]*Pol*cos(9*PI/6));
      Nphivals2[5][i] = norms[i]*(1+ANs[i]*Pol*cos(11*PI/6));
      cout << Nphivals2[0][i] << ", " << Nphivals2[1][i] << ", " << Nphivals2[2][i] << ", " << Nphivals2[3][i] << ", " << Nphivals2[4][i] << ", " << Nphivals2[5][i] << endl;

				   
    }

  int Nphi0=0; int Nphi1=0; int Nphi2=0; int Nphi3=0; int Nphi4=0; int Nphi5=0; 
  int nEntries = decayTree->GetEntries();
  cout << "There are  " << nEntries << " in the original jpsi decay tree " << endl;
  for (int i=0; i<nEntries; ++i)
    {
      decayTree->GetEntry(i);
      if ((pt1>1.3 && pt1 <1.5) || (pt2>1.3 && pt2 < 1.5))
	{
	  jpt->Fill(pt);
	  epjpsipt->Fill(pt1,pt);
	  emjpsipt->Fill(pt2,pt);
	  ejpsipt->Fill(pt1,pt);
	  ejpsipt->Fill(pt2,pt);
      if (pt1>1.3&&pt1<1.5)
      {
	    ejpsiphi->Fill(phi1,phi);
      }
      if (pt2>1.3&&pt2<1.5)
      {
	    ejpsiphi->Fill(phi2,phi);
	  }
	  
	  if ( phi > 0 && phi < PI/3)
	    Nphi0++;
	  if ( phi > PI/3 && phi < 2*PI/3)
	    Nphi1++;
	  if ( phi > 2*PI/3 && phi < PI)	  
	    Nphi2++; 
	  if ( phi > PI && phi < 4*PI/3)	  
	    Nphi3++; 
	  if ( phi > 4*PI/3 && phi < 5*PI/3)	  
	    Nphi4++; 
	  if ( phi > 5*PI/3 && phi < 2*PI)	  
	    Nphi5++; 
	  
	  for (int j=0; j<nAN; ++j)
	    {
	      if ( phi > 0 && phi < PI/3)
		{
		  if (Nphi0 > Nphivals2[0][j]) {continue;}
		  jphis[j]->Fill(phi);

		  if (pt1>1.3 && pt1 <1.5)
		    {
		      epphis[j]->Fill(phi1);
		      ephis[j]->Fill(phi1);
		    }
		  if (pt2>1.3 && pt2 <1.5)
		    {
		      emphis[j]->Fill(phi2);
		      ephis[j]->Fill(phi2);
		    }
		}
	      if ( phi > PI/3 && phi < 2*PI/3)
		{
		  if (Nphi1 > Nphivals2[1][j]) {continue;}
		  jphis[j]->Fill(phi);
		  if (pt1>1.3 && pt1 <1.5)
		    {
		      epphis[j]->Fill(phi1);
		      ephis[j]->Fill(phi1);
		    }
		  if (pt2>1.3 && pt2 <1.5)
		    {
		      emphis[j]->Fill(phi2);
		      ephis[j]->Fill(phi2);
		    }       
		}
	      if ( phi > 2*PI/3 && phi < PI)
		{
		  if (Nphi2 > Nphivals2[2][j]) {continue;}
		  jphis[j]->Fill(phi);
		  if (pt1>1.3 && pt1 <1.5)
		    {
		      epphis[j]->Fill(phi1);
		      ephis[j]->Fill(phi1);
		    }
		  if (pt2>1.3 && pt2 <1.5)
		    {
		      emphis[j]->Fill(phi2);
		      ephis[j]->Fill(phi2);
		    } 
		}
	      if ( phi > PI && phi < 4*PI/3)
		{
		  if (Nphi3 > Nphivals2[3][j]) {continue;} 
		  jphis[j]->Fill(phi);
		  if (pt1>1.3 && pt1 <1.5)
		    {
		      epphis[j]->Fill(phi1);
		      ephis[j]->Fill(phi1);
		    }
		  if (pt2>1.3 && pt2 <1.5)
		    {
		      emphis[j]->Fill(phi2);
		      ephis[j]->Fill(phi2);
		    }  
		}
	      if ( phi > 4*PI/3 && phi < 5*PI/3)
		{
		  if (Nphi4 > Nphivals2[4][j]) {continue;} 
		  jphis[j]->Fill(phi);
		  if (pt1>1.3 && pt1 <1.5)
		    {
		      epphis[j]->Fill(phi1);
		      ephis[j]->Fill(phi1);
		    }
		  if (pt2>1.3 && pt2 <1.5)
		    {
		      emphis[j]->Fill(phi2);
		      ephis[j]->Fill(phi2);
		    }
		}
	      if ( phi > 5*PI/3 && phi < 2*PI)
		{
		  if (Nphi5 > Nphivals2[5][j]) {continue;}
		  jphis[j]->Fill(phi);
		  if (pt1>1.3 && pt1 <1.5)
		    {
		      epphis[j]->Fill(phi1);
		      ephis[j]->Fill(phi1);
		    }
		  if (pt2>1.3 && pt2 <1.5)
		    {
		      emphis[j]->Fill(phi2);
		      ephis[j]->Fill(phi2);
		    } 
		}
	    }
	}
    }
  
  TF1 *jAN = new TF1("jpsiAN","[0]*(1+[1]*cos(x))", 0, 2*PI);
  TF1 *eAN = new TF1("eAN","[0]*(1+[1]*cos(x))", 0, 2*PI);
  TF1 *epAN = new TF1("epAN","[0]*(1+[1]*cos(x))", 0, 2*PI);
  TF1 *emAN = new TF1("emAN","[0]*(1+[1]*cos(x))", 0, 2*PI);

  double dilutions[nAN];
  for (int i=0; i<nAN; ++i)
    {
      jphis[i]->Fit(jpsiAN);
      epphis[i]->Fit(epAN);
      emphis[i]->Fit(emAN);
      ephis[i]->Fit(eAN);

      cout << "case : " << i << " input AN : " << ANs[i] << endl;

      float asymmetryDilution_p = epAN->GetParameter(1)/jAN->GetParameter(1);
      cout << "dilution factor ep : " << asymmetryDilution_p << endl;
      
      float asymmetryDilution_m = emAN->GetParameter(1)/jAN->GetParameter(1);
      cout << "dilution factor em : " << asymmetryDilution_m << endl;

      float asymmetryDilution = eAN->GetParameter(1)/jAN->GetParameter(1);
      cout << "dilution factor e : " << asymmetryDilution  << endl;

      dilutions[i] = asymmetryDilution;

    }
 
  double ymin = dilutions[0] - 0.05;  double ymax = dilutions[0] + 0.05;
  TGraph dilutionGraph(nAN, ANs, dilutions);
  dilutionGraph.SetMarkerStyle(21);
  dilutionGraph.SetName("dilutions");
  dilutionGraph.GetXaxis()->SetTitle("A_{N}");
  dilutionGraph.GetYaxis()->SetTitle("d");
  dilutionGraph.GetYaxis()->SetTitleOffset(1.5);
  dilutionGraph.GetYaxis()->SetRangeUser(ymin, ymax);
  dilutionGraph.SetTitle("");
  dilutionGraph.Draw("AP");

  TBox box(ANs[3], ymin, ANs[7], ymax);
  box.SetFillColor(kGray+2);
  box.SetFillStyle(3002);
  TLine nomy(-1.2, dilutions[5], 1.2, dilutions[5]);
  TLine nomx(ANs[5], ymin, ANs[5], ymax);
  box.Draw("SAME");
  nomx.Draw("SAME");
  nomy.Draw("SAME");
  c1->SaveAs("images/dilutions_ptbin1p3to1p5.png");

  TCanvas *c2 = new TCanvas("c2");
  jphis[5]->Draw();
  c2->SaveAs("images/jphiAN_pt1p3to1p5.png");
  
  TCanvas *c3 = new TCanvas("c3");
  ephis[5]->Draw();
  c3->SaveAs("images/ephiAN_pt1p3to1p5.png");

  TCanvas *c4 = new TCanvas("c4");
  ejpsiphi->Draw("COLZ");
  c4->SaveAs("images/ejpsiphi_pt1p3to1p5.png");
  outFile->cd();

  for (int i=0; i<nAN; ++i)
    {
      jphis[i]->Write();
      epphis[i]->Write();
      emphis[i]->Write();
      ephis[i]->Write();

      jphis[i]->Delete();
      epphis[i]->Delete();
      emphis[i]->Delete();
      ephis[i]->Delete();
    }

  jphi_presel->Write();

  epjpsipt->Write();
  emjpsipt->Write();
  ejpsipt->Write();
  ejpsiphi->Write();
  jpt->Write();
  dilutionGraph.Write();

  jphi_presel->Delete();
  epjpsipt->Delete();
  emjpsipt->Delete();
  ejpsiphi->Delete();
  jpt->Delete();
  ejpsipt->Delete();
  //dilutionGraph.Delete();

  outFile->Write();
  outFile->Close();
}
