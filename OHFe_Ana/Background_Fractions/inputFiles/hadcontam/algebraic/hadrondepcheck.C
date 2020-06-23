void hadrondepcheck()
{
  const bool nv = 1;

  TFile*infile = TFile::Open("/phenix/hhj/trinn/run15pro108/combinedanataxi/combined_good_ert_100mevbins.root"); 
  TNtuple* ntphsvx = (TNtuple*)infile->Get("ntphsvx");



  float chisq,ndf,dep,quality,hitpat,nhit,zed,sigemcdphi,sigemcdz,conveto,prob,n0,disp,sdips,pt,triginfo,ecore,mom,pc3dphi,spc3dphi,pc3dz,spc3dz;
  ntphsvx->SetBranchAddress("chisq",&chisq);
  ntphsvx->SetBranchAddress("ndf",&ndf);
  ntphsvx->SetBranchAddress("dep",&dep);
  ntphsvx->SetBranchAddress("quality",&quality);
  ntphsvx->SetBranchAddress("hitpat",&hitpat);
  ntphsvx->SetBranchAddress("nhit",&nhit);
  ntphsvx->SetBranchAddress("zed",&zed);
  ntphsvx->SetBranchAddress("sigemcdphi",&sigemcdphi);
  ntphsvx->SetBranchAddress("sigemcdz",&sigemcdz);
  ntphsvx->SetBranchAddress("conversionveto2x",&conveto);
  ntphsvx->SetBranchAddress("prob",&prob);
  ntphsvx->SetBranchAddress("n0",&n0);
  ntphsvx->SetBranchAddress("disp",&disp);
  ntphsvx->SetBranchAddress("triginfo",&triginfo);
  ntphsvx->SetBranchAddress("pt",&pt);
  ntphsvx->SetBranchAddress("ecore",&ecore);
  ntphsvx->SetBranchAddress("mom",&mom);
  ntphsvx->SetBranchAddress("pc3dphi",&pc3dphi);
  ntphsvx->SetBranchAddress("spc3dphi",&spc3dphi);
  ntphsvx->SetBranchAddress("pc3dz",&pc3dz);
  ntphsvx->SetBranchAddress("spc3dz",&spc3dz);


  TH1F* dep1 = new TH1F("dep1","",150,-10,5);
  TH1F* dep2 = new TH1F("dep2","",150,-10,5);
  TH1F* dep3 = new TH1F("dep3","",150,-10,5);
  TH1F* dep4 = new TH1F("dep4","",150,-10,5);
  TH1F* dep5 = new TH1F("dep5","",150,-10,5);
  TH1F* dep6 = new TH1F("dep6","",150,-10,5);
  TH1F* dep7 = new TH1F("dep7","",150,-10,5);
  TH1F* dep8 = new TH1F("dep8","",150,-10,5);
  TH1F* dep9 = new TH1F("dep9","",150,-10,5);

  TH1F* eop1 = new TH1F("eop1","",100,0,1.2);
  TH1F* eop2 = new TH1F("eop2","",100,0,1.2);
  TH1F* eop3 = new TH1F("eop3","",100,0,1.2);
  TH1F* eop4 = new TH1F("eop4","",100,0,1.2);
  TH1F* eop5 = new TH1F("eop5","",100,0,1.2);
  TH1F* eop6 = new TH1F("eop6","",100,0,1.2);
  TH1F* eop7 = new TH1F("eop7","",100,0,1.2);
  TH1F* eop8 = new TH1F("eop8","",100,0,1.2);
  TH1F* eop9 = new TH1F("eop9","",100,0,1.2);

  TH1F* h_pt_no0 = new TH1F("h_pt_no0",";pT;",100,0,10);
  TH1F* h_pt_n00 = new TH1F("h_pt_n00",";pT;",100,0,10);
  TH1F* h_pt_n01 = new TH1F("h_pt_n01",";pT;",100,0,10);
  TH1F* h_pt_n02 = new TH1F("h_pt_n02",";pT;",100,0,10);
  TH1F* h_pt_n03 = new TH1F("h_pt_n03",";pT;",100,0,10);
  TH1F* h_pt_n04 = new TH1F("h_pt_n04",";pT;",100,0,10);


  TH1F* e_pt_no0 = new TH1F("e_pt_no0",";pT;",100,0,10);
  TH1F* e_pt_n00 = new TH1F("e_pt_n00",";pT;",100,0,10);
  TH1F* e_pt_n01 = new TH1F("e_pt_n01",";pT;",100,0,10);
  TH1F* e_pt_n02 = new TH1F("e_pt_n02",";pT;",100,0,10);
  TH1F* e_pt_n03 = new TH1F("e_pt_n03",";pT;",100,0,10);
  TH1F* e_pt_n04 = new TH1F("e_pt_n04",";pT;",100,0,10);




  TH1F* hptno0 = new TH1F("hptno0","",100,0,10);
  TH1F* hptn00 = new TH1F("hptn00","",100,0,10);
  TH1F* hptn01 = new TH1F("hptn01","",100,0,10);
  TH1F* hptn03 = new TH1F("hptn03","",100,0,10);

  h_pt_n00->Sumw2();
  h_pt_no0->Sumw2();
  h_pt_n01->Sumw2();
  h_pt_n02->Sumw2();
  h_pt_n03->Sumw2();
  h_pt_n04->Sumw2();

  e_pt_n00->Sumw2();
  e_pt_no0->Sumw2();
  e_pt_n01->Sumw2();
  e_pt_n02->Sumw2();
  e_pt_n03->Sumw2();
  e_pt_n04->Sumw2();





  dep1->Sumw2();
  dep2->Sumw2();
  dep3->Sumw2();
  dep4->Sumw2();
  dep5->Sumw2();
  dep6->Sumw2();
  dep7->Sumw2();
  dep8->Sumw2();
  dep9->Sumw2();
  hptno0->Sumw2();
  hptn00->Sumw2();
  hptn01->Sumw2();
  hptn03->Sumw2();

  eop1->Sumw2();
  eop2->Sumw2();
  eop3->Sumw2();
  eop4->Sumw2();
  eop5->Sumw2();
  eop6->Sumw2();
  eop7->Sumw2();
  eop8->Sumw2();
  eop9->Sumw2();


for ( int i = 0; i < ntphsvx->GetEntries();i++)
    {
      ntphsvx->GetEntry(i);
      int trigger = triginfo;
      if (nv)
	{
	  if ((quality == 31 || quality == 63) && (hitpat&3) == 3 && nhit > 2 && chisq/ndf <= 3 && fabs(zed) <= 75 && fabs(sigemcdphi) <= 3 && fabs(sigemcdz) <= 3 && disp <= 5 )
	    {

	      if (dep < 2 && dep > -2)
		{
		  hptno0->Fill(pt);
		  if ( n0 > 0 )
		    {
		      hptn00->Fill(pt);
		    }
		  if ( n0 > 1 )
		    {
		      hptn01->Fill(pt);
		    }
		  if ( n0 > 3)
		    {
		      hptn03->Fill(pt);
		    }
		}
	      
	      
	      if ( dep < -6 )
		{
		  h_pt_no0->Fill(pt);
		  if ( n0 > 0 )
		    {
		      h_pt_n00->Fill(pt);
		    }
		  if ( n0 > 1)
		    {
		      h_pt_n01->Fill(pt);
		    }
		  if ( n0 > 2)
		    {
		      h_pt_n02->Fill(pt);
		    }
		  if ( n0 > 3)
		    {
		      h_pt_n03->Fill(pt);
		    }
		  if ( n0 > 4)
		    {
		      h_pt_n04->Fill(pt);
		    }
		}
	      
	      
	      
	      if ( dep > -1 && dep < 1 && pt < 4)
		{
		  e_pt_no0->Fill(pt);
		  if ( n0 > 0 )
		    {
		      e_pt_n00->Fill(pt);
		    }
		  if ( n0 > 1)
		    {
		      e_pt_n01->Fill(pt);
		    }
		  if ( n0 > 2)
		    {
		      e_pt_n02->Fill(pt);
		    }
		  if ( n0 > 3)
		    {
		      e_pt_n03->Fill(pt);
		    }
		  if ( n0 > 4)
		    {
		      e_pt_n04->Fill(pt);
		    }
		}
	    	      	      
	    }
	  
	}

      else
	{
	  if ((quality == 31 || quality == 63) && (hitpat&3) == 3 && nhit > 2 && chisq/ndf <= 3 && fabs(zed) <= 75 && fabs(sigemcdphi) <= 3 && fabs(sigemcdz) <= 3 && disp <= 5 && conveto == 1 )
	    {

	      if (dep < 2 && dep > -2)
		{
		  hptno0->Fill(pt);
		  if ( n0 > 0 )
		    {
		      hptn00->Fill(pt);
		    }
		  if ( n0 > 1 )
		    {
		      hptn01->Fill(pt);
		    }
		  if ( n0 > 3)
		    {
		      hptn03->Fill(pt);
		    }
		}
	      
	      
	      if ( dep < -6 )
		{
		  h_pt_no0->Fill(pt);
		  if ( n0 > 0 )
		    {
		      h_pt_n00->Fill(pt);
		    }
		  if ( n0 > 1)
		    {
		      h_pt_n01->Fill(pt);
		    }
		  if ( n0 > 2)
		    {
		      h_pt_n02->Fill(pt);
		    }
		  if ( n0 > 3)
		    {
		      h_pt_n03->Fill(pt);
		    }
		  if ( n0 > 4)
		    {
		      h_pt_n04->Fill(pt);
		    }
		}
	      
	      
	      
	      if ( dep > -1 && dep < 1 && pt < 4)
		{
		  e_pt_no0->Fill(pt);
		  if ( n0 > 0 )
		    {
		      e_pt_n00->Fill(pt);
		    }
		  if ( n0 > 1)
		    {
		      e_pt_n01->Fill(pt);
		    }
		  if ( n0 > 2)
		    {
		      e_pt_n02->Fill(pt);
		    }
		  if ( n0 > 3)
		    {
		      e_pt_n03->Fill(pt);
		    }
		  if ( n0 > 4)
		    {
		      e_pt_n04->Fill(pt);
		    }
		}
	    	      	      
	    }
	  
	}
    }
 
 if (nv)
   TFile* outfile = new TFile("dataFiles/hadrondepstudy_noveto.root","RECREATE");
 else  
   TFile* outfile = new TFile("dataFiles/hadrondepstudy.root","RECREATE");

 hptno0->Write();
 hptn00->Write();
 hptn01->Write();
 hptn03->Write();

 h_pt_no0->Write();
 h_pt_n00->Write();
 h_pt_n01->Write();
 h_pt_n02->Write();
 h_pt_n03->Write();
 h_pt_n04->Write();

 e_pt_no0->Write();
 e_pt_n00->Write();
 e_pt_n01->Write();
 e_pt_n02->Write();
 e_pt_n03->Write();
 e_pt_n04->Write();



}
