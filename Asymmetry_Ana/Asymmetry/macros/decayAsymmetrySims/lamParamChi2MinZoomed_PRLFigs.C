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

void CanvasPartition(TCanvas *C,const Int_t Nx = 2,const Int_t Ny = 2,
                     Float_t lMargin = 0.15, Float_t rMargin = 0.05,
                     Float_t bMargin = 0.15, Float_t tMargin = 0.05);

void lamParamChi2MinZoomed_PRLFigs()
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
  
	TLine *horizline = new TLine(-0.25, 0, 0.15, 0 );
	horizline->SetLineColor( kBlack );
	horizline->SetLineWidth( 2 );
	horizline->SetLineStyle( 2 );  
	TLine *vertline = new TLine( 0,-0.1, 0, 0.3 );
	vertline->SetLineColor( kBlack );	
	vertline->SetLineWidth( 2 );
	vertline->SetLineStyle( 2 );  	
	
	gStyle->SetTitleBorderSize(0);
	//gStyle->SetLabelSize(0.035, "x");
	//gStyle->SetTitleSize(0.0425, "x");	
	//gStyle->SetTitleOffset(1.4, "x");
	//gStyle->SetLabelSize(0.035, "y");
	//gStyle->SetTitleSize(0.0425, "y");		
	//gStyle->SetTitleOffset(1.65, "y");	
	//gStyle->SetLabelSize(0.035, "z");
	//gStyle->SetTitleSize(0.0425, "z");		
	//gStyle->SetTitleOffset(1.65, "z");		
  //gStyle->SetRangeUser(0.,50., "z");


   auto C = new TCanvas("C","canvas",1024,640);
   C->SetFillStyle(4000);

   // Number of PADS
   const Int_t Nx = 2;
   const Int_t Ny = 2;

   // Margins
   Float_t lMargin = 0.08;
   Float_t rMargin = 0.102;
   Float_t bMargin = 0.01;
   Float_t tMargin = 0.03;


   // Canvas setup
   CanvasPartition(C,Nx,Ny,lMargin,rMargin,bMargin,tMargin);
   TPad *pad[Nx][Ny];
   C->SetWindowSize(1000,1000);
   C->SetCanvasSize(950,950);   

   for (Int_t i=0;i<Nx;i++) {
      for (Int_t j=0;j<Ny;j++) {
         C->cd(0);
         // Get the pads previously created.
         char pname[16];
         sprintf(pname,"pad_%i_%i",i,j);
         pad[i][j] = (TPad*) gROOT->FindObject(pname);
         pad[i][j]->Draw();
        /// pad[i][j]->SetFillStyle(4000);
         pad[i][j]->SetFrameFillStyle(4000);
         pad[i][j]->cd();
      }
   }

   // Dummy histogram.
   gStyle->SetOptStat(0);
   
   C->cd(0);
  
  //TCanvas *grCan = new TCanvas("grCan","",1000, 1000);
  //grCan->Divide(2,2);
  //grCan->cd(1);     	
  pad[0][1]->cd();    
  chi2gr_plus->GetYaxis()->SetRangeUser(-0.1,0.3); 
  chi2gr_plus->SetMinimum(0.);       
  chi2gr_plus->SetMaximum(50.);
  chi2gr_plus->GetXaxis()->SetLabelSize(0.06);
  chi2gr_plus->GetXaxis()->SetTitleSize(0.06); 
  chi2gr_plus->GetXaxis()->SetTitleOffset(1.1);  
  chi2gr_plus->GetXaxis()->SetNdivisions(4,10,0,false);            
  chi2gr_plus->GetYaxis()->SetLabelSize(0.06);
  chi2gr_plus->GetYaxis()->SetTitleSize(0.06); 
  chi2gr_plus->GetYaxis()->SetNdivisions(4,10,0,false);            
  chi2gr_plus->GetYaxis()->SetTitleOffset(1.3);              
  chi2gr_plus->GetZaxis()->SetLabelSize(0.0);
  chi2gr_plus->GetZaxis()->SetTitleSize(0.0);   
  chi2gr_plus->GetXaxis()->SetLabelOffset(0.01);    
  chi2gr_plus->GetYaxis()->SetLabelOffset(0.01);        
   
  chi2gr_plus->Draw("COL"); 
	TLegend *simleg_plus = new TLegend( 0.72, 0.175, 0.999, 0.275);
	simleg_plus->SetBorderSize(0);
	simleg_plus->SetFillStyle(0);
	simleg_plus->SetTextColor(kWhite);
  simleg_plus->AddEntry((TObject*)0, "#bf{(a) e^{+}}", "");	
	simleg_plus->SetMargin(0.025);	 
	simleg_plus->SetTextSize(0.075);	
	simleg_plus->Draw();	  
  

  //grCan->cd(2);	
  pad[1][1]->cd();  
  chi2gr_minus->GetYaxis()->SetRangeUser(-0.105,0.305);  
  chi2gr_minus->SetMinimum(0.);     
  chi2gr_minus->SetMaximum(50.);
  chi2gr_minus->GetXaxis()->SetLabelSize(0.06);
  chi2gr_minus->GetXaxis()->SetTitleSize(0.06);
  chi2gr_minus->GetXaxis()->SetTitleOffset(1.1);
  chi2gr_minus->GetXaxis()->SetNdivisions(4,10,0,false);                       
  chi2gr_minus->GetYaxis()->SetLabelSize(0.0);
  chi2gr_minus->GetYaxis()->SetTitleSize(0.0);  
  chi2gr_minus->GetYaxis()->SetNdivisions(4,10,0,false);              
  chi2gr_minus->GetZaxis()->SetLabelSize(0.06);
  chi2gr_minus->GetZaxis()->SetTitleSize(0.06);  
  chi2gr_minus->GetZaxis()->SetTitleOffset(1.15);    
  chi2gr_minus->GetXaxis()->SetLabelOffset(0.01);    
  chi2gr_minus->GetYaxis()->SetLabelOffset(0.01);  
  
  // Change z-axis title to represent + and - charges (left and right panel)
  chi2gr_minus->GetZaxis()->SetTitle("#Delta#chi^{2}_{e^{#pm}}(#lambda_{f},#lambda_{d})");
                  
  chi2gr_minus->Draw("COLZ");
	TLegend *simleg_minus = new TLegend( 0.58, 0.175, 0.785, 0.275);
	simleg_minus->AddEntry((TObject*)0, "#bf{(b) e^{-}}", "");		
	simleg_minus->SetTextColor(kWhite);	
	simleg_minus->SetBorderSize(0);
	simleg_minus->SetFillStyle(0);	
	simleg_minus->SetMargin(0.025);	 
	simleg_minus->SetTextSize(0.075);	
	simleg_minus->Draw();	 	  
  
  
  
 
  //grCan->cd(3);  
  pad[1][0]->cd(); 
  /*
  chi2gr->GetYaxis()->SetRangeUser(-0.105,0.305);  
  chi2gr->GetZaxis()->SetLimits(0.,50.);
  chi2gr->SetMinimum(0.);  
  chi2gr->SetMaximum(50.);
  chi2gr->GetXaxis()->SetLabelSize(0.04);
  chi2gr->GetXaxis()->SetTitleSize(0.05);  
  chi2gr->GetYaxis()->SetLabelSize(0.04);
  chi2gr->GetYaxis()->SetTitleSize(0.05);   
  chi2gr->GetZaxis()->SetLabelSize(0.04);
  chi2gr->GetZaxis()->SetTitleSize(0.05);     
  chi2gr->Draw("COLZ");
	TLegend *simleg = new TLegend( 0.6, 0.145, 0.785, 0.35);
  simleg->AddEntry((TObject*)0, "#bf{(c)}", "");	
	simleg->SetBorderSize(0);
	simleg->SetFillStyle(0);  
  //simleg->SetTextColor(kWhite);	
	simleg->SetMargin(0.025);	 
	simleg->SetTextSize(0.075);	
	simleg->Draw();	 	 
	*/
	TLegend *infoleg = new TLegend(0.03, 0.13, 0.835, 1.0);
	infoleg->AddEntry((TObject*)0, "A_{N}(p^{#uparrow}+p #rightarrow HF(e^{+/-}) + X)", "");
	infoleg->AddEntry((TObject*)0, "#sqrt{s} = 200 GeV", "");	
	infoleg->AddEntry((TObject*)0, "|#eta| < 0.35", "");	
	infoleg->AddEntry((TObject*)0, "#bf{PHENIX}", "");		
  infoleg->AddEntry((TObject*)0, "Theory: PRD78, 114013", "");	   
	infoleg->AddEntry((TObject*)0, "A_{N}^{D^{0}/#bar{D}^{0} #rightarrow e^{+/-}}(#lambda_{f},#lambda_{d})", "");		
  infoleg->SetMargin(0.05);	
  infoleg->SetTextSize(0.075);
  infoleg->SetBorderSize(0);
	infoleg->Draw();	
  
  //grCan->cd(4);	  
  //grCan->SetMargin(0.125, 0.125, 0.125, 0.05);
  pad[0][0]->cd(); 

  chi2gr_2sig1D->SetMarkerStyle(5);
  chi2gr_2sig1D->SetMarkerColor(kBlue);  
  chi2gr_2sig1D->GetXaxis()->SetLabelSize(0.06);
  chi2gr_2sig1D->GetXaxis()->SetTitleSize(0.06); 
  chi2gr_2sig1D->GetXaxis()->SetNdivisions(4,10,0,false);
  chi2gr_2sig1D->GetXaxis()->SetTitleOffset(1.1);        
  chi2gr_2sig1D->GetYaxis()->SetLabelSize(0.06);
  chi2gr_2sig1D->GetYaxis()->SetTitleSize(0.06);
  chi2gr_2sig1D->GetYaxis()->SetTitleOffset(1.3); 
  chi2gr_2sig1D->GetYaxis()->SetNdivisions(4,10,0,false);  
  chi2gr_2sig1D->GetYaxis()->SetRangeUser(-0.1,0.3);
  chi2gr_2sig1D->GetXaxis()->SetLimits(-0.25,0.15);          
  chi2gr_2sig1D->GetXaxis()->SetLabelOffset(0.01);    
  chi2gr_2sig1D->GetYaxis()->SetLabelOffset(0.01);  
               
  chi2gr_2sig1D->Draw("AP");
  chi2gr_3sig1D->SetMarkerStyle(3);
  chi2gr_3sig1D->SetMarkerColor(kViolet+1);  
  chi2gr_3sig1D->Draw("P same");    
  chi2gr_1sig1D->SetMarkerStyle(2);
  chi2gr_1sig1D->SetMarkerColor(kRed);  
  chi2gr_1sig1D->Draw("P same");  
  chi2gr_min1D->SetPoint(0, -0.01, 0.11);
  chi2gr_min1D->SetTitle(";#lambda_{f} [GeV];#lambda_{d} [GeV]");   
  chi2gr_min1D->SetMarkerStyle(8);
  chi2gr_min1D->SetMarkerSize(2.);  
  chi2gr_min1D->Draw("P same");   
  horizline->Draw("same");
  vertline->Draw("same"); 
  TLegend *leg = new TLegend(0.16, 0.16, 0.5, 0.34);
  leg->AddEntry(chi2gr_min1D, "Best Fit", "p");
  leg->AddEntry(chi2gr_1sig1D, "1 #sigma CL", "p");  
  leg->AddEntry(chi2gr_2sig1D, "2 #sigma CL", "p");
  leg->AddEntry(chi2gr_3sig1D, "3 #sigma CL", "p");  
  //leg->SetMargin(0.05);
  leg->SetTextSize(0.06);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->Draw();    	
	TLegend *simleg_CL = new TLegend( 0.825, 0.13, 0.999, 0.35 );
  simleg_CL->AddEntry((TObject*)0, "#bf{(c)}", "");	
	simleg_CL->SetBorderSize(0);
	simleg_CL->SetFillStyle(0);  
	simleg_CL->SetMargin(0.025);	 
	simleg_CL->SetTextSize(0.075);	
	simleg_CL->Draw();	
       
  //grCan->SaveAs("images/theoryCompare/LamParamMinZoomed.png");  
  C->SaveAs("images/theoryCompare/LamParamMinZoomed.png");   
  C->SaveAs("images/theoryCompare/LamParamMinZoomed.pdf");       

	cout << " chi2 min (+/-) : " << chi2_min  << " chi2 min (+) : " << chi2_plus_min << " -- chi2 min (-) : " << chi2_minus_min << endl;  	
	
  outfile->Write();
  outfile->Delete();
	
}

