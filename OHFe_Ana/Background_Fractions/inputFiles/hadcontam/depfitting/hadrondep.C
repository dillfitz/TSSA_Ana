void hadrondep()
{

  gStyle->SetOptStat(0);

  TFile*infile = TFile::Open("/phenix/hhj/trinn/run15pro108/combinedanataxi/combined_good_ert_100mevbins.root"); 

  TH2F* h_hadron_dep_pt = (TH2F*)infile->Get("h_hadron_dep_pt");
  // h_hadron_dep_pt->SetAxisRange(-7,5,"X");

  TH1F* h_hadrondep[20];
  float bin[] = {16,18,19,21,22,24,25,27,28,30,31,35,36,40,41,45,46,50,51,60};


  float ptrange[] = {1.5,1.8,2.1,2.4,2.7,3,3.5,4,4.5,5,6};

  for (int i = 0; i < 10; i++)
    {
      stringstream aa;
      aa << "h_hadrondep_"<<i;
      string name = aa.str();
      h_hadrondep[i] = (TH1F*)h_hadron_dep_pt->ProjectionX(name.c_str(),bin[2*i],bin[2*i+1]);
      h_hadrondep[i]->Rebin(2);

      h_hadrondep[i]->SetAxisRange(-8,5,"X");
    }


  TF1* fits[14];
  for (int i = 0; i < 10;i++)
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

  for ( int i = 0; i < 10; i++)
    {
      h_hadrondep[i]->Fit(fits[i],"0","",-8,5);
    }





  TCanvas *c1 = new TCanvas("c1","",600,800);
  c1->Divide(3,4);
  for ( int i = 1 ; i < 11; i ++)
    {
      stringstream aa;
      aa <<ptrange[i-1] << " < p_{T} < " <<ptrange[i] << " GeV/c;dep;";
      string histotitle = aa.str();
      h_hadrondep[i-1]->SetTitle(histotitle.c_str());

      h_hadrondep[i-1]->SetTitleSize(0.05,"X");
      c1->cd(i);
      // gPad->SetLogy();
      h_hadrondep[i-1]->Draw();
      fits[i-1]->Draw("SAME");
    }


  TFile* outfile = new TFile("dataFiles/hadronfits.root","RECREATE");

  for ( int i = 0; i < 10; i ++ )
    {
      fits[i]->Write();
    }

  c1->SaveAs("hadrondep_ert_finebins.png");























}
