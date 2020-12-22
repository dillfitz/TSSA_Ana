#include "../../../../../Asymmetry_Ana/Constants.h"
void fitdep_minus()
{
  const bool nv = 0;

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);



  // For binning out to 6 GeV in pT //
  //float ptrange[nbins+1] = {1.5,1.8,2.1,2.7,6.0};



  // For binning out to 5 GeV in pT //
  const int nbins = NUM_VALUE_BINS;
  float ptrange[nbins+1] = {1.5,1.8,2.1,2.7,5.0};


 
  TString outfileName = "dataFiles/hadronnorm_depfit";

  if (nv)
    {
      TFile* infile1 = TFile::Open("dataFiles/depstudy_noveto_minus.root");
      outfileName += "_noveto";
    }
  else
    {
      TFile* infile1 = TFile::Open("dataFiles/depstudy_minus.root");
    }
  outfileName += "_minus.root";

  TH1F* h_dep[nbins];
  for (int i = 0; i < nbins; i ++)
    {
      stringstream aa;
      aa<< "h_dep"<<i;
      string histname = aa.str();
      h_dep[i] = (TH1F*)infile1->Get(histname.c_str());
    }

  TFile* infile2 = TFile::Open("dataFiles/hadronfits_minus.root");

  TF1* f_hadfits[nbins];
  for (int i = 0 ; i < nbins ; i++)
    {
      stringstream aa;
      aa<< "fits"<<i;
      string histname = aa.str();
      f_hadfits[i] = (TF1*)infile2->Get(histname.c_str());
    }

  TF1* f_ecfits[nbins];
  for (int i = 0; i < nbins;i++)
    {
      stringstream aa;
      aa << "("<<f_hadfits[i]->GetTitle()<<")*[7]+gaus(8)";
      string function = aa.str();
      // cout << function << endl;

      stringstream bb;
      bb << "f_ecfits"<<i;
      string name = bb.str();

      f_ecfits[i] = new TF1(name.c_str(),function.c_str(),-7,5);
      for (int q=0;q< 7;q++)
	{
	  f_ecfits[i]->FixParameter(q,f_hadfits[i]->GetParameter(q));
	}
      h_dep[i]->SetAxisRange(-7,5,"X");

      f_ecfits[i]->SetParameter(7,0.001);
      if ( i > 9) { h_dep[i]->Rebin(2);h_dep[i]->SetAxisRange(-7,5,"X");}

      f_ecfits[i]->SetParameter(8,h_dep[i]->GetMaximum());
      f_ecfits[i]->SetParameter(9,0);
      f_ecfits[i]->SetParameter(10,1);
    }

  for (int i = 0; i < nbins ;i++)
    {
      stringstream aa;
      aa <<VALUE_BINS[i] << " < p_{T} < " <<VALUE_BINS[i+1] << ";dep;";
      string histotitle = aa.str();
      h_dep[i]->SetTitle(histotitle.c_str());
      if ( i > 6)
	{
	  h_dep[i]->Rebin(2);
	  h_dep[i]->SetAxisRange(-7,5,"X");
	}
      h_dep[i]->Fit(f_ecfits[i],"0","",-7,5);
    }


  TCanvas *c1 = new TCanvas("c1","",600,800);
  c1->Divide(3,4);


  for ( int i = 1 ; i <= nbins; i ++)
    {
      // h_dep[i-1]->SetAxisRange(-7,5,"X");
      // if ( i > 10) { h_dep[i-1]->Rebin(2);h_dep[i-1]->SetAxisRange(-7,5,"X");}
  

      c1->cd(i);
      // gPad->SetLogy();
      h_dep[i-1]->Draw();
      f_ecfits[i-1]->Draw("SAME");
    }

  if (nv == 0)
    c1->SaveAs("electroncanddep_minus.png");

  TF1* f_hadrons[nbins];
  TF1* f_hadronsuc[nbins];
  TF1* f_electrons[nbins];

  // float ptbins[11] = {1.5,1.8,2.1,2.4,2.7,3,3.5,4,4.5,5,6};
  TH1F* h_hadfrac = new TH1F("h_hadfrac",";p_{T};",nbins,VALUE_BINS);

  for (int i = 0; i < nbins; i ++)
    {
      stringstream aa;
      aa << "("<<f_hadfits[i]->GetTitle()<<")*[7]";
      string function = aa.str();
      stringstream bb;
      bb << "f_hadrons"<<i;
      string name = bb.str();
      stringstream cc;
      cc << "f_hadronsuc"<<i;
      string name2 = cc.str();


      f_hadrons[i] = new TF1(name.c_str(),function.c_str(),-7,5);
      f_hadronsuc[i] = new TF1(name2.c_str(),function.c_str(),-7,5);
      for (int q = 0; q < 8;q++)
	{  
	  f_hadrons[i]->SetParameter(q,f_ecfits[i]->GetParameter(q));
	  f_hadronsuc[i]->SetParameter(q,f_ecfits[i]->GetParameter(q));
	}
      f_hadronsuc[i]->SetParameter(7,f_ecfits[i]->GetParameter(7)+f_ecfits[i]->GetParError(7));
      // float n_hadfrac = f_hadrons[i]->Integral(-2,2)/f_ecfits[i]->Integral(-2,2);
      // float n_hadfracuc = f_hadronsuc[i]->Integral(-2,2)/f_ecfits[i]->Integral(-2,2);
      float n_hadfrac = f_hadrons[i]->Integral(-2,2)/h_dep[i]->GetBinWidth(10)/h_dep[i]->Integral(h_dep[i]->FindBin(-2),h_dep[i]->FindBin(2));
      float n_hadfracuc = f_hadronsuc[i]->Integral(-2,2)/f_ecfits[i]->Integral(-2,2);
      if (h_dep[i]->Integral(h_dep[i]->FindBin(-2),h_dep[i]->FindBin(2)) ==0){n_hadfrac = 0; n_hadfracuc = 0;}

      // if ( ptbins[i] >4.8)
      // 	{
      // 	  n_hadfrac = f_hadrons[i]->Integral(-2,2)/f_ecfits[i]->Integral(-2,2);
      // 	  n_hadfracuc = f_hadronsuc[i]->Integral(-2,2)/f_ecfits[i]->Integral(-2,2);

      // 	}

      cout << n_hadfrac << endl;
      h_hadfrac->SetBinContent(i+1,n_hadfrac);
      h_hadfrac->SetBinError(i+1,fabs(n_hadfracuc-n_hadfrac));
    }

  TFile* outfile = new TFile(outfileName,"RECREATE");
  cout << " outputting depfit fractions into : " << outfileName << endl;

  TCanvas*c2 = new TCanvas("c2","",500,500);
  h_hadfrac->Draw();
  h_hadfrac->Write();

  for (int i = 0; i < nbins;i++)
    {
      f_hadrons[i]->Write();
      h_dep[i]->Write();
      f_ecfits[i]->Write();
    }

}
