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

void lamParamChi2MinZoomed_sepFigs()
{
	bool prelim = 1;
	bool theory_compare = 0;
	int verbosity = -99;
	const int nbins = NUM_VALUE_BINS;
	double x_AN_plus[nbins], y_AN_plus[nbins], x_AN_minus[nbins], y_AN_minus[nbins];
	double x_AN_theory_plus[nbins], y_AN_theory_plus[nbins], x_AN_theory_minus[nbins], y_AN_theory_minus[nbins];
	double AN_err_plus[nbins], AN_err_minus[nbins];
  const int Npar = 41;
  double fs[Npar] = {-0.20, -0.19, -0.18, -0.17, -0.16, -0.15, -0.14, -0.13, -0.12, -0.11, -0.10, -0.09, -0.08, -0.07, -0.06, -0.05, -0.04, -0.03, -0.02, -0.01, 0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.10, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19, 0.20};
  double ds[Npar] = {-0.10, -0.09, -0.08, -0.07, -0.06, -0.05, -0.04, -0.03, -0.02, -0.01, 0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.10, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19, 0.20, 0.21, 0.22, 0.23, 0.24, 0.25, 0.26, 0.27, 0.28, 0.29, 0.30};	
  //TString par_labels[Npar] = {"m1", "m0p95", "m0p9", "m0p85", "m0p8", "m0p75", "m0p7", "m0p65", "m0p6", "m0p55", "m0p5", "m0p45", "m0p4", "m0p35", "m0p3", "m0p25", "m0p2", "m0p15", "m0p1", "m0p07", "m0p05", "0", "0p05", "0p07", "0p1", "0p15", "0p2", "0p25", "0p3", "0p35", "0p4", "0p45", "0p5", "0p55", "0p6", "0p65", "0p7", "0p75", "0p8", "0p85", "0p9", "0p95", "1"};
  TString par_labels_reduced[3] = {"m0p2", "0", "0p2"};  
    
 
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
	TString graphname = "";  	
	int counter = 0;
  for (int j=0; j<Npar; ++j)
  {
    for (int k=0; k<Npar; ++k)
	  {
	    graphname = "eAN";
	    ostringstream ss;
	    ss<<"_"<<fs[j]<<"_"<<ds[k];
	    graphname += ss.str();
	    string graph_name(graphname);
	    replace(graph_name.begin(), graph_name.end(), '.', 'p');
	    replace(graph_name.begin(), graph_name.end(), '-', 'm');
	    graphname = graph_name;
      eANtheory_plus[j][k] = (TGraph*)infile_theory_plus->Get(graphname);
      eANtheory_minus[j][k] = (TGraph*)infile_theory_minus->Get(graphname);
      chi2_plus_sig[j][k] = 0;
      chi2_minus_sig[j][k] = 0;      
      
	  }
  }  
  //candiv_plus->SaveAs("images/theoryCompare/theoryComparePlus.png");
  //candiv_minus->SaveAs("images/theoryCompare/theoryCompareMinus.png"); 
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
  chi2gr->SetTitle(";#lambda_{f} [GeV];#lambda_{d} [GeV];#Delta#chi^{2}_{e^{+/-}}(#lambda_{f},#lambda_{d})");	   	 
  chi2gr_2sig1D->SetTitle(";#lambda_{f} [GeV];#lambda_{d} [GeV]");  	                  
  TGraph2D *chi2gr_plus = new TGraph2D(); 
  TGraph2D *chi2gr_plus_min = new TGraph2D();   
  TGraph2D *chi2gr_plus_1sig = new TGraph2D();    
  TGraph *chi2gr_plus_min1D = new TGraph();   
  TGraph *chi2gr_plus_2sig1D = new TGraph();  
  TGraph *chi2gr_plus_1sig1D = new TGraph();            
  chi2gr_plus->SetTitle(";#lambda_{f} [GeV];#lambda_{d} [GeV];#Delta#chi^{2}_{e^{+}}(#lambda_{f},#lambda_{d})"); 		     
  TGraph2D *chi2gr_minus = new TGraph2D();   
  TGraph2D *chi2gr_minus_min = new TGraph2D();     
  TGraph2D *chi2gr_minus_1sig = new TGraph2D();
  TGraph *chi2gr_minus_min1D = new TGraph();   
  TGraph *chi2gr_minus_2sig1D = new TGraph(); 
  TGraph *chi2gr_minus_1sig1D = new TGraph();               
	chi2gr_minus->SetTitle(";#lambda_{f} [GeV];#lambda_{d} [GeV];#Delta#chi^{2}_{e^{-}}(#lambda_{f},#lambda_{d})");
      
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
		    
		    //chi2_sig[j][k] += pow(y_AN_plus[i] - y_AN_theory_plus[i],2)/pow(AN_err_plus[i],2);			    
		    //chi2_sig[j][k] += pow(y_AN_minus[i] - y_AN_theory_minus[i],2)/pow(AN_err_minus[i],2); 
	    } 
	    chi2_sig[j][k] = chi2_plus_sig[j][k] + chi2_minus_sig[j][k]; 
	    // Set Points for Graphs //
		  chi2gr_plus->SetPoint(npoint, fs[j],ds[k], chi2_plus_sig[j][k] - 1.56237);
		  chi2gr_minus->SetPoint(npoint, fs[j],ds[k], chi2_minus_sig[j][k] - 5.50653);
		  chi2gr->SetPoint(npoint, fs[j],ds[k], chi2_sig[j][k] - 7.10154);		
		  npoint++;	
		  
	    chi2_vec.push_back(chi2_sig[j][k]);	    
	    chi2_plus_vec.push_back(chi2_plus_sig[j][k]);	    
	    chi2_minus_vec.push_back(chi2_minus_sig[j][k]);	  
	    //if (chi2_sig[j][k] == 7.17578) 
	      //cout << "Min params... f : " << fs[j] << " d : " << ds[k] << endl;
		  if(chi2_sig[j][k] - 7.10154 < 0.0001) { cout << " (+/-) f : " << fs[j] << " d : " << ds[k] << endl; }	 	    
		  if(chi2_plus_sig[j][k] - 1.56237 < 0.0001) { cout << " (+) f : " << fs[j] << " d : " << ds[k] << endl; }	 
		  if(chi2_minus_sig[j][k] - 5.50653 < 0.0001) { cout << " (-) f : " << fs[j] << " d : " << ds[k] << endl; }	   
		  // 3 sigma CL // 
		  if( (chi2_sig[j][k] - 7.10154 < 9.) && (chi2_sig[j][k] - 7.10154 > 4.) ) 
		  { 
		    chi2gr_3sig1D->SetPoint(npoint_3sig, fs[j],ds[k]); 		    
		    npoint_3sig++;   
		    //cout << " (+/-) f : " << fs[j] << " d : " << ds[k] << endl;
		  }			  		  
		  // 2 sigma CL  //   
		  if( (chi2_sig[j][k] - 7.10154 < 4.) && (chi2_sig[j][k] - 7.10154 > 1.) ) 
		  { 
		    chi2gr_2sig1D->SetPoint(npoint_pm, fs[j],ds[k]); 		    
		    npoint_pm++;   
		    //cout << " (+/-) f : " << fs[j] << " d : " << ds[k] << endl;
		  }	
		  //1 sigma CL//   
		  //if(chi2_sig[j][k] - 7.17578 < 12.899) 
		  if(chi2_sig[j][k] - 7.10154 < 1.)
		  { 
		    chi2gr_1sig->SetPoint(npoint_70_pm, fs[j],ds[k], chi2_sig[j][k]); 		  
		    chi2gr_1sig1D->SetPoint(npoint_70_pm, fs[j],ds[k]); 		    
		    npoint_70_pm++;   
		    //cout << " (+/-) f : " << fs[j] << " d : " << ds[k] << endl;
		  }	   	 		     	  		  
		  // 2 sigma CL //   
		  //if(chi2_plus_sig[j][k] - 1.50191 < 11.070) 
		  if( (chi2_plus_sig[j][k] - 1.56237 < 4.) && (chi2_plus_sig[j][k] - 1.56237 > 1.) ) 		  
		  { 
		    chi2gr_plus_2sig1D->SetPoint(npoint_plus, fs[j],ds[k]); 		     
		    npoint_plus++;   
		  }	 		  	
		  //if(chi2_minus_sig[j][k] - 5.45783 < 11.070) 
		  if( (chi2_minus_sig[j][k] - 5.50653 < 4.) && (chi2_minus_sig[j][k] - 5.50653 > 1.) ) 		  
		  { 

		    chi2gr_minus_2sig1D->SetPoint(npoint_minus, fs[j],ds[k]); 		     		     
		    npoint_minus++;  
		  }	   
		  // 1 sigma CL  //   
		  //if(chi2_plus_sig[j][k] - 1.50191 < 6.064) 
		  if(chi2_plus_sig[j][k] - 1.56237 < 1.) 		  
		  {
		    chi2gr_plus_1sig->SetPoint(npoint_70_plus, fs[j],ds[k], chi2_plus_sig[j][k]);		   
		    chi2gr_plus_1sig1D->SetPoint(npoint_70_plus, fs[j],ds[k]); 		     
		    npoint_70_plus++;   
		  }	 		  	
		  //if(chi2_minus_sig[j][k] - 5.45783 < 6.064) 
		  if(chi2_minus_sig[j][k] - 5.50653 < 1.) 		  
		  { 
		    chi2gr_minus_1sig->SetPoint(npoint_70_minus, fs[j],ds[k], chi2_minus_sig[j][k]);		  
		    chi2gr_minus_1sig1D->SetPoint(npoint_70_minus, fs[j],ds[k]); 		     		     
		    npoint_70_minus++;  
		  }			  		    	     		  	    	        
	    //cout << " chi2 (+) : " << chi2_plus_sig[j][k] << " -- chi2 (-) : " << chi2_minus_sig[j][k] << endl;
    }
  }
  
	TFile *outfile = new TFile("lamParamChi2MinZoomed.root", "RECREATE");
	outfile->cd();				      

    
  double chi2_min = *min_element(chi2_vec.begin(), chi2_vec.end());
  double chi2_plus_min = *min_element(chi2_plus_vec.begin(), chi2_plus_vec.end());	
  double chi2_minus_min = *min_element(chi2_minus_vec.begin(), chi2_minus_vec.end()); 	
  
	TLine *horizline = new TLine(-0.2, 0, 0.2, 0 );
	horizline->SetLineColor( kBlack );
	horizline->SetLineWidth( 2 );
	horizline->SetLineStyle( 2 );  
	TLine *vertline = new TLine( 0,-0.1, 0, 0.3 );
	vertline->SetLineColor( kBlack );	
	vertline->SetLineWidth( 2 );
	vertline->SetLineStyle( 2 );  	
	
	gStyle->SetTitleBorderSize(0);
	gStyle->SetLabelSize(0.035, "x");
	gStyle->SetTitleSize(0.035, "x");	
	gStyle->SetTitleOffset(1.4, "x");
	gStyle->SetLabelSize(0.035, "y");
	gStyle->SetTitleSize(0.035, "y");		
	gStyle->SetTitleOffset(1.65, "y");	
	gStyle->SetLabelSize(0.035, "z");
	gStyle->SetTitleSize(0.035, "z");		
	gStyle->SetTitleOffset(1.65, "z");		
  gStyle->SetPadTopMargin(0.2);
  gStyle->SetPadRightMargin(0.2);
  gStyle->SetPadBottomMargin(0.2);
  gStyle->SetPadLeftMargin(0.2);	
  //gStyle->SetRangeUser(0.,50., "z");

  
  TCanvas *grCan = new TCanvas("grCan","",1000,500);   
  grCan->Divide(2,1);
  grCan->cd(1);
  chi2gr_plus->GetYaxis()->SetRangeUser(-0.105,0.305); 
  chi2gr_plus->SetMinimum(0.);       
  chi2gr_plus->SetMaximum(50.);
  chi2gr_plus->Draw("COL"); 
  TLegend *dataleg_plus = new TLegend(0.5625, 0.685, 0.8, 0.8);
	dataleg_plus->AddEntry((TObject*)0, "A_{N}(p^{#uparrow}+p #rightarrow HF(e^{+}) + X)", "");
	dataleg_plus->AddEntry((TObject*)0, "#sqrt{s} = 200 GeV", "");	
	dataleg_plus->AddEntry((TObject*)0, "|#eta| < 0.35", "");		
	dataleg_plus->SetMargin(0.015);	
	dataleg_plus->SetTextSize(0.024);	
	dataleg_plus->Draw(); 
	TLegend *simleg_plus = new TLegend( 0.5625, 0.2, 0.8, 0.28);
  simleg_plus->AddEntry((TObject*)0, "Theory: PRD78, 114013", "");	
	simleg_plus->AddEntry((TObject*)0, "A_{N}^{D^{0} #rightarrow e^{+}}(#lambda_{f},#lambda_{d})", "");	
	simleg_plus->SetMargin(0.015);	 
	simleg_plus->SetTextSize(0.024);	
	simleg_plus->Draw();	  
  
  grCan->cd(2);
  chi2gr_minus->GetYaxis()->SetRangeUser(-0.105,0.305);  
  chi2gr_minus->SetMinimum(0.);     
  chi2gr_minus->SetMaximum(50.);
  chi2gr_minus->Draw("COLZ");
  TLegend *dataleg_minus = new TLegend(0.5625, 0.685, 0.8, 0.8);
	dataleg_minus->AddEntry((TObject*)0, "A_{N}(p^{#uparrow}+p #rightarrow HF(e^{-}) + X)", "");
	dataleg_minus->AddEntry((TObject*)0, "#sqrt{s} = 200 GeV", "");	
	dataleg_minus->AddEntry((TObject*)0, "|#eta| < 0.35", "");		
	dataleg_minus->SetMargin(0.015);	
	dataleg_minus->SetTextSize(0.024);	
	dataleg_minus->Draw();  
	TLegend *simleg_minus = new TLegend(  0.5625, 0.2, 0.8, 0.28);
	simleg_minus->AddEntry((TObject*)0, "Theory: PRD78, 114013", "");		
	simleg_minus->AddEntry((TObject*)0, "A_{N}^{#bar{D}^{0} #rightarrow e^{-}}(#lambda_{f},#lambda_{d})", "");	
	simleg_minus->SetMargin(0.015);	 
	simleg_minus->SetTextSize(0.024);	
	simleg_minus->Draw();	 	 
  grCan->SaveAs("images/theoryCompare/LamParamMinZoomed_delchi2_pm.png"); 		 
  
  grCan->cd(1);  
  chi2gr->GetYaxis()->SetRangeUser(-0.105,0.305);  
  chi2gr->GetZaxis()->SetLimits(0.,50.);
  chi2gr->SetMinimum(0.);  
  chi2gr->SetMaximum(50.);
  chi2gr->Draw("COLZ");
  TLegend *dataleg = new TLegend(0.5625, 0.685, 0.8, 0.8); 
	dataleg->AddEntry((TObject*)0, "A_{N}(p^{#uparrow}+p #rightarrow HF(e^{+/-}) + X)", "");
	dataleg->AddEntry((TObject*)0, "#sqrt{s} = 200 GeV", "");	
	dataleg->AddEntry((TObject*)0, "|#eta| < 0.35", "");		
	dataleg->SetMargin(0.015);	
	dataleg->SetTextSize(0.024);	
	dataleg->Draw();   
	TLegend *simleg = new TLegend( 0.5625, 0.2, 0.8, 0.28);
  simleg->AddEntry((TObject*)0, "Theory: PRD78, 114013", "");	
	simleg->AddEntry((TObject*)0, "A_{N}^{D^{0}/#bar{D}^{0} #rightarrow e^{+/-}}(#lambda_{f},#lambda_{d})", "");	
	simleg->SetMargin(0.015);	 
	simleg->SetTextSize(0.024);	
	simleg->Draw();	 	  
	

  
  grCan->cd(2);
  //grCan->SetMargin(0.125, 0.125, 0.125, 0.05);
  chi2gr_2sig1D->GetYaxis()->SetRangeUser(-0.105,0.305);
  chi2gr_2sig1D->GetXaxis()->SetLimits(-0.2,0.2); 
  chi2gr_2sig1D->SetMarkerStyle(5);
  chi2gr_2sig1D->SetMarkerColor(kBlue);  
  chi2gr_2sig1D->Draw("AP");
  chi2gr_3sig1D->SetMarkerStyle(3);
  chi2gr_3sig1D->SetMarkerColor(kViolet+1);  
  chi2gr_3sig1D->Draw("P same");    
  chi2gr_1sig1D->SetMarkerStyle(2);
  chi2gr_1sig1D->SetMarkerColor(kRed);  
  chi2gr_1sig1D->Draw("P same");  
  chi2gr_min1D->SetPoint(0, -0.01, 0.11);
  chi2gr_min1D->SetTitle("; #lambda_{f} [GeV]; #lambda_{d} [GeV]");   
  chi2gr_min1D->SetMarkerStyle(8);
  chi2gr_min1D->SetMarkerSize(2.);  
  chi2gr_min1D->Draw("P same");   
  horizline->Draw("same");
  vertline->Draw("same"); 
	TLegend *dataleg_CL = new TLegend(0.5625, 0.685, 0.8, 0.8);
	dataleg_CL->AddEntry((TObject*)0, "A_{N}(p^{#uparrow}+p #rightarrow HF(e^{+/-}) + X)", "");
	dataleg_CL->AddEntry((TObject*)0, "#sqrt{s} = 200 GeV", "");	
	dataleg_CL->AddEntry((TObject*)0, "|#eta| < 0.35", "");		
	dataleg_CL->SetMargin(0.015);	
	dataleg_CL->SetTextSize(0.024);
	dataleg_CL->Draw(); 
  TLegend *leg = new TLegend(0.2, 0.2, 0.325, 0.34);
  leg->AddEntry(chi2gr_min1D, "Best Fit", "p");
  leg->AddEntry(chi2gr_1sig1D, "1 #sigma CL", "p");  
  leg->AddEntry(chi2gr_2sig1D, "2 #sigma CL", "p");
  leg->AddEntry(chi2gr_3sig1D, "3 #sigma CL", "p");  
  //leg->SetMargin(0.05);
  leg->SetTextSize(0.024);
  leg->Draw();    	
	TLegend *simleg_CL = new TLegend( 0.5625, 0.2, 0.8, 0.28);
  simleg_CL->AddEntry((TObject*)0, "Theory: PRD78, 114013", "");	
	simleg_CL->AddEntry((TObject*)0, "A_{N}^{D^{0}/#bar{D}^{0} #rightarrow e^{+/-}}(#lambda_{f},#lambda_{d})", "");	
	simleg_CL->SetMargin(0.015);	 
	simleg_CL->SetTextSize(0.024);	
	simleg_CL->Draw();	 	 	
       
  grCan->SaveAs("images/theoryCompare/LamParamMinZoomed_CL.png");  

	cout << " chi2 min (+/-) : " << chi2_min  << " chi2 min (+) : " << chi2_plus_min << " -- chi2 min (-) : " << chi2_minus_min << endl;  	
	
  outfile->Write();
  outfile->Delete();
	
}
