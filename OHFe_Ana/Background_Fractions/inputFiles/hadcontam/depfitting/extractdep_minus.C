#include "../../../../../Asymmetry_Ana/Constants.h"
void extractdep_minus()
{
  const bool nv = 0;

  TFile*infile = TFile::Open("../../../../../AllRuns_725_ana651.root");
  TNtuple*eSvxTree = (TTree*)infile->Get("e_svx_tree");

  TString outfileName = "dataFiles/depstudy";
  if (nv)
    {
      outfileName += "_noveto";
    }

  outfileName += "_minus.root";
  cout << " outputting histograms into " << outfileName << endl;

  TFile* outfile = new TFile(outfileName,"RECREATE");

  bool conveto;
  int ndf, quality,hitpat, nhit, n0, triginfo,charge;
  float chisq,dep,zed,sigemcdphi,sigemcdz,prob,disp,sdips,pt,ecore,mom;
  eSvxTree->SetBranchAddress("chisq",&chisq);
  eSvxTree->SetBranchAddress("ndf",&ndf);
  eSvxTree->SetBranchAddress("dep",&dep);
  eSvxTree->SetBranchAddress("quality",&quality);
  eSvxTree->SetBranchAddress("hitpattern",&hitpat);
  eSvxTree->SetBranchAddress("nhit",&nhit);
  eSvxTree->SetBranchAddress("zed",&zed);
  eSvxTree->SetBranchAddress("sigemcdphi",&sigemcdphi);
  eSvxTree->SetBranchAddress("sigemcdz",&sigemcdz);
  eSvxTree->SetBranchAddress("conversionveto2x",&conveto);
  eSvxTree->SetBranchAddress("prob",&prob);
  eSvxTree->SetBranchAddress("n0",&n0);
  eSvxTree->SetBranchAddress("disp",&disp);
  eSvxTree->SetBranchAddress("triginfo",&triginfo);
  eSvxTree->SetBranchAddress("pt",&pt);
  eSvxTree->SetBranchAddress("ecore",&ecore);
  eSvxTree->SetBranchAddress("mom",&mom);
  eSvxTree->SetBranchAddress("charge",&charge);

  const int nbins = NUM_VALUE_BINS;

  TH1F* h_dep[nbins];
  TH1F* h_eop[nbins];


  for (int i =0; i < nbins;i++)
    {
      stringstream aa;
      aa << "h_dep"<<i;
      string depname = aa.str();
      stringstream bb;
      bb << "h_eop"<<i;
      string eopname = bb.str();

      // cout << depname.c_str() << endl;
      h_dep[i] = new TH1F(depname.c_str(),"",150,-10,5);
      h_eop[i] = new TH1F(eopname.c_str(),"",300,0,2);
      h_dep[i]->Sumw2();   
      h_eop[i]->Sumw2();
    }


  for ( int i = 0; i < eSvxTree->GetEntries();i++)
    {
      eSvxTree->GetEntry(i);
      int trigger = triginfo;
      if (nv)
	{
	  if ((quality == 31 || quality == 63) && (hitpat&3) == 3 && nhit > 2 && chisq/ndf <= 3 && fabs(zed) <= 75 && fabs(sigemcdphi) <= 3 && fabs(sigemcdz) <= 3 && disp <= 5 && n0 > 1 && charge == -1)
	    {
	      if( prob > 0.01 && pt < 5 )
		{
		  for (int q = 0; q < nbins;q++)
		    {
		      if (pt > VALUE_BINS[q] && pt < VALUE_BINS[q+1])
			{
			  h_dep[q]->Fill(dep);
			  h_eop[q]->Fill(ecore/mom);
			}
		    }
		}
	      if ( pt > 5 && prob > 0.2 && n0 > 3)
		{
		  for (int q = 0; q < nbins;q++)
		    {
		      if (pt > VALUE_BINS[q] && pt < VALUE_BINS[q+1])
			{
			  h_dep[q]->Fill(dep);
			  h_eop[q]->Fill(ecore/mom);
			}
		    }
		}
	      
	    }	  
	}

      else
	{
	  if ((quality == 31 || quality == 63) && (hitpat&3) == 3 && nhit > 2 && chisq/ndf <= 3 && fabs(zed) <= 75 && fabs(sigemcdphi) <= 3 && fabs(sigemcdz) <= 3 && disp <= 5 && n0 > 1 && conveto==1 && charge == -1)
	    {
	      if( prob > 0.01 && pt < 5 )
		{
		  for (int q = 0; q < nbins;q++)
		    {
		      if (pt > VALUE_BINS[q] && pt < VALUE_BINS[q+1])
			{
			  h_dep[q]->Fill(dep);
			  h_eop[q]->Fill(ecore/mom);
			}
		    }		  
		}
	      if ( pt > 5 && prob > 0.2 && n0 > 3)
		{
		  for (int q = 0; q < nbins;q++)
		    {
		      if (pt > VALUE_BINS[q] && pt < VALUE_BINS[q+1])
			{
			  h_dep[q]->Fill(dep);
			  h_eop[q]->Fill(ecore/mom);
			}
		    }
		}
	      
	    }	  
	}
    }
  

  //TCanvas*c1 = new TCanvas("c1","",500,500);

  for (int i = 0; i < nbins;i++)
    {
      h_dep[i]->Write();
      h_eop[i]->Write();
    }
  

}
