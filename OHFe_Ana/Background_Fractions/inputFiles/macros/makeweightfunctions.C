void makeweightfunctions()
{
  gStyle->SetOptStat(0);
  double pi = 3.14159265359;

  
  Float_t jbins[26] = {0,0.25,0.5,0.75,1,1.25,1.5,1.75,2,2.25,2.5,2.75,3,3.25,3.5,3.75,4,4.25,4.5,4.75,5,6,7,8,9,20};

  // Float_t jbins2[22] = {0,0.25,0.5,0.75,1,1.25,1.5,1.75,2,2.25,2.5,2.75,3,3.25,3.5,3.75,4,5,6,7,8,9};

  Float_t kbins[17] = {0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2};


  TH1F* jpsi = new TH1F("jpsi",";pT;2Pi*dsig/dydpt",25,jbins);
  // TH1F* jpsi2 = new TH1F("jpsi2","",21,jbins);
  TH1F* kp = new TH1F("kp","",16,kbins);
  TH1F* km = new TH1F("km","",16,kbins);

  //extracted values from ppg104

 
  float jval[24] = { 4.9,3.9,3.9,3.5,3.19,2.21,1.69,1.42,0.95,0.66,0.56,0.37,0.29,0.199,0.136,0.106,0.0689,0.044,0.047,0.031,0.0135,0.0041,0.0016,0.00037};
  float jerr1[24] = {0.5,0.3,0.2,0.2,0.17,0.14,0.12,0.1,0.08,0.09,0.06,0.05,0.04,0.036,0.028,0.025,0.023,0.017,0.015,0.012,0.0031,0.0013,0.0007,0.00037};

  float jerr2[24] = {0.6,0.5,0.5,0.4,0.38,0.27,0.2,0.17,0.12,0.08,0.07,0.05,0.04,0.024,0.017,0.013,0.008,0.005,0.006,0.004,0.0018,0.0006,0.0002,0.0006};

  for (int i = 1; i < 25; i++)
    {
      // jpsi->SetBinContent(i,jval[i-1]*((jbins[i]+jbins[i-1])/2)*1/(2*pi));
      // jpsi->SetBinContent(i,jval[i-1]);

      // jpsi->SetBinError(i,TMath::Sqrt(jerr1[i-1]*jerr1[i-1]+jerr2[i-1]*jerr2[i-1])*((jbins[i]+jbins[i-1])/2)*1/(2*pi));

    }


  jpsi->Draw();

  // from ppg 101
  float kpval[16] = {1.96,1.35,0.871,0.586,0.395,0.260,0.172,0.125,0.0852,0.0608,0.0459,0.0329,0.0239,0.0186,0.0149,0.0113};
  float kmval[16] = {1.89,1.37,0.828,0.56,0.387,0.254,0.183,0.116,0.0897,0.0623,0.0427,0.0321,0.0223,0.0181,0.0136,0.0103};

  float kperr1[16] = {0.05,0.03,0.019,0.013,0.0087,0.0058,0.0039,0.0021,0.0015,0.0012,0.0009,0.00066,0.00053,0.00044,0.00035};
  float kperr2[16] = {0.14,0.094,0.061,0.041,0.028,0.018,0.012,0.0089,0.006,0.0043,0.0032,0.0023,0.0017,0.0013,0.001,0.00079};
  
  float kmerr1[16] = {0.07,0.043,0.023,0.016,0.011,0.0073,0.0055,0.0028,0.002,0.0014,0.0011,0.00074,0.00062,0.00047,0.00037};
  float kmerr2[16] = {0.13,0.096,0.058,0.039,0.027,0.018,0.013,0.0081,0.0063,0.0044,0.003,0.0022,0.0016,0.0013,0.00095,0.00072};


  for (int i = 1; i < 17;i++)
    {
      kp->SetBinContent(i,kpval[i-1]*((kbins[i]+kbins[i-1])/2)*1/(2*pi));
      kp->SetBinError(i,TMath::Sqrt(kperr1[i-1]*kperr1[i-1]+kperr2[i-1]*kperr2[i-1])*((kbins[i]+kbins[i-1])/2)*1/(2*pi));

      km->SetBinContent(i,kmval[i-1]*((kbins[i]+kbins[i-1])/2)*1/(2*pi));
      km->SetBinError(i,TMath::Sqrt(kmerr1[i-1]*kmerr1[i-1]+kmerr2[i-1]*kmerr2[i-1])*((kbins[i]+kbins[i-1])/2)*1/(2*pi));




    }
  gPad->SetLogy();
  jpsi->SetLineColor(1);
  jpsi->Draw();
  kp->Draw("SAME");
  km->SetLineColor(2);
  km->Draw("SAME");




  // TF1* f_jpsi_spectrum = new TF1("f_jpsi_spectrum", "2*TMath::Pi()*x*[0]/TMath::Power((TMath::Exp(-[1]*x-[2]*x*x)+(x/[3])),[4])", 0, 25.0);
  // f_jpsi_spectrum->SetParameters(0.242345, -0.0827585, 0.00918447, 4.13943, 13.6974);

  TF1* f_jpsi_spectrum = new TF1("f_jpsi_spectrum", "0.0527311*(2*TMath::Pi()*x*([0]/TMath::Power((TMath::Exp(-[1]*TMath::Sqrt(x**2+3.09691**2-0.134976**2)-[2]*TMath::Sqrt(x**2+3.09691**2-0.134976**2)*TMath::Sqrt(x**2+3.09691**2-0.134976**2))+(TMath::Sqrt(x**2+3.09691**2-0.134976**2)/[3])),[4])))", 0.0, 20.0);

  f_jpsi_spectrum->SetParameters(254.066, 0.470186, 0.0380066, 0.737713, 8.28442);



  // TF1* f_kp_spectrum = new TF1("f_kp_spectrum", "2*TMath::Pi()*x*[0]/TMath::Power((TMath::Exp(-[1]*x)+(x/[2])),[3])", 0, 25.0);
  // f_kp_spectrum->SetParameters(0.242345, 0.00918447, 4.13943, 13.6974);

  // TF1* f_km_spectrum = new TF1("f_km_spectrum", "2*TMath::Pi()*x*[0]/TMath::Power((TMath::Exp(-[1]*x)+(x/[2])),[3])", 0, 25.0);
  // f_km_spectrum->SetParameters(0.242345, 0.00918447, 4.13943, 13.6974);


  // TF1* f_kp_spectrum = new TF1("f_kp_spectrum_0", "0.32*(2*TMath::Pi()*TMath::Sqrt(x**2+0.493677**2-0.134976**2)*([0]/TMath::Power((TMath::Exp(-[1]*TMath::Sqrt(x**2+0.493677**2-0.134976**2)-[2]*TMath::Sqrt(x**2+0.493677**2-0.134976**2)*TMath::Sqrt(x**2+0.493677**2-0.134976**2))+(TMath::Sqrt(x**2+0.493677**2-0.134976**2)/[3])),[4])))", 0.0, 18.0);


 TF1* f_kp_spectrum = new TF1("f_kp_spectrum", "0.361089*(2*TMath::Pi()*x*([0]/TMath::Power((TMath::Exp(-[1]*TMath::Sqrt(x**2+0.493677**2-0.134976**2)-[2]*TMath::Sqrt(x**2+0.493677**2-0.134976**2)*TMath::Sqrt(x**2+0.493677**2-0.134976**2))+(TMath::Sqrt(x**2+0.493677**2-0.134976**2)/[3])),[4])))", 0.0, 18.0);
  f_kp_spectrum->SetParameters(254.066, 0.470186, 0.0380066, 0.737713, 8.28442);


  // TF1* f_km_spectrum = new TF1("f_km_spectrum_0", "0.32*(2*TMath::Pi()*TMath::Sqrt(x**2+0.493677**2-0.134976**2)*([0]/TMath::Power((TMath::Exp(-[1]*TMath::Sqrt(x**2+0.493677**2-0.134976**2)-[2]*TMath::Sqrt(x**2+0.493677**2-0.134976**2)*TMath::Sqrt(x**2+0.493677**2-0.134976**2))+(TMath::Sqrt(x**2+0.493677**2-0.134976**2)/[3])),[4])))", 0.0, 18.0);

 TF1* f_km_spectrum = new TF1("f_km_spectrum", "0.361089*(2*TMath::Pi()*x*([0]/TMath::Power((TMath::Exp(-[1]*TMath::Sqrt(x**2+0.493677**2-0.134976**2)-[2]*TMath::Sqrt(x**2+0.493677**2-0.134976**2)*TMath::Sqrt(x**2+0.493677**2-0.134976**2))+(TMath::Sqrt(x**2+0.493677**2-0.134976**2)/[3])),[4])))", 0.0, 20.0);
  f_km_spectrum->SetParameters(254.066, 0.470186, 0.0380066, 0.737713, 8.28442);

 






  TF1* f_photon_spectrum = new TF1("f_photon_spectrum", "2*TMath::Pi()*x*[0]/TMath::Power((TMath::Exp(-[1]*x-[2]*x*x)+(x/[3])),[4])", 0.0, 25.0);
f_photon_spectrum[0]->SetParameters(0.242345, -0.0827585, 0.00918447, 4.13943, 13.6974);

 f_photon_spectrum->SetLineColor(1);

 TF1* f_pizero_spectrum = new TF1("f_pizero_spectrum", "1.07134*2*TMath::Pi()*x*([0]/TMath::Power((TMath::Exp(-[1]*x-[2]*x*x)+(x/[3])),[4]))", 0.0, 25.0);
f_pizero_spectrum[0]->SetParameters(254.066, 0.470186, 0.0380066, 0.737713, 8.28442);
 f_pizero_spectrum->SetLineColor(4);

 // TF1* f_eta_spectrum = new TF1("f_eta_spectrum", "TMath::Power(TMath::Sqrt(1+(0.135/x)*(0.135/x)),-1.0)*0.5*TMath::Sqrt(1+(0.135/x)*(0.135/x))*(2*TMath::Pi()*x*([0]/TMath::Power((TMath::Exp(-[1]*x-[2]*x*x)+(x/[3])),[4])))", 0.0, 20.0);

  TF1* f_eta_spectrum = new TF1("f_eta_spectrum", "0.557063*(2*TMath::Pi()*x*([0]/TMath::Power((TMath::Exp(-[1]*TMath::Sqrt(x**2+0.547862**2-0.134976**2)-[2]*TMath::Sqrt(x**2+0.547862**2-0.134976**2)*TMath::Sqrt(x**2+0.547862**2-0.134976**2))+(TMath::Sqrt(x**2+0.547862**2-0.134976**2)/[3])),[4])))", 0.0, 20.0);
f_eta_spectrum[0]->SetParameters(254.066, 0.470186, 0.0380066, 0.737713, 8.28442);




 f_eta_spectrum->SetLineColor(kGreen+2);


 // jpsi->Fit(f_jpsi_spectrum);
 // f_jpsi_spectrum->Draw("SAME");

 // kp->Fit(f_kp_spectrum);
 // km->Fit(f_km_spectrum);
 jpsi->SetAxisRange(0.000000001,100,"Y");
 // jpsi->SetAxisRange(0,40,"X");
 jpsi->Draw();
 // kp->Draw("SAME");
 // km->Draw("SAME");
 f_kp_spectrum->SetLineColor(5);
 f_km_spectrum->SetLineColor(6);
 f_jpsi_spectrum->SetLineColor(7);
 f_jpsi_spectrum->Draw("SAME");

 f_kp_spectrum->Draw("SAME");
 f_km_spectrum->Draw("SAME");
 f_photon_spectrum->Draw("SAME");
 f_pizero_spectrum->Draw("SAME");
 f_eta_spectrum->Draw("SAME");


 TLegend* l1 = new TLegend(0.7,0.6,0.9,0.9);
 l1->AddEntry(f_pizero_spectrum,"pi0");
 l1->AddEntry(f_eta_spectrum,"eta");
 l1->AddEntry(f_jpsi_spectrum,"jpsi");
 l1->AddEntry(f_photon_spectrum,"photon");
 l1->AddEntry(f_km_spectrum,"km, kp");
 l1->Draw();

 TFile*outfile = new TFile("../weightfunctions.root","RECREATE");
 f_pizero_spectrum->Write();
 f_eta_spectrum->Write();
 f_jpsi_spectrum->Write();
 f_photon_spectrum->Write();
 f_kp_spectrum->Write();
 f_km_spectrum->Write();



}
