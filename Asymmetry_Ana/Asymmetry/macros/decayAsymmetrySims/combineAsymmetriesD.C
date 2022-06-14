#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "../../Constants.h"
#include "TGraph.h"
#include "TString.h"
#include "TAxis.h"
#include "TAttAxis.h"

void combineAsymmetriesD()
{
  bool d0 = 1;
	bool prelim = 1;
	bool theory_compare = 0;
	int verbosity = -99;
	const int nbins = NUM_VALUE_BINS;
	double x_AN_plus[nbins], y_AN_plus[nbins], x_AN_minus[nbins], y_AN_minus[nbins];
  double AN_err_plus[nbins], AN_err_minus[nbins];
	double y_AN_theory_plus_KG[nbins], y_AN_theory_minus_KG[nbins];
	double y_AN_theory_plus_KGp[nbins], y_AN_theory_minus_KGp[nbins];	
	double x_AN_theory_plus_KG_d0[nbins], y_AN_theory_plus_KG_d0[nbins], x_AN_theory_minus_KG_d0bar[nbins], y_AN_theory_minus_KG_d0bar[nbins];
	double x_AN_theory_plus_KGp_d0[nbins], y_AN_theory_plus_KGp_d0[nbins], x_AN_theory_minus_KGp_d0bar[nbins], y_AN_theory_minus_KGp_d0bar[nbins];		
	double x_AN_theory_plus_KG_dplus[nbins], y_AN_theory_plus_KG_dplus[nbins], x_AN_theory_minus_KG_dminus[nbins], y_AN_theory_minus_KG_dminus[nbins];
	double x_AN_theory_plus_KGp_dplus[nbins], y_AN_theory_plus_KGp_dplus[nbins], x_AN_theory_minus_KGp_dminus[nbins], y_AN_theory_minus_KGp_dminus[nbins];		
  const int Npar = 101;
  double KGs[Npar] = {-0.005, -0.0049, -0.0048, -0.0047, -0.0046, -0.0045, -0.0044, -0.0043, -0.0042, -0.0041, -0.004, -0.0039, -0.0038, -0.0037, -0.0036, -0.0035, -0.0034, -0.0033, -0.0032, -0.0031, -0.003, -0.0029, -0.0028, -0.0027, -0.0026, -0.0025, -0.0024, -0.0023, -0.0022, -0.0021, -0.002, -0.0019, -0.0018, -0.0017, -0.0016, -0.0015, -0.0014, -0.0013, -0.0012, -0.0011, -0.0010, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0004, -0.0003, -0.0002, -0.0001, 0.0, 0.0001, 0.0002, 0.0003, 0.0004, 0.0005, 0.0006, 0.0007, 0.0008, 0.0009, 0.0010, 0.0011, 0.0012, 0.0013, 0.0014, 0.0015 , 0.0016, 0.0017, 0.0018, 0.0019, 0.002, 0.0021, 0.0022, 0.0023, 0.0024, 0.0025 , 0.0026, 0.0027, 0.0028, 0.0029, 0.003, 0.0031, 0.0032, 0.0033, 0.0034, 0.0035 , 0.0036, 0.0037, 0.0038, 0.0039, 0.004, 0.0041, 0.0042, 0.0043, 0.0044, 0.0045, 0.0046, 0.0047, 0.0048, 0.0049, 0.005};
  double KGps[Npar] = {-0.00025, -0.00024, -0.00023, -0.00022, -0.00021, -0.0002, -0.00019, -0.00018, -0.00017, -0.00016, -0.00015, -0.00014, -0.00013, -0.00012, -0.00011, -0.0001, -0.00009, -0.00008, -0.00007, -0.00006, -0.00005, -0.00004, -0.00003, -0.00002, -0.00001, 0.0, 0.00001, 0.00002, 0.00003, 0.00004, 0.00005, 0.00006, 0.00007, 0.00008, 0.00009, 0.0001, 0.00011, 0.00012, 0.00013, 0.00014, 0.00015, 0.00016, 0.00017, 0.00018, 0.00019, 0.0002, 0.00021, 0.00022, 0.00023, 0.00024, 0.00025, 0.00026, 0.00027, 0.00028, 0.00029, 0.0003, 0.00031, 0.00032, 0.00033, 0.00034, 0.00035, 0.00036, 0.00037, 0.00038, 0.00039 , 0.0004, 0.00041, 0.00042, 0.00043, 0.00044, 0.00045, 0.00046, 0.00047, 0.00048, 0.00049, 0.0005, 0.00051, 0.00052, 0.00053, 0.00054, 0.00055, 0.00056, 0.00057, 0.00058, 0.00059, 0.0006, 0.00061, 0.00062, 0.00063, 0.00064, 0.00065, 0.00066, 0.00067, 0.00068, 0.00069, 0.0007, 0.00071, 0.00072, 0.00073, 0.00074, 0.00075};
 
	TFile *infile_theory_plus_d0 = TFile::Open("dataFiles/d0DecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");
  TFile *infile_theory_minus_d0bar = TFile::Open("dataFiles/antid0DecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");

  TFile *infile_theory_plus_dplus = TFile::Open("dataFiles/dplusDecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");
  TFile *infile_theory_minus_dminus = TFile::Open("dataFiles/dminusDecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");
  
	TFile *infile_dfracs = TFile::Open("dataFiles/d_fractions.root");  
    
	TH1D *h_d0fracs = (TH1D*)infile_dfracs->Get("d0fracs");	
	TH1D *h_antid0fracs = (TH1D*)infile_dfracs->Get("antid0fracs");		
  TGraph *eANtheory_plus_KG_d0[Npar], *eANtheory_minus_KG_d0bar[Npar];
  TGraph *eANtheory_plus_KGp_d0[Npar], *eANtheory_minus_KGp_d0bar[Npar];  
  TGraph *eANtheory_plus_KG_dplus[Npar], *eANtheory_minus_KG_dminus[Npar];
  TGraph *eANtheory_plus_KGp_dplus[Npar], *eANtheory_minus_KGp_dminus[Npar];    
  TGraph *eANtheory_plus_KG[Npar], *eANtheory_minus_KG[Npar];
  TGraph *eANtheory_plus_KGp[Npar], *eANtheory_minus_KGp[Npar];     
             
  TString graphname = "";
  string graph_name_KG = ""; 
  string graph_name_KGp = ""; 
  TString graphname_KG = "";
  TString graphname_KGp = "";  		
  TCanvas *candiv_plus = new TCanvas();
  TCanvas *candiv_minus = new TCanvas();

  for (int j=0; j<Npar; ++j)
  {
	  graphname = "eAN";
	  ostringstream ss1;
	  ostringstream ss2;
	  ss1<<KGs[j];
	  ss2<<KGps[j];
	  graphname_KG = graphname;
	  graphname_KG += "_KG_";
	  graphname_KG += ss1.str();
	  graphname_KGp = graphname; 
	  graphname_KGp += "_KGp_";
	  graphname_KGp += ss2.str();

	  string graph_name_KG = graphname_KG;
	  replace(graph_name_KG.begin(), graph_name_KG.end(), '.', 'p');
	  replace(graph_name_KG.begin(), graph_name_KG.end(), '-', 'm');

	  string graph_name_KGp = graphname_KGp;
	  replace(graph_name_KGp.begin(), graph_name_KGp.end(), '.', 'p');
	  replace(graph_name_KGp.begin(), graph_name_KGp.end(), '-', 'm');

	  graphname_KG = graph_name_KG;
	  graphname_KGp = graph_name_KGp;
	  
    eANtheory_plus_KG_d0[j] = (TGraph*)infile_theory_plus_d0->Get(graphname_KG);
    eANtheory_minus_KG_d0bar[j] = (TGraph*)infile_theory_minus_d0bar->Get(graphname_KG);
    eANtheory_plus_KGp_d0[j] = (TGraph*)infile_theory_plus_d0->Get(graphname_KGp);
    eANtheory_minus_KGp_d0bar[j] = (TGraph*)infile_theory_minus_d0bar->Get(graphname_KGp);  
    
    eANtheory_plus_KG_dplus[j] = (TGraph*)infile_theory_plus_dplus->Get(graphname_KG);
    eANtheory_minus_KG_dminus[j] = (TGraph*)infile_theory_minus_dminus->Get(graphname_KG);
    eANtheory_plus_KGp_dplus[j] = (TGraph*)infile_theory_plus_dplus->Get(graphname_KGp);
    eANtheory_minus_KGp_dminus[j] = (TGraph*)infile_theory_minus_dminus->Get(graphname_KGp);   
    
    eANtheory_plus_KG[j] = new TGraph(); 
    eANtheory_plus_KG[j]->SetName(graphname_KG);  
    eANtheory_plus_KGp[j] = new TGraph(); 
    eANtheory_plus_KGp[j]->SetName(graphname_KGp);      
    eANtheory_minus_KG[j] = new TGraph();  
    eANtheory_minus_KG[j]->SetName(graphname_KG);     
    eANtheory_minus_KGp[j] = new TGraph();  
    eANtheory_minus_KGp[j]->SetName(graphname_KGp);            
           
  }  
  
  for (int j=0; j<Npar; ++j)
  {
      for (int i=0; i<nbins; ++i)
	    {		
		    
		    eANtheory_plus_KG_d0[j]->GetPoint(i, x_AN_theory_plus_KG_d0[i], y_AN_theory_plus_KG_d0[i]);
		    eANtheory_plus_KG_dplus[j]->GetPoint(i, x_AN_theory_plus_KG_dplus[i], y_AN_theory_plus_KG_dplus[i]);		    
		    y_AN_theory_plus_KG[i] = h_d0fracs->GetBinContent(i+1)*y_AN_theory_plus_KG_d0[i] + (1-h_d0fracs->GetBinContent(i+1))*y_AN_theory_plus_KG_dplus[i];
		    eANtheory_plus_KG[j]->SetPoint(i, x_AN_theory_plus_KG_d0[i], y_AN_theory_plus_KG[i]);
		    	    
		    eANtheory_plus_KGp_d0[j]->GetPoint(i, x_AN_theory_plus_KGp_d0[i], y_AN_theory_plus_KGp_d0[i]);
		    eANtheory_plus_KGp_dplus[j]->GetPoint(i, x_AN_theory_plus_KGp_dplus[i], y_AN_theory_plus_KGp_dplus[i]);	
		    y_AN_theory_plus_KGp[i] = h_d0fracs->GetBinContent(i+1)*y_AN_theory_plus_KGp_d0[i] + (1-h_d0fracs->GetBinContent(i+1))*y_AN_theory_plus_KGp_dplus[i];		    	    
		    eANtheory_plus_KGp[j]->SetPoint(i, x_AN_theory_plus_KGp_d0[i], y_AN_theory_plus_KGp[i]);		      
		    
		    eANtheory_minus_KG_d0bar[j]->GetPoint(i, x_AN_theory_minus_KG_d0bar[i], y_AN_theory_minus_KG_d0bar[i]);
		    eANtheory_minus_KG_dminus[j]->GetPoint(i, x_AN_theory_minus_KG_dminus[i], y_AN_theory_minus_KG_dminus[i]);		 
		    y_AN_theory_minus_KG[i] = h_antid0fracs->GetBinContent(i+1)*y_AN_theory_minus_KG_d0bar[i] + (1-h_antid0fracs->GetBinContent(i+1))*y_AN_theory_minus_KG_dminus[i];		   		        
		    eANtheory_minus_KG[j]->SetPoint(i, x_AN_theory_minus_KG_d0bar[i], y_AN_theory_minus_KG[i]);		     
		    
		    eANtheory_minus_KGp_d0bar[j]->GetPoint(i, x_AN_theory_minus_KGp_d0bar[i], y_AN_theory_minus_KGp_d0bar[i]);
		    eANtheory_minus_KGp_dminus[j]->GetPoint(i, x_AN_theory_minus_KGp_dminus[i], y_AN_theory_minus_KGp_dminus[i]);		    
		    y_AN_theory_minus_KGp[i] = h_antid0fracs->GetBinContent(i+1)*y_AN_theory_minus_KGp_d0bar[i] + (1-h_antid0fracs->GetBinContent(i+1))*y_AN_theory_minus_KGp_dminus[i];		   		    
		    eANtheory_minus_KGp[j]->SetPoint(i, x_AN_theory_minus_KGp_d0bar[i], y_AN_theory_minus_KGp[i]);		    		      
	    } 
  }
  	
  TFile *outfile_plus = new TFile("dataFiles/dcombinedDecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root", "RECREATE");
  outfile_plus->cd();
  for (int j=0; j<Npar; ++j)
  {
    eANtheory_plus_KG[j]->Write();
    eANtheory_plus_KGp[j]->Write();
    eANtheory_plus_KG[j]->Delete();
    eANtheory_plus_KGp[j]->Delete();    
  }	
  TFile *outfile_minus = new TFile("dataFiles/antidcombinedDecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root", "RECREATE");    
  outfile_minus->cd();
  for (int j=0; j<Npar; ++j)
  {
    eANtheory_minus_KG[j]->Write();
    eANtheory_minus_KGp[j]->Write();
    eANtheory_minus_KG[j]->Delete();
    eANtheory_minus_KGp[j]->Delete();    
  }	  
  outfile_plus->Write();
  outfile_plus->Delete();
  outfile_minus->Write();
  outfile_minus->Delete();  
	
}
