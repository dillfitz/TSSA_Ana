#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "../../Constants.h"

void lamParamChi2Min()
{
	bool prelim = 1;
	bool theory_compare = 0;
	int verbosity = -99;
	const int nbins = NUM_VALUE_BINS;
	double x_AN_plus[nbins], y_AN_plus[nbins], x_AN_minus[nbins], y_AN_minus[nbins];
	double x_AN_theory_plus[nbins], y_AN_theory_plus[nbins], x_AN_theory_minus[nbins], y_AN_theory_minus[nbins];
	double AN_err_plus[nbins], AN_err_minus[nbins];
	
	const int Npar = 43;
  double fs[Npar] = {-1., -0.95, -0.9, -0.85, -0.8, -0.75, -0.7, -0.65, -0.6, -0.55, -0.5, -0.45, -0.4, -0.35, -0.3, -0.25, -0.2, -0.15, -0.1, -0.07, -0.05, 0.0, 0.05, 0.07, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1};
  double ds[Npar] = {-1., -0.95, -0.9, -0.85, -0.8, -0.75, -0.7, -0.65, -0.6, -0.55, -0.5, -0.45, -0.4, -0.35, -0.3, -0.25, -0.2, -0.15, -0.1, -0.07, -0.05, 0.0, 0.05, 0.07, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1}; 
  TString par_labels[Npar] = {"m1", "m0p95", "m0p9", "m0p85", "m0p8", "m0p75", "m0p7", "m0p65", "m0p6", "m0p55", "m0p5", "m0p45", "m0p4", "m0p35", "m0p3", "m0p25", "m0p2", "m0p15", "m0p1", "m0p07", "m0p05", "0", "0p05", "0p07", "0p1", "0p15", "0p2", "0p25", "0p3", "0p35", "0p4", "0p45", "0p5", "0p55", "0p6", "0p65", "0p7", "0p75", "0p8", "0p85", "0p9", "0p95", "1"};
  TString par_labels_reduced[3] = {"m0p2", "0", "0p2"};  
   
 
	TFile *infile_data_plus = TFile::Open("../dataFiles/bgCorrected_ohfe_AN_plus.root");
	TFile *infile_data_minus = TFile::Open("../dataFiles/bgCorrected_ohfe_AN_minus.root");
	TFile *infile_theory_plus = TFile::Open("dataFiles/d0DecayAsymmetry_fitted_10B_weightsLoop.root");
  TFile *infile_theory_minus = TFile::Open("dataFiles/antid0DecayAsymmetry_fitted_10B_weightsLoop.root");
	TGraphAsymmErrors *ohfeAN_plus = (TGraphAsymmErrors*)infile_data_plus->Get("ohfeAN");
	TGraphAsymmErrors *ohfeAN_minus = (TGraphAsymmErrors*)infile_data_minus->Get("ohfeAN");
  TGraph *eANtheory_plus[Npar][Npar], *eANtheory_minus[Npar][Npar];
  double chi2_plus_sig[Npar][Npar];  
  double chi2_minus_sig[Npar][Npar];
  double chi2_sig[Npar][Npar];        
  TCanvas *can[Npar][Npar];  
	TString graphname = "";  
	TCanvas *candiv_plus = new TCanvas();
	candiv_plus->Divide(3,3);
	TCanvas *candiv_minus = new TCanvas();
	candiv_minus->Divide(3,3);	
	int counter = 0;
  for (int j=0; j<Npar; ++j)
  {
    for (int k=0; k<Npar; ++k)
	  {
	    graphname = "eAN";
	    graphname += "_";
      graphname += par_labels[j];
      graphname += "_";
      graphname += par_labels[k]; 
      eANtheory_plus[j][k] = (TGraph*)infile_theory_plus->Get(graphname);
      eANtheory_minus[j][k] = (TGraph*)infile_theory_minus->Get(graphname);
      chi2_plus_sig[j][k] = 0;
      chi2_minus_sig[j][k] = 0;
      if (/*par_labels[j] == "m1" || par_labels[j] == "m0p8" || par_labels[j] == "m0p6" || par_labels[j] == "m0p4" ||*/ par_labels[j] == "m0p2" || par_labels[j] == "0" /*|| par_labels[j] == "1" || par_labels[j] == "0p8" || par_labels[j] == "0p6" || par_labels[j] == "0p4" */|| par_labels[j] == "0p2" )
      {
        if (/*par_labels[k] == "m1" || par_labels[k] == "m0p8" || par_labels[k] == "m0p6" || par_labels[k] == "m0p4" || */par_labels[k] == "m0p2" || par_labels[k] == "0" /*|| par_labels[k] == "1" || par_labels[k] == "0p8" || par_labels[k] == "0p6" || par_labels[k] == "0p4" */|| par_labels[k] == "0p2" )
        {      
          counter++;
          ostringstream ss;
	        ss<<"("<<fs[j]<<" GeV , "<<ds[k] << " GeV)";
          candiv_plus->cd(counter);
          TString title = "(#lambda_{f}, #lambda_{d}) = ";
          title += ss.str();
          eANtheory_plus[j][k]->SetTitle(title);
          ohfeAN_plus->SetTitle(title);        
          ohfeAN_plus->SetMarkerStyle(25);                      
          eANtheory_plus[j][k]->SetMarkerStyle(kCircle);
          eANtheory_plus[j][k]->GetYaxis()->SetRangeUser(-0.06, 0.08);          
          eANtheory_plus[j][k]->Draw("AP");          
          ohfeAN_plus->Draw("P"); 
          TLegend *leg_plus = new TLegend(0.65, 0.65, 0.9,  0.9);
          leg_plus->AddEntry(ohfeAN_plus, "e^{+} data","ep");         
          leg_plus->AddEntry(eANtheory_plus[j][k], "e^{+} theory","ep");   
          leg_plus->Draw();                
       
          candiv_minus->cd(counter);
          eANtheory_minus[j][k]->SetTitle(title);
          ohfeAN_minus->SetTitle(title);        
          ohfeAN_minus->SetMarkerStyle(25);                      
          eANtheory_minus[j][k]->SetMarkerStyle(kCircle);
          eANtheory_minus[j][k]->GetYaxis()->SetRangeUser(-0.06, 0.08);          
          eANtheory_minus[j][k]->Draw("AP");                 
          ohfeAN_minus->Draw("P"); 
          TLegend *leg_minus = new TLegend(0.65, 0.65, 0.9,  0.9);
          leg_minus->AddEntry(ohfeAN_plus, "e^{-} data","ep");         
          leg_minus->AddEntry(eANtheory_minus[j][k], "e^{-} theory","ep");   
          leg_minus->Draw();       
        }
      }  
	  }
  }  
  candiv_plus->SaveAs("images/theoryCompare/theoryComparePlus.png");
  candiv_minus->SaveAs("images/theoryCompare/theoryCompareMinus.png"); 
  vector<double> chi2_vec;  
  vector<double> chi2_plus_vec; 
  vector<double> chi2_minus_vec;
  TGraph2D *chi2gr = new TGraph2D(); 
  TGraph2D *chi2gr_min = new TGraph2D();   
  TGraph2D *chi2gr_1sig = new TGraph2D();
  TGraph *chi2gr_min1D = new TGraph();   
  TGraph *chi2gr_2sig1D = new TGraph();    
  TGraph *chi2gr_1sig1D = new TGraph();   
  TGraph *chi2gr_3sig1D = new TGraph();             
  chi2gr->SetTitle("#chi^{2}_{e^{+/-}}(#lambda_{f}, #lambda_{d}); #lambda_{f} [GeV]; #lambda_{d} [GeV]");  
  chi2gr_2sig1D->SetTitle("; #lambda_{f} [GeV]; #lambda_{d} [GeV]"); 	 
  chi2gr_2sig1D->GetXaxis()->SetTitleSize(3.);    
  chi2gr_2sig1D->GetYaxis()->SetTitleSize(3.);                  
  TGraph2D *chi2gr_plus = new TGraph2D(); 
  TGraph2D *chi2gr_plus_min = new TGraph2D();   
  TGraph2D *chi2gr_plus_1sig = new TGraph2D();    
  TGraph *chi2gr_plus_min1D = new TGraph();   
  TGraph *chi2gr_plus_2sig1D = new TGraph();  
  TGraph *chi2gr_plus_1sig1D = new TGraph();            
  chi2gr_plus->SetTitle("#chi^{2}_{e^{+}}(#lambda_{f}, #lambda_{d}); #lambda_{f} [GeV]; #lambda_{d} [GeV]");    
  chi2gr_plus_2sig1D->SetTitle("; #lambda_{f} [GeV]; #lambda_{d} [GeV]");	 
  chi2gr_plus_2sig1D->GetXaxis()->SetTitleSize(3.);    
  chi2gr_plus_2sig1D->GetYaxis()->SetTitleSize(3.);            
  TGraph2D *chi2gr_minus = new TGraph2D();   
  TGraph2D *chi2gr_minus_min = new TGraph2D();     
  TGraph2D *chi2gr_minus_1sig = new TGraph2D();
  TGraph *chi2gr_minus_min1D = new TGraph();   
  TGraph *chi2gr_minus_2sig1D = new TGraph(); 
  TGraph *chi2gr_minus_1sig1D = new TGraph();               
	chi2gr_minus->SetTitle("#chi^{2}_{e^{-}}(#lambda_{f}, #lambda_{d}); #lambda_{f} [GeV]; #lambda_{d} [GeV]");  
  chi2gr_minus_2sig1D->SetTitle("; #lambda_{f} [GeV]; #lambda_{d} [GeV]");	
  chi2gr_minus_2sig1D->GetXaxis()->SetTitleSize(3.);    
  chi2gr_minus_2sig1D->GetYaxis()->SetTitleSize(3.);        
  int npoint = 0; int npoint_plus = 0; int npoint_minus = 0; int npoint_pm = 0; 
  int npoint_70_pm = 0; int npoint_70_plus = 0; int npoint_70_minus = 0; int npoint_3sig = 0;
  for (int j=0; j<Npar; ++j)
  {
    for (int k=0; k<Npar; ++k)
    {
    	for (int i=0; i<nbins; ++i)
	    {		
		    ohfeAN_plus->GetPoint(i, x_AN_plus[i], y_AN_plus[i]);
		    AN_err_plus[i] = ohfeAN_plus->GetErrorYhigh(i);
		    eANtheory_plus[j][k]->GetPoint(i, x_AN_theory_plus[i], y_AN_theory_plus[i]);
		    chi2_plus_sig[j][k] += pow(y_AN_plus[i] - y_AN_theory_plus[i],2)/pow(AN_err_plus[i],2);	
		    
		    ohfeAN_minus->GetPoint(i, x_AN_minus[i], y_AN_minus[i]);
		    AN_err_minus[i] = ohfeAN_minus->GetErrorYhigh(i);
		    eANtheory_minus[j][k]->GetPoint(i, x_AN_theory_minus[i], y_AN_theory_minus[i]);
		    chi2_minus_sig[j][k] += pow(y_AN_minus[i] - y_AN_theory_minus[i],2)/pow(AN_err_minus[i],2);
		    
		    chi2_sig[j][k] += pow(y_AN_plus[i] - y_AN_theory_plus[i],2)/pow(AN_err_plus[i],2);			    
		    chi2_sig[j][k] += pow(y_AN_minus[i] - y_AN_theory_minus[i],2)/pow(AN_err_minus[i],2); 
	    } 
	    // Set Points for Graphs //
		  chi2gr_plus->SetPoint(npoint, fs[j],ds[k], chi2_plus_sig[j][k]);
		  chi2gr_minus->SetPoint(npoint, fs[j],ds[k], chi2_minus_sig[j][k]);
		  chi2gr->SetPoint(npoint, fs[j],ds[k], chi2_sig[j][k]);	
		  npoint++;	
		  
	    chi2_vec.push_back(chi2_sig[j][k]);	    
	    chi2_plus_vec.push_back(chi2_plus_sig[j][k]);	    
	    chi2_minus_vec.push_back(chi2_minus_sig[j][k]);	  
	    //if (chi2_sig[j][k] == 7.17578) 
	      //cout << "Min params... f : " << fs[j] << " d : " << ds[k] << endl;
		  if(chi2_sig[j][k] - 7.17578 < 0.0001) { cout << " (+/-) f : " << fs[j] << " d : " << ds[k] << endl; }	 	    
		  if(chi2_plus_sig[j][k] - 1.50191 < 0.0001) { cout << " (+) f : " << fs[j] << " d : " << ds[k] << endl; }	 
		  if(chi2_minus_sig[j][k] - 5.45783 < 0.0001) { cout << " (-) f : " << fs[j] << " d : " << ds[k] << endl; }	   		  
		  // 3 sigma CL // 
		  if(chi2_sig[j][k] - 7.10154 < 9.) 
		  { 
		    chi2gr_3sig1D->SetPoint(npoint_3sig, fs[j],ds[k]); 		    
		    npoint_3sig++;   
		    //cout << " (+/-) f : " << fs[j] << " d : " << ds[k] << endl;
		  }					  
		  // 2 sigma CL //   
		  if(chi2_sig[j][k] - 7.17578 < 4.) 
		  { 
		    chi2gr_2sig1D->SetPoint(npoint_pm, fs[j],ds[k]); 		    
		    npoint_pm++;   
		    //cout << " (+/-) f : " << fs[j] << " d : " << ds[k] << endl;
		  }	
		  // 1 sigma CL //   
		  //if(chi2_sig[j][k] - 7.17578 < 12.899) 
		  if(chi2_sig[j][k] - 7.17578 < 1.)
		  { 
		    chi2gr_1sig->SetPoint(npoint_70_pm, fs[j],ds[k], chi2_sig[j][k]); 		  
		    chi2gr_1sig1D->SetPoint(npoint_70_pm, fs[j],ds[k]); 		    
		    npoint_70_pm++;   
		    //cout << " (+/-) f : " << fs[j] << " d : " << ds[k] << endl;
		  }	   	 		     	  		  
		  // 2 sigma CL //  		  
		  //if(chi2_plus_sig[j][k] - 1.50191 < 11.070) 
		  if(chi2_plus_sig[j][k] - 1.50191 < 4.) 		  
		  { 
		    chi2gr_plus_2sig1D->SetPoint(npoint_plus, fs[j],ds[k]); 		     
		    npoint_plus++;   
		  }	 		  	
		  //if(chi2_minus_sig[j][k] - 5.45783 < 11.070) 
		  if(chi2_minus_sig[j][k] - 5.45783 < 4.) 		  
		  { 

		    chi2gr_minus_2sig1D->SetPoint(npoint_minus, fs[j],ds[k]); 		     		     
		    npoint_minus++;  
		  }	   
		  // 1 sigma CL //  		  
		  //if(chi2_plus_sig[j][k] - 1.50191 < 6.064) 
		  if(chi2_plus_sig[j][k] - 1.50191 < 1.) 		  
		  {
		    chi2gr_plus_1sig->SetPoint(npoint_70_plus, fs[j],ds[k], chi2_plus_sig[j][k]);		   
		    chi2gr_plus_1sig1D->SetPoint(npoint_70_plus, fs[j],ds[k]); 		     
		    npoint_70_plus++;   
		  }	 		  	
		  //if(chi2_minus_sig[j][k] - 5.45783 < 6.064) 
		  if(chi2_minus_sig[j][k] - 5.45783 < 1.) 		  
		  { 
		    chi2gr_minus_1sig->SetPoint(npoint_70_minus, fs[j],ds[k], chi2_minus_sig[j][k]);		  
		    chi2gr_minus_1sig1D->SetPoint(npoint_70_minus, fs[j],ds[k]); 		     		     
		    npoint_70_minus++;  
		  }			  		    	     		  	    	        
	    //cout << " chi2 (+) : " << chi2_plus_sig[j][k] << " -- chi2 (-) : " << chi2_minus_sig[j][k] << endl;
    }
  }
  
	TFile *outfile = new TFile("lamParamChi2Min.root", "RECREATE");
	outfile->cd();				      

    
  double chi2_min = *min_element(chi2_vec.begin(), chi2_vec.end());
  double chi2_plus_min = *min_element(chi2_plus_vec.begin(), chi2_plus_vec.end());	
  double chi2_minus_min = *min_element(chi2_minus_vec.begin(), chi2_minus_vec.end()); 	
  
  TCanvas *gr2DCan = new TCanvas();
  chi2gr->Draw("COLZ");
  //chi2gr_1sig->Draw("TRI2 same");
  chi2gr_min->SetPoint(0, 0.0, 0.1, chi2_min);
  chi2gr_min->SetMarkerSize(2.0);
  //Schi2gr_min->Draw("P0 same"); 
  gr2DCan->SetTheta(45.);
  gr2DCan->SetPhi(30.);
  gr2DCan->SaveAs("images/theoryCompare/LamParamMin.png");
  TCanvas *grCan = new TCanvas();
  chi2gr_2sig1D->GetYaxis()->SetRangeUser(-1.,1.);
  chi2gr_2sig1D->GetXaxis()->SetLimits(-1.,1.); 
  chi2gr_2sig1D->SetMarkerStyle(5);
  chi2gr_2sig1D->SetMarkerColor(kBlue);  
  chi2gr_2sig1D->Draw("AP");
  chi2gr_3sig1D->SetMarkerStyle(25);
  chi2gr_3sig1D->SetMarkerColor(kViolet+1);  
  chi2gr_3sig1D->Draw("P same");     
  chi2gr_1sig1D->SetMarkerStyle(2);
  chi2gr_1sig1D->SetMarkerColor(kRed);  
  chi2gr_1sig1D->Draw("P same");  
  chi2gr_min1D->SetPoint(0, 0.0, 0.1);
  chi2gr_min1D->SetMarkerStyle(8);
  chi2gr_min1D->SetMarkerSize(2.);  
  chi2gr_min1D->Draw("P same");    
  TLegend *leg = new TLegend(0.65, 0.65, 0.9, 0.9);
  leg->AddEntry(chi2gr_min1D, "Best Fit Value", "p");
  leg->AddEntry(chi2gr_1sig1D, "1 #sigma CL", "p");  
  leg->AddEntry(chi2gr_2sig1D, "2 #sigma CL", "p");
  leg->AddEntry(chi2gr_3sig1D, "3 #sigma CL", "p");  
  leg->Draw();    
  grCan->SaveAs("images/theoryCompare/LamParamMin_1d.png");  
        
  TCanvas *gr2DCan_plus = new TCanvas();
  chi2gr_plus->Draw("COLZ");
  //chi2gr_plus_1sig->Draw("TRI2 same");
  chi2gr_plus_min->SetPoint(0, 0.3, 0.95, chi2_plus_min);
  chi2gr_plus_min->SetMarkerSize(2.0);
  //chi2gr_plus_min->Draw("P0 same"); 
  gr2DCan_plus->SetTheta(45.);
  gr2DCan_plus->SetPhi(30.);
  gr2DCan_plus->SaveAs("images/theoryCompare/LamParamMinPlus.png");
  TCanvas *grCan_plus = new TCanvas();
  chi2gr_plus_2sig1D->GetYaxis()->SetRangeUser(-1.,1.);
  chi2gr_plus_2sig1D->GetXaxis()->SetLimits(-1.,1.); 
  chi2gr_plus_2sig1D->SetMarkerStyle(5);
  chi2gr_plus_2sig1D->SetMarkerColor(kBlue);  
  chi2gr_plus_2sig1D->Draw("AP");
  chi2gr_plus_1sig1D->SetMarkerStyle(2);
  chi2gr_plus_1sig1D->SetMarkerColor(kRed);  
  chi2gr_plus_1sig1D->Draw("P same");  
  chi2gr_plus_min1D->SetPoint(0, 0.3, 0.95);
  chi2gr_plus_min1D->SetMarkerStyle(8);
  chi2gr_plus_min1D->SetMarkerSize(2.);  
  chi2gr_plus_min1D->Draw("P same");    
  TLegend *leg_plus = new TLegend(0.65, 0.65, 0.9, 0.9);
  leg_plus->AddEntry(chi2gr_plus_min1D, "Best Fit Value", "p");
  leg_plus->AddEntry(chi2gr_plus_1sig1D, "1 #sigma CL", "p");  
  leg_plus->AddEntry(chi2gr_plus_2sig1D, "2 #sigma CL", "p");
  leg_plus->Draw();    
  grCan_plus->SaveAs("images/theoryCompare/LamParamMinPlus_1d.png");    

  TCanvas *gr2DCan_minus = new TCanvas();
  chi2gr_minus->Draw("COLZ");
  //chi2gr_minus_1sig->Draw("TRI2 same");  
  chi2gr_minus_min->SetPoint(0, 0.4, -1, chi2_minus_min);  
  chi2gr_minus_min->SetMarkerSize(2.0);  
  //chi2gr_minus_min->Draw("P0 same");   
  gr2DCan_minus->SetTheta(45.);
  gr2DCan_minus->SetPhi(30.);  
  gr2DCan_minus->SaveAs("images/theoryCompare/LamParamMinMinus.png");  
  TCanvas *grCan_minus = new TCanvas();
  chi2gr_minus_2sig1D->GetYaxis()->SetRangeUser(-1.,1.);
  chi2gr_minus_2sig1D->GetXaxis()->SetLimits(-1.,1.); 
  chi2gr_minus_2sig1D->SetMarkerStyle(5);
  chi2gr_minus_2sig1D->SetMarkerColor(kBlue);  
  chi2gr_minus_2sig1D->Draw("AP");
  chi2gr_minus_1sig1D->SetMarkerStyle(2);
  chi2gr_minus_1sig1D->SetMarkerColor(kRed);  
  chi2gr_minus_1sig1D->Draw("P same");  
  chi2gr_minus_min1D->SetPoint(0, 0.4, -1);
  chi2gr_minus_min1D->SetMarkerStyle(8);
  chi2gr_minus_min1D->SetMarkerSize(2.);  
  chi2gr_minus_min1D->Draw("P same");    
  TLegend *leg_minus = new TLegend(0.65, 0.65, 0.9, 0.9);
  leg_minus->AddEntry(chi2gr_minus_min1D, "Best Fit Value", "p");
  leg_minus->AddEntry(chi2gr_minus_1sig1D, "1 #sigma CL", "p");  
  leg_minus->AddEntry(chi2gr_minus_2sig1D, "2 #sigma CL", "p");
  leg_minus->Draw();      
  grCan_minus->SaveAs("images/theoryCompare/LamParamMinMinus_1d.png");   

	cout << " chi2 min (+/-) : " << chi2_min  << " chi2 min (+) : " << chi2_plus_min << " -- chi2 min (-) : " << chi2_minus_min << endl;  	
	
  outfile->Write();
  outfile->Delete();
	
}
