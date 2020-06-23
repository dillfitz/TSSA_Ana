void datasurvival()
{
  gStyle->SetOptStat(0);
  TFile*infile1 = TFile::Open("../../../../AllRuns_736_ana644.root");

  TH1F* hadconversions = (TH1F*) infile1->Get("hadconversions");
  TH1F* hadconversions2 = (TH1F*) infile1->Get("hadconversions_2");
  TH1F* hadconversions3 = (TH1F*) infile1->Get("hadconversions_3");
  TH1F* hadconversions4 = (TH1F*) infile1->Get("hadconversions_4");
  TH1F* hadconversions5 = (TH1F*) infile1->Get("hadconversions_5");

  TH1F* hadnotconversions = (TH1F*) infile1->Get("hadnotconversions");
  TH1F* hadnotconversions2 = (TH1F*) infile1->Get("hadnotconversions_2");
  TH1F* hadnotconversions3 = (TH1F*) infile1->Get("hadnotconversions_3");
  TH1F* hadnotconversions4 = (TH1F*) infile1->Get("hadnotconversions_4");
  TH1F* hadnotconversions5 = (TH1F*) infile1->Get("hadnotconversions_5");

  // double bins[10]  = {1.0, 1.5, 2.0, 2.5, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0};
  // double bins[16]  = {1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.5, 4.0, 4.5 , 5.0, 6.0, 8.0, 10.0};
  double bins[16]  = {1.0, 1.2, 1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.5, 4.0, 4.5 , 5.0, 6.0, 8.0, 10.0};

  TH1F* hadconversions = (TH1F*)hadconversions->Rebin(14,"hadconversions",bins);
  TH1F* hadconversions2 = (TH1F*)hadconversions2->Rebin(14,"hadconversions2",bins);
  TH1F* hadconversions3 = (TH1F*)hadconversions3->Rebin(14,"hadconversions3",bins);
  TH1F* hadconversions4 = (TH1F*)hadconversions4->Rebin(14,"hadconversions4",bins);
  TH1F* hadconversions5 = (TH1F*)hadconversions5->Rebin(14,"hadconversions5",bins);

  TH1F* hadnotconversions = (TH1F*)hadnotconversions->Rebin(14,"hadnotconversions",bins);
  TH1F* hadnotconversions2 = (TH1F*)hadnotconversions2->Rebin(14,"hadnotconversions2",bins);
  TH1F* hadnotconversions3 = (TH1F*)hadnotconversions3->Rebin(14,"hadnotconversions3",bins);
  TH1F* hadnotconversions4 = (TH1F*)hadnotconversions4->Rebin(14,"hadnotconversions4",bins);
  TH1F* hadnotconversions5 = (TH1F*)hadnotconversions5->Rebin(14,"hadnotconversions5",bins);



  TH1F* h_hadrons = (TH1F*) hadconversions->Clone("h_hadrons");
  TH1F* h_hadrons2 = (TH1F*) hadconversions2->Clone("h_hadrons2");
  TH1F* h_hadrons3 = (TH1F*) hadconversions3->Clone("h_hadrons3");
  TH1F* h_hadrons4 = (TH1F*) hadconversions4->Clone("h_hadrons4");
  TH1F* h_hadrons5 = (TH1F*) hadconversions5->Clone("h_hadrons5");

  h_hadrons->Add(hadnotconversions);
  h_hadrons2->Add(hadnotconversions2);
  h_hadrons3->Add(hadnotconversions3);
  h_hadrons4->Add(hadnotconversions4);
  h_hadrons5->Add(hadnotconversions5);

  TH1F* h_survival = (TH1F*) hadnotconversions->Clone("h_survival");
  TH1F* h_survival2 = (TH1F*) hadnotconversions2->Clone("h_survival2");
  TH1F* h_survival3 = (TH1F*) hadnotconversions3->Clone("h_survival3");
  TH1F* h_survival4 = (TH1F*) hadnotconversions4->Clone("h_survival4");
  TH1F* h_survival5 = (TH1F*) hadnotconversions5->Clone("h_survival5");

  h_survival->Divide(h_hadrons);
  h_survival2->Divide(h_hadrons2);
  h_survival3->Divide(h_hadrons3);
  h_survival4->Divide(h_hadrons4);
  h_survival5->Divide(h_hadrons5);




  h_survival->SetLineColor(1);
  h_survival2->SetLineColor(2);
  h_survival3->SetLineColor(3);
  h_survival4->SetLineColor(4);
  h_survival5->SetLineColor(kViolet+1);





  h_survival->SetAxisRange(0.5,1,"Y");

  h_survival->SetTitle(";p_{T};#epsilon_{uc}");
  h_survival3->SetTitle(";p_{T};#epsilon_{uc}");

  TCanvas*c1 = new TCanvas("c1","",500,500);
  h_survival->Draw();
  h_survival2->Draw("SAME");
  h_survival3->DrawCopy("SAME");
  h_survival4->Draw("SAME");
  h_survival5->Draw("SAME");

  TLegend* l1 = new TLegend(0.1,0.1,0.3,0.4);
  l1->AddEntry(h_survival,"1x");
  l1->AddEntry(h_survival2,"1.5x");
  l1->AddEntry(h_survival3,"2x");
  l1->AddEntry(h_survival4,"4x");
  l1->AddEntry(h_survival5,"10x");
  l1->Draw();

  // l1->Draw();

  c1->SaveAs("plots/data_survival.png");

  h_survival3->SetAxisRange(0.5,1,"Y");
  h_survival3->SetLineColor(1);
  TCanvas*c2 = new TCanvas("c2","",500,500);
  h_survival3->SetAxisRange(1.5,5.5,"X");
  h_survival3->Draw();


  TFile* outfile = new TFile("../survivalrates_data.root","RECREATE");
  h_survival->Write();
  h_survival2->Write();
  h_survival3->Write();
  h_survival4->Write();
  h_survival5->Write();




}
