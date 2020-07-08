void fnpcalcc_ept()
{

  const int nbins = 10;
  double bins[nbins+1]  = {1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.5, 4.0, 4.5 , 5.0, 6.0};
  TFile*data = TFile::Open("/phenix/hhj/trinn/run15pro108/combinedanataxi/combined_good_ert_100mevbins.root");

  
  //const int nbins = 6;
  //double bins[nbins+1]  = {1.0, 1.25, 1.5, 1.8, 2.1, 2.7, 6.0};
  //TFile*data = TFile::Open("/phenix/spin/phnxsp01/dillfitz/taxi/Run15pp200CAERTP108/AN_SAMPLE/16370/data/AllRuns_725_ana644.root");
  


  
  TCut lowpt  = "pt < 5 && (quality == 31 ||quality == 63) && chisq/ndf < 3 && (hitpat&3) == 3 && nhit > 2 && abs(zed)<75 && sigemcdphi > -3 && sigemcdphi < 3 && sigemcdz > -3 && sigemcdz < 3 && disp < 5 && dep < 2 && dep > -2 && n0 > 1 && prob > 0.01 && dcat > -0.2 && dcat < 0.2";
  TCut highpt = "pt >= 5&& (quality == 31 ||quality == 63) && chisq/ndf < 3 && (hitpat&3) == 3 && nhit > 2 && abs(zed)<75 && sigemcdphi > -3 && sigemcdphi < 3 && sigemcdz > -3 && sigemcdz < 3 && disp < 5 && dep < 2 && dep > -2 && n0 > 3 && prob > 0.2 && dcat > -0.2 && dcat < 0.2";
  
  /*
  TCut lowpt  = "pt < 5 && (quality == 31 ||quality == 63) && chisq/ndf < 3 && (hitpattern&3) == 3 && nhit > 2 && abs(zed)<75 && sigemcdphi > -3 && sigemcdphi < 3 && sigemcdz > -3 && sigemcdz < 3 && disp < 5 && dep < 2 && dep > -2 && n0 > 1 && prob > 0.01 && dcat > -0.2 && dcat < 0.2";
  TCut highpt = "pt >= 5&& (quality == 31 ||quality == 63) && chisq/ndf < 3 && (hitpattern&3) == 3 && nhit > 2 && abs(zed)<75 && sigemcdphi > -3 && sigemcdphi < 3 && sigemcdz > -3 && sigemcdz < 3 && disp < 5 && dep < 2 && dep > -2 && n0 > 3 && prob > 0.2 && dcat > -0.2 && dcat < 0.2";
  */


  TH1F* h_ept_lowpt_nv = new TH1F("h_ept_lowpt_nv",";pT;",nbins,bins);
  TH1F* h_ept_highpt_nv = new TH1F("h_ept_highpt_nv",";pT;",nbins,bins);

  TH1F* h_ept_lowpt_conveto = new TH1F("h_ept_lowpt_conveto",";pT;",nbins,bins);
  TH1F* h_ept_highpt_conveto = new TH1F("h_ept_highpt_conveto",";pT;",nbins,bins);

  h_ept_lowpt_nv->Sumw2();
  h_ept_highpt_nv->Sumw2();

  h_ept_lowpt_conveto->Sumw2();
  h_ept_highpt_conveto->Sumw2();

  TCanvas*c3 = new TCanvas("c3","",500,500);


  ntpesvx->Draw("pt>>h_ept_lowpt_nv",lowpt);
  ntpesvx->Draw("pt>>h_ept_highpt_nv",highpt);
  ntpesvx->Draw("pt>>h_ept_lowpt_conveto",lowpt && "conversionveto2x == 1");
  ntpesvx->Draw("pt>>h_ept_highpt_conveto",highpt && "conversionveto2x == 1");

  //e_svx_tree->Draw("pt>>h_ept_lowpt_nv",lowpt);
  //e_svx_tree->Draw("pt>>h_ept_highpt_nv",highpt);
  //e_svx_tree->Draw("pt>>h_ept_lowpt_conveto",lowpt && "conversionveto2x == 1");
  //e_svx_tree->Draw("pt>>h_ept_highpt_conveto",highpt && "conversionveto2x == 1");

  TH1F* h_ept_nv = (TH1F*)h_ept_lowpt_nv->Clone("h_ept_nv");
  h_ept_nv->Add(h_ept_highpt_nv);

  TH1F* h_ept_conveto = (TH1F*)h_ept_lowpt_conveto->Clone("h_ept_conveto");
  h_ept_conveto->Add(h_ept_highpt_conveto);



  TFile* outfile = new TFile("../forfnp_test.root","RECREATE");

  h_ept_nv->Write();
  h_ept_conveto->Write();

}
