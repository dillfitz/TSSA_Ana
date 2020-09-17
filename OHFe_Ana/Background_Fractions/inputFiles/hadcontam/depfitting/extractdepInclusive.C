void extractdepInclusive()
{
  const bool nv = 0;

  //TFile*infile = TFile::Open("/phenix/hhj/trinn/run15pro108/combinedanataxi/combined_good_ert_100mevbins.root");
  // TNtuple*ntpesvx = (TNtuple*)infile->Get("ntpesvx");


  TFile*infile = TFile::Open("../../../../../AllRuns_725_ana644.root");
  TNtuple*eSvxTree = (TTree*)infile->Get("e_svx_tree");

 TString outfileName = "dataFiles/depstudyInclusive";
  if (nv)
    {
      outfileName += "_noveto";
    }

  outfileName += ".root";
  cout << " outputting histograms into " << outfileName << endl;

  TFile* outfile = new TFile(outfileName,"RECREATE");

  bool conveto;
  int ndf, quality,hitpat, nhit, n0, triginfo;
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



  //double ptbins[16]  = {1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.5, 4.0, 4.5 , 5.0, 6.0};

  // Binning out to 6 GeV in pT //
  //double ptbins[nbins+1]  = {1.5, 1.8, 2.1, 2.7, 6.0};

  // Binning out to 5 GeV in pT //
  const int nbins = 5;
  double ptbins[nbins+1]  = {1.5, 1.8, 2.1, 2.7, 5.0, 8.0};

  TH1F* h_dep[nbins];
  TH1F* h_eop[nbins];
  int bincounter[nbins];


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
      bincounter[i] = 0;
    }


  for ( int i = 0; i < eSvxTree->GetEntries();i++)
    {
      eSvxTree->GetEntry(i);
      int trigger = triginfo;
      if (nv)
	{
	  //if ((quality == 31 || quality == 63) && (hitpat&3) == 3 && nhit > 2 && chisq/ndf < 3 && fabs(zed) < 75 && fabs(sigemcdphi) < 3 && fabs(sigemcdz) < 3 && disp < 5 && n0 > 1 && prob > 0.01)
          if ((quality == 31 || quality == 63) && chisq/ndf < 3 && fabs(zed) < 75 && fabs(sigemcdphi) < 3 && fabs(sigemcdz) < 3 && disp < 5 && n0 > 1 && prob > 0.01)
	    {
                if (pt > 5 && n0 <=2) {continue;}
                for (int q = 0; q < nbins;q++)
		{

		    if (pt > ptbins[q] && pt < ptbins[q+1])
		    {
                        h_dep[q]->Fill(dep);
			h_eop[q]->Fill(ecore/mom);
                        if ( fabs(dep) < 2 )
                            bincounter[q]++;
                    }
		}
	      
	    }	  
	}

      else
	{
	  //if ((quality == 31 || quality == 63) && (hitpat&3) == 3 && nhit > 2 && chisq/ndf < 3 && fabs(zed) < 75 && fabs(sigemcdphi) < 3 && fabs(sigemcdz) < 3 && disp < 5 && n0 > 1 && prob > 0.01 && conveto==1)
	  if ((quality == 31 || quality == 63) && chisq/ndf < 3 && fabs(zed) < 75 && fabs(sigemcdphi) < 3 && fabs(sigemcdz) < 3 && disp < 5 && n0 > 1 && prob > 0.01 && conveto==1 )
	    {
                if (pt > 5 && n0 <=2) {continue;}
                for (int q = 0; q < nbins;q++)
		{
		    if (pt > ptbins[q] && pt < ptbins[q+1])
		    {
                        h_dep[q]->Fill(dep);
			h_eop[q]->Fill(ecore/mom);
                        if ( fabs(dep) < 2 )
                            bincounter[q]++;
                    }
		}	  
	    }	  
	}
    }
  

  //TCanvas*c1 = new TCanvas("c1","",500,500);

  for (int i = 0; i < nbins;i++)
    {
      cout << "N_e in pT bin " << i << " : " << bincounter[i] << endl;
      h_dep[i]->Write();
      h_eop[i]->Write();
    }
  

}
