//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jul 18 14:02:35 2019 by ROOT version 5.34/36
// from TTree jpsi_tree/Tree of jpsi observables from electron pairs
// found on file: AllRuns.root
//////////////////////////////////////////////////////////

#ifndef JPsi_Ana_h
#define JPsi_Ana_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class JPsi_Ana {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   TFile           *outfile;     // pointer to the output file

   // Declaration of leaf types
   Int_t           fillnumber;
   Int_t           run;
   Int_t           event;
   Int_t           spinpattern;
   Int_t           xing;
   Int_t           triginfo;
   Int_t           type;
   Float_t         mass;
   Float_t         pt;
   Float_t         phiv;
   Float_t         costs;
   Int_t           qual1;
   Int_t           qual2;
   Float_t         zed1;
   Float_t         zed2;
   Int_t           n01;
   Int_t           n02;
   Float_t         ecore1;
   Float_t         ecore2;
   Float_t         prob1;
   Float_t         prob2;
   Float_t         mom1;
   Float_t         mom2;
   Int_t           npe1;
   Int_t           npe2;
   Float_t         disp1;
   Float_t         disp2;
   Float_t         chi21;
   Float_t         chi22;
   Float_t         dep1;
   Float_t         dep2;
   Float_t         minm1;
   Float_t         minm2;
   Float_t         emcdphi1;
   Float_t         emcdphi2;
   Float_t         emcdz1;
   Float_t         emcdz2;
   Float_t         zvtx;
   Float_t         cent;
   Int_t           ert1;
   Int_t           ert2;
   Float_t         pt1;
   Float_t         pt2;
   Float_t         px1;
   Float_t         px2;
   Int_t           armsect1;
   Int_t           armsect2;
   Int_t           side1;
   Int_t           side2;
   Float_t         pairangle;
   Float_t         mult;
   Float_t         emcee;
   Float_t         emcew;
   Float_t         goodmult;
   Float_t         rtr;
   Float_t         theta1;
   Float_t         theta2;
   Float_t         phi1;
   Float_t         phi2;
   Float_t         dchalpha1;
   Float_t         dchalpha2;
   Float_t         dchphi1;
   Float_t         dchphi2;
   Float_t         truepairpt;


   // List of branches
   TBranch        *b_fillnumber;   //!
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_spinpattern;   //!
   TBranch        *b_xing;   //!
   TBranch        *b_triginfo;   //!
   TBranch        *b_type;   //!
   TBranch        *b_mass;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_phiv;   //!
   TBranch        *b_costs;   //!
   TBranch        *b_qual1;   //!
   TBranch        *b_qual2;   //!
   TBranch        *b_zed1;   //!
   TBranch        *b_zed2;   //!
   TBranch        *b_n01;   //!
   TBranch        *b_n02;   //!
   TBranch        *b_ecore1;   //!
   TBranch        *b_ecore2;   //!
   TBranch        *b_prob1;   //!
   TBranch        *b_prob2;   //!
   TBranch        *b_mom1;   //!
   TBranch        *b_mom2;   //!
   TBranch        *b_npe1;   //!
   TBranch        *b_npe2;   //!
   TBranch        *b_disp1;   //!
   TBranch        *b_disp2;   //!
   TBranch        *b_chi21;   //!
   TBranch        *b_chi22;   //!
   TBranch        *b_dep1;   //!
   TBranch        *b_dep2;   //!
   TBranch        *b_minm1;   //!
   TBranch        *b_minm2;   //!
   TBranch        *b_emcdphi1;   //!
   TBranch        *b_emcdphi2;   //!
   TBranch        *b_emcdz1;   //!
   TBranch        *b_emcdz2;   //!
   TBranch        *b_zvtx;   //!
   TBranch        *b_cent;   //!
   TBranch        *b_ert1;   //!
   TBranch        *b_ert2;   //!
   TBranch        *b_pt1;   //!
   TBranch        *b_pt2;   //!
   TBranch        *b_px1;   //!
   TBranch        *b_px2;   //!
   TBranch        *b_armsect1;   //!
   TBranch        *b_armsect2;   //!
   TBranch        *b_side1;   //!
   TBranch        *b_side2;   //!
   TBranch        *b_pairangle;   //!
   TBranch        *b_mult;   //!
   TBranch        *b_emcee;   //!
   TBranch        *b_emcew;   //!
   TBranch        *b_goodmult;   //!
   TBranch        *b_rtr;   //!
   TBranch        *b_theta1;   //!
   TBranch        *b_theta2;   //!
   TBranch        *b_phi1;   //!
   TBranch        *b_phi2;   //!
   TBranch        *b_dchalpha1;   //!
   TBranch        *b_dchalpha2;   //!
   TBranch        *b_dchphi1;   //!
   TBranch        *b_dchphi2;   //!
   TBranch        *b_truepairpt;   //!
   //JPsi_Ana::JPsi  a;

   JPsi_Ana(TTree *tree=0);
   virtual ~JPsi_Ana();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

};

#endif

#ifdef JPsi_Ana_cxx
JPsi_Ana::JPsi_Ana(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("AllRuns.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("AllRuns.root");
      }
      f->GetObject("jpsi_tree",tree);

   }
   Init(tree);
}

JPsi_Ana::~JPsi_Ana()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t JPsi_Ana::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t JPsi_Ana::LoadTree(Long64_t entry)
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

