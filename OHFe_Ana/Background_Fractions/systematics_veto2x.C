void systematics_veto2x()
{

  gStyle->SetOptStat(0);

  const int nbins = 4;

  double bins[nbins+1]  = {1.5, 1.8, 2.1, 2.7, 5.0};

  //************************************
  // read in files and create histograms
  //************************************
  // ke3
  //****
  // kp
  //****

  // for ( int fds = 0; fds < 10;fds++)
  // {
  //   cout << fds << endl;

  TFile*infile1 = TFile::Open("inputFiles/MCData/kp_no3_moreveto_full_set_sync.root");
  TFile*infile2 = TFile::Open("inputFiles/MCData/kp_more_full_set_truth.root");
  TH2F* h_kp_ptdca = (TH2F*)infile1->Get("h_epthpt3");
  // TH1F* h_kp_pt = (TH1F*)h_kp_ptdca->ProjectionX("h_kp_pt");
  TH1F* h_kp_hadronpt2 = (TH1F*)infile2->Get("hadronpt");
  h_kp_hadronpt2->Rebin(2);


  TFile* file_kpshapemod = TFile::Open("inputFiles/reweighting/kpcorrection.root");
  // TF1* f_kpshapemod = (TF1*)file_kpshapemod->Get("f_ratio_central");
  TF1* f_kp_cc = (TF1*)file_kpshapemod->Get("f_current_central")->Clone("f_kp_cc");;
  TF1* f_kp_fit = (TF1*)file_kpshapemod->Get("f_published_spectrum_fit")->Clone("f_kp_fit");
  TF1* f_kp_rc = new TF1("f_kp_rc","f_kp_fit/f_kp_cc",0.1,20);

  TGraphErrors* f_kp_m1sig = (TGraphErrors*)file_kpshapemod->Get("m_1sig");
  // TF1* f_kp = new TF1("f_kp","gaus(0)",-3,3);
  // f_kp->SetParameters(1,0,1);
  // double n_kpucfrac = f_kp->GetRandom();






  //***********************
  // use kp as proxy for km
  //***********************
  TH2F* h_km_ptdca = (TH2F*)infile1->Get("h_epthpt3");
  // TH1F* h_km_pt = (TH1F*)h_km_ptdca->ProjectionX("h_km_pt");
  TH1F* h_km_hadronpt2 = (TH1F*)infile2->Get("hadronpt");
  h_km_hadronpt2->Rebin(2);

  TFile* file_kmshapemod = TFile::Open("inputFiles/reweighting/kpcorrection.root");
  TF1* f_km_cc = (TF1*)file_kmshapemod->Get("f_current_central")->Clone("f_km_cc");;
  TF1* f_km_fit = (TF1*)file_kmshapemod->Get("f_published_spectrum_fit")->Clone("f_km_fit");
  TF1* f_km_rc = new TF1("f_km_rc","f_km_fit/f_km_cc",0.1,20);



  TGraphErrors* f_km_m1sig = (TGraphErrors*)file_kmshapemod->Get("m_1sig");
 




  //*******
  // kshort
  //*******

  TFile* infile5 = TFile::Open("inputFiles/MCData/kshort_no3_moreveto_full_set_sync.root");
  TFile* infile6 = TFile::Open("inputFiles/MCData/kshort_more_full_set_truth.root");
  TH2F* h_ks_ptdca = (TH2F*)infile5->Get("h_epthpt3");
  // TH1F* h_ks_pt = (TH1F*)h_ks_ptdca->ProjectionX("h_ks_pt");
  TH1F* h_ks_hadronpt2 = (TH1F*)infile6->Get("hadronpt");
  h_ks_hadronpt2->Rebin(2);


  TFile* file_ksshapemod = TFile::Open("inputFiles/reweighting/kshortcorrection.root");
  // TF1* f_kpshapemod = (TF1*)file_kpshapemod->Get("f_ratio_central");
  TF1* f_ks_cc = (TF1*)file_ksshapemod->Get("f_current_central")->Clone("f_ks_cc");;
  TF1* f_ks_fit = (TF1*)file_ksshapemod->Get("f_published_spectrum_fit")->Clone("f_ks_fit");
  TF1* f_ks_rc = new TF1("f_ks_rc","f_ks_fit/f_ks_cc",0.1,20);



  TGraphErrors* f_ks_m1sig = (TGraphErrors*)file_ksshapemod->Get("m_1sig");
 



  //****
  // piz
  //****




  TFile*infile7 = TFile::Open("inputFiles/MCData/piz_no3_moreveto_full_set_sync.root");
  TFile*infile8 = TFile::Open("inputFiles/MCData/piz_more_full_set_truth.root");

  TH2F* h_piz_ptdca = (TH2F*)infile7->Get("h_epthpt3");
  TH2F* h_piz_pt_con_2d = (TH2F*)infile7->Get("h_wcepthpt3")->Clone("h_piz_pt_con_2d");

 TH1F* h_piz_tmp = (TH1F*)h_piz_ptdca->ProjectionY("h_piz_tmp");
 for (int i = 1; i < 41;i++)
    {
     float ept =  h_piz_tmp->GetBinCenter(i);
     int minbin = h_piz_tmp->FindBin(ept-0.7);
     for (int j = 1; j < minbin+1;j++)
       {
	 h_piz_ptdca->SetBinContent(i,j,0);
	 h_piz_pt_con_2d->SetBinContent(i,j,0);
       }
    }




  // TH1F* h_piz_pt = (TH1F*)h_piz_ptdca->ProjectionX("h_piz_pt");
  TH1F* h_piz_hadronpt = (TH1F*)infile8->Get("hadronpt");
  h_piz_hadronpt->Rebin(2);


  TFile* file_pizshapemod = TFile::Open("inputFiles/reweighting/pi0.root");
  TF1* f_piz_fit = (TF1*)file_pizshapemod->Get("f_published_spectrum_fit")->Clone("f_piz_fit");

  TGraphErrors* f_piz_m1sig = (TGraphErrors*)file_pizshapemod->Get("m_1sig");
  








  //****
  // eta
  //****
  TFile*infile9 = TFile::Open("inputFiles/MCData/eta_no3_moreveto_full_set_sync.root");
  TFile*infile10 = TFile::Open("inputFiles/MCData/eta_more_full_set_truth.root");
  TH2F* h_eta_ptdca = (TH2F*)infile9->Get("h_epthpt3");
  // TH1F* h_eta_pt = (TH1F*)h_eta_ptdca->ProjectionX("h_eta_pt");
  TH2F* h_eta_pt_con_2d = (TH2F*)infile9->Get("h_wcepthpt3")->Clone("h_eta_pt_con_2d");
  // TH1F* h_eta_pt_con = (TH1F*)h_eta_pt_con_2d->ProjectionX("h_eta_pt_con");
  TH1F* h_eta_hadronpt2 = (TH1F*)infile10->Get("hadronpt");
  h_eta_hadronpt2->Rebin(2);
  // TH1F* h_eta_pt_conveto = (TH1F*)h_eta_pt->Clone("h_eta_pt_conveto");



  TFile* file_etashapemod = TFile::Open("inputFiles/reweighting/etacorrection.root");
  // TF1* f_etashapemod = (TF1*)file_etashapemod->Get("f_ratio_central");
  TF1* f_eta_cc = (TF1*)file_etashapemod->Get("f_current_central")->Clone("f_eta_cc");;
  TF1* f_eta_fit = (TF1*)file_etashapemod->Get("f_published_spectrum_fit")->Clone("f_eta_fit");
  TF1* f_eta_rc = new TF1("f_eta_rc","f_eta_fit/f_eta_cc",0.1,20);


  TGraphErrors* f_eta_m1sig = (TGraphErrors*)file_etashapemod->Get("m_1sig");
 

  //*******
  // photon
  //*******

  TFile*infile11 = TFile::Open("inputFiles/MCData/gamma_no3_moreveto_full_set_sync.root");
  TFile*infile12 = TFile::Open("inputFiles/MCData/gamma_more_full_set_truth.root");
  TH2F* h_photon_ptdca = (TH2F*)infile11->Get("h_epthpt3");
  // TH1F* h_photon_pt = (TH1F*)h_photon_ptdca->ProjectionX("h_photon_pt");
  TH2F* h_photon_pt_con_2d = (TH2F*)infile11->Get("h_wcepthpt3")->Clone("h_photon_pt_con_2d");
  // TH1F* h_photon_pt_con = (TH1F*)h_photon_pt_con_2d->ProjectionX("h_photon_pt_con");

  TH1F* h_photon_hadronpt = (TH1F*)infile12->Get("hadronpt");
  h_photon_hadronpt->Rebin(2);

// TH1F* h_photon_pt_conveto = (TH1F*) h_photon_pt->Clone("h_photon_pt_conveto");









  TH2F* h_photonepthpt = (TH2F*)h_photon_ptdca->Clone("h_photonepthpt");
  h_photonepthpt->Reset();

  TH2F* h_photonepthpt_con = (TH2F*)h_photon_pt_con_2d->Clone("h_photonepthpt_con");
  h_photonepthpt_con->Reset();




  //*****
  // jpsi
  //*****

  TFile*infile15 = TFile::Open("inputFiles/MCData/jpsi_no3_moreveto_full_set_sync.root");
  TFile*infile16 = TFile::Open("inputFiles/MCData/jpsi_more_full_set_truth.root");
  TH2F* h_jpsi_ptdca = (TH2F*)infile15->Get("h_epthpt3");
  TH1F* h_jpsi_pt = (TH1F*)h_jpsi_ptdca->ProjectionX("h_jpsi_pt");


  TH1F* h_jpsi_hadronpt2 = (TH1F*)infile16->Get("hadronpt");
  h_jpsi_hadronpt2->Rebin(2);




  TFile* file_jpsishapemod = TFile::Open("inputFiles/reweighting/jpsicorrection.root");
  // TF1* f_jpsishapemod = (TF1*)file_jpsishapemod->Get("f_ratio_central");
  TF1* f_jpsi_cc = (TF1*)file_jpsishapemod->Get("f_jpsi_spectrum")->Clone("f_jpsi_cc");;
  TF1* f_jpsi_fit = (TF1*)file_jpsishapemod->Get("fref")->Clone("f_jpsi_fit");
  TF1* f_jpsi_rc = new TF1("f_jpsi_rc","f_jpsi_fit/f_jpsi_cc",0.1,20);


  TGraphErrors* f_jpsi_m1sig = (TGraphErrors*)file_jpsishapemod->Get("m_1sig");
 







  TFile* wfile = TFile::Open("inputFiles/weightfunctions.root");
  // TF1*f_kp_spectrum = (TF1*)wfile->Get("f_kp_spectrum");
  // TF1*f_km_spectrum = (TF1*)wfile->Get("f_km_spectrum");
  // TF1*f_jpsi_spectrum = (TF1*)wfile->Get("f_jpsi_spectrum");
  TF1*f_pizero_spectrum = (TF1*)wfile->Get("f_pizero_spectrum");
  TF1*f_photon_spectrum = (TF1*)wfile->Get("f_photon_spectrum");
  // TF1*f_eta_spectrum = (TF1*)wfile->Get("f_eta_spectrum");





  //*****************************
  // read in hadron contamination
  //*****************************

  TFile* hadroncontam = TFile::Open("inputFiles/hadcontam_noveto.root");
  TH1F* h_hadroncontam = (TH1F*) hadroncontam->Get("h_hadcontam")->Clone("h_hadroncontam");

  TFile* hadroncontamwv = TFile::Open("inputFiles/hadcontam.root");
  TH1F* h_hadroncontamwv = (TH1F*) hadroncontamwv->Get("h_hadcontam")->Clone("h_hadroncontamwv");

  h_hadroncontam->SetAxisRange(0.0000001,1,"Y");
  h_hadroncontamwv->SetAxisRange(0.0000001,1,"Y");


  TFile* infile_fnpdata = TFile::Open("inputFiles/forfnp.root");
  TH1F* h_ept_nv = (TH1F*)infile_fnpdata->Get("h_ept_nv");
  TH1F* h_ept_conveto = (TH1F*)infile_fnpdata->Get("h_ept_conveto");


  TFile* datanp = TFile::Open("inputFiles/survivalrates_data.root");

  TH1F* h_nonphotonic_survival = (TH1F*)datanp->Get("h_survival3")->Clone("h_nonphotonic_survival");





  TH1F* h_myfnp = new TH1F("h_myfnp",";pT;F_{np}",nbins,bins);
  TH1F* h_myfnp_conveto = new TH1F("h_myfnp_conveto",";pT;F_{np}",nbins,bins);

  TH1F* h_oldfnp_conveto = new TH1F("h_oldfnp_conveto",";pT;F_{np}",nbins,bins);


  //------------------------------------
  //--Define histograms for iterations--
  //------------------------------------

  TH1F* h_sysfnp[nbins];
  TH1F* h_syspizn[nbins];
  TH1F* h_sysetan[nbins];
  TH1F* h_syske3n[nbins];
  TH1F* h_sysjpsin[nbins];
  TH1F* h_sysphotonn[nbins];

  float fnprange[] = {0.60,0.68,0.66,0.74,0.70,0.75,0.75,0.82,0.75,0.84,0.80,0.86,0.84,0.88,0.85,0.9,0.9,0.92,0.88,0.92};
  float piznormrange[] = {0.25,0.32,0.2,0.32,0.16,0.3,0.13,0.2,0.12,0.18,.08,0.17,0.05,0.15,0.05,0.13,0.05,0.1,0.05,0.13,0.05,0.13};
  float etanormrange[] = {0.04,0.1,0.03,0.1,0.025,0.1,0.025,0.08,0.02,0.08,0.02,0.08,0.02,0.04,0.01,0.04,0.01,0.04,0.01,0.04};
  float jpsinormrange[] = {0,0.07,0.0,0.12,0.0,0.16,0.03,0.2,0.03,0.2,0.05,0.3,0.1,0.3,0.08,0.3,0.08,0.3,0.1,0.4,0.1,0.4};
  float ke3normrange[] = {0,0.008,0,0.006,0,0.006,0,0.006,0,0.006,0,0.006,0,0.006,0,0.006,0,0.006,0,0.008,0,0.008};
  float photonnormrange[] = {0.,0.01,0.0,0.012,0.0,0.02,0.0,0.02,0.0,0.02,0.0,0.02,0.0,0.02,0.0,0.02,0.0,0.02,0.0,0.02};


  for (int i = 0; i < nbins; i++)
    {
      stringstream aa; 
      aa << "h_sysfnp"<<i;
      string aname = aa.str();
      h_sysfnp[i] = new TH1F(aname.c_str(),";F_{np};",200,fnprange[2*i],fnprange[2*i+1]);
      stringstream bb; 
      bb << "h_syspizn"<<i;
      string bname = bb.str();
      h_syspizn[i] = new TH1F(bname.c_str(),";piz_{n};",200,piznormrange[2*i],piznormrange[2*i+1]);
      stringstream cc; 
      cc << "h_sysetan"<<i;
      string cname = cc.str();
      h_sysetan[i] = new TH1F(cname.c_str(),";eta_{n};",200,etanormrange[2*i],etanormrange[2*i+1]);
      stringstream dd; 
      dd << "h_sysjpsin"<<i;
      string dname = dd.str();
      h_sysjpsin[i] = new TH1F(dname.c_str(),";jpsi_{n};",200,jpsinormrange[2*i],jpsinormrange[2*i+1]);
      stringstream tt; 
      tt << "h_syske3n"<<i;
      string ename = tt.str();
      h_syske3n[i] = new TH1F(ename.c_str(),";ke3_{n};",200,ke3normrange[2*i],ke3normrange[2*i+1]);
      stringstream ff; 
      ff << "h_sysphotonn"<<i;
      string fname = ff.str();
      h_sysphotonn[i] = new TH1F(fname.c_str(),";photon_{n};",200,photonnormrange[2*i],photonnormrange[2*i+1]);

    }


  //-----------------------------------------------
  //apply weighting stuff ugily
  //-----------------------------------------------

  TF1* f_kp = new TF1("f_kp","gaus(0)",-3,3);
  f_kp->SetParameters(1,0,1);
  TF1* f_km = new TF1("f_km","gaus(0)",-3,3);
  f_km->SetParameters(1,0,1);
  TF1* f_ks = new TF1("f_ks","gaus(0)",-3,3);
  f_ks->SetParameters(1,0,1);
  TF1* f_eta = new TF1("f_eta","gaus(0)",-3,3);
  f_eta->SetParameters(1,0,1);
  TF1* f_piz = new TF1("f_piz","gaus(0)",-3,3);
  f_piz->SetParameters(1,0,1);
  TF1* f_jpsi = new TF1("f_jpsi","gaus(0)",-3,3);
  f_jpsi->SetParameters(1,0,1);
  TF1* f_photon = new TF1("f_photon","gaus(0)",-3,3);
  f_photon->SetParameters(1,0,1);


  for ( int fds = 0; fds <1500;fds++)
  {
    cout << fds << endl;

  double n_kpucfrac = f_kp->GetRandom();

  TH1F* h_kp_hadronpt = (TH1F*)h_kp_hadronpt2->Clone("h_kp_hadronpt");
  TH1F* h_km_hadronpt = (TH1F*)h_km_hadronpt2->Clone("h_km_hadronpt");
  TH1F* h_ks_hadronpt = (TH1F*)h_ks_hadronpt2->Clone("h_ks_hadronpt");
  TH1F* h_eta_hadronpt = (TH1F*)h_eta_hadronpt2->Clone("h_eta_hadronpt");
  TH1F* h_jpsi_hadronpt = (TH1F*)h_jpsi_hadronpt2->Clone("h_jpsi_hadronpt");





  TH2F* h_kpepthpt = (TH2F*)h_kp_ptdca->Clone("h_kpepthpt");
  h_kpepthpt->Reset();
  TH1F* h_hkppt = (TH1F*)h_kp_ptdca->ProjectionY("h_hkppt");
  for (int i = 1; i < 201;i++)
    {
      Double_t x,y;
      f_kp_m1sig->GetPoint(i-1,x,y);
      double ucweight = 1;
      if (n_kpucfrac > 0)
	{
	  ucweight = f_kp_rc(h_hkppt->GetBinCenter(i))*(1+(1-y))**n_kpucfrac;
	}
      if (n_kpucfrac < 0)
	{
	  ucweight = f_kp_rc(h_hkppt->GetBinCenter(i))*(y**(fabs(n_kpucfrac)));  
	}
      if (n_kpucfrac == 0 )
	{
	  ucweight = f_kp_rc(h_hkppt->GetBinCenter(i));
	}

      double weight = f_kp_rc(h_hkppt->GetBinCenter(i));

      h_kp_hadronpt->SetBinContent(i,h_kp_hadronpt->GetBinContent(i)*weight);
      h_kp_hadronpt->SetBinError(i,h_kp_hadronpt->GetBinError(i)*weight);
      for (int k = 1; k < 101; k++)
	{
	  double binvalue = h_kp_ptdca->GetBinContent(k,i)*ucweight;
	  double binerror = h_kp_ptdca->GetBinError(k,i)*ucweight;

	  h_kpepthpt->SetBinContent(k,i,binvalue);
	  h_kpepthpt->SetBinError(k,i,binerror);
	} 
    }
  TH1F* h_kp_pt = (TH1F*)h_kpepthpt->ProjectionX("h_kp_pt");




  double n_kmucfrac = f_km->GetRandom();



  TH2F* h_kmepthpt = (TH2F*)h_km_ptdca->Clone("h_kmepthpt");
  h_kmepthpt->Reset();
  TH1F* h_hkmpt = (TH1F*)h_km_ptdca->ProjectionY("h_hkmpt");
  for (int i = 1; i < 201;i++)
    {
      Double_t x,y;
      f_km_m1sig->GetPoint(i-1,x,y);
      double ucweight = 1;
 

     if (n_kmucfrac > 0)
	{
	  ucweight = f_km_rc(h_hkmpt->GetBinCenter(i))*(1+(1-y))**n_kmucfrac;
	}
      if (n_kmucfrac < 0)
	{
	  ucweight = f_km_rc(h_hkmpt->GetBinCenter(i))*(y**(fabs(n_kmucfrac)));  
	}
      if (n_kmucfrac == 0 )
	{
	  ucweight = f_km_rc(h_hkmpt->GetBinCenter(i));
	}





      double weight = f_km_rc(h_hkmpt->GetBinCenter(i));

      // double weight = f_km_rc(h_hkmpt->GetBinCenter(i));
      h_km_hadronpt->SetBinContent(i,h_km_hadronpt->GetBinContent(i)*weight);
      h_km_hadronpt->SetBinError(i,h_km_hadronpt->GetBinError(i)*weight);
      for (int k = 1; k < 101; k++)
	{
	  double binvalue = h_km_ptdca->GetBinContent(k,i)*ucweight;
	  double binerror = h_km_ptdca->GetBinError(k,i)*ucweight;

	  h_kmepthpt->SetBinContent(k,i,binvalue);
	  h_kmepthpt->SetBinError(k,i,binerror);
	} 
    }
  TH1F* h_km_pt = (TH1F*)h_kmepthpt->ProjectionX("h_km_pt");




  double n_ksucfrac = f_ks->GetRandom();



  TH2F* h_ksepthpt = (TH2F*)h_ks_ptdca->Clone("h_ksepthpt");
  h_ksepthpt->Reset();
  TH1F* h_hkspt = (TH1F*)h_ks_ptdca->ProjectionY("h_hkspt");
  for (int i = 1; i < 201;i++)
    {

      Double_t x,y;
      f_ks_m1sig->GetPoint(i-1,x,y);
      double ucweight = 1;


     if (n_ksucfrac > 0)
	{
	  ucweight = f_ks_rc(h_hkspt->GetBinCenter(i))*(1+(1-y))**n_ksucfrac;
	}
      if (n_ksucfrac < 0)
	{
	  ucweight = f_ks_rc(h_hkspt->GetBinCenter(i))*(y**(fabs(n_ksucfrac)));  
	}
      if (n_ksucfrac == 0 )
	{
	  ucweight = f_ks_rc(h_hkspt->GetBinCenter(i));
	}




      double weight = f_ks_rc(h_hkspt->GetBinCenter(i));

      h_ks_hadronpt->SetBinContent(i,h_ks_hadronpt->GetBinContent(i)*weight);
      h_ks_hadronpt->SetBinError(i,h_ks_hadronpt->GetBinError(i)*weight);
      for (int k = 1; k < 101; k++)
	{
	  double binvalue = h_ks_ptdca->GetBinContent(k,i)*ucweight;
	  double binerror = h_ks_ptdca->GetBinError(k,i)*ucweight;

	  h_ksepthpt->SetBinContent(k,i,binvalue);
	  h_ksepthpt->SetBinError(k,i,binerror);
	} 
    }
  TH1F* h_ks_pt = (TH1F*)h_ksepthpt->ProjectionX("h_ks_pt");




  double n_pizucfrac = f_piz->GetRandom();



  TH2F* h_pizepthpt = (TH2F*)h_piz_ptdca->Clone("h_pizepthpt");
  h_pizepthpt->Reset();

  TH2F* h_pizepthpt_con = (TH2F*)h_piz_pt_con_2d->Clone("h_pizepthpt_con");
  h_pizepthpt_con->Reset();



  TH1F* h_hpizpt = (TH1F*)h_piz_ptdca->ProjectionY("h_hpizpt");
  for (int i = 1; i < 201;i++)
    {

      Double_t x,y;
      f_piz_m1sig->GetPoint(i-1,x,y);
      double ucweight = 1;



     if (n_pizucfrac > 0)
	{
	  ucweight = (1+(1-y))**n_pizucfrac;
	}
      if (n_pizucfrac < 0)
	{
	  ucweight = (y**(fabs(n_pizucfrac)));  
	}


      h_piz_hadronpt->SetBinContent(i,h_piz_hadronpt->GetBinContent(i));
      h_piz_hadronpt->SetBinError(i,h_piz_hadronpt->GetBinError(i));
      for (int k = 1; k < 101; k++)
	{
	  double binvalue = h_piz_ptdca->GetBinContent(k,i)*ucweight;
	  double binerror = h_piz_ptdca->GetBinError(k,i)*ucweight;

	  h_pizepthpt->SetBinContent(k,i,binvalue);
	  h_pizepthpt->SetBinError(k,i,binerror);


	  h_pizepthpt_con->SetBinContent(k,i,h_piz_pt_con_2d->GetBinContent(k,i)*ucweight);
	  h_pizepthpt_con->SetBinError(k,i,h_piz_pt_con_2d->GetBinError(k,i)*ucweight);


	} 
    }
  TH1F* h_piz_pt = (TH1F*)h_pizepthpt->ProjectionX("h_piz_pt");


  TH1F* h_piz_pt_con = (TH1F*)h_pizepthpt_con->ProjectionX("h_piz_pt_con");
  TH1F* h_piz_pt_conveto = (TH1F*)h_piz_pt->Clone("h_piz_pt_conveto");




  double n_etaucfrac = f_eta->GetRandom();


  TH2F* h_etaepthpt = (TH2F*)h_eta_ptdca->Clone("h_etaepthpt");

  h_etaepthpt->Reset();

  TH2F* h_etaepthpt_con = (TH2F*)h_eta_pt_con_2d->Clone("h_etaepthpt_con");
  h_etaepthpt_con->Reset();


  // TCanvas* tmp = new TCanvas("tmp","",500,500);
  // f_eta_rc->Draw("AC");


  // TH1F* h_eta_hadronpt = (TH1F*)h_eta_hadronpt2->Clone("h_eta_hadronpt");
  TH1F* h_hetapt = (TH1F*)h_eta_ptdca->ProjectionY("h_hetapt");
  for (int i = 1; i < 201;i++)
    {
      // double weight = f_eta_rc(h_hetapt->GetBinCenter(i));

      Double_t x,y;
      f_eta_m1sig->GetPoint(i-1,x,y);
      double ucweight = 1;
      // y = 1;
      // cout << (1+(1-y)*n_etaucfrac) << endl;
 

     if (n_etaucfrac > 0)
	{
	  ucweight = f_eta_rc(h_hetapt->GetBinCenter(i))*(1+(1-y))**n_etaucfrac;
	}
      if (n_etaucfrac < 0)
	{
	  ucweight = f_eta_rc(h_hetapt->GetBinCenter(i))*(y**(fabs(n_etaucfrac)));  
	}
      if (n_etaucfrac == 0 )
	{
	  ucweight = f_eta_rc(h_hetapt->GetBinCenter(i));
	}





      // double weight = f_eta_rc(h_hetapt->GetBinCenter(i));
      double weight = f_eta_rc(h_hetapt->GetBinCenter(i));
      // cout << weight << endl;

      h_eta_hadronpt->SetBinContent(i,h_eta_hadronpt->GetBinContent(i)*weight);
      h_eta_hadronpt->SetBinError(i,h_eta_hadronpt->GetBinError(i)*weight);
      for (int k = 1; k < 101; k++)
	{
	  double binvalue = h_eta_ptdca->GetBinContent(k,i)*weight;
	  double binerror = h_eta_ptdca->GetBinError(k,i)*weight;

	  h_etaepthpt->SetBinContent(k,i,binvalue);
	  h_etaepthpt->SetBinError(k,i,binerror);


	  h_etaepthpt_con->SetBinContent(k,i,h_eta_pt_con_2d->GetBinContent(k,i)*weight);
	  h_etaepthpt_con->SetBinError(k,i,h_eta_pt_con_2d->GetBinError(k,i)*weight);



	} 
    }
  TH1F* h_eta_pt = (TH1F*)h_etaepthpt->ProjectionX("h_eta_pt");

  TH1F* h_eta_pt_conveto = (TH1F*)h_eta_pt->Clone("h_eta_pt_conveto");
  TH1F* h_eta_pt_con = (TH1F*)h_etaepthpt_con->ProjectionX("h_eta_pt_con");


  double n_photonucfrac = f_photon->GetRandom();



  TH1F* h_hphotonpt = (TH1F*)h_photon_ptdca->ProjectionY("h_hphotonpt");
  for (int i = 1; i < 201;i++)
    {

      Double_t x,y;
      y = 0.5;
      double ucweight = 1;

      // cout << weight << endl;

     if (n_photonucfrac > 0)
	{
	  ucweight = (1+(1-y))**n_photonucfrac;
	}
      if (n_photonucfrac < 0)
	{
	  ucweight = (y**(fabs(n_photonucfrac)));  
	}


      h_photon_hadronpt->SetBinContent(i,h_photon_hadronpt->GetBinContent(i));
      h_photon_hadronpt->SetBinError(i,h_photon_hadronpt->GetBinError(i));
      for (int k = 1; k < 101; k++)
	{
	  double binvalue = h_photon_ptdca->GetBinContent(k,i)*ucweight;
	  double binerror = h_photon_ptdca->GetBinError(k,i)*ucweight;

	  h_photonepthpt->SetBinContent(k,i,binvalue);
	  h_photonepthpt->SetBinError(k,i,binerror);


	  h_photonepthpt_con->SetBinContent(k,i,h_photon_pt_con_2d->GetBinContent(k,i)*ucweight);
	  h_photonepthpt_con->SetBinError(k,i,h_photon_pt_con_2d->GetBinError(k,i)*ucweight);


	} 
    }
  TH1F* h_photon_pt = (TH1F*)h_photonepthpt->ProjectionX("h_photon_pt");

  TH1F* h_photon_pt_con = (TH1F*)h_photonepthpt_con->ProjectionX("h_photon_pt_con");
  TH1F* h_photon_pt_conveto = (TH1F*)h_photon_pt->Clone("h_photon_pt_conveto");



  double n_jpsiucfrac = f_jpsi->GetRandom();





  TH2F* h_jpsiepthpt = (TH2F*)h_jpsi_ptdca->Clone("h_jpsiepthpt");
  h_jpsiepthpt->Reset();
  TH1F* h_hjpsipt = (TH1F*)h_jpsi_ptdca->ProjectionY("h_hjpsipt");
  for (int i = 1; i < 201;i++)
    {


      Double_t x,y;
      f_jpsi_m1sig->GetPoint(i-1,x,y);
      double ucweight = 1;
      // if ( (1+(1-y)*n_jpsiucfrac) > 0)
      // 	{
      // 	  ucweight = f_jpsi_rc(h_hjpsipt->GetBinCenter(i))*(1+(1-y)*n_jpsiucfrac);
      // 	}
      // else 
      // 	{
      // 	  ucweight = f_jpsi_rc(h_hjpsipt->GetBinCenter(i))*(0.00001);
      // 	}



     if (n_jpsiucfrac > 0)
	{
	  ucweight = f_jpsi_rc(h_hjpsipt->GetBinCenter(i))*(1+(1-y))**n_jpsiucfrac;
	}
      if (n_jpsiucfrac < 0)
	{
	  ucweight = f_jpsi_rc(h_hjpsipt->GetBinCenter(i))*(y**(fabs(n_jpsiucfrac)));  
	}
      if (n_jpsiucfrac == 0 )
	{
	  ucweight = f_jpsi_rc(h_hjpsipt->GetBinCenter(i));
	}


      double weight = f_jpsi_rc(h_hjpsipt->GetBinCenter(i));

      h_jpsi_hadronpt->SetBinContent(i,h_jpsi_hadronpt->GetBinContent(i)*weight);
      h_jpsi_hadronpt->SetBinError(i,h_jpsi_hadronpt->GetBinError(i)*weight);
      for (int k = 1; k < 101; k++)
	{
	  double binvalue = h_jpsi_ptdca->GetBinContent(k,i)*ucweight;
	  double binerror = h_jpsi_ptdca->GetBinError(k,i)*ucweight;

	  h_jpsiepthpt->SetBinContent(k,i,binvalue);
	  h_jpsiepthpt->SetBinError(k,i,binerror);
	} 
    }
  TH1F* h_jpsi_pt = (TH1F*)h_jpsiepthpt->ProjectionX("h_jpsi_pt");









  TF1* f_jpsi_fit_yield = new TF1("f_jpsi_fit_yield","2*3.1415*x*f_jpsi_fit",0,20);
  TF1* f_eta_fit_yield = new TF1("f_eta_fit_yield","2*3.1415*x*f_eta_fit",0,20);
  TF1* f_kp_fit_yield = new TF1("f_kp_fit_yield","2*3.1415*x*f_kp_fit",0,20);
  TF1* f_km_fit_yield = new TF1("f_km_fit_yield","2*3.1415*x*f_km_fit",0,20);
  TF1* f_ks_fit_yield = new TF1("f_ks_fit_yield","2*3.1415*x*f_ks_fit",0,20);


  //******************************************************************
  //Calculate Normalization Factors of hadrons to 2pipT* cross section
  //******************************************************************

  // double n_piz_integral = f_pizero_spectrum->Integral(0,20);
  // double n_photon_integral = f_photon_spectrum->Integral(0,20);
  // double n_jpsi_integral = f_jpsi_fit->Integral(0,20)/(10000);
  // double n_eta_integral = f_eta_fit->Integral(0,20);
  // double n_kp_integral = f_kp_fit->Integral(0,20);
  // double n_km_integral = f_km_fit->Integral(0,20);
  // double n_ks_integral = f_ks_fit->Integral(0,20);


  double n_piz_integral = f_pizero_spectrum->Integral(0,20);
  double n_photon_integral = f_photon_spectrum->Integral(0,20);
  double n_jpsi_integral = f_jpsi_fit_yield->Integral(0,20)/(1000000*0.0602);
  double n_eta_integral = f_eta_fit_yield->Integral(0,20);
  double n_kp_integral = f_kp_fit_yield->Integral(0,20);
  double n_km_integral = f_km_fit_yield->Integral(0,20);
  double n_ks_integral = f_ks_fit_yield->Integral(0,20);




  // if ( n_pizucfrac > 0)
  //   {
  //     weight = n_pizucfrac;
  //   }
  // else 
  //   {
  //     weight = 0.00001;
  //   }
  





  double n_piz_hadronnorm =    n_piz_integral/h_piz_hadronpt->Integral();
  double n_eta_hadronnorm =    n_eta_integral/h_eta_hadronpt->Integral();
  double n_photon_hadronnorm = n_photon_integral/h_photon_hadronpt->Integral();
  double n_jpsi_hadronnorm =   n_jpsi_integral/h_jpsi_hadronpt->Integral();
  double n_kp_hadronnorm =     n_kp_integral/h_kp_hadronpt->Integral();
  double n_km_hadronnorm =     n_km_integral/h_km_hadronpt->Integral();
  double n_ks_hadronnorm =     n_ks_integral/h_ks_hadronpt->Integral(); 


  //*********************************
  //Account for simulated decay nodes
  //*********************************
  n_jpsi_hadronnorm = n_jpsi_hadronnorm*0.060200;
  n_eta_hadronnorm = n_eta_hadronnorm*0.4437;
  n_piz_hadronnorm = n_piz_hadronnorm * 1;
  n_photon_hadronnorm = n_photon_hadronnorm * 1;
  n_kp_hadronnorm = n_kp_hadronnorm * 0.0482;
  n_km_hadronnorm = n_km_hadronnorm * 0.0482;
  n_ks_hadronnorm = n_ks_hadronnorm * 0.3069;


  //********************
  //Apply Factors to ept
  //********************
  h_eta_pt->Scale(n_eta_hadronnorm);
  h_eta_pt_con->Scale(n_eta_hadronnorm);
  h_eta_pt_conveto->Scale(n_eta_hadronnorm);
  h_piz_pt->Scale(n_piz_hadronnorm);
  h_piz_pt_con->Scale(n_piz_hadronnorm);
  h_piz_pt_conveto->Scale(n_piz_hadronnorm);

  h_jpsi_pt->Scale(n_jpsi_hadronnorm);
  h_photon_pt->Scale(n_photon_hadronnorm);
  h_photon_pt_con->Scale(n_photon_hadronnorm);
  h_photon_pt_conveto->Scale(n_photon_hadronnorm);
  h_kp_pt->Scale(n_kp_hadronnorm);
  h_km_pt->Scale(n_km_hadronnorm);
  h_ks_pt->Scale(n_ks_hadronnorm);





  // h_ks_pt->SetAxisRange(1,8,"X");
  // TCanvas*c1 = new TCanvas("c1","",500,500);
  // gPad->SetLogy();
  // h_kp_pt->SetLineColor(1);
  // h_km_pt->SetLineColor(2);
  // h_ks_pt->SetLineColor(3);
  // h_ks_pt->Draw();
  // h_km_pt->Draw("SAME");
  // h_kp_pt->Draw("SAME");


  TH1F* h_ke3_pt = (TH1F*)h_kp_pt->Clone("h_ke3_pt");
  h_ke3_pt->Add(h_km_pt);
  h_ke3_pt->Add(h_ks_pt);


  // h_ke3_pt->SetAxisRange(0.0000000000001,0.01,"Y");


  h_ke3_pt->SetLineColor(1);
  h_piz_pt->SetLineColor(2);
  h_eta_pt->SetLineColor(3);
  h_jpsi_pt->SetLineColor(4);
  h_photon_pt->SetLineColor(6);

  h_piz_pt_con->SetLineColor(2);
  h_eta_pt_con->SetLineColor(3);
  h_photon_pt_con->SetLineColor(6);

  h_piz_pt_conveto->SetLineColor(2);
  h_eta_pt_conveto->SetLineColor(3);
  h_photon_pt_conveto->SetLineColor(6);


  // double bins[16]  = {1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.5, 4.0, 4.5 , 5.0, 6.0, 8.0, 10.0};
  // double bins[16]  = {1.0, 1.2, 1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.5, 4.0, 4.5 , 5.0, 6.0, 8.0, 10.0};
  // double bins[11]  = {1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.5, 4.0, 4.5 , 5.0, 6.0};

  // double bins[10]  = {1.0, 1.5, 2.0, 2.5, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0};
  TH1F* h_ke3_pt = (TH1F*)h_ke3_pt->Rebin(nbins,"h_ke3_pt",bins);
  TH1F* h_eta_pt = (TH1F*)h_eta_pt->Rebin(nbins,"h_eta_pt",bins);
  TH1F* h_piz_pt = (TH1F*)h_piz_pt->Rebin(nbins,"h_piz_pt",bins);
  TH1F* h_jpsi_pt = (TH1F*)h_jpsi_pt->Rebin(nbins,"h_jpsi_pt",bins);
  TH1F* h_photon_pt = (TH1F*)h_photon_pt->Rebin(nbins,"h_photon_pt",bins);


  TH1F* h_piz_pt_con  = (TH1F*)h_piz_pt_con->Rebin(nbins,"h_piz_pt_con",bins);
  TH1F* h_piz_pt_conveto  = (TH1F*)h_piz_pt_conveto->Rebin(nbins,"h_piz_pt_conveto",bins);
  
  TH1F* h_eta_pt_con  = (TH1F*)h_eta_pt_con->Rebin(nbins,"h_eta_pt_con",bins);
  TH1F* h_eta_pt_conveto  = (TH1F*)h_eta_pt_conveto->Rebin(nbins,"h_eta_pt_conveto",bins);
  
  TH1F* h_photon_pt_con  = (TH1F*)h_photon_pt_con->Rebin(nbins,"h_photon_pt_con",bins);
  TH1F* h_photon_pt_conveto  = (TH1F*)h_photon_pt_conveto->Rebin(nbins,"h_photon_pt_conveto",bins);


  // TLegend*l1 = new TLegend(0.65,0.7,0.9,0.9);
  // l1->AddEntry(h_ke3_pt,"ke3");
  // l1->AddEntry(h_piz_pt,"pi0");
  // l1->AddEntry(h_jpsi_pt,"jpsi");
  // l1->AddEntry(h_eta_pt,"eta");
  // l1->AddEntry(h_photon_pt,"gamma");

  // h_ke3_pt->SetAxisRange(1,10,"X");
  // TCanvas*c2 = new TCanvas("c2","",500,500);
  // gPad->SetLogy();
  // h_ke3_pt->Draw();
  // h_jpsi_pt->Draw("SAME");
  // h_piz_pt->Draw("SAME");
  // h_eta_pt->Draw("SAME");
  // h_photon_pt->Draw("SAME");
  // l1->Draw();





  //****************************
  //calculate photonic fractions
  //****************************


  TH1F* h_photonic_pt_veto = (TH1F*)h_piz_pt->Clone("h_photonic_pt");
  h_photonic_pt_veto->Add(h_eta_pt);
  h_photonic_pt_veto->Add(h_photon_pt);



  TH1F* h_piz_f_veto = (TH1F*)h_piz_pt->Clone("h_piz_f_veto");
  TH1F* h_eta_f_veto = (TH1F*)h_eta_pt->Clone("h_eta_f_veto");
  TH1F* h_photon_f_veto = (TH1F*)h_photon_pt->Clone("h_photon_f_veto");

  h_piz_f_veto->Divide(h_photonic_pt_veto);
  h_eta_f_veto->Divide(h_photonic_pt_veto);
  h_photon_f_veto->Divide(h_photonic_pt_veto);




  TH1F* h_photonic_pt = (TH1F*)h_piz_pt_con->Clone("h_photonic_pt");
  h_photonic_pt->Add(h_piz_pt_conveto);
  h_photonic_pt->Add(h_eta_pt_con);
  h_photonic_pt->Add(h_eta_pt_conveto);

  h_photonic_pt->Add(h_photon_pt_con);
  h_photonic_pt->Add(h_photon_pt_conveto);



  TH1F* h_piz_f = (TH1F*)h_piz_pt_con->Clone("h_piz_f");
  h_piz_f->Add(h_piz_pt_conveto);
  TH1F* h_eta_f = (TH1F*)h_eta_pt_con->Clone("h_eta_f");
  h_eta_f->Add(h_eta_pt_conveto);

  TH1F* h_photon_f = (TH1F*)h_photon_pt_con->Clone("h_photon_f");
  h_photon_f->Add(h_photon_pt_conveto);

  h_piz_f->Divide(h_photonic_pt);
  h_eta_f->Divide(h_photonic_pt);
  h_photon_f->Divide(h_photonic_pt);



  h_piz_f->SetTitle(";p_{T};photonic fraction");

  TLegend* lphotonic = new TLegend(0.7,0.7,0.9,0.9);
  lphotonic->AddEntry(h_piz_f,"#pi_{0}");
  lphotonic->AddEntry(h_eta_f,"#eta");
  lphotonic->AddEntry(h_photon_f,"#gamma");

  // h_piz_f->SetAxisRange(0.001,10,"Y");
  // TCanvas* cphotonic = new TCanvas("cphotonic","",500,500);
  // gPad->SetLogy();
  // h_piz_f->Draw();
  // h_eta_f->Draw("SAME");
  // h_photon_f->Draw("SAME");
  // lphotonic->Draw();
  // cphotonic->SaveAs("plots/photonic_fraction.png");

  //***********************
  //calculate ratios to piz
  //***********************

  TH1F* h_ke3_f = (TH1F*)h_ke3_pt->Clone("h_ke3_f");
  TH1F* h_jpsi_f = (TH1F*)h_jpsi_pt->Clone("h_jpsi_f");
  h_ke3_f->Divide(h_piz_pt);
  h_jpsi_f->Divide(h_piz_pt);

  h_ke3_f->SetAxisRange(0.0001,80,"Y");
  h_ke3_f->SetTitle(";p_{T};ratio to #pi_{0}");
  // TCanvas* cratiopiz = new TCanvas("cratiopiz","",500,500);
  // gPad->SetLogy();
  // h_ke3_f->Draw();
  // h_jpsi_f->Draw("SAME");

  // TLegend* lratio = new TLegend(0.7,0.8,0.9,0.9);
  // lratio->AddEntry(h_ke3_f,"ke3");
  // lratio->AddEntry(h_jpsi_f,"j/#psi");
  // lratio->Draw();
  // cratiopiz->SaveAs("plots/ratiotopi0_2x.png");


  //******************************
  // read in non photonic survival
  //******************************

  // TFile* datanp = TFile::Open("histograms/survivalrates_data.root");

  // TH1F* h_nonphotonic_survival = (TH1F*)datanp->Get("h_survival3")->Clone("h_nonphotonic_survival");


  //***************************
  //calculate photonic survival
  //***************************

 TH1F* h_piz_ept = (TH1F*)h_piz_pt_con->Clone("h_piz_ept");
  TH1F* h_eta_ept = (TH1F*)h_eta_pt_con->Clone("h_eta_ept");
  TH1F* h_photon_ept = (TH1F*)h_photon_pt_con->Clone("h_photon_ept");
  h_piz_ept->Add(h_piz_pt_conveto);
  h_eta_ept->Add(h_eta_pt_conveto);
  h_photon_ept->Add(h_photon_pt_conveto);

  TH1F* h_photonic_numerator = (TH1F*)h_piz_pt_conveto->Clone("h_photonic_numerator");
  h_photonic_numerator->Add(h_eta_pt_conveto);
  h_photonic_numerator->Add(h_photon_pt_conveto);

  TH1F* h_photonic_denomenator = (TH1F*)h_piz_ept->Clone("h_photonic_denomenator");
  h_photonic_denomenator->Add(h_eta_ept);
  h_photonic_denomenator->Add(h_photon_ept);



  TH1F* h_piz_survival = (TH1F*)h_piz_pt_conveto->Clone("h_piz_survival");
  h_piz_survival->Divide(h_piz_ept);

  TH1F* h_eta_survival = (TH1F*)h_eta_pt_conveto->Clone("h_eta_survival");
  h_eta_survival->Divide(h_eta_ept);

  TH1F* h_gamma_survival = (TH1F*)h_photon_pt_conveto->Clone("h_gamma_survival");
  h_gamma_survival->Divide(h_photon_ept);


  // TCanvas*ctmp = new TCanvas("ctmp","",500,500);
  // h_photon_pt_conveto->Draw();
  // h_photon_ept->SetLineColor(kGreen+2);
  // h_photon_ept->Draw("SAME");



  TH1F*  h_piz_survivaltmp = (TH1F*)h_piz_survival->Clone("h_piz_survivaltmp");
  TH1F*  h_eta_survivaltmp = (TH1F*)h_eta_survival->Clone("h_eta_survivaltmp");
  TH1F*  h_gamma_survivaltmp = (TH1F*)h_gamma_survival->Clone("h_gamma_survivaltmp");


  // cout << h_piz_survival->GetNbinsX() <<" , " << h_piz_f->GetNbinsX() << endl;
  h_piz_survival->Multiply(h_piz_f);
  h_eta_survival->Multiply(h_eta_f);
  h_gamma_survival->Multiply(h_photon_f);

  // TH1F* h_photonic_survival = (TH1F*)h_piz_survival->Clone("h_photonic_survival");
  // h_photonic_survival->Add(h_eta_survival);
  // h_photonic_survival->Add(h_gamma_survival);
  TH1F* h_photonic_survival = (TH1F*)h_photonic_numerator->Clone("h_photonic_survival");
  h_photonic_survival->Divide(h_photonic_denomenator);


  // TCanvas*c6 = new TCanvas("c6","",500,500);
  // h_piz_survivaltmp->SetAxisRange(0,0.7,"Y");
  // h_piz_survivaltmp->SetLineColor(2);
  // h_eta_survivaltmp->SetLineColor(3);
  // h_gamma_survivaltmp->SetLineColor(6);
  // h_photonic_survival->SetLineColor(4);

  // TLegend* l2 = new TLegend(0.6,0.7,0.9,0.9);
  // l2->AddEntry(h_piz_survivaltmp,"pi0");
  // l2->AddEntry(h_eta_survivaltmp,"eta");
  // l2->AddEntry(h_gamma_survivaltmp,"direct \gamma");
  // l2->AddEntry(h_photonic_survival,"photonic");
  // h_piz_survivaltmp->Draw();
  // h_eta_survivaltmp->Draw("SAME");
  // h_gamma_survivaltmp->Draw("SAME");
  // h_photonic_survival->Draw("SAME");
  // l2->Draw();
  // c6->SaveAs("plots/photonic_survival_2x.png");
  // c2->SaveAs("plots/electron_cocktail_2x.png");





  //***************************
  // read in data electron tree
  //***************************

  // TFile* infile_fnpdata = TFile::Open("/gpfs/mnt/gpfs02/phenix/hhj/hhj1/trinn/run15pro108/combinedanataxi/300mev_bins/histograms/forfnp.root");
  // TH1F* h_ept_nv = (TH1F*)infile_fnpdata->Get("h_ept_nv");
  // TH1F* h_ept_conveto = (TH1F*)infile_fnpdata->Get("h_ept_conveto");




  for (int i = 1; i <= nbins;i++)
    {

      float n_conveto_e = h_ept_conveto->GetBinContent(i);
      float n_nv_e = h_ept_nv->GetBinContent(i);
      float n_hadcontam = h_hadroncontam->GetBinContent(i)*( n_nv_e);
      float n_hadcontamcv = h_hadroncontamwv->GetBinContent(i)*( n_conveto_e);

      float n_survival_photonic = h_photonic_survival->GetBinContent(i);
      float n_survival_nonphotonic = h_nonphotonic_survival->GetBinContent(i);

      float uc_conveto_e = h_ept_conveto->GetBinError(i);
      float uc_nv_e = h_ept_nv->GetBinError(i);
      float uc_survival_photonic = h_photonic_survival->GetBinError(i);
      float uc_survival_nonphotonic = h_nonphotonic_survival->GetBinError(i);
      float uc_hadcontam = h_hadroncontam->GetBinError(i)*( n_nv_e);
      float uc_hadcontamcv = h_hadroncontamwv->GetBinError(i)*( n_nv_e);



      double n_numerator_conveto = n_survival_photonic * n_survival_nonphotonic * ( n_nv_e - n_hadcontam) +  n_hadcontamcv - n_conveto_e;
      double n_denomenator_conveto =  n_conveto_e * (n_survival_photonic - 1);
      double n_denomenator_conveto_old =  (n_conveto_e-n_hadcontamcv) * (n_survival_photonic - 1);


      double n_fnp_veto = n_numerator_conveto/n_denomenator_conveto;
      double n_fnp_vetoold = n_numerator_conveto/n_denomenator_conveto_old;



      h_myfnp_conveto->SetBinContent(i,n_fnp_veto);
      h_oldfnp_conveto->SetBinContent(i,n_fnp_vetoold);

      // cout << n_fnp_veto + h_hadroncontamwv->GetBinContent(i) << endl;


      double ae = uc_survival_nonphotonic;
      double be = uc_survival_photonic;
      double ce =  uc_nv_e;
      double de =  uc_conveto_e;
      double ee =  uc_hadcontam;
      double fe =  uc_hadcontamcv;

      double a = n_survival_nonphotonic;
      double b = n_survival_photonic;
      double c = n_nv_e;
      double d = n_conveto_e;
      double e = n_hadcontam;
      double f =  n_hadcontamcv;



      double uc_fnp_conveto = TMath::Sqrt((a**2 * (b - 1)**2 * b**2 * ((c - e)**2 *(de**2 + fe**2) + (ce**2 + ee**2)* (d - f)**2) + ae**2 * (b - 1)**2 * b**2 * (c - e)**2 * (d - f)**2 + be**2  * (d - f)**2 * (a* c - a* e - d + f)**2)/((b - 1)**4 * (d - f)**4));
      h_myfnp_conveto->SetBinError(i,uc_fnp_conveto);
      h_oldfnp_conveto->SetBinError(i,uc_fnp_conveto);

    }



  h_oldfnp_conveto->SetAxisRange(0.1,1.1,"Y");
  // TCanvas*c4 = new TCanvas("c4","",500,500);
  // h_oldfnp_conveto->Draw();




   TH1F* h_piz_n = (TH1F*) h_piz_f_veto->Clone("h_piz_n");
   TH1F* h_eta_n = (TH1F*) h_eta_f_veto->Clone("h_eta_n");
   TH1F* h_photon_n = (TH1F*) h_photon_f_veto->Clone("h_photon_n");
   TH1F* h_ke3_n = (TH1F*) h_ke3_f->Clone("h_ke3_n");
   TH1F* h_jpsi_n = (TH1F*) h_jpsi_f->Clone("h_jpsi_n");
   TH1F* h_hadcontam_n = (TH1F*)h_hadroncontamwv->Clone("h_hadcontam_n");


   for (int i = 1; i <= nbins; i ++) 
     {
       float n_nv_e = h_ept_nv->GetBinContent(i);

       float n_conveto_e = h_ept_conveto->GetBinContent(i);
       float n_hadcontam = h_hadroncontamwv->GetBinContent(i);

       float uc_conveto_e = h_ept_conveto->GetBinError(i);
       float uc_hadcontam = h_hadroncontamwv->GetBinError(i);






       h_piz_n->SetBinContent(i,(1-h_hadcontam_n->GetBinContent(i))*h_piz_n->GetBinContent(i)*(1.0 - h_oldfnp_conveto->GetBinContent(i)));
       h_eta_n->SetBinContent(i,(1-h_hadcontam_n->GetBinContent(i))*h_eta_n->GetBinContent(i)*(1.0 - h_oldfnp_conveto->GetBinContent(i)));
       h_photon_n->SetBinContent(i,(1-h_hadcontam_n->GetBinContent(i))*h_photon_n->GetBinContent(i)*(1.0 - h_oldfnp_conveto->GetBinContent(i)));


       // cout << h_piz_n->GetBinContent(i) << endl;
       h_piz_n->SetBinError(i,h_piz_n->GetBinContent(i)*TMath::Sqrt((h_piz_f->GetBinError(i)/h_piz_f->GetBinContent(i))**2+(h_oldfnp_conveto->GetBinError(i)/h_oldfnp_conveto->GetBinContent(i))**2));
       h_eta_n->SetBinError(i,h_eta_n->GetBinContent(i)*TMath::Sqrt((h_eta_f->GetBinError(i)/h_eta_f->GetBinContent(i))**2+(h_oldfnp_conveto->GetBinError(i)/h_oldfnp_conveto->GetBinContent(i))**2));
       h_photon_n->SetBinError(i,h_photon_n->GetBinContent(i)*TMath::Sqrt((h_photon_f->GetBinError(i)/h_photon_f->GetBinContent(i))**2+(h_oldfnp_conveto->GetBinError(i)/h_oldfnp_conveto->GetBinContent(i))**2));

     
     }

   h_ke3_n->Multiply(h_piz_n);
   h_jpsi_n->Multiply(h_piz_n);

   h_hadcontam_n->SetLineColor(kYellow+1);

   // h_ke3_n->SetAxisRange(0.000001,200,"Y");
   // h_ke3_n->SetAxisRange(1,7.5,"X");
   // TCanvas*c5 = new TCanvas("c5","",500,500);
   // gPad->SetLogy();
   // h_ke3_n->Draw();
   // h_piz_n->Draw("SAME");
   // h_eta_n->Draw("SAME");
   // h_photon_n->Draw("SAME");
   // h_jpsi_n->Draw("SAME");
   // h_hadcontam_n->Draw("SAME");

   // TLegend*l5 = new TLegend(0.65,0.65,0.9,0.9);
   // l5->AddEntry(h_ke3_pt,"ke3");
   // l5->AddEntry(h_piz_pt,"pi0");
   // l5->AddEntry(h_jpsi_pt,"jpsi");
   // l5->AddEntry(h_eta_pt,"eta");
   // l5->AddEntry(h_photon_pt,"gamma");
   
   // l5->AddEntry(h_hadcontam_n,"Hadrons");
   // l5->Draw();


   // c4->SaveAs("plots/fnp_2x.png");
   // c5->SaveAs("plots/normalizations_2x.png");

   for (int i = 0; i < nbins;i++)
     {
       h_syspizn[i]->Fill(h_piz_n->GetBinContent(i+1));
       h_sysetan[i]->Fill(h_eta_n->GetBinContent(i+1));
       h_sysfnp[i]->Fill(h_oldfnp_conveto->GetBinContent(i+1));
       h_sysjpsin[i]->Fill(h_jpsi_n->GetBinContent(i+1));
       h_syske3n[i]->Fill(h_ke3_n->GetBinContent(i+1));
       h_sysphotonn[i]->Fill(h_photon_n->GetBinContent(i+1));



   }
  }

   TFile* outfile = new TFile("outputFiles/normalizations.root","RECREATE");


   h_ke3_n->Write();
   h_piz_n->Write();
   h_jpsi_n->Write();
   h_eta_n->Write();
   h_photon_n->Write();
   h_hadcontam_n->Write();
   h_ke3_pt->Write();
   h_piz_pt->Write();
   h_eta_pt->Write();
   h_jpsi_pt->Write();
   h_photon_pt->Write();
   h_photonic_survival->Write();

   h_oldfnp_conveto->Write();



   h_nonphotonic_survival->Write();
   h_piz_survivaltmp->Write();
   h_eta_survivaltmp->Write();
   h_gamma_survivaltmp->Write();
   h_ept_conveto->Write();
   h_ept_nv->Write();
   h_hadroncontam->Write();
   h_hadroncontamwv->Write();
   h_kp_pt->Write();
   h_km_pt->Write();
   h_ks_pt->Write();

   h_piz_survivaltmp->Write();

   TCanvas*ctmp = new TCanvas("ctmp","",900,900);
   ctmp->Divide(3,4);
   for (int i = 1; i <= nbins;i++)
     {
       ctmp->cd(i);
       h_syspizn[i-1]->Draw();
     }

   TCanvas*ctmp2 = new TCanvas("ctmp2","",900,900);
   ctmp2->Divide(3,4);
   for (int i = 1; i <= nbins;i++)
     {
       ctmp2->cd(i);
       h_sysetan[i-1]->Draw();
     }
   TCanvas*ctmp3 = new TCanvas("ctmp3","",900,900);
   ctmp3->Divide(3,4);
   for (int i = 1; i <= nbins;i++)
     {
       ctmp3->cd(i);
       h_sysjpsin[i-1]->Draw();
     }
  TCanvas*ctmp4 = new TCanvas("ctmp4","",900,900);
  ctmp4->Divide(3,4);
   for (int i = 1; i <= nbins;i++)
     {
       ctmp4->cd(i);
       h_sysfnp[i-1]->Draw();
     }

  TCanvas*ctmp5 = new TCanvas("ctmp5","",900,900);
  ctmp5->Divide(3,4);
   for (int i = 1; i <= nbins;i++)
     {
       ctmp5->cd(i);
       h_syske3n[i-1]->Draw();
     }
  TCanvas*ctmp6 = new TCanvas("ctmp6","",900,900);
  ctmp6->Divide(3,4);
   for (int i = 1; i <= nbins;i++)
     {
       ctmp6->cd(i);
       h_sysphotonn[i-1]->Draw();
     }

   TH1F* h_sys_piz = new TH1F("h_sys_piz",";p_{T};",nbins,bins);
   TH1F* h_sys_eta = new TH1F("h_sys_eta",";p_{T};",nbins,bins);
   TH1F* h_sys_jpsi = new TH1F("h_sys_jpsi",";p_{T};",nbins,bins);
   TH1F* h_sys_ke3 = new TH1F("h_sys_ke3",";p_{T};",nbins,bins);
   TH1F* h_sys_photon = new TH1F("h_sys_photon",";p_{T};",nbins,bins);
   TH1F* h_sys_fnp = new TH1F("h_sys_fnp",";p_{T};",nbins,bins);


   TH1F* h_mean_piz = new TH1F("h_mean_piz",";p_{T};",nbins,bins);
   TH1F* h_mean_eta = new TH1F("h_mean_eta",";p_{T};",nbins,bins);
   TH1F* h_mean_jpsi = new TH1F("h_mean_jpsi",";p_{T};",nbins,bins);
   TH1F* h_mean_ke3 = new TH1F("h_mean_ke3",";p_{T};",nbins,bins);
   TH1F* h_mean_photon = new TH1F("h_mean_photon",";p_{T};",nbins,bins);

   TH1F* h_fnp_mean = new TH1F("h_fnp_mean",";p_{T};",nbins,bins);

   for (int i = 1; i <= nbins; i++)
     {
       h_sys_piz->SetBinContent(i,h_syspizn[i-1]->GetRMS());
       h_sys_eta->SetBinContent(i,h_sysetan[i-1]->GetRMS());
       h_sys_ke3->SetBinContent(i,h_syske3n[i-1]->GetRMS());
       h_sys_jpsi->SetBinContent(i,h_sysjpsin[i-1]->GetRMS());
       h_sys_photon->SetBinContent(i,h_sysphotonn[i-1]->GetRMS());

       h_mean_piz->SetBinContent(i,h_syspizn[i-1]->GetMean());
       h_mean_eta->SetBinContent(i,h_sysetan[i-1]->GetMean());
       h_mean_ke3->SetBinContent(i,h_syske3n[i-1]->GetMean());
       h_mean_jpsi->SetBinContent(i,h_sysjpsin[i-1]->GetMean());
       h_mean_photon->SetBinContent(i,h_sysphotonn[i-1]->GetMean());


       h_sys_fnp->SetBinContent(i,h_sysfnp[i-1]->GetRMS());
       h_fnp_mean->SetBinContent(i,h_sysfnp[i-1]->GetMean());
     }

   TFile*outfile = new TFile("outputFiles/systematics.root","RECREATE");
   h_sys_piz->Write();
   h_sys_eta->Write();
   h_sys_ke3->Write();
   h_sys_jpsi->Write();
   h_sys_photon->Write();
   h_fnp_mean->Write();
   h_sys_fnp->Write();
   h_mean_piz->Write();
   h_mean_eta->Write();
   h_mean_ke3->Write();
   h_mean_jpsi->Write();
   h_mean_photon->Write();
}
