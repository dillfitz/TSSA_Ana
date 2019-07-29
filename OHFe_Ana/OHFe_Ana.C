#define OHFe_Ana_cxx
#include "OHFe_Ana.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void OHFe_Ana::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L OHFe_Ana.C
//      Root > OHFe_Ana t
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
   logfile.open("ohfe.log");

   // Open a file for the output ntuple
   outfile = new TFile("ohfe.root", "RECREATE");

   // Declaration of output histograms 
   bool cut_eff = 1; // Set this to fill histogram for determining cut efficiency.
   int cutval = 0;
   const int nPt_bins = 10;
   TH1F *e_dcat_binned[nPt_bins], *e_dep_binned[nPt_bins]; 
   TH1F *e_prob_binned[nPt_bins], *e_chisq_ndf_binned[nPt_bins];
   TH1I *e_n0_binned[nPt_bins], *e_nhit_binned[nPt_bins];
   TH1F *e_dcat, *e_pt, *e_dep, *e_eop;
   TH1F *e_ecore, *e_prob, *e_chisq_ndf, *cuts;
   TH1I *e_n0, *e_nhit;
   
   float Pt_bins_low[nPt_bins] = {1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.5, 4.0, 4.5, 5.0};
   float Pt_bins_high[nPt_bins] = {1.8, 2.1, 2.4, 2.7, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0};

   TString hist_labels[nPt_bins] = { "_1.5_1.8","_1.8_2.1", "_2.1_2.4", "_2.4_2.7", "_2.7_3.0", "_3.0_3.5", "_3.5_4.0", "_4.0_4.5", "_4.5_5.0", "_5.0_6.0"};


   // Initialize some histograms why not //
   
   for (int i=0; i<nPt_bins; i++) {
      hist_name = "dcat" + hist_labels[i];
      e_dcat_binned[i] = new TH1F(hist_name, hist_name, 40, -0.1, 0.1);
      e_dcat_binned[i]->Sumw2();

      hist_name = "dep" + hist_labels[i];
      e_dep_binned[i] = new TH1F(hist_name, hist_name, 50, -5., 5.);
      e_dep_binned[i]->Sumw2();

      hist_name = "prob" + hist_labels[i];
      e_prob_binned[i] = new TH1F(hist_name, hist_name, 20, 0., 1.);
      e_prob_binned[i]->Sumw2();

      hist_name = "chisq_ndf" + hist_labels[i];
      e_chisq_ndf_binned[i] = new TH1F(hist_name, hist_name, 50, 0., 10.);
      e_chisq_ndf_binned[i]->Sumw2();

      hist_name = "n0" + hist_labels[i];
      e_n0_binned[i] = new TH1I(hist_name, hist_name, 20, 0, 20);
      e_n0_binned[i]->Sumw2();

      hist_name = "nhits" + hist_labels[i];
      e_nhit_binned[i] = new TH1I(hist_name, hist_name, 20, 0, 20);
      e_nhit_binned[i]->Sumw2();      
   }
    
   e_dcat = new TH1F("dcat","dcat", 40, -0.1, 0.1);
   e_dcat->Sumw2();

   e_pt = new TH1F("pt","pt", 45, 1.5, 6.);
   e_pt->Sumw2();

   e_dep = new TH1F("dep","dep", 50, -5., 5.);
   e_dep->Sumw2();

   e_eop = new TH1F("eop","eop", 80, 0., 2.);
   e_eop->Sumw2();

   e_ecore = new TH1F("ecore","ecore", 50, 0., 10.);
   e_ecore->Sumw2();

   e_prob = new TH1F("prob","prob", 20, 0., 1.);
   e_prob->Sumw2();

   e_chisq_ndf = new TH1F("chisq_ndf", "chisq_ndf", 50, 0., 10.);
   e_chisq_ndf->Sumw2();

   e_n0 = new TH1I("n0","n0", 20, 0, 20);

   e_nhit = new TH1I("nhit","nhit", 20, 0, 20);

   cuts = new TH1F("cuts","cuts", 20, -15, 5);
   //cuts_efficiency = new TH1F("cuts_e","cuts_e", 20, 0., 1.);

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      // STILL NEED TO ADD CONVERSION VETO HERE!! //
      for (int i=0; i<nPt_bins; i++) {
         if(pt > Pt_bins_low[i] && pt < Pt_bins_high[i]) {
            if(abs(sigemcdphi)<3.&&abs(sigemcdz)<3.&&abs(zed)<75.&&disp<5.&&(quality==63||quality==31)&&(hitpattern&3)==3&&conversionveto2x==1) {
               if (pt<5.) {
                  if (n0>1 && prob>0.01 && nhit>2 && chisq/ndf<3.        ) { e_dep_binned[i]->Fill(dep); }
                  if (n0>1 && prob>0.01 && nhit>2 && abs(dep)<2.         ) { e_chisq_ndf_binned[i]->Fill(chisq/ndf); }                  
                  if (n0>1 && prob>0.01 && abs(dep)<2. && chisq/ndf<3.   ) { e_nhit_binned[i]->Fill(nhit); }
                  if (n0>1 && abs(dep)>2. && nhit>2 && chisq/ndf<3.      ) { e_prob_binned[i]->Fill(prob); } 
                  if (prob>0.01 && abs(dep)<2. && nhit>2 && chisq/ndf<3. ) { e_n0_binned[i]->Fill(n0); }                                                                                         
               }
                         
               else {
                  if (n0>3 && prob>0.2 && nhit>2 && chisq/ndf<3.        ) { e_dep_binned[i]->Fill(dep); }
                  if (n0>3 && prob>0.2 && nhit>2 && abs(dep)<2.         ) { e_chisq_ndf_binned[i]->Fill(chisq/ndf); }                  
                  if (n0>3 && prob>0.2 && abs(dep)<2. && chisq/ndf<3.   ) { e_nhit_binned[i]->Fill(nhit); }
                  if (n0>3 && abs(dep)>2. && nhit>2 && chisq/ndf<3.     ) { e_prob_binned[i]->Fill(prob); } 
                  if (prob>0.2 && abs(dep)<2. && nhit>2 && chisq/ndf<3. ) { e_n0_binned[i]->Fill(n0); }  
               }
            }
         }  
      }
      
      // Fill cuts hist to determine efficiency of selection criteria //
      if (cut_eff) {
         for (int i=0; i<13; i++) {
            if ((pt>1.5 && pt < 6.0) && i==0)                     {cuts->Fill(-1);}
            if ((conversionveto2x==1) && i==1)                    {cuts->Fill(-2);}
            if ((abs(dep)<2.) && i==2)                            {cuts->Fill(-3);}
            if ((abs(sigemcdphi)<3.&& abs(sigemcdz)<3.) && i==3)  {cuts->Fill(-4);}
            if ((abs(zed)<75) && i==4)                            {cuts->Fill(-5);}
            if ((disp<5.) && i==5)                                {cuts->Fill(-6);} 
            if (ndf!=0 && i==6) {
               if ((chisq/ndf < 3.) && i==6)                      {cuts->Fill(-7);}
            }
            if ((nhit > 2 ) && i==7)                              {cuts->Fill(-8);}
            if ((quality == 63 || quality ==31) && i==8)          {cuts->Fill(-9);}
            if (((hitpattern&3)==3) && i==9)                      {cuts->Fill(-10);}
            if (pt<5.) {
               if ((n0 > 1 ) && i==10)                            {cuts->Fill(-11);}
               if ((prob > 0.01) && i==11)                        {cuts->Fill(-12);}
            }
            else {
               if ((n0 > 3 ) && i==10)                            {cuts->Fill(-11);}
               if ((prob > 0.2) && i==11)                         {cuts->Fill(-12);}
            }
            if (ndf==0)                                           {cuts->Fill(-13);}

            if (i==0) {cuts->Fill(0);}
         }
      }
      cutval = Cut(ientry);
      if (!(cutval<0)) { cuts->Fill(1);}
      if (cutval<0)    {continue; }   
      for (int i=0; i<nPt_bins; i++) {
         if(pt > Pt_bins_low[i] && pt < Pt_bins_high[i]) {
            e_dcat_binned[i]->Fill(dcat);
         }      
      }

      e_dcat->Fill(dcat);
      e_pt->Fill(pt);
      e_dep->Fill(dep);
      e_eop->Fill(ecore/mom);
      e_ecore->Fill(ecore);
      e_prob->Fill(prob);
      e_chisq_ndf->Fill(chisq/ndf);
      e_n0->Fill(n0);
      e_nhit->Fill(nhit);
      
   }
   outfile->Write();
   outfile->Close();
}
