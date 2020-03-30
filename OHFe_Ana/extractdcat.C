void extractdcat()
{
  TFile*infile = TFile::Open("AllRuns.root");
  TTree *tesvx = (TTree*)infile->Get("e_svx_tree");
  //TTree*tesvxbg = (TTree*)infile->Get("ntpesvxbg");
  //TTree*tsesvx = (TTree*)infile->Get("ntpshsvx");




  TCut chisqndf = "chisq/ndf < 3";
  TCut dep1 = "dep < 2 && dep > -2";
  TCut dep2 = "dep < 2 && dep > -2";
  TCut quality = "quality == 31 || quality == 63";
  TCut hitpat = "(hitpattern&3) == 3";
  TCut nhit = "nhit > 2";
  TCut zed = "abs(zed) < 75";
  TCut sigemcdphi = "abs(sigemcdphi) < 3";
  TCut sigemcdz = "abs(sigemcdz) < 3";
  TCut convveto = "conversionveto2x == 1";
  TCut prob1 = "prob > 0.01";
  TCut prob2 = "prob > 0.2";
  TCut n01 = "n0 > 1";
  TCut n02 = "n0 > 3";
  TCut sn01 = "sn0 > 1";
  TCut sn02 = "sn0 > 1";
  TCut disp = "disp < 5";
  TCut sdisp = "sdisp < 5";


  TCut lowpt = "pt < 5" && chisqndf && quality && hitpat && nhit && zed && sigemcdphi && sigemcdz && convveto && prob1 && n01 && disp && dep1;

  TCut highpt = "pt >= 5 " && chisqndf && quality && hitpat && nhit && zed && sigemcdphi && sigemcdz && convveto && prob2 && n02 && disp && dep2;

  TCut slowpt = "pt < 5" && chisqndf && quality && hitpat && nhit && zed && sigemcdphi && sigemcdz && convveto && prob1 && sn01 && sdisp && dep1;

  TCut shighpt = "pt >= 5 " && chisqndf && quality && hitpat && nhit && zed && sigemcdphi && sigemcdz && convveto && prob2 && sn02 && sdisp && dep1;

  TH2F* ptdcat1 = new TH2F("ptdcat1","",100,0,10,200,-0.2,0.2);
  TH2F* ptdcat2 = new TH2F("ptdcat2","",100,0,10,200,-0.2,0.2);

  TH2F* bgptdcat1 = new TH2F("bgptdcat1","",100,0,10,200,-0.2,0.2);
  TH2F* bgptdcat2 = new TH2F("bgptdcat2","",100,0,10,200,-0.2,0.2);

  TH2F* shptdcat1 = new TH2F("shptdcat1","",100,0,10,200,-0.2,0.2);
  TH2F* shptdcat2 = new TH2F("shptdcat2","",100,0,10,200,-0.2,0.2);

  // Float_t bins[] = {1,1.5,2,2.5,3,4,5,6,8};
  // float bin[] = {16,18,19,21,22,24,25,27,28,30,31,35,36,40,41,45,46,50,51,60};
  double ptbins[16]  = {1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.5, 4.0, 4.5 , 5.0, 6.0};


  TH1F* ptdist1 = new TH1F("ptdist1",";pT;",10,ptbins);
  TH1F* ptdist2 = new TH1F("ptdist2",";pT;",10,ptbins);


  ptdist1->Sumw2();
  ptdist2->Sumw2();


  ptdcat1->Sumw2();
  ptdcat2->Sumw2();

  //bgptdcat1->Sumw2();
  //bgptdcat2->Sumw2();

  //shptdcat1->Sumw2();
  //shptdcat2->Sumw2();

  tesvx->Draw("dcat:pt>>ptdcat1",lowpt);
  tesvx->Draw("dcat:pt>>ptdcat2",highpt);

  //ntpesvxbg->Draw("dcat:pt>>bgptdcat1",lowpt);
  //ntpesvxbg->Draw("dcat:pt>>bgptdcat2",highpt);

  //ntpshsvx->Draw("dcat:pt>>shptdcat1",slowpt);
  //ntpshsvx->Draw("dcat:pt>>shptdcat2",shighpt);

  tesvx->Draw("pt>>ptdist1",lowpt);
  tesvx->Draw("pt>>ptdist2",highpt);




  TFile* outfile = new TFile("ert_dcat.root","RECREATE");
  ptdcat1->Write();
  ptdcat2->Write();

  ptdist1->Write();
  ptdist2->Write();







}
