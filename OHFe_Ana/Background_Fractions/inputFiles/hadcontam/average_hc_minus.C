#include "../../../../Asymmetry_Ana/Constants.h"

void average_hc_minus()
{
  const bool nv = 0;
  const int nbins = NUM_VALUE_BINS;

  if (nv)
    TFile* infile1 = TFile::Open("depfitting/dataFiles/hadronnorm_depfit_noveto_minus.root");
  else
    TFile* infile1 = TFile::Open("depfitting/dataFiles/hadronnorm_depfit_minus.root");

  TH1F* h_depfit = (TH1F*)infile1->Get("h_hadfrac")->Clone("h_depfit");

  if (nv)
    TFile*infile2= TFile::Open("algebraic/dataFiles/algebraic_hadroncontam_noveto_minus.root");
  else
    TFile*infile2= TFile::Open("algebraic/dataFiles/algebraic_hadroncontam_minus.root");


  TH1F* h_algebraic_high = (TH1F*)infile2->Get("h_hadron_contam_n03");
  TH1F* h_algebraic_low = (TH1F*)infile2->Get("h_hadron_contam_n01");
  TH1F* h_algebraic = (TH1F*)infile2->Get("h_hadron_contam")->Clone("h_algebraic");

  
  TFile*infile3= TFile::Open("ert_sh_2x_n03.root");
  TH1F* shpt1 = (TH1F*)infile3->Get("shptdist1");
  TH1F* shpt2 = (TH1F*)infile3->Get("shptdist2");
  TH1F* ecpt1 = (TH1F*)infile3->Get("ptdist1");
  TH1F* ecpt2 = (TH1F*)infile3->Get("ptdist2");
  shpt1->Add(shpt2);
  ecpt1->Add(ecpt2);

  TH1F* h_shcontam = (TH1F*)shpt1->Clone("h_shcontam");
  h_shcontam->Divide(ecpt1);

  // TH1F* h_algebraic = (TH1F*)h_algebraic_low->Clone("h_algebraic");
  // h_algebraic->SetBinContent(13,h_algebraic_high->GetBinContent(13));
  h_algebraic->SetLineColor(kGreen+2);

  h_algebraic->SetAxisRange(0,0.1,"Y");
  h_depfit->SetAxisRange(0,0.1,"Y");
  //h_depfit->SetAxisRange(1,5.5,"X");
  h_algebraic->SetTitle(";p_{T};n_{hadrons}/n_{ec}");
  TCanvas*c1 = new TCanvas("c1","",500,500);
  h_algebraic->SetTitleOffset(1.5,"Y");
  h_algebraic->Draw();
  h_depfit->Draw("SAME");
  h_shcontam->SetLineColor(kViolet);
  //h_shcontam->Draw("SAME");

  c1->SaveAs("hadcontammethods_minus.png");

  TH1F* h_hadcontam = (TH1F*)h_depfit->Clone("h_hadcontam");

  for (int i = 1; i <= nbins;i++)
    {
      float mean = (h_algebraic->GetBinContent(i)/h_algebraic->GetBinError(i) + h_depfit->GetBinContent(i)/h_depfit->GetBinError(i))/( 1/h_algebraic->GetBinError(i) + 1/h_depfit->GetBinError(i));
      h_hadcontam->SetBinContent(i,mean);

      // h_hadcontam->SetBinError(i,TMath::Sqrt(h_algebraic->GetBinError(i)**2 + h_depfit->GetBinError(i)**2));
      h_hadcontam->SetBinError(i,TMath::Sqrt(1/((1/h_algebraic->GetBinError(i))**2 + (1/h_depfit->GetBinError(i))**2)));


    }

  h_hadcontam->SetLineColor(2);
  // h_hadcontam->Draw("SAME");


  TLegend* l1 = new TLegend(0.1,0.80,0.4,0.9);
  l1->AddEntry(h_depfit,"dep fitting");
  //l1->AddEntry(h_shcontam,"swapped");
  l1->AddEntry(h_algebraic,"algebraic");
  // l1->AddEntry(h_hadcontam,"average");
  l1->Draw();

  h_hadcontam->SetLineColor(kYellow+2);
  h_hadcontam->SetTitle(";p_{T};n_{hadrons}/n_{ec}");
  h_hadcontam->SetTitleOffset(1.5,"Y");

  TBox*box[nbins];
  for (int i = 0; i < nbins ;i++)
    {
      box[i] = new TBox(h_hadcontam->GetBinCenter(i+1)-0.05,h_depfit->GetBinContent(i+1),h_hadcontam->GetBinCenter(i+1)+0.05,h_algebraic->GetBinContent(i+1));
      box[i]->SetFillColor(kYellow+2);
      box[i]->SetFillStyle(3001);
    }



  TCanvas*c2 = new TCanvas("c2","",500,500);
  h_hadcontam->Draw();
  for (int i = 0; i < nbins;i++)
    {
      box[i]->Draw("SAME");
    }
  c2->SaveAs("hadcontam_minus.png");
  TLegend* l2 = new TLegend(0.1,0.83,0.6,0.9);
  l2->AddEntry(h_hadcontam,"Hadron Contamination");
  l2->Draw();

  TString outfileName = "../hadcontam";
  if (nv)
    outfileName += "_noveto";

  outfileName += "_minus.root";

  TFile* outfile = new TFile(outfileName,"RECREATE");
  cout << " outputting hadron contamination calculations into : " << outfileName << endl;
    
  h_hadcontam->Write();
  h_algebraic->Write();
  h_depfit->Write();

}
