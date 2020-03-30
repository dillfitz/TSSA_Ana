#define hadrons_cxx
#include "hadrons.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void hadrons::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L hadrons.C
//      Root > hadrons t
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

   // Open a file for log message output
   ofstream logfile;
   logfile.open("hadrons.log");

   // Open a file for the output ntuple
   outfile = new TFile("hadrons.root", "RECREATE");

   // Declare *histos 
   TH1F *had_plus_pt_west, *had_minus_pt_west, *had_plus_pt_east, *had_plus_pt_west, *cuts;

   had_plus_pt_west = new TH1F("west_plus","", 50, 0., 10.);
   had_plus_pt_west->Sumw2();
   had_plus_pt_west->SetMarkerStyle(21);
   had_plus_pt_west->SetMarkerColor(kBlue);
   had_plus_pt_west->GetXaxis()->SetTitle("p_{T} (GeV)");
   had_plus_pt_west->GetYaxis()->SetTitle("Counts");
   had_plus_pt_west->GetYaxis()->SetTitleOffset(1.25);

   had_minus_pt_west = new TH1F("west_minus","", 50, 0., 10.);
   had_minus_pt_west->Sumw2();
   had_minus_pt_west->SetMarkerStyle(21);
   had_minus_pt_west->SetMarkerColor(kGreen);
   had_minus_pt_west->GetXaxis()->SetTitle("p_{T} (GeV)");
   had_minus_pt_west->GetYaxis()->SetTitle("Counts");
   had_minus_pt_west->GetYaxis()->SetTitleOffset(1.25);

   had_plus_pt_east = new TH1F("east_plus","", 50, 0., 10.);
   had_plus_pt_east->Sumw2();
   had_plus_pt_east->SetMarkerStyle(22);
   had_plus_pt_east->SetMarkerSize(1.25);
   had_plus_pt_east->SetMarkerColor(kRed);
   had_plus_pt_east->GetXaxis()->SetTitle("p_{T} (GeV)");
   had_plus_pt_east->GetYaxis()->SetTitle("Counts");
   had_plus_pt_east->GetYaxis()->SetTitleOffset(1.25);

   had_minus_pt_east = new TH1F("east_minus","", 50, 0., 10.);
   had_minus_pt_east->Sumw2();
   had_minus_pt_east->SetMarkerStyle(22);
   had_minus_pt_east->SetMarkerSize(1.25);
   had_minus_pt_east->SetMarkerColor(6);
   had_minus_pt_east->GetXaxis()->SetTitle("p_{T} (GeV)");
   had_minus_pt_east->GetYaxis()->SetTitle("Counts");
   had_minus_pt_east->GetYaxis()->SetTitleOffset(1.25);

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if (Cut(ientry) < 0) continue;
      if (charge==1 && arm == 0) {had_plus_pt_east->Fill(pt);}      
      if (charge==-1 && arm == 0) {had_minus_pt_east->Fill(pt);}    
      if (charge==1 && arm == 1) {had_plus_pt_west->Fill(pt);}      
      if (charge==-1 && arm == 1) {had_minus_pt_west->Fill(pt);}     
   }

   outfile->Write();
   outfile->Close();
}