void CanvasPartition(TCanvas *C,const Int_t Nx,const Int_t Ny,
                     Float_t lMargin, Float_t rMargin,
                     Float_t bMargin, Float_t tMargin)
{
   if (!C) return;

   // Setup Pad layout:
   Float_t vSpacing = 0.0;
   Float_t vStep  = (1.- bMargin - tMargin - (Ny-1) * vSpacing) / Ny;

   Float_t hSpacing = 0.0;
   Float_t hStep  = (1.- lMargin - rMargin - (Nx-1) * hSpacing) / Nx;

   Float_t vposd,vposu,vmard,vmaru,vfactor;
   Float_t hposl,hposr,hmarl,hmarr,hfactor;

   for (Int_t i=0;i<Nx;i++) {

      if (i==0) {
         hposl = 0.0;
         hposr = lMargin + hStep;
         hfactor = hposr-hposl;
         hmarl = lMargin / hfactor;
         hmarr = 0.055;
      } else if (i == Nx-1) {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep + rMargin;
         hfactor = hposr-hposl;
         hmarl = 0.055;
         hmarr = rMargin / (hposr-hposl);
      } else {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep;
         hfactor = hposr-hposl;
         hmarl = 0.0;
         hmarr = 0.0;
      }

      for (Int_t j=0;j<Ny;j++) {

         if (j==0) {
            vposd = 0.0;
            vposu = bMargin + vStep;
            vfactor = vposu-vposd;
            vmard = bMargin / vfactor;
            vmaru = 0.03;
         } else if (j == Ny-1) {
            vposd = vposu + vSpacing;
            vposu = vposd + vStep + tMargin;
            vfactor = vposu-vposd;
            vmard = 0.0;
            vmaru = tMargin / (vposu-vposd);
         } else {
            vposd = vposu + vSpacing;
            vposu = vposd + vStep;
            vfactor = vposu-vposd;
            vmard = 0.0;
            vmaru = 0.0;
         }

         C->cd(0);

         vmard = 0.155;         
         char name[16];
         sprintf(name,"pad_%i_%i",i,j);
         TPad *pad = (TPad*) gROOT->FindObject(name);
         if (pad) delete pad;
         pad = new TPad(name,"",hposl,vposd,hposr,vposu);
         pad->SetLeftMargin(hmarl);
         pad->SetRightMargin(hmarr);
         pad->SetBottomMargin(vmard);
         pad->SetTopMargin(vmaru);

         pad->SetFrameBorderMode(0);
         pad->SetBorderMode(0);
         pad->SetBorderSize(0);

         pad->Draw();
      }
   }
}
