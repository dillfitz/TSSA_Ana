#define JPsi_Ana_cxx
#include "JPsi_Ana.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>

void JPsi_Ana::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L JPsi_Ana.C
//      Root > JPsi_Ana t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   // Open a file for log message output
   ofstream logfile;
   logfile.open("jpsi.log");

   // Open a file for the output ntuple
   outfile = new TFile("jpsi.root", "RECREATE");

   // Decalare some histograms why not //
   const int nPt_bins_ee = 5, nPt_bins_e = 10;
   TH1F *M_ee_binned_ee[nPt_bins_ee], *M_ee_binned_e1[nPt_bins_e], *M_ee_binned_e2[nPt_bins_e]; 
   TH1F *M_ee, *pt_ee;
   TH1I *cuts;

   float Pt_bins_low_ee[nPt_bins_ee] = {0.0, 0.5, 1.0, 2.0, 5.0};
   float Pt_bins_high_ee[nPt_bins_ee] = {0.5, 1.0, 2.0, 5.0, 10.0};
   float Pt_bins_low_e[nPt_bins_e] = {1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.5, 4.0, 4.5, 5.0};
   float Pt_bins_high_e[nPt_bins_e] = {1.8, 2.1, 2.4, 2.7, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0};

   TString hist_labels_ee[nPt_bins_ee] = { "_0.0_0.5","_0.5_1.0", "_1.0_2.0", "_2.0_5.0", "_5.0_10.0"};
   TString hist_labels_e[nPt_bins_e] = { "_1.5_1.8","_1.8_2.1", "_2.1_2.4", "_2.4_2.7", "_2.7_3.0", "_3.0_3.5", "_3.5_4.0", "_4.0_4.5", "_4.5_5.0", "_5.0_6.0"};

   for (int i=0; i<nPt_bins_ee; i++) {
      hist_name = "M_ee" + hist_labels_ee[i];
      M_ee_binned_ee[i] = new TH1F(hist_name, hist_name, 30, 2.7, 4.2);
      M_ee_binned_ee[i]->Sumw2();
   } 

   for (int i=0; i<nPt_bins_e; i++) {
      hist_name = "M_e1" + hist_labels_e[i];
      M_ee_binned_e1[i] = new TH1F(hist_name, hist_name, 30, 2.7, 4.2);
      M_ee_binned_e1[i]->Sumw2();

      hist_name = "M_e2" + hist_labels_e[i];
      M_ee_binned_e2[i] = new TH1F(hist_name, hist_name, 30, 2.7, 4.2);
      M_ee_binned_e2[i]->Sumw2();
   } 

   M_ee = new TH1F("M_ee","M_ee", 60, 2.7, 4.2);
   M_ee->Sumw2();

   pt_ee = new TH1F("pt_ee","pt_ee", 40, 0., 10.);
   pt_ee->Sumw2();

   cuts = new TH1I("cuts", "cuts", 20, 0, -20);

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      // Check out how many candidates could be lost to each cut //
      // Keep in mind there would be overlap //
      cuts->Fill(Cut(ientry));
      if (Cut(ientry) < 0) continue;

      for (int i=0; i<nPt_bins_ee; i++) {
         if(pt > Pt_bins_low_ee[i] && pt < Pt_bins_high_ee[i]) {
            M_ee_binned_ee[i]->Fill(mass);
         }      
      }

      for (int i=0; i<nPt_bins_e; i++) {
         if(pt1 > Pt_bins_low_e[i] && pt1 < Pt_bins_high_e[i]) {
            M_ee_binned_e1[i]->Fill(mass);
         }
         if(pt2 > Pt_bins_low_e[i] && pt2 < Pt_bins_high_e[i]) {
            M_ee_binned_e2[i]->Fill(mass);
         }         
      }

      M_ee->Fill(mass);
      pt_ee->Fill(mass);
      if (ientry<10) { logfile << mass << endl; }
   }
   outfile->Write();
   outfile->Close();
}
