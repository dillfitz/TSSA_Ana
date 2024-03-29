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

void lamParamChi2MinZoomed_KGKGp_dcombined()
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
 
	TFile *infile_data_plus = TFile::Open("../dataFiles/bgCorrected_ohfe_AN_plus.root");
	TFile *infile_data_minus = TFile::Open("../dataFiles/bgCorrected_ohfe_AN_minus.root");

	TFile *infile_theory_plus_d0 = TFile::Open("dataFiles/d0DecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");
  TFile *infile_theory_minus_d0bar = TFile::Open("dataFiles/antid0DecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");

  TFile *infile_theory_plus_dplus = TFile::Open("dataFiles/dplusDecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");
  TFile *infile_theory_minus_dminus = TFile::Open("dataFiles/dminusDecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");
  
	TFile *infile_dfracs = TFile::Open("dataFiles/d_fractions.root");  
    
	TGraphAsymmErrors *ohfeAN_plus = (TGraphAsymmErrors*)infile_data_plus->Get("ohfeAN");
	TGraphAsymmErrors *ohfeAN_minus = (TGraphAsymmErrors*)infile_data_minus->Get("ohfeAN");
	TH1D *h_d0fracs = (TH1D*)infile_dfracs->Get("d0fracs");	
	TH1D *h_antid0fracs = (TH1D*)infile_dfracs->Get("antid0fracs");		
  TGraph *eANtheory_plus_KG_d0[Npar], *eANtheory_minus_KG_d0bar[Npar];
  TGraph *eANtheory_plus_KGp_d0[Npar], *eANtheory_minus_KGp_d0bar[Npar];  
  TGraph *eANtheory_plus_KG_dplus[Npar], *eANtheory_minus_KG_dminus[Npar];
  TGraph *eANtheory_plus_KGp_dplus[Npar], *eANtheory_minus_KGp_dminus[Npar];    
  TGraph *eANtheory_plus_KG[Npar], *eANtheory_minus_KG[Npar];
  TGraph *eANtheory_plus_KGp[Npar], *eANtheory_minus_KGp[Npar];     
  double chi2_plus_sig_KG[Npar], chi2_plus_sig_KGp[Npar];  
  double chi2_minus_sig_KG[Npar], chi2_minus_sig_KGp[Npar];
  double chi2_sig_KG[Npar], chi2_sig_KGp[Npar];   
  double dchi2_plus_sig_KG[Npar], dchi2_plus_sig_KGp[Npar];  
  double dchi2_minus_sig_KG[Npar], dchi2_minus_sig_KGp[Npar];
  double dchi2_sig_KG[Npar], dchi2_sig_KGp[Npar];                 
  TString graphname = "";
  string graph_name_KG = ""; 
  string graph_name_KGp = ""; 
  TString graphname_KG = "";
  TString graphname_KGp = "";  		
  TCanvas *candiv_plus = new TCanvas();
  TCanvas *candiv_minus = new TCanvas();
	int counter = 0;
	candiv_plus->Divide(3,3);
	candiv_minus->Divide(3,3);
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
    eANtheory_plus_KGp[j] = new TGraph();   
    eANtheory_minus_KG[j] = new TGraph();   
    eANtheory_minus_KGp[j] = new TGraph();          
       
    chi2_plus_sig_KG[j] = 0;
    chi2_minus_sig_KG[j] = 0; 
    chi2_sig_KG[j] = 0;               
    chi2_plus_sig_KGp[j] = 0;
    chi2_minus_sig_KGp[j] = 0;  
    chi2_sig_KGp[j] = 0;        
    
    dchi2_plus_sig_KG[j] = 0;
    dchi2_minus_sig_KG[j] = 0; 
    dchi2_sig_KG[j] = 0;         
    dchi2_plus_sig_KGp[j] = 0;
    dchi2_minus_sig_KGp[j] = 0;       
    dchi2_sig_KGp[j] = 0;  
          
  }  
    
  vector<double> chi2_vec_KG, chi2_vec_KGp;  
  vector<double> chi2_plus_vec_KG, chi2_plus_vec_KGp; 
  vector<double> chi2_minus_vec_KG, chi2_minus_vec_KGp;
     
  vector<double> KGs_1sigma, KGs_2sigma;
  vector<double> KGps_1sigma, KGps_2sigma;
  for (int j=0; j<Npar; ++j)
  {
      for (int i=0; i<nbins; ++i)
	    {		
		    ohfeAN_plus->GetPoint(i, x_AN_plus[i], y_AN_plus[i]);
		    AN_err_plus[i] = ohfeAN_plus->GetErrorYhigh(i);
		    
		    eANtheory_plus_KG_d0[j]->GetPoint(i, x_AN_theory_plus_KG_d0[i], y_AN_theory_plus_KG_d0[i]);
		    eANtheory_plus_KG_dplus[j]->GetPoint(i, x_AN_theory_plus_KG_dplus[i], y_AN_theory_plus_KG_dplus[i]);		    
		    y_AN_theory_plus_KG[i] = h_d0fracs->GetBinContent(i+1)*y_AN_theory_plus_KG_d0[i] + (1-h_d0fracs->GetBinContent(i+1))*y_AN_theory_plus_KG_dplus[i];
		    chi2_plus_sig_KG[j] += pow(y_AN_plus[i] - y_AN_theory_plus_KG[i],2)/pow(AN_err_plus[i],2);		
		    eANtheory_plus_KG[j]->SetPoint(i, x_AN_theory_plus_KG_d0[i], y_AN_theory_plus_KG[i]);
		    	    
		    eANtheory_plus_KGp_d0[j]->GetPoint(i, x_AN_theory_plus_KGp_d0[i], y_AN_theory_plus_KGp_d0[i]);
		    eANtheory_plus_KGp_dplus[j]->GetPoint(i, x_AN_theory_plus_KGp_dplus[i], y_AN_theory_plus_KGp_dplus[i]);	
		    y_AN_theory_plus_KGp[i] = h_d0fracs->GetBinContent(i+1)*y_AN_theory_plus_KGp_d0[i] + (1-h_d0fracs->GetBinContent(i+1))*y_AN_theory_plus_KGp_dplus[i];		    	    
		    chi2_plus_sig_KGp[j] += pow(y_AN_plus[i] - y_AN_theory_plus_KGp[i],2)/pow(AN_err_plus[i],2);			  
		    eANtheory_plus_KGp[j]->SetPoint(i, x_AN_theory_plus_KGp_d0[i], y_AN_theory_plus_KGp[i]);		      
		    
		    
		    ohfeAN_minus->GetPoint(i, x_AN_minus[i], y_AN_minus[i]);
		    AN_err_minus[i] = ohfeAN_minus->GetErrorYhigh(i);
		    
		    eANtheory_minus_KG_d0bar[j]->GetPoint(i, x_AN_theory_minus_KG_d0bar[i], y_AN_theory_minus_KG_d0bar[i]);
		    eANtheory_minus_KG_dminus[j]->GetPoint(i, x_AN_theory_minus_KG_dminus[i], y_AN_theory_minus_KG_dminus[i]);		 
		    y_AN_theory_minus_KG[i] = h_antid0fracs->GetBinContent(i+1)*y_AN_theory_minus_KG_d0bar[i] + (1-h_antid0fracs->GetBinContent(i+1))*y_AN_theory_minus_KG_dminus[i];		   		        
		    chi2_minus_sig_KG[j] += pow(y_AN_minus[i] - y_AN_theory_minus_KG[i],2)/pow(AN_err_minus[i],2);	
		    eANtheory_minus_KG[j]->SetPoint(i, x_AN_theory_minus_KG_d0bar[i], y_AN_theory_minus_KG[i]);		     
		    
		    eANtheory_minus_KGp_d0bar[j]->GetPoint(i, x_AN_theory_minus_KGp_d0bar[i], y_AN_theory_minus_KGp_d0bar[i]);
		    eANtheory_minus_KGp_dminus[j]->GetPoint(i, x_AN_theory_minus_KGp_dminus[i], y_AN_theory_minus_KGp_dminus[i]);		    
		    y_AN_theory_minus_KGp[i] = h_antid0fracs->GetBinContent(i+1)*y_AN_theory_minus_KGp_d0bar[i] + (1-h_antid0fracs->GetBinContent(i+1))*y_AN_theory_minus_KGp_dminus[i];		   		    
		    chi2_minus_sig_KGp[j] += pow(y_AN_minus[i] - y_AN_theory_minus_KGp[i],2)/pow(AN_err_minus[i],2);	  
		    eANtheory_minus_KGp[j]->SetPoint(i, x_AN_theory_minus_KGp_d0bar[i], y_AN_theory_minus_KGp[i]);		    		      
	    } 
	    chi2_sig_KG[j] = chi2_plus_sig_KG[j] + chi2_minus_sig_KG[j]; 
	    chi2_sig_KGp[j] = chi2_plus_sig_KGp[j] + chi2_minus_sig_KGp[j]; 	    
		  
	    chi2_vec_KG.push_back(chi2_sig_KG[j]);	    
	    chi2_plus_vec_KG.push_back(chi2_plus_sig_KG[j]);	    
	    chi2_minus_vec_KG.push_back(chi2_minus_sig_KG[j]);	 

	    chi2_vec_KGp.push_back(chi2_sig_KGp[j]);	    
	    chi2_plus_vec_KGp.push_back(chi2_plus_sig_KGp[j]);	    
	    chi2_minus_vec_KGp.push_back(chi2_minus_sig_KGp[j]);
	  //cout << KGs[j] << " " << KGps[j] << endl;  
    if (j%10==0) 
    {
      candiv_plus->cd(counter+1);
      ostringstream tempstr;
      tempstr << "K_{G} = " << KGs[j] <<  ", K_{G}' = " << KGps[j];
      cout << tempstr.str() << endl;
      TString title;
      title = tempstr.str();
      eANtheory_plus_KG[j]->SetTitle(title);   
      eANtheory_plus_KG[j]->GetXaxis()->SetTitle("p_{T} [GeV/c]"); 
      eANtheory_plus_KG[j]->GetYaxis()->SetTitle("A_{N}");               
      ohfeAN_plus->SetMarkerStyle(25);                      
      eANtheory_plus_KG[j]->SetMarkerStyle(kCircle);
      eANtheory_plus_KG[j]->GetYaxis()->SetRangeUser(-0.14, 0.14);          
      eANtheory_plus_KG[j]->Draw("AP");   
      eANtheory_plus_KGp[j]->SetMarkerStyle(3);
      eANtheory_plus_KGp[j]->Draw("P");                  
      ohfeAN_plus->Draw("P"); 
      TLegend *leg_plus = new TLegend(0.65, 0.65, 0.9,  0.9);
      leg_plus->AddEntry(ohfeAN_plus, "e^{+} data","ep");         
      leg_plus->AddEntry(eANtheory_plus_KG[j], "e^{+} theory K_{G}","ep");  
      leg_plus->AddEntry(eANtheory_plus_KGp[j], "e^{+} theory K_{G}'","ep");          
      leg_plus->Draw();     
      
      candiv_minus->cd(counter+1);
      eANtheory_minus_KG[j]->SetTitle(title);    
      eANtheory_minus_KG[j]->GetXaxis()->SetTitle("p_{T} [GeV/c]"); 
      eANtheory_minus_KG[j]->GetYaxis()->SetTitle("A_{N}");           
      ohfeAN_minus->SetTitle("");        
      ohfeAN_minus->SetMarkerStyle(25);                      
      eANtheory_minus_KG[j]->SetMarkerStyle(kCircle);
      eANtheory_minus_KG[j]->GetYaxis()->SetRangeUser(-0.14, 0.14);          
      eANtheory_minus_KG[j]->Draw("AP");   
      eANtheory_minus_KGp[j]->SetMarkerStyle(3);
      eANtheory_minus_KGp[j]->Draw("P");                  
      ohfeAN_minus->Draw("P"); 
      TLegend *leg_minus = new TLegend(0.65, 0.65, 0.9,  0.9);
      leg_minus->AddEntry(ohfeAN_minus, "e^{-} data","ep");         
      leg_minus->AddEntry(eANtheory_minus_KG[j], "e^{-} theory K_{G}","ep");  
      leg_minus->AddEntry(eANtheory_minus_KGp[j], "e^{-} theory K_{G}'","ep");          
      leg_minus->Draw();                         
      counter++;   

    }   	    
	
		  if(chi2_sig_KG[j] - 8.21973 < 0.0001) { cout << " (+/-) KG : " << KGs[j] << endl; }	 	     
		    
		  if(chi2_sig_KGp[j] - 7.07002 < 0.0001) { cout << " (+/-) KGp : " << KGps[j] << endl; }	 	    
	 
		  // 2 sigma CL  //  
		  // +- // 
		  if( (chi2_sig_KG[j] - 8.21973 < 4.) && (chi2_sig_KG[j] - 8.21973 > 1.) ) 
		  {    
		    KGs_2sigma.push_back(KGs[j]);
		  }	
		      		    	    
		  // 1 sigma CL//   
      // +- //
		  if(chi2_sig_KG[j] - 8.21973 < 1.)
		  { 
		    //cout << " KG : " << KGs[j] << endl;
		    KGs_1sigma.push_back(KGs[j]);
		  }	   	  		  
		  // 2 sigma CL  //  
		  // +- // 
		  if( (chi2_sig_KGp[j] - 7.10078 < 4.) && (chi2_sig_KGp[j] - 7.10078 > 1.) ) 
	    {    
	      KGps_2sigma.push_back(KGps[j]);
	    }	
	     		    	    
	    // 1 sigma CL//   
      // +- //
	    if(chi2_sig_KGp[j] - 7.10078 < 1.)
	    { 
	      //cout << " KGp : " << KGps[j] << endl;
	      KGps_1sigma.push_back(KGps[j]);
		  }	   		    	 	     
  }
  
  candiv_plus->SaveAs("images/theoryCompare/theoryComparePlus_KGKGp.png");
  candiv_minus->SaveAs("images/theoryCompare/theoryCompareMinus_KGKGp.png");   
  
	TFile *outfile = new TFile("lamParamChi2MinZoomed_KGKGp.root", "RECREATE");
	outfile->cd();				      
    
  double chi2_min_KG = *min_element(chi2_vec_KG.begin(), chi2_vec_KG.end());
  double chi2_plus_min_KG = *min_element(chi2_plus_vec_KG.begin(), chi2_plus_vec_KG.end());	
  double chi2_minus_min_KG = *min_element(chi2_minus_vec_KG.begin(), chi2_minus_vec_KG.end()); 	
  
  double chi2_min_KGp = *min_element(chi2_vec_KGp.begin(), chi2_vec_KGp.end());
  double chi2_plus_min_KGp = *min_element(chi2_plus_vec_KGp.begin(), chi2_plus_vec_KGp.end());	
  double chi2_minus_min_KGp = *min_element(chi2_minus_vec_KGp.begin(), chi2_minus_vec_KGp.end()); 	


  for (int j=0; j<Npar; ++j)
  {
    dchi2_sig_KG[j] = chi2_sig_KG[j] - chi2_min_KG;
    dchi2_plus_sig_KG[j] = chi2_plus_sig_KG[j] - chi2_plus_min_KG;    
    dchi2_minus_sig_KG[j] = chi2_minus_sig_KG[j] - chi2_minus_min_KG;        
    
    dchi2_sig_KGp[j] = chi2_sig_KGp[j] - chi2_min_KGp;
    dchi2_plus_sig_KGp[j] = chi2_plus_sig_KGp[j] - chi2_plus_min_KGp;    
    dchi2_minus_sig_KGp[j] = chi2_minus_sig_KGp[j] - chi2_minus_min_KGp;      
  }
  
  const int KGs_1sig_count = KGs_1sigma.size();  
  double KGs_1sig_ys[KGs_1sig_count];
  double KGs_1sig_array[KGs_1sig_count];
  for (int i=0; i<KGs_1sigma.size(); ++i)
  {
    KGs_1sig_array[i] = KGs_1sigma.at(i);
    KGs_1sig_ys[i] = 0.;
  }
  const int KGs_2sig_count = KGs_2sigma.size();    
  double KGs_2sig_ys[KGs_2sig_count];  
  double KGs_2sig_array[KGs_2sig_count];
  for (int i=0; i<KGs_2sigma.size(); ++i)
  {
    KGs_2sig_array[i] = KGs_2sigma.at(i);
    KGs_2sig_ys[i] = 0.;
  }  
  const int KGps_1sig_count = KGps_1sigma.size(); 
  double KGps_1sig_ys[KGps_1sig_count];       
  double KGps_1sig_array[KGps_1sig_count];
  for (int i=0; i<KGps_1sigma.size(); ++i)
  {
    KGps_1sig_array[i] = KGps_1sigma.at(i);
    KGps_1sig_ys[i] = 0.;   
  }  
  const int KGps_2sig_count = KGps_2sigma.size();   
  double KGps_2sig_ys[KGps_2sig_count];     
  double KGps_2sig_array[KGps_2sig_count];  
  for (int i=0; i<KGps_2sigma.size(); ++i)
  {
    KGps_2sig_array[i] = KGps_2sigma.at(i);
    KGps_2sig_ys[i] = 0.;       
  }
  TGraph *KGs_chi2_min = new TGraph();
  KGs_chi2_min->SetPoint(0,0.0006, 0.);
  KGs_chi2_min->SetMarkerStyle(20);
  TGraph *KGps_chi2_min = new TGraph();
  KGps_chi2_min->SetPoint(0,0.00025, 0.);  
  KGps_chi2_min->SetMarkerStyle(20);  
    
  TGraph *KGs_1sig_graph = new TGraph(KGs_1sigma.size(), KGs_1sig_array, KGs_1sig_ys);
  KGs_1sig_graph->SetMarkerStyle(5);
  KGs_1sig_graph->SetMarkerColor(kBlue);   
  
  TGraph *KGs_2sig_graph = new TGraph(KGs_2sigma.size(), KGs_2sig_array, KGs_2sig_ys); 
  KGs_2sig_graph->SetMarkerStyle(2);
  KGs_2sig_graph->SetMarkerColor(kRed); 
    
  TGraph *KGps_1sig_graph = new TGraph(KGps_1sigma.size(), KGps_1sig_array, KGps_1sig_ys);
  KGps_1sig_graph->SetMarkerStyle(5);
  KGps_1sig_graph->SetMarkerColor(kBlue); 
    
  TGraph *KGps_2sig_graph = new TGraph(KGps_2sigma.size(), KGps_2sig_array, KGps_2sig_ys); 
  KGps_2sig_graph->SetMarkerStyle(2);
  KGps_2sig_graph->SetMarkerColor(kRed); 
  
  TCanvas *can_chi2 = new TCanvas();
  can_chi2->Divide(2,3);
  can_chi2->cd(1);
  TGraph *chi2gr_KG = new TGraph(chi2_vec_KG.size(), KGs, dchi2_sig_KG);
  chi2gr_KG->SetTitle("(+/-)");
  chi2gr_KG->GetXaxis()->SetTitle("K_{G}");
  chi2gr_KG->GetYaxis()->SetTitle("#Delta #chi^{2}");  
  chi2gr_KG->Draw();
  
  can_chi2->cd(2);
  TGraph *chi2gr_KGp = new TGraph(chi2_vec_KGp.size(), KGps, dchi2_sig_KGp);
  chi2gr_KGp->SetTitle("(+/-)");
  chi2gr_KGp->GetXaxis()->SetTitle("K_{G}'");
  chi2gr_KGp->GetYaxis()->SetTitle("#Delta #chi^{2}");  
  chi2gr_KGp->Draw();    
  
  can_chi2->cd(3);
  TGraph *chi2gr_plus_KG = new TGraph(chi2_plus_vec_KG.size(), KGs, dchi2_plus_sig_KG);
  chi2gr_plus_KG->SetTitle("(+)");
  chi2gr_plus_KG->GetXaxis()->SetTitle("K_{G}");
  chi2gr_plus_KG->GetYaxis()->SetTitle("#Delta #chi^{2}");  
  chi2gr_plus_KG->Draw();      
  
  can_chi2->cd(4);
  TGraph *chi2gr_plus_KGp = new TGraph(chi2_plus_vec_KGp.size(), KGps, dchi2_plus_sig_KGp);
  chi2gr_plus_KGp->SetTitle("(+)");
  chi2gr_plus_KGp->GetXaxis()->SetTitle("K_{G}'");
  chi2gr_plus_KGp->GetYaxis()->SetTitle("#Delta #chi^{2}");  
  chi2gr_plus_KGp->Draw();    
  
  can_chi2->cd(5);
  TGraph *chi2gr_minus_KG = new TGraph(chi2_minus_vec_KG.size(), KGs, dchi2_minus_sig_KG);
  chi2gr_minus_KG->SetTitle("(-)");
  chi2gr_minus_KG->GetXaxis()->SetTitle("K_{G}");
  chi2gr_minus_KG->GetYaxis()->SetTitle("#Delta #chi^{2}");  
  chi2gr_minus_KG->Draw();      
  
  can_chi2->cd(6);
  TGraph *chi2gr_minus_KGp = new TGraph(chi2_minus_vec_KGp.size(), KGps, dchi2_minus_sig_KGp);
  chi2gr_minus_KGp->SetTitle("(-)");
  chi2gr_minus_KGp->GetXaxis()->SetTitle("K_{G}'");
  chi2gr_minus_KGp->GetYaxis()->SetTitle("#Delta #chi^{2}");  
  chi2gr_minus_KGp->Draw();    
  
  can_chi2->SaveAs("images/theoryCompare/KGKGp_chi2.png");


  TCanvas *can = new TCanvas();
  can->Divide(1,2);
  can->cd(1);
  KGs_1sig_graph->GetYaxis()->SetRangeUser(-1., 1.);
  KGs_1sig_graph->GetXaxis()->SetLimits(-0.005, 0.005);    
  KGs_1sig_graph->SetTitle("");
  KGs_1sig_graph->GetXaxis()->SetTitle("K_{G}");
  KGs_1sig_graph->GetXaxis()->SetTitleSize(0.06);  
  KGs_1sig_graph->GetXaxis()->SetLabelSize(0.045);    
  KGs_1sig_graph->GetXaxis()->SetTitleOffset(0.75);    
  KGs_1sig_graph->Draw("AP");
  KGs_2sig_graph->Draw("P same");
  KGs_chi2_min->Draw("P same");
  
  can->cd(2);  
  KGps_1sig_graph->GetYaxis()->SetRangeUser(-1., 1.);
  KGps_1sig_graph->GetXaxis()->SetLimits(-0.00025, 0.00075);    
  KGps_1sig_graph->SetTitle("");
  KGps_1sig_graph->GetXaxis()->SetTitle("K_{G}'");  
  KGps_1sig_graph->GetXaxis()->SetTitleSize(0.06);  
  KGps_1sig_graph->GetXaxis()->SetLabelSize(0.045);    
  KGps_1sig_graph->GetXaxis()->SetTitleOffset(0.75);     
  KGps_1sig_graph->Draw("AP");
  KGps_2sig_graph->Draw("P same");
  KGps_chi2_min->Draw("P same");  
  
  can->SaveAs("images/theoryCompare/KGKGp_CL.png");

	cout << " KG : chi2 min (+/-) : " << chi2_min_KG  << " chi2 min (+) : " << chi2_plus_min_KG << " -- chi2 min (-) : " << chi2_minus_min_KG << endl;  	
	cout << " KGp : chi2 min (+/-) : " << chi2_min_KGp  << " chi2 min (+) : " << chi2_plus_min_KGp << " -- chi2 min (-) : " << chi2_minus_min_KGp << endl;  	
	
  outfile->Write();
  outfile->Delete();
	
}
