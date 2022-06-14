#include "../../Asymmetry_Ana/Constants.h"

void sysplots_minus()
{

  //gStyle->SetCanvasPreferGL(1);
  gStyle->SetOptStat(0);

  const int nbins = NUM_VALUE_BINS;

  TFile* infile1= TFile::Open("outputFiles/normalizations_minus.root");

  TH1F* h_piz_n = (TH1F*)infile1->Get("h_piz_n");
  TH1F* h_eta_n = (TH1F*)infile1->Get("h_eta_n");
  TH1F* h_jpsi_n = (TH1F*)infile1->Get("h_jpsi_n");
  TH1F* h_ke3_n = (TH1F*)infile1->Get("h_ke3_n");
  TH1F* h_photon_n = (TH1F*)infile1->Get("h_photon_n");
  TH1F* h_hadcontam_n = (TH1F*)infile1->Get("h_hadcontam_n");



  TFile* infile2 = TFile::Open("outputFiles/systematics_minus.root");

  TH1F* h_sys_piz = (TH1F*)infile2->Get("h_sys_piz");
  TH1F* h_sys_eta = (TH1F*)infile2->Get("h_sys_eta");
  TH1F* h_sys_ke3 = (TH1F*)infile2->Get("h_sys_ke3");
  TH1F* h_sys_jpsi = (TH1F*)infile2->Get("h_sys_jpsi");
  TH1F* h_sys_photon = (TH1F*)infile2->Get("h_sys_photon");



  TFile* infile3 = TFile::Open("inputFiles/hadcontam_minus.root");

  TH1F* h_sys_hadlow = (TH1F*)infile3->Get("h_depfit")->Clone("h_sys_hadlow");
  TH1F* h_sys_hadhigh = (TH1F*)infile3->Get("h_algebraic")->Clone("h_sys_hadhigh");
  TH1F* h_sys_hadnom = (TH1F*)infile3->Get("h_hadcontam")->Clone("h_sys_hadnom");

  TBox* pizuc[nbins];
  TBox* etauc[nbins];
  TBox* jpsiuc[nbins];
  TBox* ke3uc[nbins];
  TBox* photonuc[nbins];
  TBox* hadronuc[nbins];

  // it appears that the pt array is here solely to determine the width of the systematic box //
  if (nbins == 4)
  {
    float pt[] = {1.6,1.7,1.9,2.0,2.35,2.45,3.85,3.95};
  }
  else if (nbins == 6)
  {
    float pt[] = {1.1, 1.2, 1.35, 1.45, 1.6,1.7,1.9,2.0,2.35,2.45,3.8,3.9};  
  }
  for (int i = 0; i < nbins;i++)
    {
      pizuc[i] = new TBox(pt[2*i],h_piz_n->GetBinContent(i+1)-h_sys_piz->GetBinContent(i+1),pt[2*i+1],h_piz_n->GetBinContent(i+1)+h_sys_piz->GetBinContent(i+1));
      etauc[i] = new TBox(pt[2*i],h_eta_n->GetBinContent(i+1)-h_sys_eta->GetBinContent(i+1),pt[2*i+1],h_eta_n->GetBinContent(i+1)+h_sys_eta->GetBinContent(i+1));
      jpsiuc[i] = new TBox(pt[2*i],h_jpsi_n->GetBinContent(i+1)-h_sys_jpsi->GetBinContent(i+1),pt[2*i+1],h_jpsi_n->GetBinContent(i+1)+h_sys_jpsi->GetBinContent(i+1));
      ke3uc[i] = new TBox(pt[2*i],h_ke3_n->GetBinContent(i+1)-h_sys_ke3->GetBinContent(i+1),pt[2*i+1],h_ke3_n->GetBinContent(i+1)+h_sys_ke3->GetBinContent(i+1));
      photonuc[i] = new TBox(pt[2*i],h_photon_n->GetBinContent(i+1)-h_sys_photon->GetBinContent(i+1),pt[2*i+1],h_photon_n->GetBinContent(i+1)+h_sys_photon->GetBinContent(i+1));
      hadronuc[i] = new TBox(pt[2*i],h_sys_hadlow->GetBinContent(i+1),pt[2*i+1],h_sys_hadhigh->GetBinContent(i+1));

    }

  TLegend* legend = new TLegend(0.55,0.6,0.9,0.9);
	legend->SetLineColor(0);  
  legend->AddEntry(h_piz_n," #pi^{0}");
  legend->AddEntry(h_eta_n," #eta");
  legend->AddEntry(h_jpsi_n," J/#psi");
  //legend->AddEntry(h_ke3_n," Ke3");
  legend->AddEntry(h_photon_n," direct #gamma");
  legend->AddEntry(h_hadcontam_n," hadron contamination");
	legend->SetMargin(0.1);
	legend->SetTextSize(0.04);
	legend->SetMargin(0.05);  
  
	TLegend *legend2 = new TLegend(0.21, 0.7, 0.375, 0.9);
	legend2->SetLineColor(0);
	legend2->AddEntry((TObject*)0, "p^{#uparrow}+p #rightarrow e^{-} + X", "");
	legend2->AddEntry((TObject*)0, "#sqrt{s} = 200 GeV", "");	
	legend2->AddEntry((TObject*)0, "|#eta| < 0.35", "");		
	legend2->AddEntry((TObject*)0, "#bf{PHENIX}", "");					
	legend2->SetTextSize(0.04);
	legend2->SetMargin(0.05);  

  h_jpsi_n->SetLineColor(51);
  h_photon_n->SetLineColor(4);
  h_hadcontam_n->SetLineColor(kGreen+2);
  h_ke3_n->SetLineColor(kOrange);
  h_eta_n->SetLineColor(1);

  h_piz_n->SetMarkerStyle(20);
  h_jpsi_n->SetMarkerStyle(21);
  h_photon_n->SetMarkerStyle(22);
  h_hadcontam_n->SetMarkerStyle(34);
  h_ke3_n->SetMarkerStyle(23);
  h_eta_n->SetMarkerStyle(33);
  
  h_photon_n->SetMarkerSize(1.3);
  h_eta_n->SetMarkerSize(1.7);  
  h_ke3_n->SetMarkerSize(1.3);  
  h_hadcontam_n->SetMarkerSize(1.3);  

  h_piz_n->SetMarkerColor(h_piz_n->GetLineColor());
  h_jpsi_n->SetMarkerColor(h_jpsi_n->GetLineColor());
  h_photon_n->SetMarkerColor(h_photon_n->GetLineColor());
  h_hadcontam_n->SetMarkerColor(h_hadcontam_n->GetLineColor());
  h_ke3_n->SetMarkerColor(h_ke3_n->GetLineColor());
  h_eta_n->SetMarkerColor(h_eta_n->GetLineColor());

  cout << "bg fractions minus" << endl;
  cout << std::setprecision(3) << "                \\hline" << endl;
  cout << "                1.0 -- 1.3 & 1.161  & " << h_piz_n->GetBinContent(1) << " & " << h_eta_n->GetBinContent(1) << " & " << h_photon_n->GetBinContent(1) << " & " << h_jpsi_n->GetBinContent(1) << " & " << h_hadcontam_n->GetBinContent(1) << " \\\\ " << endl;
  cout << "                1.3 -- 1.5 & 1.398  & " << h_piz_n->GetBinContent(2) << " & " << h_eta_n->GetBinContent(2) << " & " << h_photon_n->GetBinContent(2) << " & " << h_jpsi_n->GetBinContent(2) << " & " << h_hadcontam_n->GetBinContent(2) << " \\\\ " << endl;
  cout << "                1.5 -- 1.8 & 1.639  & " << h_piz_n->GetBinContent(3) << " & " << h_eta_n->GetBinContent(3) << " & " << h_photon_n->GetBinContent(3) << " & " << h_jpsi_n->GetBinContent(3) << " & " << h_hadcontam_n->GetBinContent(3) << " \\\\ " << endl;
  cout << "                1.8 -- 2.1 & 1.936  & " << h_piz_n->GetBinContent(4) << " & " << h_eta_n->GetBinContent(4) << " & " << h_photon_n->GetBinContent(4) << " & " << h_jpsi_n->GetBinContent(4) << " & " << h_hadcontam_n->GetBinContent(4) << " \\\\ " << endl;
  cout << "                2.1 -- 2.7 & 2.349  & " << h_piz_n->GetBinContent(5) << " & " << h_eta_n->GetBinContent(5) << " & " << h_photon_n->GetBinContent(5) << " & " << h_jpsi_n->GetBinContent(5) << " & " << h_hadcontam_n->GetBinContent(5) << " \\\\ " << endl;
  cout << "                2.7 -- 5.0 & 3.290  & " << h_piz_n->GetBinContent(6) << " & " << h_eta_n->GetBinContent(6) << " & " << h_photon_n->GetBinContent(6) << " & " << h_jpsi_n->GetBinContent(6) << " & " << h_hadcontam_n->GetBinContent(6) << " \\\\ " << endl;
  cout << "                \\hline" << endl;
  cout << "                \\hline" << endl;   

  h_piz_n->SetAxisRange(0.0005,300,"Y");
  h_piz_n->SetTitle(";p_{T} [GeV/c]; background fractions");
  h_piz_n->GetXaxis()->SetTitleOffset(1.2); 
  h_piz_n->GetXaxis()->SetTitleSize(0.05);  
  h_piz_n->GetXaxis()->SetLabelSize(0.045);          
  h_piz_n->GetYaxis()->SetTitleOffset(1.3);
  h_piz_n->GetYaxis()->SetTitleSize(0.05);  
  h_piz_n->GetYaxis()->SetLabelSize(0.045);
  TCanvas*c1 = new TCanvas("c1","", 600, 600);
  c1->SetMargin(0.15,0.05,0.15,0.05);
  
  gPad->SetLogy();
  h_piz_n->Draw();
  
  for( int i = 0; i < nbins;i++)
    {

      pizuc[i]->SetFillColor(h_piz_n->GetLineColor());
      pizuc[i]->SetFillStyle(3001);
      pizuc[i]->Draw("SAME");


      jpsiuc[i]->SetFillColor(h_jpsi_n->GetLineColor());
      jpsiuc[i]->SetFillStyle(3001);
      jpsiuc[i]->Draw("SAME");

      photonuc[i]->SetFillColor(h_photon_n->GetLineColor());
      photonuc[i]->SetFillStyle(3001);
      photonuc[i]->Draw("SAME");


      hadronuc[i]->SetFillColor(h_hadcontam_n->GetLineColor());
      hadronuc[i]->SetFillStyle(3001);
      hadronuc[i]->Draw("SAME");


      etauc[i]->SetFillColor(h_eta_n->GetLineColor());
      etauc[i]->SetFillStyle(3001);
      etauc[i]->Draw("SAME");



      //ke3uc[i]->SetFillColor(h_ke3_n->GetLineColor());
      //ke3uc[i]->SetFillStyle(3001);
      //ke3uc[i]->Draw("SAME");

    }
  h_piz_n->Draw("SAME");
  h_eta_n->Draw("SAME");
  h_jpsi_n->Draw("SAME");
  //h_ke3_n->Draw("SAME");
  h_photon_n->Draw("SAME");
  h_hadcontam_n->Draw("SAME");

  legend->Draw();
  legend2->Draw();


  c1->SaveAs("normalizations_w_sys_minus.png");
  c1->SaveAs("normalizations_w_sys_minus.pdf");  


}
