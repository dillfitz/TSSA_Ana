//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jul 19 02:18:43 2019 by ROOT version 5.34/38
// from TTree e_svx_tree/Tree of e svx observables
// found on file: AllRuns.root
//////////////////////////////////////////////////////////

#ifndef OHFe_Ana_h
#define OHFe_Ana_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class OHFe_Ana {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   TFile           *outfile;     // pointer to the output file
   TTree           *t_esvx;      // pointer to the output tree

   TString hist_name = "";

   // Declaration of leaf types
   Int_t           fillnumber;
   Int_t           run;
   Int_t           event;
   Int_t           spinpattern;
   Int_t           xing;
   Int_t           triginfo;
   Int_t           arm;
   Int_t           vertexsource;
   Float_t         precisex;
   Float_t         precisey;
   Float_t         precisez;
   Float_t         seedx;
   Float_t         seedy;
   Float_t         seedz;
   Float_t         combinedx;
   Float_t         combinedy;
   Float_t         combinedz;
   Int_t           ngoodseg;
   Int_t           nseg;
   Int_t           neinevent;
   Float_t         dcat;
   Float_t         dcal;
   Float_t         mom;
   Float_t         chisq;
   Int_t           ndf;
   Float_t         phi0;
   Float_t         phi;
   Int_t           nhit;
   Int_t           hitpattern;
   Float_t         pt;
   Float_t         pz;
   Int_t           quality;
   Int_t           n0;
   Float_t         disp;
   Float_t         dep;
   Float_t         zed;
   Float_t         emcdphi;
   Float_t         emcdz;
   Float_t         emce;
   Float_t         ecore;
   Float_t         sigemcdphi;
   Float_t         sigemcdz;
   Bool_t          conversionveto;
   Int_t           n1;
   Float_t         npe0;
   Float_t         prob;
   Float_t         rchisq;
   Bool_t          conversionveto1p5;
   Bool_t          conversionveto2x;
   Bool_t          conversionveto4x;
   Bool_t          conversionveto10x;

   // List of branches
   TBranch        *b_fillnumber;   //!
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_spinpattern;   //!
   TBranch        *b_xing;   //!
   TBranch        *b_triginfo;   //!
   TBranch        *b_arm;   //!
   TBranch        *b_vertexsource;   //!
   TBranch        *b_precisex;   //!
   TBranch        *b_precisey;   //!
   TBranch        *b_precisez;   //!
   TBranch        *b_seedx;   //!
   TBranch        *b_seedy;   //!
   TBranch        *b_seedz;   //!
   TBranch        *b_combinedx;   //!
   TBranch        *b_combinedy;   //!
   TBranch        *b_combinedz;   //!
   TBranch        *b_ngoodseg;   //!
   TBranch        *b_nseg;   //!
   TBranch        *b_neinevent;   //!
   TBranch        *b_dcat;   //!
   TBranch        *b_dcal;   //!
   TBranch        *b_mom;   //!
   TBranch        *b_chisq;   //!
   TBranch        *b_ndf;   //!
   TBranch        *b_phi0;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_nhit;   //!
   TBranch        *b_hitpattern;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_pz;   //!
   TBranch        *b_quality;   //!
   TBranch        *b_n0;   //!
   TBranch        *b_disp;   //!
   TBranch        *b_dep;   //!
   TBranch        *b_zed;   //!
   TBranch        *b_emcdphi;   //!
   TBranch        *b_emcdz;   //!
   TBranch        *b_emce;   //!
   TBranch        *b_ecore;   //!
   TBranch        *b_sigemcdphi;   //!
   TBranch        *b_sigemcdz;   //!
   TBranch        *b_conversionveto;   //!
   TBranch        *b_n1;   //!
   TBranch        *b_npe0;   //!
   TBranch        *b_prob;   //!
   TBranch        *b_rchisq;   //!
   TBranch        *b_conversionveto1p5;   //!
   TBranch        *b_conversionveto2x;   //!
   TBranch        *b_conversionveto4x;   //!
   TBranch        *b_conversionveto10x;   //!

   OHFe_Ana(TTree *tree=0);
   virtual ~OHFe_Ana();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

};

#endif

#ifdef OHFe_Ana_cxx
OHFe_Ana::OHFe_Ana(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("AllRuns.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("AllRuns.root");
      }
      f->GetObject("e_svx_tree",tree);

   }
   Init(tree);
}

