#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <pair>
#include "../../Constants.h"

void lamParamChi2MinZoomed_variationswSysDenom(int num = 1)
{
	bool prelim = 1;
	bool theory_compare = 0;
	int verbosity = -99;
	const int nbins = NUM_VALUE_BINS;
	double x_AN_plus[nbins], y_AN_plus[nbins], x_AN_minus[nbins], y_AN_minus[nbins];
	double x_AN_theory_plus[nbins], y_AN_theory_plus[nbins], x_AN_theory_minus[nbins], y_AN_theory_minus[nbins];
	double AN_err_plus[nbins], AN_err_minus[nbins];
	const int N_iter = 100;
  const int Npar = 41;
  double fs[Npar] = {-0.20, -0.19, -0.18, -0.17, -0.16, -0.15, -0.14, -0.13, -0.12, -0.11, -0.10, -0.09, -0.08, -0.07, -0.06, -0.05, -0.04, -0.03, -0.02, -0.01, 0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.10, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19, 0.20};
  double ds[Npar] = {-0.10, -0.09, -0.08, -0.07, -0.06, -0.05, -0.04, -0.03, -0.02, -0.01, 0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.10, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19, 0.20, 0.21, 0.22, 0.23, 0.24, 0.25, 0.26, 0.27, 0.28, 0.29, 0.30};	
  
  double sys_plus_upper[nbins] = {0.0171, 0.00926, 0.00751, 0.00905, 0.00656, 0.0075};
  double sys_plus_lower[nbins] = {0.017, 0.00922, 0.0075, 0.00903, 0.0065, 0.00584};  
  
  double sys_minus_upper[nbins] = {0.0164, 0.0121, 0.00946, 0.00715, 0.0063, 0.0066};
  double sys_minus_lower[nbins] = {0.0161, 0.0116, 0.00943, 0.00709, 0.0063, 0.00524};    
     
	TFile *infile_data_plus = TFile::Open("../dataFiles/bgCorrected_ohfe_AN_plus.root");
	TFile *infile_data_minus = TFile::Open("../dataFiles/bgCorrected_ohfe_AN_minus.root");
	TFile *infile_theory_plus = TFile::Open("dataFiles/d0DecayAsymmetry_fitted_10B_weightsLoopZoomed.root");
  TFile *infile_theory_minus = TFile::Open("dataFiles/antid0DecayAsymmetry_fitted_10B_weightsLoopZoomed.root");
	TGraphAsymmErrors *ohfeAN_plus = (TGraphAsymmErrors*)infile_data_plus->Get("ohfeAN");
	TGraphAsymmErrors *ohfeAN_minus = (TGraphAsymmErrors*)infile_data_minus->Get("ohfeAN");
  TGraph *eANtheory_plus[Npar][Npar], *eANtheory_minus[Npar][Npar];
  double chi2_plus_sig[Npar][Npar];  
  double chi2_minus_sig[Npar][Npar];
  double chi2_sig[Npar][Npar];    
  double chi2_min[N_iter];    
  double chi2_plus_min[N_iter];
  double chi2_minus_min[N_iter];          
	TString graphname = "";  
  for (int j=0; j<Npar; ++j)
  {
    for (int k=0; k<Npar; ++k)
	  {
	    graphname = "eAN";
	    ostringstream ss;
	    ss<<"_"<<fs[j]<<"_"<<ds[k];
	    graphname += ss.str();
	    string graph_name = graphname;
	    replace(graph_name.begin(), graph_name.end(), '.', 'p');
	    replace(graph_name.begin(), graph_name.end(), '-', 'm');
	    graphname = graph_name;
      eANtheory_plus[j][k] = (TGraph*)infile_theory_plus->Get(graphname);
      eANtheory_minus[j][k] = (TGraph*)infile_theory_minus->Get(graphname);
      chi2_plus_sig[j][k] = 0;
      chi2_minus_sig[j][k] = 0;      
      
	  }
  }  
  
  vector<double> chi2_vec;  
  vector<double> chi2_plus_vec; 
  vector<double> chi2_minus_vec;
  vector<pair<double,pair<double,double>>> bestfitpars;
  vector<pair<double,pair<double,double>>> bestfitpars_plus;
  vector<pair<double,pair<double,double>>> bestfitpars_minus;
  
  for (int l=0; l<N_iter; ++l)
  { 
    cout << "l : " << l << endl;
    chi2_min[l] = 0;
    chi2_plus_min[l] = 0;	
    chi2_minus_min[l] = 0;
    chi2_vec.clear();
    chi2_plus_vec.clear();
    chi2_minus_vec.clear();     
    for (int j=0; j<Npar; ++j)
    {
      for (int k=0; k<Npar; ++k)
      {
        chi2_plus_sig[j][k] = 0;
        chi2_minus_sig[j][k] = 0; 
        chi2_sig[j][k] = 0;                       
    	  for (int i=0; i<nbins; ++i)
	      {	
	        TUUID uniq;
	        //cout << uniq.Hash() << endl;
	        TRandom3 rando(uniq.Hash());       
	        // To do, include systematics as well somehow... // 
		      ohfeAN_plus->GetPoint(i, x_AN_plus[i], y_AN_plus[i]);
		      AN_err_plus[i] = ohfeAN_plus->GetErrorYhigh(i);
		      eANtheory_plus[j][k]->GetPoint(i, x_AN_theory_plus[i], y_AN_theory_plus[i]);
		      double error_plus = sqrt(pow(AN_err_plus[i],2) + pow(max(sys_plus_upper[i],sys_plus_lower[i]),2));		      		      
		      double resamp_plus = rando.Gaus(y_AN_plus[i], error_plus);
		      //cout << max(sys_plus_upper[i],sys_plus_lower[i]) << endl;
		      

		      
		      chi2_plus_sig[j][k] += pow(resamp_plus - y_AN_theory_plus[i],2)/pow(error_plus,2);	
		    
		      ohfeAN_minus->GetPoint(i, x_AN_minus[i], y_AN_minus[i]);
		      AN_err_minus[i] = ohfeAN_minus->GetErrorYhigh(i);
		      eANtheory_minus[j][k]->GetPoint(i, x_AN_theory_minus[i], y_AN_theory_minus[i]);
		      double error_minus = sqrt(pow(AN_err_minus[i],2) + pow(max(sys_minus_upper[i],sys_minus_lower[i]),2));		      		      
		      double resamp_minus = rando.Gaus(y_AN_minus[i], error_minus);	
		      
		      //cout << max(sys_minus_upper[i],sys_minus_lower[i]) << endl;		      
		      chi2_minus_sig[j][k] += pow(resamp_minus - y_AN_theory_minus[i],2)/pow(error_minus,2);
		      
		      //cout << " resampled value (+) : " << resamp_plus << " true value (+) : " << y_AN_plus[i] << endl;
		      //cout << " resampled value (+) : " << resamp_plus << " true value (+) : " << y_AN_plus[i] << endl;		    
	      } 
	      
		    chi2_sig[j][k] = chi2_plus_sig[j][k] + chi2_minus_sig[j][k];			    
		  
	      chi2_vec.push_back(chi2_sig[j][k]);	  
	      pair<double,double> lams = make_pair(fs[j],ds[k]);
	      pair<double,pair<double,double>> chi2lams = make_pair(chi2_sig[j][k], lams);
	      bestfitpars.push_back(chi2lams); 
	      chi2_plus_vec.push_back(chi2_plus_sig[j][k]);	   
	      pair<double,pair<double,double>> chi2lams_plus = make_pair(chi2_plus_sig[j][k], lams);
	      bestfitpars_plus.push_back(chi2lams_plus); 	       
	      chi2_minus_vec.push_back(chi2_minus_sig[j][k]);	
	      pair<double,pair<double,double>> chi2lams_minus = make_pair(chi2_minus_sig[j][k], lams);
	      bestfitpars_minus.push_back(chi2lams_minus); 		        
	      
      }
    }
    chi2_min[l] = *min_element(chi2_vec.begin(), chi2_vec.end());
    chi2_plus_min[l] = *min_element(chi2_plus_vec.begin(), chi2_plus_vec.end());	
    chi2_minus_min[l] = *min_element(chi2_minus_vec.begin(), chi2_minus_vec.end());    
  }			

  TGraph *lamparam = new TGraph();    
  TH2D *lamparamhist = new TH2D("lamparam", "", 41, -0.2, -0.2, 41, -0.1, 0.3);                              
  TGraph *lamparam_plus = new TGraph();         
  TGraph *lamparam_minus = new TGraph();     
  int npoint = 0; int npoint_plus = 0; int npoint_minus = 0;         	
  for (int l=0; l<N_iter; ++l)
  {
    for (int i=0; i<bestfitpars.size(); ++i)
    {
      if (bestfitpars.at(i).first == chi2_min[l]) 
      {
        cout << " l : " << l << " chi2 min : " << bestfitpars.at(i).first << " f : " << bestfitpars.at(i).second.first << " d : " <<  bestfitpars.at(i).second.second << endl;
        lamparam->SetPoint(npoint, bestfitpars.at(i).second.first, bestfitpars.at(i).second.second); 
        npoint++; 
        lamparamhist->Fill(bestfitpars.at(i).second.first, bestfitpars.at(i).second.second);        
         
      } 
      if (bestfitpars_plus.at(i).first == chi2_plus_min[l]) 
      {
        lamparam_plus->SetPoint(npoint_plus, bestfitpars_plus.at(i).second.first, bestfitpars_plus.at(i).second.second);  
        npoint_plus++;
         
      }       
      if (bestfitpars_minus.at(i).first == chi2_minus_min[l]) 
      {
        lamparam_minus->SetPoint(npoint_minus, bestfitpars_minus.at(i).second.first, bestfitpars_minus.at(i).second.second);  
        npoint_minus++;
         
      }           
    }
  }
  
  TString outfilename = "dataFiles/lamParamChi2Zoomed_variations_wSysDenom_";
  outfilename += N_iter;
  outfilename += "_";
  outfilename += num;
  outfilename += ".root";
  TFile *outfile = new TFile(outfilename, "RECREATE");
  TCanvas *grCan = new TCanvas();
  lamparam->GetYaxis()->SetRangeUser(-0.1,0.3);
  lamparam->GetXaxis()->SetLimits(-0.2,0.2); 
  lamparam->SetMarkerStyle(25);
  lamparam->SetMarkerColor(kViolet+1);  
  lamparam->SetTitle("(+/-);#lambda_{f} (GeV);#lambda_{d} (GeV)");
  lamparam->SetName("lamparam");    
  lamparam->Draw("AP");
  TString imgname = "images/theoryCompare/LamParamMinZoomed_variations_wSysDenom_";
  imgname += N_iter;
  imgname += "_";
  imgname += num;  
  imgname += ".png";
  grCan->SaveAs(imgname);
  
  TCanvas *histCan = new TCanvas();
  TString outfilename = "dataFiles/lamParamChi2Zoomed_variations_wSys_hist";
  outfilename += N_iter;
  outfilename += "_";
  outfilename += num;
  outfilename += ".root";   
  lamparamhist->GetYaxis()->SetRangeUser(-0.1,0.3);
  lamparamhist->GetXaxis()->SetLimits(-0.2,0.2);   
  lamparamhist->Draw("COLZ");
  TString imgname = "images/theoryCompare/LamParamMinZoomed_variations_wSys_hist";
  imgname += N_iter;
  imgname += "_";
  imgname += num;  
  imgname += ".png";
  histCan->SaveAs(imgname);        

  TCanvas *grCan_plus = new TCanvas();
  lamparam_plus->GetYaxis()->SetRangeUser(-0.1,0.3);
  lamparam_plus->GetXaxis()->SetLimits(-0.2,0.2); 
  lamparam_plus->SetMarkerStyle(25);
  lamparam_plus->SetMarkerColor(kViolet+1);  
  lamparam_plus->SetTitle("(+);#lambda_{f} (GeV);#lambda_{d} (GeV)");
  lamparam_plus->SetName("lamparam_plus");    
  lamparam_plus->Draw("AP");
  imgname = "images/theoryCompare/LamParamMinZoomed_plus_variations_wSysDenom_";
  imgname += N_iter;
  imgname += "_";
  imgname += num;  
  imgname += ".png";
  grCan_plus->SaveAs(imgname);  
  
  TCanvas *grCan_minus = new TCanvas();
  lamparam_minus->GetYaxis()->SetRangeUser(-0.1,0.3);
  lamparam_minus->GetXaxis()->SetLimits(-0.2,0.2); 
  lamparam_minus->SetMarkerStyle(25);
  lamparam_minus->SetMarkerColor(kViolet+1);  
  lamparam_minus->SetTitle("(-);#lambda_{f} (GeV);#lambda_{d} (GeV)");  
  lamparam_minus->SetName("lamparam_minus");      
  lamparam_minus->Draw("AP");
  imgname = "images/theoryCompare/LamParamMinZoomed_minus_variations_wSysDenom_";
  imgname += N_iter;
  imgname += "_";
  imgname += num;  
  imgname += ".png";  
  grCan_minus->SaveAs(imgname);   
  lamparam->Write();
  lamparam_plus->Write();
  lamparam_minus->Write();    
  outfile->Write();
  outfile->Close();
	
}
