void D_e_fractions()
{
	TFile *d0decayFile = TFile::Open("dataFiles/condor/d0decay_0.root");
	TFile *antid0decayFile = TFile::Open("dataFiles/condor/antid0decay_0.root");
	TFile *dplusdecayFile = TFile::Open("dataFiles/condor/dplusdecay_0.root");
	TFile *dminusdecayFile = TFile::Open("dataFiles/condor/dminusdecay_0.root");
	
  //double ptd0_e, ptd0bar_e, ptdplus_e, ptdminus_e;	
  TTree *d0tree = (TTree*)d0decayFile->Get( "d0" );
  //d0Tree->SetBranchAddress("pt1", &ptd0_e);    
  TTree *antid0tree = (TTree*)antid0decayFile->Get( "d0bar" );  
  //antid0Tree->SetBranchAddress("pt1", &ptd0bar_e);     
  TTree *dplustree = (TTree*)dplusdecayFile->Get( "dplus" );   
  //dplusTree->SetBranchAddress("pt1", &ptdplus_e);       
  TTree *dminustree = (TTree*)dminusdecayFile->Get( "dminus" );
  //dminusTree->SetBranchAddress("pt1", &ptdminus_e);               

  const int nbins = 6;
  double ptbins[nbins+1] = {1.0, 1.3, 1.5, 1.8, 2.1, 2.7, 5.0};
  
  TH1D *h_ptd0_e = new TH1D("ptd0_e", "", nbins, ptbins);
  d0tree->Draw("pt1>>ptd0_e");
  h_ptd0_e->Scale(0.0649);

  TH1D *h_ptd0bar_e = new TH1D("ptd0bar_e", "", nbins, ptbins);
  antid0tree->Draw("pt1>>ptd0bar_e");  
  h_ptd0bar_e->Scale(0.0649);  
  
  TH1D *h_ptdplus_e = new TH1D("ptdplus_e", "", nbins, ptbins);
  dplustree->Draw("pt1>>ptdplus_e");    
  h_ptdplus_e->Scale(0.1607);  
  h_ptdplus_e->Scale(0.5);       
  
  TH1D *h_ptdminus_e = new TH1D("ptdminus_e", "", nbins, ptbins);
  dminustree->Draw("pt1>>ptdminus_e");   
  h_ptdminus_e->Scale(0.1607); 
  h_ptdminus_e->Scale(0.5);     
   
  TH1D *h_d0fracs = new TH1D("d0fracs", "", nbins, ptbins); 
  TH1D *h_antid0fracs = new TH1D("antid0fracs", "", nbins, ptbins);   
  
  double d0_fracs[nbins];
  double d0bar_fracs[nbins];  
  for (int i=0; i<nbins; ++i)
  {
    d0_fracs[i] =  h_ptd0_e->GetBinContent(i+1)/(h_ptd0_e->GetBinContent(i+1) + h_ptdplus_e->GetBinContent(i+1));   
    d0bar_fracs[i] = h_ptd0bar_e->GetBinContent(i+1)/(h_ptd0bar_e->GetBinContent(i+1) + h_ptdminus_e->GetBinContent(i+1));       
    h_d0fracs->SetBinContent(i+1, d0_fracs[i]);             
    h_antid0fracs->SetBinContent(i+1, d0bar_fracs[i]);                 
  } 
  
  h_d0fracs->Draw();
  h_antid0fracs->Draw();
  TFile *outfile = new TFile("dataFiles/d_fractions.root", "RECREATE");
  outfile->cd();
  h_d0fracs->Write();
  h_antid0fracs->Write();
  outfile->Write();
  outfile->Close();
 }