OHFe_Ana::~OHFe_Ana()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t OHFe_Ana::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t OHFe_Ana::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void OHFe_Ana::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("fillnumber", &fillnumber, &b_fillnumber);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("spinpattern", &spinpattern, &b_spinpattern);
   fChain->SetBranchAddress("xing", &xing, &b_xing);
   fChain->SetBranchAddress("triginfo", &triginfo, &b_triginfo);
   fChain->SetBranchAddress("arm", &arm, &b_arm);
   fChain->SetBranchAddress("vertexsource", &vertexsource, &b_vertexsource);
   fChain->SetBranchAddress("precisex", &precisex, &b_precisex);
   fChain->SetBranchAddress("precisey", &precisey, &b_precisey);
   fChain->SetBranchAddress("precisez", &precisez, &b_precisez);
   fChain->SetBranchAddress("seedx", &seedx, &b_seedx);
   fChain->SetBranchAddress("seedy", &seedy, &b_seedy);
   fChain->SetBranchAddress("seedz", &seedz, &b_seedz);
   fChain->SetBranchAddress("combinedx", &combinedx, &b_combinedx);
   fChain->SetBranchAddress("combinedy", &combinedy, &b_combinedy);
   fChain->SetBranchAddress("combinedz", &combinedz, &b_combinedz);
   fChain->SetBranchAddress("ngoodseg", &ngoodseg, &b_ngoodseg);
   fChain->SetBranchAddress("nseg", &nseg, &b_nseg);
   fChain->SetBranchAddress("neinevent", &neinevent, &b_neinevent);
   fChain->SetBranchAddress("dcat", &dcat, &b_dcat);
   fChain->SetBranchAddress("dcal", &dcal, &b_dcal);
   fChain->SetBranchAddress("mom", &mom, &b_mom);
   fChain->SetBranchAddress("chisq", &chisq, &b_chisq);
   fChain->SetBranchAddress("ndf", &ndf, &b_ndf);
   fChain->SetBranchAddress("phi0", &phi0, &b_phi0);
   fChain->SetBranchAddress("phi", &phi, &b_phi);
   fChain->SetBranchAddress("nhit", &nhit, &b_nhit);
   fChain->SetBranchAddress("hitpattern", &hitpattern, &b_hitpattern);
   fChain->SetBranchAddress("pt", &pt, &b_pt);
   fChain->SetBranchAddress("pz", &pz, &b_pz);
   fChain->SetBranchAddress("quality", &quality, &b_quality);
   fChain->SetBranchAddress("n0", &n0, &b_n0);
   fChain->SetBranchAddress("disp", &disp, &b_disp);
   fChain->SetBranchAddress("dep", &dep, &b_dep);
   fChain->SetBranchAddress("zed", &zed, &b_zed);
   fChain->SetBranchAddress("emcdphi", &emcdphi, &b_emcdphi);
   fChain->SetBranchAddress("emcdz", &emcdz, &b_emcdz);
   fChain->SetBranchAddress("emce", &emce, &b_emce);
   fChain->SetBranchAddress("ecore", &ecore, &b_ecore);
   fChain->SetBranchAddress("sigemcdphi", &sigemcdphi, &b_sigemcdphi);
   fChain->SetBranchAddress("sigemcdz", &sigemcdz, &b_sigemcdz);
   fChain->SetBranchAddress("conversionveto", &conversionveto, &b_conversionveto);
   fChain->SetBranchAddress("n1", &n1, &b_n1);
   fChain->SetBranchAddress("npe0", &npe0, &b_npe0);
   fChain->SetBranchAddress("prob", &prob, &b_prob);
   fChain->SetBranchAddress("rchisq", &rchisq, &b_rchisq);
   fChain->SetBranchAddress("conversionveto1p5", &conversionveto1p5, &b_conversionveto1p5);
   fChain->SetBranchAddress("conversionveto2x", &conversionveto2x, &b_conversionveto2x);
   fChain->SetBranchAddress("conversionveto4x", &conversionveto4x, &b_conversionveto4x);
   fChain->SetBranchAddress("conversionveto10x", &conversionveto10x, &b_conversionveto10x);
   Notify();
}

Bool_t OHFe_Ana::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void OHFe_Ana::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
 
Int_t OHFe_Ana::Cut(Long64_t entry)
{
 //This function may be called from Loop.
   if (pt<=1.5 || pt >= 6.0)                     {return -1;}
   if (conversionveto2x==0)                      {return -2;}
   //if (conversionveto1p5==0)                      {return -2;}
   if (abs(dep)>=2.)                             {return -3;}
   if (abs(sigemcdphi)>=3.||abs(sigemcdz)>=3.)   {return -4;}
   if (abs(zed)>= 75)                            {return -5;}
   if (disp>=5.)                                 {return -6;}
   if (ndf != 0) {
      if (chisq/ndf >= 3.)                       {return -7;}      
   } 
   if (nhit <= 2 )                               {return -8;}
   if (quality != 63 && quality !=31)            {return -9;}
   if (!((hitpattern&3)==3))                     {return -10;}
   if (pt<5.) {
      if (n0 <= 1 )                              {return -11;}
      if (prob <= 0.01)                          {return -12;}
   }
   else {
      if (n0 <=3 )                               {return -11;}
      if (prob <= 0.2)                           {return -12;}
   }
   if (ndf == 0 )                                {return -13;}

   // Le Survivors //
   return 1;
}

#endif // #ifdef OHFe_Ana_cxx
