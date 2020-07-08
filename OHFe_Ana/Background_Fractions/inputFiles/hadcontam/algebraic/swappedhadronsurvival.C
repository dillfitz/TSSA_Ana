void swappedhadronsurvival()
{

  //TFile*infile = TFile::Open("/phenix/hhj/trinn/run15pro108/combinedanataxi/combined_good_ert_whadrons.root"); 
  //TNtuple* ntphsvx = (TNtuple*)infile->Get("ntpshsvx");

  TFile*infile = TFile::Open("../../../../../AllRuns_725_ana644.root"); 
  TNtuple* shSvxTree = (TTree*)infile->Get("sh_svx_tree");
  
  int ndf, quality, hitpat, nhit, n0, triginfo;
  float chisq,dep,zed,sigemcdphi,sigemcdz,prob,disp,sdips,pt,ecore,mom;
  shSvxTree->SetBranchAddress("chisq",&chisq);
  shSvxTree->SetBranchAddress("ndf",&ndf);
  shSvxTree->SetBranchAddress("dep",&dep);
  shSvxTree->SetBranchAddress("quality",&quality);
  shSvxTree->SetBranchAddress("hitpattern",&hitpat);
  shSvxTree->SetBranchAddress("nhit",&nhit);
  shSvxTree->SetBranchAddress("zed",&zed);
  shSvxTree->SetBranchAddress("sigemcdphi",&sigemcdphi);
  shSvxTree->SetBranchAddress("sigemcdz",&sigemcdz);
  shSvxTree->SetBranchAddress("prob",&prob);
  shSvxTree->SetBranchAddress("sn0",&n0);
  shSvxTree->SetBranchAddress("sdisp",&disp);
  shSvxTree->SetBranchAddress("triginfo",&triginfo);
  shSvxTree->SetBranchAddress("pt",&pt);
  shSvxTree->SetBranchAddress("ecore",&ecore);
  shSvxTree->SetBranchAddress("mom",&mom);

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

  TH1F* h_pt_no0 = new TH1F("h_pt_no0",";pT;",40,0,10);
  TH1F* h_pt_n00 = new TH1F("h_pt_n00",";pT;",40,0,10);
  TH1F* h_pt_n01 = new TH1F("h_pt_n01",";pT;",40,0,10);
  TH1F* h_pt_n02 = new TH1F("h_pt_n02",";pT;",40,0,10);
  TH1F* h_pt_n03 = new TH1F("h_pt_n03",";pT;",40,0,10);
  TH1F* h_pt_n04 = new TH1F("h_pt_n04",";pT;",40,0,10);


  TH1F* hptno0 = new TH1F("hptno0","",10,0,10);
  TH1F* hptn00 = new TH1F("hptn00","",10,0,10);
  TH1F* hptn01 = new TH1F("hptn01","",10,0,10);
  TH1F* hptn03 = new TH1F("hptn03","",10,0,10);

  h_pt_n00->Sumw2();
  h_pt_no0->Sumw2();
  h_pt_n01->Sumw2();
  h_pt_n02->Sumw2();
  h_pt_n03->Sumw2();
  h_pt_n04->Sumw2();




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



for ( int i = 0; i < shSvxTree->GetEntries();i++)
    {
      shSvxTree->GetEntry(i);
      if ((quality == 31 || quality == 63) && (hitpat&3) == 3 && nhit > 2 && chisq/ndf <= 3 && fabs(zed) <= 75 && fabs(sigemcdphi) <= 3 && fabs(sigemcdz) <= 3 && disp <= 5)
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
	  

	  if ( pt > 1 && pt < 1.5 )
	    {
	      dep1->Fill(dep);
	    }
	  if ( pt > 1.5 && pt < 2 )
	    {
	      dep2->Fill(dep);
	    }
	  if ( pt > 2 && pt < 2.5 )
	    {
	      dep3->Fill(dep);
	    }
	  if ( pt > 2.5 && pt < 3 )
	    {
	      dep4->Fill(dep);
	    }

	  if ( pt > 3 && pt < 3.5 )
	    {
	      dep5->Fill(dep);
	    }

	  if ( pt > 3.5 && pt < 4 )
	    {
	      dep6->Fill(dep);
	    }

	  if ( pt > 4 && pt < 5 )
	    {
	      dep7->Fill(dep);
	    }

	  if ( pt > 5 && pt < 6 )
	    {
	      dep8->Fill(dep);
	    }
	  if ( pt > 6 && pt < 8 )
	    {
	      dep9->Fill(dep);
	    }
	}
    }

 TFile* outfile = new TFile("dataFiles/swappedhadrondepstudy.root","RECREATE");
 cout << " outputting swapped hadron information (from data) into : dataFiles/swappedhadronstudy.root" << endl;
 // hptno0->Write();
 // hptn00->Write();
 // hptn01->Write();
 // hptn03->Write();
 // dep1->Write();
 // dep2->Write();
 // dep3->Write();
 // dep4->Write();
 // dep5->Write();
 // dep6->Write();
 // dep7->Write();
 // dep8->Write();
 // dep9->Write();
 h_pt_no0->Write();
 h_pt_n00->Write();
 h_pt_n01->Write();
 h_pt_n02->Write();
 h_pt_n03->Write();
 h_pt_n04->Write();

}
