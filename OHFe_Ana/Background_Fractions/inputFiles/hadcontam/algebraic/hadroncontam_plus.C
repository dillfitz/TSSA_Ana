#include "../../../../../Asymmetry_Ana/Constants.h"
void hadroncontam_plus()
{
  const bool nv = 0;

  gStyle->SetOptStat(0);

  const int nbins = NUM_VALUE_BINS;

  Double_t bins[nbins+1];
  for (int i=0; i<nbins+1; ++i)
  {
    bins[i] = VALUE_BINS[i];
    std::cout << bins[i] << std::endl;
  }



  if (nv)
    TFile*infile = TFile::Open("dataFiles/hadrondepstudy_noveto_plus.root");
  else
    TFile*infile = TFile::Open("dataFiles/hadrondepstudy_plus.root");

  // It appears that the sh histos are not actually used in the calculation... confirm with Tim //
  // It appears that the pions MC input is not used either... Though it is used to generate h_survival_pi_n00, this is not used anywhere in the calculation. //
  // As long as hadrondepstudy.root comes from dep<-6 in the electron tree, then the analysis notes claims reflect what is done in this file. //
  TH1F* h_ptno0 = (TH1F*)infile->Get("hptno0");
  TH1F* h_ptn00 = (TH1F*)infile->Get("hptn00");
  TH1F* h_ptn01 = (TH1F*)infile->Get("hptn01");
  TH1F* h_ptn03 = (TH1F*)infile->Get("hptn03");
  TH1F* h_pt_non0 = (TH1F*)infile->Get("h_pt_no0");
  TH1F* h_pt_n01 = (TH1F*)infile->Get("h_pt_n01");
  TH1F* h_pt_n03 = (TH1F*)infile->Get("h_pt_n03");

  TH1F* h_dept_non0 = (TH1F*)infile->Get("e_pt_no0");
  TH1F* h_dept_n01 = (TH1F*)infile->Get("e_pt_n01");
  TH1F* h_dept_n03 = (TH1F*)infile->Get("e_pt_n03");


  TFile* infile2 = TFile::Open("dataFiles/swappedhadrondepstudy.root");
  TH1F* h_sh_non0 = (TH1F*)infile2->Get("h_pt_no0");
  TH1F* h_sh_n00 = (TH1F*)infile2->Get("h_pt_n00");
  TH1F* h_sh_n01 = (TH1F*)infile2->Get("h_pt_n01");
  TH1F* h_sh_n03 = (TH1F*)infile2->Get("h_pt_n03");




  TFile* electrons = TFile::Open("dataFiles/electronsurvival.root");

  TH1F* h_ept_non0 = (TH1F*)electrons->Get("h_pt_non0");
  TH1F* h_ept_n00 = (TH1F*)electrons->Get("h_pt_n00");
  TH1F* h_ept_n03 = (TH1F*)electrons->Get("h_pt_n03");
  TH1F* h_ept_n01 = (TH1F*)electrons->Get("h_pt_n01");


  //TFile* pions = TFile::Open("dataFiles/pionsurvival.root");

  //TH1F* h_hpt_non0 = (TH1F*)pions->Get("h_pt_non0");
  //TH1F* h_hpt_n00 = (TH1F*)pions->Get("h_pt_n00");
  //TH1F* h_hpt_n03 = (TH1F*)pions->Get("h_pt_n03");
  //TH1F* h_hpt_n01 = (TH1F*)pions->Get("h_pt_n01");


  TH1F* h_ptno0 = (TH1F*) h_ptno0->Rebin(nbins,"h_ptno0",bins);
  TH1F* h_ptn00 = (TH1F*) h_ptn00->Rebin(nbins,"h_ptn00",bins);
  TH1F* h_ptn01 = (TH1F*) h_ptn01->Rebin(nbins,"h_ptn01",bins);
  TH1F* h_ptn03 = (TH1F*) h_ptn03->Rebin(nbins,"h_ptn03",bins);
  TH1F* h_pt_non0 = (TH1F*) h_pt_non0->Rebin(nbins,"h_pt_non0",bins);
  TH1F* h_pt_n01 = (TH1F*) h_pt_n01->Rebin(nbins,"h_pt_n01",bins);
  TH1F* h_pt_n03 = (TH1F*) h_pt_n03->Rebin(nbins,"h_pt_n03",bins);

  TH1F* h_dept_non0 = (TH1F*) h_dept_non0->Rebin(nbins,"h_dept_non0",bins);
  TH1F* h_dept_n01 = (TH1F*) h_dept_n01->Rebin(nbins,"h_dept_n01",bins);
  TH1F* h_dept_n03 = (TH1F*) h_dept_n03->Rebin(nbins,"h_dept_n03",bins);



  TH1F* h_sh_non0 = (TH1F*) h_sh_non0->Rebin(nbins,"h_sh_non0",bins);
  TH1F* h_sh_n00 = (TH1F*) h_sh_n00->Rebin(nbins,"h_sh_n00",bins);
  TH1F* h_sh_n01 = (TH1F*) h_sh_n01->Rebin(nbins,"h_sh_n01",bins);
  TH1F* h_sh_n03 = (TH1F*) h_sh_n03->Rebin(nbins,"h_sh_n03",bins);

  TH1F* h_ept_non0 = (TH1F*) h_ept_non0->Rebin(nbins,"h_ept_non0",bins);
  TH1F* h_ept_n00 = (TH1F*) h_ept_n00->Rebin(nbins,"h_ept_n00",bins); 
  TH1F* h_ept_n01 = (TH1F*) h_ept_n01->Rebin(nbins,"h_ept_n03",bins); 
  TH1F* h_ept_n03 = (TH1F*) h_ept_n03->Rebin(nbins,"h_ept_n01",bins); 

  //TH1F* h_hpt_non0 = (TH1F*) h_hpt_non0->Rebin(nbins,"h_hpt_non0",bins);
  //TH1F* h_hpt_n00 = (TH1F*) h_hpt_n00->Rebin(nbins,"h_hpt_n00",bins); 
  //TH1F* h_hpt_n01 = (TH1F*) h_hpt_n01->Rebin(nbins,"h_hpt_n03",bins); 
  //TH1F* h_hpt_n03 = (TH1F*) h_hpt_n03->Rebin(nbins,"h_hpt_n01",bins); 





  //apply correction factor to swapped hadrons to account for downscale
  for (int i = 1; i < nbins+4;i++)
    {
      if ( i < nbins )
	{
	  h_sh_n00->SetBinContent(i,0.1*h_sh_n00->GetBinContent(i));
	  h_sh_n01->SetBinContent(i,0.1*h_sh_n01->GetBinContent(i));
	  h_sh_n03->SetBinContent(i,0.1*h_sh_n03->GetBinContent(i));
	} 
      if ( i == nbins+1 )
	{
	  h_sh_n00->SetBinContent(i,0.5*h_sh_n00->GetBinContent(i));
	  h_sh_n01->SetBinContent(i,0.5*h_sh_n01->GetBinContent(i));
	  h_sh_n03->SetBinContent(i,0.5*h_sh_n03->GetBinContent(i));
	} 

    }


  TH1F* h_survival_sh_n00 = (TH1F*)h_sh_n00->Clone("h_survival_sh_n00");
  TH1F* h_survival_sh_n01 = (TH1F*)h_sh_n01->Clone("h_survival_sh_n01");
  TH1F* h_survival_sh_n03 = (TH1F*)h_sh_n03->Clone("h_survival_sh_n03");

  TH1F* h_survival_e_n00 = (TH1F*)h_ept_n00->Clone("h_survival_ept_n00");
  TH1F* h_survival_e_n01 = (TH1F*)h_ept_n01->Clone("h_survival_ept_n01");
  TH1F* h_survival_e_n03 = (TH1F*)h_ept_n03->Clone("h_survival_ept_n03");
  TH1F* h_survival_e_n01n03 = (TH1F*)h_ept_n03->Clone("h_survival_ept_n01n03");

  //TH1F* h_survival_pi_n00 = (TH1F*)h_hpt_n00->Clone("h_survival_hpt_n00");
  TH1F* h_survival_pi_n01 = (TH1F*)h_pt_n01->Clone("h_survival_hpt_n01");
  TH1F* h_survival_pi_n03 = (TH1F*)h_pt_n03->Clone("h_survival_hpt_n03");
  TH1F* h_survival_pi_n01n03 = (TH1F*)h_pt_n03->Clone("h_survival_pi_n01n03");


  // TH1F* h_survival_de_n00 = (TH1F*)h_dept_n00->Clone("h_survival_hpt_n00");
  TH1F* h_survival_de_n01 = (TH1F*)h_dept_n01->Clone("h_survival_hpt_n01");
  TH1F* h_survival_de_n03 = (TH1F*)h_dept_n03->Clone("h_survival_hpt_n03");
  TH1F* h_survival_de_n01n03 = (TH1F*)h_dept_n03->Clone("h_survival_hpt_n01n03");




  h_survival_sh_n00->Divide(h_pt_non0);
  h_survival_sh_n01->Divide(h_pt_non0);
  h_survival_sh_n03->Divide(h_pt_non0);

  h_survival_e_n00->Divide(h_ept_non0);
  h_survival_e_n01->Divide(h_ept_non0);
  h_survival_e_n03->Divide(h_ept_non0);
  h_survival_e_n01n03->Divide(h_ept_n01);

  //h_survival_pi_n00->Divide(h_hpt_non0);
  // h_survival_pi_n01->Divide(h_hpt_non0);
  h_survival_pi_n01->Divide(h_pt_non0);
  h_survival_pi_n03->Divide(h_pt_non0);
  h_survival_pi_n01n03->Divide(h_pt_n01);

  h_survival_de_n01->Divide(h_dept_non0);
  h_survival_de_n03->Divide(h_dept_non0);
  h_survival_de_n01n03->Divide(h_dept_n01);


  h_survival_sh_n01->SetAxisRange(0,1.1,"Y");
  h_survival_e_n01->SetAxisRange(0,1.2,"Y");
  h_survival_pi_n01->SetAxisRange(0.0001,1.1,"Y");

  h_survival_sh_n03->SetLineColor(2);
  h_survival_e_n03->SetLineColor(2);
  h_survival_pi_n03->SetLineColor(2);
  h_survival_e_n01n03->SetLineColor(kViolet+2);
  h_survival_pi_n01n03->SetLineColor(kViolet+2);

  TLegend* l1 = new TLegend(0.7,0.77,0.9,0.9);
  l1->AddEntry(h_survival_pi_n01,"#epsilon_{n0>1}");
  //l1->AddEntry(h_survival_pi_n03,"#epsilon_{n0>3}");
  // l1->AddEntry(h_survival_pi_n01n03,"n0 > 3/n0 > 1");


  h_survival_e_n01->SetTitle("Electrons;p_{T};#epsilon_{e}");
  h_survival_pi_n01->SetTitle("Hadrons;p_{T};#epsilon_{h}");
  h_survival_de_n01n03->SetLineColor(kGreen+2);

  TCanvas*c1 = new TCanvas("c1","",800,400);
  c1->Divide(2,1);

  c1->cd(1);
  h_survival_e_n01->Draw();
  //h_survival_e_n03->Draw("SAME");

  // h_survival_de_n01n03->Draw("SAME");
  // h_survival_e_n01n03->Draw("SAME");

  l1->Draw();
  c1->cd(2);
  gPad->SetLogy();
  h_survival_pi_n01->Draw();
  //h_survival_pi_n03->Draw("SAME");
  // h_survival_pi_n01n03->Draw("SAME");
  l1->Draw();



  TH1F* h_hadron_contam_n01 = new TH1F("h_hadron_contam_n01",";p_{T};",nbins,bins);
  TH1F* h_hadron_contam = new TH1F("h_hadron_contam",";p_{T};",nbins,bins);
  TH1F* h_hadron_contam_n03 = new TH1F("h_hadron_contam_n03",";p_{T};",nbins,bins);
  TH1F* h_hadron_contam_n01n03 = new TH1F("h_hadron_contam_n01n03",";p_{T};",nbins,bins);
  TH1F* h_hadron_contam_n03n01 = new TH1F("h_hadron_contam_n03n01",";p_{T};",nbins,bins);

  // TH1F* h_numtmp = (TH1F*)h_ptno0->Clone("h_numtmp");
  // TH1F* h_denomtmp = (TH1F*)h_survival_h->Clone("h_denotmp");
  // h_denomtmp->Add(h_survival_e,-1);
  // h_numtmp->Multiply(h_survival_e,-1);
  // h_numtmp->Add(h_ptn01);

  // h_numtmp->Divide(h_denomtmp);

  for (int i = 1; i < nbins+4;i++)
    {
      float n_esurv = h_survival_e_n01->GetBinContent(i);
      float n_hsurv = h_survival_pi_n01->GetBinContent(i);
      float n_shsurv = h_survival_sh_n01->GetBinContent(i);
      float n_non0 = h_ptno0->GetBinContent(i);
      float n_n01 = h_ptn01->GetBinContent(i);

      float uc_esurv = h_survival_e_n01->GetBinError(i);
      float uc_hsurv = h_survival_pi_n01->GetBinError(i);
      // float uc_non0 = 1/TMath::Sqrt(h_ptno0->GetBinContent(i));
      // float uc_n01 = 1/TMath::Sqrt(h_ptn01->GetBinContent(i));
      float uc_n01 = h_ptn01->GetBinError(i);
      float uc_non0 = h_ptno0->GetBinError(i);


      // float n_hadrons = (n_n01 - (n_shsurv+ n_esurv)*n_non0)/(n_hsurv-n_esurv);
      float n_hadrons = (n_n01 - (n_esurv)*n_non0)/(n_hsurv-n_esurv);

      float n_fhadrons_n01 = n_hadrons*n_hsurv/n_n01;
      float n_hsurv_n03 = h_survival_pi_n03->GetBinContent(i);
      float n_n03 = h_ptn03->GetBinContent(i);

      float n_fhadrons_n03 = n_hadrons*n_hsurv_n03/n_n03;

      // cout << n_n01 << " , " << uc_n01 << endl;

      float uc1 = n_esurv**2 * n_non0**2 * ( n_esurv-n_hsurv)**2*n_hsurv**2 *uc_n01**2;
      float uc2 = n_n01**2 * (n_hsurv**2 * (n_n01 - n_non0*n_hsurv)**2 * uc_esurv**2);

      float uc3 = n_n01**2 * n_esurv**2 * ((n_esurv-n_hsurv)**2 *n_hsurv**2 * uc_non0**2 + (n_n01 - n_esurv*n_non0)**2 * uc_hsurv**2);
      // float uc3 =  n_n01**2 * n_esurv**2*( *(n_esurv-n_hsurv)**2 * n_hsurv**2 * uc_non0**2 + (n_n01 - n_esurv*n_non0)**2*uc_hsurv**2);
      float uc4 = n_n01**4 *(n_esurv-n_hsurv)**4;

      // float uc_nh = TMath::Sqrt( ( (n_esurv - n_hsurv)**2*uc_n01**2 + (n_n01-n_non0*n_hsurv)**2 * uc_esurv**2 + n_esurv**2*(n_esurv-n_hsurv)**2 *uc_non0**2 + (n_n01 - n_esurv*n_non0)**2 *uc_hsurv**2)/((n_esurv-n_hsurv)**4));


      float uc_nh = TMath::Sqrt((uc1 + uc2 + uc3)/uc4);
      cout << uc_nh << endl;
      // cout << uc1 << " , " << uc2 << " , " << uc_nh << endl;


      h_hadron_contam_n01->SetBinContent(i,n_fhadrons_n01);

      h_hadron_contam_n03n01->SetBinContent(i,n_fhadrons_n03);
      // For 6 GeV Binning //
      //if ( i < nbins ) 
      // For 5 GeV Binning //
      if ( i <= nbins ) 
	{
	  h_hadron_contam->SetBinContent(i,n_fhadrons_n01);
	  h_hadron_contam->SetBinError(i,uc_nh);
	}
      // This is not quite right... since the cut is in the middle of the bin this method will not work... going to have to proceed with binning up to 5 GeV for now // 
      /*
      // For 6 GeV Binning //
      if ( i >= nbins) 
	{
	  h_hadron_contam->SetBinContent(i,n_fhadrons_n03);
	  h_hadron_contam->SetBinError(i,uc_nh);

	}
      */
    }
  for (int i = 1; i <= nbins;i++)
    {
      float n_esurv = h_survival_e_n03->GetBinContent(i);
      float n_hsurv = h_survival_pi_n03->GetBinContent(i);
      float n_shsurv = h_survival_sh_n03->GetBinContent(i);
      float n_non0 = h_ptno0->GetBinContent(i);
      float n_n03 = h_ptn03->GetBinContent(i);

      cout << n_esurv << " , " << n_hsurv << endl;

      // float n_hadrons = (n_n01 - (n_shsurv+ n_esurv)*n_non0)/(n_hsurv-n_esurv);
      float n_hadrons = (n_n03 - (n_esurv)*n_non0)/(n_hsurv-n_esurv);

      float n_fhadrons_n03 = n_hadrons*n_hsurv/n_n03;
      h_hadron_contam_n03->SetBinContent(i,n_fhadrons_n03);
    }

 


  for ( int i = 1 ; i <= nbins; i ++)
    {
      float n_n01 = h_ptn01->GetBinContent(i);
      float n_n03 = h_ptn03->GetBinContent(i);
      float n_fhadrons_n01 = h_hadron_contam_n01->GetBinContent(i);
      float n_fhadrons_n03 = h_hadron_contam_n03->GetBinContent(i);

      float n_e_n01 = n_n01 - (n_n01*n_fhadrons_n01);
      float n_e_n03 = n_n03 - (n_n03*n_fhadrons_n03);

      cout << n_e_n03/n_e_n01 << endl;

    }


  h_hadron_contam_n01n03->SetLineColor(kViolet);

  h_hadron_contam->SetLineColor(kGreen);
  TCanvas*c2 = new TCanvas("c2","",800,400);
  c2->Divide(2,1);
  c2->cd(1);
  // h_hadron_contam_n01->Draw();
  h_hadron_contam_n01n03->Draw();
  h_hadron_contam_n01->Draw("SAME");
  h_hadron_contam->Draw("SAME");
  c2->cd(2);
  h_hadron_contam_n03->Draw();
  h_hadron_contam_n03n01->SetLineColor(kViolet);
  h_hadron_contam_n03n01->Draw("SAME");




  h_survival_de_n01n03->SetTitle(";p_{T};n03/n01");
  h_survival_de_n01n03->SetAxisRange(0,1.2,"Y");
  TCanvas*c3 = new TCanvas("c3","",500,500);
  h_survival_de_n01n03->Draw();
  h_survival_e_n01n03->Draw("SAME");

  TLegend* l4 = new TLegend(0.65,0.8,0.9,0.9);
  l4->AddEntry(h_survival_de_n01n03,"data");
  l4->AddEntry(h_survival_e_n01n03,"sims");
  l4->Draw();




  c1->SaveAs("plots/n0survival.png");
  c2->SaveAs("plots/n0study_hadroncontam.png");
  c3->SaveAs("plots/n03n01study.png");

  TString outfileName = "dataFiles/algebraic_hadroncontam";

  if (nv)
    outfileName += "_noveto";

  outfileName += "_plus.root";

  TFile* outfile = new TFile(outfileName,"RECREATE");
  cout << " outputting algebraic hadron contamination fractions into : " << outfileName << endl;

  h_hadron_contam_n01->Write();
  h_hadron_contam->Write();
  h_hadron_contam_n03->Write();
}


