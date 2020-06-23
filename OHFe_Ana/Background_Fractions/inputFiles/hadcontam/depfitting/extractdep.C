void extractdep()
{
  const bool nv = 1;

  TFile*infile = TFile::Open("/phenix/hhj/trinn/run15pro108/combinedanataxi/combined_good_ert_100mevbins.root");
  TNtuple*ntpesvx = (TNtuple*)infile->Get("ntpesvx");


  float chisq,ndf,dep,quality,hitpat,nhit,zed,sigemcdphi,sigemcdz,conveto,prob,n0,disp,sdips,pt,triginfo,ecore,mom;
  ntpesvx->SetBranchAddress("chisq",&chisq);
  ntpesvx->SetBranchAddress("ndf",&ndf);
  ntpesvx->SetBranchAddress("dep",&dep);
  ntpesvx->SetBranchAddress("quality",&quality);
  ntpesvx->SetBranchAddress("hitpat",&hitpat);
  ntpesvx->SetBranchAddress("nhit",&nhit);
  ntpesvx->SetBranchAddress("zed",&zed);
  ntpesvx->SetBranchAddress("sigemcdphi",&sigemcdphi);
  ntpesvx->SetBranchAddress("sigemcdz",&sigemcdz);
  ntpesvx->SetBranchAddress("conversionveto2x",&conveto);
  ntpesvx->SetBranchAddress("prob",&prob);
  ntpesvx->SetBranchAddress("n0",&n0);
  ntpesvx->SetBranchAddress("disp",&disp);
  ntpesvx->SetBranchAddress("triginfo",&triginfo);
  ntpesvx->SetBranchAddress("pt",&pt);
  ntpesvx->SetBranchAddress("ecore",&ecore);
  ntpesvx->SetBranchAddress("mom",&mom);


  TH1F* h_dep[20];
  TH1F* h_eop[20];


  for (int i =0; i < 20;i++)
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

  // float ptbins[15] = {1,1.25,1.5,1.75,2,2.25,2.5,2.75,3,3.5,4,4.5,5,6,8};
  double ptbins[16]  = {1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.5, 4.0, 4.5 , 5.0, 6.0};

  for ( int i = 0; i < ntpesvx->GetEntries();i++)
    {
      ntpesvx->GetEntry(i);
      int trigger = triginfo;
      if (nv)
	{
	  if ((quality == 31 || quality == 63) && (hitpat&3) == 3 && nhit > 2 && chisq/ndf <= 3 && fabs(zed) <= 75 && fabs(sigemcdphi) <= 3 && fabs(sigemcdz) <= 3 && disp <= 5 && n0 > 1 )
	    {
	      if( prob > 0.01 && pt < 5 )
		{
		  for (int q = 0; q < 10;q++)
		    {
		      if (pt > ptbins[q] && pt < ptbins[q+1])
			{
			  h_dep[q]->Fill(dep);
			  h_eop[q]->Fill(ecore/mom);
			}
		    }
		}
	      if ( pt > 5 && prob > 0.2 && n0 > 3)
		{
		  for (int q = 0; q < 10;q++)
		    {
		      if (pt > ptbins[q] && pt < ptbins[q+1])
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
	  if ((quality == 31 || quality == 63) && (hitpat&3) == 3 && nhit > 2 && chisq/ndf <= 3 && fabs(zed) <= 75 && fabs(sigemcdphi) <= 3 && fabs(sigemcdz) <= 3 && disp <= 5 && n0 > 1 && conveto==1 )
	    {
	      if( prob > 0.01 && pt < 5 )
		{
		  for (int q = 0; q < 10;q++)
		    {
		      if (pt > ptbins[q] && pt < ptbins[q+1])
			{
			  h_dep[q]->Fill(dep);
			  h_eop[q]->Fill(ecore/mom);
			}
		    }		  
		}
	      if ( pt > 5 && prob > 0.2 && n0 > 3)
		{
		  for (int q = 0; q < 10;q++)
		    {
		      if (pt > ptbins[q] && pt < ptbins[q+1])
			{
			  h_dep[q]->Fill(dep);
			  h_eop[q]->Fill(ecore/mom);
			}
		    }
		}
	      
	    }	  
	}
    }
  

  TCanvas*c1 = new TCanvas("c1","",500,500);



  if (nv)
    TFile* outfile = new TFile("dataFiles/depstudy_noveto.root","RECREATE");
  else
    TFile* outfile = new TFile("dataFiles/depstudy.root","RECREATE");

  for (int i = 0; i < 10;i++)
    {
      h_dep[i]->Write();
      h_eop[i]->Write();
    }
  

}