void JPsi_Ana::Init(TTree *tree)
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
   fChain->SetBranchAddress("type", &type, &b_type);
   fChain->SetBranchAddress("mass", &mass, &b_mass);
   fChain->SetBranchAddress("pt", &pt, &b_pt);
   fChain->SetBranchAddress("phiv", &phiv, &b_phiv);
   fChain->SetBranchAddress("costs", &costs, &b_costs);
   fChain->SetBranchAddress("qual1", &qual1, &b_qual1);
   fChain->SetBranchAddress("qual2", &qual2, &b_qual2);
   fChain->SetBranchAddress("zed1", &zed1, &b_zed1);
   fChain->SetBranchAddress("zed2", &zed2, &b_zed2);
   fChain->SetBranchAddress("n01", &n01, &b_n01);
   fChain->SetBranchAddress("n02", &n02, &b_n02);
   fChain->SetBranchAddress("ecore1", &ecore1, &b_ecore1);
   fChain->SetBranchAddress("ecore2", &ecore2, &b_ecore2);
   fChain->SetBranchAddress("prob1", &prob1, &b_prob1);
   fChain->SetBranchAddress("prob2", &prob2, &b_prob2);
   fChain->SetBranchAddress("mom1", &mom1, &b_mom1);
   fChain->SetBranchAddress("mom2", &mom2, &b_mom2);
   fChain->SetBranchAddress("npe1", &npe1, &b_npe1);
   fChain->SetBranchAddress("npe2", &npe2, &b_npe2);
   fChain->SetBranchAddress("disp1", &disp1, &b_disp1);
   fChain->SetBranchAddress("disp2", &disp2, &b_disp2);
   fChain->SetBranchAddress("chi21", &chi21, &b_chi21);
   fChain->SetBranchAddress("chi22", &chi22, &b_chi22);
   fChain->SetBranchAddress("dep1", &dep1, &b_dep1);
   fChain->SetBranchAddress("dep2", &dep2, &b_dep2);
   fChain->SetBranchAddress("minm1", &minm1, &b_minm1);
   fChain->SetBranchAddress("minm2", &minm2, &b_minm2);
   fChain->SetBranchAddress("emcdphi1", &emcdphi1, &b_emcdphi1);
   fChain->SetBranchAddress("emcdphi2", &emcdphi2, &b_emcdphi2);
   fChain->SetBranchAddress("emcdz1", &emcdz1, &b_emcdz1);
   fChain->SetBranchAddress("emcdz2", &emcdz2, &b_emcdz2);
   fChain->SetBranchAddress("zvtx", &zvtx, &b_zvtx);
   fChain->SetBranchAddress("cent", &cent, &b_cent);
   fChain->SetBranchAddress("ert1", &ert1, &b_ert1);
   fChain->SetBranchAddress("ert2", &ert2, &b_ert2);
   fChain->SetBranchAddress("pt1", &pt1, &b_pt1);
   fChain->SetBranchAddress("pt2", &pt2, &b_pt2);
   fChain->SetBranchAddress("px1", &px1, &b_px1);
   fChain->SetBranchAddress("px2", &px2, &b_px2);
   fChain->SetBranchAddress("armsect1", &armsect1, &b_armsect1);
   fChain->SetBranchAddress("armsect2", &armsect2, &b_armsect2);
   fChain->SetBranchAddress("side1", &side1, &b_side1);
   fChain->SetBranchAddress("side2", &side2, &b_side2);
   fChain->SetBranchAddress("pairangle", &pairangle, &b_pairangle);
   fChain->SetBranchAddress("mult", &mult, &b_mult);
   fChain->SetBranchAddress("emcee", &emcee, &b_emcee);
   fChain->SetBranchAddress("emcew", &emcew, &b_emcew);
   fChain->SetBranchAddress("goodmult", &goodmult, &b_goodmult);
   fChain->SetBranchAddress("rtr", &rtr, &b_rtr);
   fChain->SetBranchAddress("theta1", &theta1, &b_theta1);
   fChain->SetBranchAddress("theta2", &theta2, &b_theta2);
   fChain->SetBranchAddress("phi1", &phi1, &b_phi1);
   fChain->SetBranchAddress("phi2", &phi2, &b_phi2);
   fChain->SetBranchAddress("dchalpha1", &dchalpha1, &b_dchalpha1);
   fChain->SetBranchAddress("dchalpha2", &dchalpha2, &b_dchalpha2);
   fChain->SetBranchAddress("dchphi1", &dchphi1, &b_dchphi1);
   fChain->SetBranchAddress("dchphi2", &dchphi2, &b_dchphi2);
   fChain->SetBranchAddress("truepairpt", &truepairpt, &b_truepairpt);
   Notify();
}

Bool_t JPsi_Ana::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void JPsi_Ana::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t JPsi_Ana::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   if (type != 0) {return -1;}
   if (pt >= 10) {return -2;}
   if (mom1<=0.5 || mom2<=0.5) { return -3; }
   if (abs(dep1)>=2. || abs(dep2)>=2. || disp1>=5. || disp2>=5.) { return -4; }
   // need sigmalized values here... //
//   if (abs(emcdphi1)>=3. || abs(emcdphi2)>=3. || abs(emcdz1)>=3. || abs(emcdz2)>=3.) {return -1;}
   if (pt1<5.) 
    {
       if (n01 <= 1 || prob1 <= 0.01) {return -5;}
    }
   else if (n01 <=3 || prob1 <= 0.2) {return -6;}
   if (pt2<5.) 
    {
       if (n02 <= 1 || prob2 <= 0.01) {return -7;}
    }
   else if (n02 <=3 || prob2 <= 0.2) {return -8;}
   if ((qual1 != 63 && qual1 !=31) || (qual2 !=63 && qual2 !=33)) {return -9;}
   if (abs(zed1)>= 75 || abs(zed2) >= 75) {return -10;}
   // need to add svx info and chisq/ndf cuts // 

   // Le Survivors //
   return 1;
}
#endif // #ifdef JPsi_Ana_cxx
