#define OHFe_Ana_cxx
#include "OHFe_Ana.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

//#include "Constants.h"
//#include "findBin.h"


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

   // Create output tree and branches //
   t_esvx = new TTree( "e_svx_tree", "Tree of electron candidates from SvxCentralTracks");	  
   t_esvx->Branch("fillnumber", &fillnumber, "fillnumber/I" );
   t_esvx->Branch("run", &run, "run/I" );
   t_esvx->Branch("event", &event, "event/I" );
   t_esvx->Branch("spinpattern", &spinpattern, "spinpattern/I");
   t_esvx->Branch("xing", &xing, "xing/I");
   t_esvx->Branch("triginfo", &triginfo, "triginfo/I");
   t_esvx->Branch("arm", &arm, "arm/I");
   t_esvx->Branch("vertexsource", &vertexsource, "vertexsource/I"); 
   t_esvx->Branch("ngoodseg", &ngoodseg, "ngoodseg/I"); 
   t_esvx->Branch("nseg", &nseg, "nseg/I"); 
   t_esvx->Branch("neinevent", &neinevent, "neinevent/I"); 
   t_esvx->Branch("dcat", &dcat, "dcat/F"); 
   t_esvx->Branch("dcal", &dcal, "dcal/F"); 
   t_esvx->Branch("mom", &mom, "mom/F"); 
   t_esvx->Branch("chisq", &chisq, "chisq/F"); 
   t_esvx->Branch("ndf", &ndf, "ndf/I"); 
   t_esvx->Branch("phi0", &phi0, "phi0/F"); 
   t_esvx->Branch("phi", &phi, "phi/F");  
   t_esvx->Branch("nhit", &nhit, "nhit/I"); 
   t_esvx->Branch("hitpattern", &hitpattern, "hitpattern/I"); 
   t_esvx->Branch("pt", &pt, "pt/F"); 
   t_esvx->Branch("pz", &pz, "pz/F"); 
   t_esvx->Branch("quality", &quality, "quality/I"); 
   t_esvx->Branch("n0", &n0, "n0/I"); 
   t_esvx->Branch("disp", &disp, "disp/F"); 
   t_esvx->Branch("dep", &dep, "dep/F"); 
   t_esvx->Branch("zed", &zed, "zed/F"); 
   t_esvx->Branch("emcdphi", &emcdphi, "emcdphi/F"); 
   t_esvx->Branch("emcdz", &emcdz, "emcdz/F"); 
   t_esvx->Branch("emce", &emce, "emce/F"); 
   t_esvx->Branch("ecore", &ecore, "ecore/F"); 
   t_esvx->Branch("sigemcdphi", &sigemcdphi, "sigemcdphi/F"); 
   t_esvx->Branch("sigemcdz", &sigemcdz, "sigemcdz/F"); 
   t_esvx->Branch("conversionveto", &conversionveto, "conversionveto/O");  
   t_esvx->Branch("n1", &n1, "n1/I"); 
   t_esvx->Branch("npe0", &npe0, "npe0/F"); 
   t_esvx->Branch("prob", &prob, "prob/F"); 
   t_esvx->Branch("rchisq", &rchisq, "rchisq/F"); 
   t_esvx->Branch("conversionveto1p5", &conversionveto1p5, "conversionveto1p5/O"); 
   t_esvx->Branch("conversionveto2x", &conversionveto2x, "conversionveto2x/O"); 
   t_esvx->Branch("conversionveto4x", &conversionveto4x, "conversionveto4x/O"); 
   t_esvx->Branch("conversionveto10x", &conversionveto10x, "conversionveto10x/O"); 

   // Declaration of output histograms 
   bool cut_eff = 0; // Set this to fill histogram for determining cut efficiency.
   int cutval = 0;
   const int nPt_bins = 6;

   TH1F *e_dcat_binned[nPt_bins], *e_dep_binned[nPt_bins]; 
   TH1F *e_prob_binned[nPt_bins], *e_chisq_ndf_binned[nPt_bins];
   TH1I *e_n0_binned[nPt_bins], *e_nhit_binned[nPt_bins];
   TH1F *e_dcat, *e_pt, *e_dep, *e_eop;
   TH1F *e_ecore, *e_prob, *e_chisq_ndf, *cuts;
   TH1I *e_n0, *e_nhit;
   
   float Pt_bins_low[nPt_bins] = {1.0, 1.3, 1.5, 1.8, 2.1, 2.7};
   float Pt_bins_high[nPt_bins] = {1.3, 1.5, 1.8, 2.1, 2.7, 5.0};

   TString hist_labels[nPt_bins] = { "_1.0_1.3", "_1.3_1.5", "_1.5_1.8","_1.8_2.1", "_2.1_2.7", "_2.7_5.0"};
   TString hist_titles[nPt_bins] = { "1.0 < p_{T} < 1.3", "1.3 < p_{T} < 1.5", "1.5 < p_{T} < 1.8","1.8 < p_{T} < 2.1", "2.1 < p_{T} < 2.7", "2.7 < p_{T} < 5.0"};


   // Initialize some histograms //
   for (int i=0; i<nPt_bins; i++) {
      hist_name = "dcat" + hist_labels[i];
      hist_title =  hist_titles[i] + ";DCA_{T} [cm];"; 
      cout << hist_title << endl;
      e_dcat_binned[i] = new TH1F(hist_name, hist_title, 80, -0.2, 0.2);
      e_dcat_binned[i]->Sumw2();

      hist_name = "dep" + hist_labels[i];
      hist_title =  hist_titles[i] + ";dep;"; 
      e_dep_binned[i] = new TH1F(hist_name, hist_title, 50, -5., 5.);
      e_dep_binned[i]->Sumw2();

      hist_name = "prob" + hist_labels[i];
      hist_title =  hist_titles[i] + ";prob;"; 
      e_prob_binned[i] = new TH1F(hist_name, hist_title, 20, 0., 1.);
      e_prob_binned[i]->Sumw2();

      hist_name = "chisq_ndf" + hist_labels[i];
      hist_title =  hist_titles[i] + ";#chi^{2}/ndf;"; 
      e_chisq_ndf_binned[i] = new TH1F(hist_name, hist_title, 50, 0., 10.);
      e_chisq_ndf_binned[i]->Sumw2();

      hist_name = "n0" + hist_labels[i];
      hist_title =  hist_titles[i] + ";n0;"; 
      e_n0_binned[i] = new TH1I(hist_name, hist_title, 20, 0, 20);
      e_n0_binned[i]->Sumw2();

      hist_name = "nhits" + hist_labels[i];
      hist_title =  hist_titles[i] + ";nhits;"; 
      e_nhit_binned[i] = new TH1I(hist_name, hist_title, 20, 0, 20);
      e_nhit_binned[i]->Sumw2();      
   }
    
   e_dcat = new TH1F("dcat","dcat", 80, -0.2, 0.2);
   e_dcat->Sumw2();

   e_pt = new TH1F("pt","pt", 45, 1.5, 5.);
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


      // This is to show the distributions before each respective cut is made.. bin by bin! //
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
         if (pt>1.0 && pt < 5.0)                     {cuts->Fill(-1);}
         if (conversionveto2x==1)                    {cuts->Fill(-2);}
         if (abs(dep)<2.)                            {cuts->Fill(-3);}
         if (abs(sigemcdphi)<3.&& abs(sigemcdz)<3.)  {cuts->Fill(-4);}
         if (abs(zed)<75)                            {cuts->Fill(-5);}
         if (disp<5.)                                {cuts->Fill(-6);} 
         if (ndf!=0) {
            if (chisq/ndf < 3.)                      {cuts->Fill(-7);}
         }
         if (nhit > 2)                               {cuts->Fill(-8);}
         if (quality == 63 || quality == 31)         {cuts->Fill(-9);}
         if ((hitpattern&3)==3)                      {cuts->Fill(-10);}
         if (pt<5.) {
            if (n0 > 1)                              {cuts->Fill(-11);}
            if (prob > 0.01)                         {cuts->Fill(-12);}
         }
         else {
            if (n0 > 3 )                             {cuts->Fill(-11);}
            if (prob > 0.2)                          {cuts->Fill(-12);}
         }
         if (ndf==0)                                 {cuts->Fill(-13);}

         cuts->Fill(0);
      }
   
      cutval = Cut(ientry);
      if (!(cutval<0)) { cuts->Fill(1);}
      // Implement analysis cuts -- see header file //
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

      t_esvx->Fill();
      
   }
   outfile->Write();
   outfile->Close();
}
