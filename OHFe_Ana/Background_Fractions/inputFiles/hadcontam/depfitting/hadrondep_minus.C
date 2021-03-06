#include "../../../../../Asymmetry_Ana/Constants.h"
void hadrondep_minus()
{

  //const bool inclusive=1;
  gStyle->SetOptStat(0);

  // TFile*infile = TFile::Open("/phenix/hhj/trinn/run15pro108/combinedanataxi/combined_good_ert_100mevbins.root"); 
  TFile*infile = TFile::Open("../../../../../AllRuns_725_ana651.root"); 


  TH2F* h_hadron_dep_pt = (TH2F*)infile->Get("h_hadron_dep_pt_minus");
  // h_hadron_dep_pt->SetAxisRange(-7,5,"X");



  // For binning out to 6 GeV in pT //
  //float ptrange[nbins+1] = {1.5,1.8,2.1,2.7,6.0};

  // For binning out to 5 GeV in pT //
 
  const int nbins = NUM_VALUE_BINS;
  //float ptrange[nbins+1] = {1.5,1.8,2.1,2.7,5.0};


  TH1F* h_hadrondep[nbins];

  if (nbins == 4)
  {
    float bin[] = {16,18,19,21,22,27,28,50};
  }
  else if (nbins == 6)
  {
    float bin[] = {11,13,14,15,16,18,19,21,22,27,28,50};
  }
  else
    std::cout << "Invalid binning!" << std::endl;

  for (int i = 0; i < nbins; i++)
    {
      stringstream aa;
      aa << "h_hadrondep_"<<i;
      string name = aa.str();
      h_hadrondep[i] = (TH1F*)h_hadron_dep_pt->ProjectionX(name.c_str(),bin[2*i],bin[2*i+1]);
      h_hadrondep[i]->Rebin(2);

      h_hadrondep[i]->SetAxisRange(-8,5,"X");
    }


  TF1* fits[14];
  for (int i = 0; i < nbins;i++)
    {
      stringstream bb;
      bb<< "fits"<<i;
      string name = bb.str();
      fits[i] = new TF1(name.c_str(),"[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x+[6]*x*x*x*x*x*x",-8,5);
       // fits[i] = new TF1(name.c_str(),"[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x",-8,5);
      fits[i]->SetParameters(1,1,1,1,1,1,1);
    }




  // TF1* f1 = new TF1("f1","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x",-8,5);
  // TF1* f2 = new TF1("f2","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x",-8,5);
  // TF1* f3 = new TF1("f3","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x",-8,5);
  // TF1* f4 = new TF1("f4","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x",-8,5);
  // TF1* f5 = new TF1("f5","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x",-8,5);
  // TF1* f6 = new TF1("f6","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x+[6]*x*x*x*x*x*x",-8,5);
  // TF1* f7 = new TF1("f7","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x+[6]*x*x*x*x*x*x",-8,5);
  // TF1* f8 = new TF1("f8","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x+[6]*x*x*x*x*x*x",-8,5);
  // TF1* f9 = new TF1("f9","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x+[6]*x*x*x*x*x*x",-8,5);
  // TF1* f9 = new TF1("f9","expo(0)",-5,5);




  // h_hadrondep_9->SetTitle("7 < p_{T} < 8 ; dep;");

  for ( int i = 0; i < nbins; i++)
    {
      h_hadrondep[i]->Fit(fits[i],"0","",-8,5);
    }





  TCanvas *c1 = new TCanvas("c1","",600,800);
  c1->Divide(3,4);
  for ( int i = 1 ; i <= nbins; i ++)
    {
      stringstream aa;
      aa <<VALUE_BINS[i-1] << " < p_{T} < " <<VALUE_BINS[i] << " GeV/c;dep;";
      string histotitle = aa.str();
      h_hadrondep[i-1]->SetTitle(histotitle.c_str());

      h_hadrondep[i-1]->SetTitleSize(0.05,"X");
      c1->cd(i);
      // gPad->SetLogy();
      h_hadrondep[i-1]->Draw();
      fits[i-1]->Draw("SAME");
    }

  TFile* outfile = new TFile("dataFiles/hadronfits_minus.root","RECREATE");

  for ( int i = 0; i < nbins; i ++ )
    {
      fits[i]->Write();
    }

  c1->SaveAs("hadrondep_minus.png");























}
