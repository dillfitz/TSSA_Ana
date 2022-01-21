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

void lamParamChi2MinZoomed_KGKGp()
{
  bool d0 = 1;
	bool prelim = 1;
	bool theory_compare = 0;
	int verbosity = -99;
	const int nbins = NUM_VALUE_BINS;
	double x_AN_plus[nbins], y_AN_plus[nbins], x_AN_minus[nbins], y_AN_minus[nbins];
  double AN_err_plus[nbins], AN_err_minus[nbins];
	double x_AN_theory_plus_KG[nbins], y_AN_theory_plus_KG[nbins], x_AN_theory_minus_KG[nbins], y_AN_theory_minus_KG[nbins];
	double x_AN_theory_plus_KGp[nbins], y_AN_theory_plus_KGp[nbins], x_AN_theory_minus_KGp[nbins], y_AN_theory_minus_KGp[nbins];	
  const int Npar = 41;
  double KGs[Npar] = {-0.002, -0.0019, -0.0018, -0.0017, -0.0016, -0.0015, -0.0014, -0.0013, -0.0012, -0.0011, -0.0010, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0004, -0.0003, -0.0002, -0.0001, 0.0, 0.0001, 0.0002, 0.0003, 0.0004, 0.0005, 0.0006, 0.0007, 0.0008, 0.0009, 0.0010, 0.0011, 0.0012, 0.0013, 0.0014, 0.0015 , 0.0016, 0.0017, 0.0018, 0.0019, 0.002};
  double KGps[Npar] = {-0.002, -0.0019, -0.0018, -0.0017, -0.0016, -0.0015, -0.0014, -0.0013, -0.0012, -0.0011, -0.0010, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0004, -0.0003, -0.0002, -0.0001, 0.0, 0.0001, 0.0002, 0.0003, 0.0004, 0.0005, 0.0006, 0.0007, 0.0008, 0.0009, 0.0010, 0.0011, 0.0012, 0.0013, 0.0014, 0.0015 , 0.0016, 0.0017, 0.0018, 0.0019, 0.002};
  //TString par_labels[Npar] = {"m1", "m0p95", "m0p9", "m0p85", "m0p8", "m0p75", "m0p7", "m0p65", "m0p6", "m0p55", "m0p5", "m0p45", "m0p4", "m0p35", "m0p3", "m0p25", "m0p2", "m0p15", "m0p1", "m0p07", "m0p05", "0", "0p05", "0p07", "0p1", "0p15", "0p2", "0p25", "0p3", "0p35", "0p4", "0p45", "0p5", "0p55", "0p6", "0p65", "0p7", "0p75", "0p8", "0p85", "0p9", "0p95", "1"};
  TString par_labels_reduced[3] = {"m0p2", "0", "0p2"};  
    
 
	TFile *infile_data_plus = TFile::Open("../dataFiles/bgCorrected_ohfe_AN_plus.root");
	TFile *infile_data_minus = TFile::Open("../dataFiles/bgCorrected_ohfe_AN_minus.root");
	if (d0) 
	{
	  TFile *infile_theory_plus = TFile::Open("dataFiles/d0DecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");
    TFile *infile_theory_minus = TFile::Open("dataFiles/antid0DecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");
  }
  else
  {
    TFile *infile_theory_plus = TFile::Open("dataFiles/dplusDecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");
    TFile *infile_theory_minus = TFile::Open("dataFiles/dminusDecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");
  }
    
	TGraphAsymmErrors *ohfeAN_plus = (TGraphAsymmErrors*)infile_data_plus->Get("ohfeAN");
	TGraphAsymmErrors *ohfeAN_minus = (TGraphAsymmErrors*)infile_data_minus->Get("ohfeAN");
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
	candiv_plus->Divide(3,2);
	candiv_minus->Divide(3,2);
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
	  
    eANtheory_plus_KG[j] = (TGraph*)infile_theory_plus->Get(graphname_KG);
    eANtheory_minus_KG[j] = (TGraph*)infile_theory_minus->Get(graphname_KG);
    eANtheory_plus_KGp[j] = (TGraph*)infile_theory_plus->Get(graphname_KGp);
    eANtheory_minus_KGp[j] = (TGraph*)infile_theory_minus->Get(graphname_KGp);   
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
    
    if (j%10==0) 
    {
      candiv_plus->cd(counter+1);
      TString title = "K_{G} = K_{G}' = ";
      title += KGs[j];
      eANtheory_plus_KG[j]->SetTitle(title);        
      ohfeAN_plus->SetMarkerStyle(25);                      
      eANtheory_plus_KG[j]->SetMarkerStyle(kCircle);
      eANtheory_plus_KG[j]->GetYaxis()->SetRangeUser(-0.06, 0.08);          
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
      ohfeAN_minus->SetTitle("");        
      ohfeAN_minus->SetMarkerStyle(25);                      
      eANtheory_minus_KG[j]->SetMarkerStyle(kCircle);
      eANtheory_minus_KG[j]->GetYaxis()->SetRangeUser(-0.06, 0.08);          
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
      
  }  
  if (d0)
  {
    candiv_plus->SaveAs("images/theoryCompare/theoryComparePlus_KGKGp_d0.png");
    candiv_minus->SaveAs("images/theoryCompare/theoryCompareMinus_KGKGp_d0.png"); 
  }
  else
  {
    candiv_plus->SaveAs("images/theoryCompare/theoryComparePlus_KGKGp_d.png");
    candiv_minus->SaveAs("images/theoryCompare/theoryCompareMinus_KGKGp_d.png"); 
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
		    eANtheory_plus_KG[j]->GetPoint(i, x_AN_theory_plus_KG[i], y_AN_theory_plus_KG[i]);
		    chi2_plus_sig_KG[j] += pow(y_AN_plus[i] - y_AN_theory_plus_KG[i],2)/pow(AN_err_plus[i],2);	
		    eANtheory_plus_KGp[j]->GetPoint(i, x_AN_theory_plus_KGp[i], y_AN_theory_plus_KGp[i]);
		    chi2_plus_sig_KGp[j] += pow(y_AN_plus[i] - y_AN_theory_plus_KGp[i],2)/pow(AN_err_plus[i],2);			    
		    
		    ohfeAN_minus->GetPoint(i, x_AN_minus[i], y_AN_minus[i]);
		    AN_err_minus[i] = ohfeAN_minus->GetErrorYhigh(i);
		    eANtheory_minus_KG[j]->GetPoint(i, x_AN_theory_minus_KG[i], y_AN_theory_minus_KG[i]);
		    chi2_minus_sig_KG[j] += pow(y_AN_minus[i] - y_AN_theory_minus_KG[i],2)/pow(AN_err_minus[i],2);	 
		    eANtheory_minus_KGp[j]->GetPoint(i, x_AN_theory_minus_KGp[i], y_AN_theory_minus_KGp[i]);
		    chi2_minus_sig_KGp[j] += pow(y_AN_minus[i] - y_AN_theory_minus_KGp[i],2)/pow(AN_err_minus[i],2);	  		      
	    } 
	    chi2_sig_KG[j] = chi2_plus_sig_KG[j] + chi2_minus_sig_KG[j]; 
	    chi2_sig_KGp[j] = chi2_plus_sig_KGp[j] + chi2_minus_sig_KGp[j]; 	    
	    // Set Points for Graphs //
		  //chi2gr_plus->SetPoint(npoint, fs[j],ds[k], chi2_plus_sig[j] - 1.56237);
		  //chi2gr_minus->SetPoint(npoint, fs[j],ds[k], chi2_minus_sig[j] - 5.50653);
		  //chi2gr->SetPoint(npoint, fs[j],ds[k], chi2_sig[j] - 7.10154);		
		  //npoint++;	
		  
	    chi2_vec_KG.push_back(chi2_sig_KG[j]);	    
	    chi2_plus_vec_KG.push_back(chi2_plus_sig_KG[j]);	    
	    chi2_minus_vec_KG.push_back(chi2_minus_sig_KG[j]);	 

	    chi2_vec_KGp.push_back(chi2_sig_KGp[j]);	    
	    chi2_plus_vec_KGp.push_back(chi2_plus_sig_KGp[j]);	    
	    chi2_minus_vec_KGp.push_back(chi2_minus_sig_KGp[j]);	 	     

      if (d0) 
      {
		    if(chi2_sig_KG[j] - 8.50086 < 0.0001) { cout << " (+/-) KG : " << KGs[j] << endl; }	 	    
		    //if(chi2_plus_sig_KG[j] - 1.72827 < 0.0001) { cout << " (+) KG : " << KGs[j] << endl; }	 
		    //if(chi2_minus_sig_KG[j] - 5.64099 < 0.0001) { cout << " (-) KG : " << KGs[j] << endl; }	  
		    
		    if(chi2_sig_KGp[j] - 7.36615 < 0.0001) { cout << " (+/-) KGp : " << KGps[j] << endl; }	 	    
		    //if(chi2_plus_sig_KGp[j] - 1.61912 < 0.0001) { cout << " (+) KGp : " << KGps[j] << endl; }	 
		    //if(chi2_minus_sig_KGp[j] - 5.5703 < 0.0001) { cout << " (-) KGp : " << KGps[j] << endl; }	  		    
		    /*
		    /// KG ///
		    // 3 sigma CL // 
		    // +- //
		    if( (chi2_sig_KG[j] - 8.50086 < 9.) && (chi2_sig_KG[j] - 7.10154 > 4.) ) 
		    { 
		    //cout << " (+/-) f : " << fs[j] << " d : " << ds[k] << endl;
		    }
		    */ 			  		  
		    // 2 sigma CL  //  
		    // +- // 
		    if( (chi2_sig_KG[j] - 8.50086 < 4.) && (chi2_sig_KG[j] - 8.50086 > 1.) ) 
		    {    
		      KGs_2sigma.push_back(KGs[j]);
		    }	
		    /*
		    // + //
		    if( (chi2_plus_sig_KG[j] - 1.72827 < 4.) && (chi2_plus_sig_KG[j] - 1.56237 > 1.) ) 		  
		    { 

		    }	 	
		    // - //	  		
		    if( (chi2_minus_sig_KG[j] - 5.64099 < 4.) && (chi2_minus_sig_KG[j] - 5.50653 > 1.) ) 		  
		    { 

		    }
		    */
		      		    	    
		    // 1 sigma CL//   
        // +- //
		    if(chi2_sig_KG[j] - 8.50086 < 1.)
		    { 
		      //cout << " KG : " << KGs[j] << endl;
		      KGs_1sigma.push_back(KGs[j]);
		    }	   
		    /*
		    // + //	 		     	  		  
		    if(chi2_plus_sig_KG[j] - 1.72827 < 1.) 		  
		    {
 
		    }	 		  	
        // - //
		    if(chi2_minus_sig_KG[j] - 5.64099 < 1.) 		  
		    { 

		    }			  		    	    
		    */
		    /*
		    /// KGp /// 
		    // 3 sigma CL // 
		    // +- //
		    if( (chi2_sig_KGp[j] - 7.36615 < 9.) && (chi2_sig_KGp[j] - 7.10154 > 4.) ) 
		    { 
		    //cout << " (+/-) f : " << fs[j] << " d : " << ds[k] << endl;
		    } 
		    */			  		  
		    // 2 sigma CL  //  
		    // +- // 
		    if( (chi2_sig_KGp[j] - 7.36615 < 4.) && (chi2_sig_KGp[j] - 7.36615 > 1.) ) 
		    {    
		      KGps_2sigma.push_back(KGps[j]);
		    }	
		    /*
		    // + //
		    if( (chi2_plus_sig_KGp[j] - 1.72827 < 4.) && (chi2_plus_sig_KGp[j] - 1.61912 > 1.) ) 		  
		    { 

		    }	 	
		    // - //	  		
		    if( (chi2_minus_sig_KGp[j] - 5.64099 < 4.) && (chi2_minus_sig_KGp[j] - 5.5703 > 1.) ) 		  
		    { 

		    }	 
		    */
		      		    	    
		    // 1 sigma CL//   
        // +- //
		    if(chi2_sig_KGp[j] - 7.36615 < 1.)
		    { 
		      //cout << " KGp : " << KGps[j] << endl;
		      KGps_1sigma.push_back(KGps[j]);
		    }	   
		    /*
		    // + //	 		     	  		  
		    if(chi2_plus_sig_KGp[j] - 1.61912 < 1.) 		  
		    {
 
		    }	 		  	
        // - //
		    if(chi2_minus_sig_KGp[j] - 5.5703 < 1.) 		  
		    { 

		    }	
		    */			    		  	    	        
      }
      else 
      {
		    if(chi2_sig_KG[j] - 7.97837 < 0.0001) { cout << " (+/-) KG : " << KGs[j] << endl; }	 	    
		    if(chi2_plus_sig_KG[j] - 1.39348 < 0.0001) { cout << " (+) KG : " << KGs[j] << endl; }	 
		    if(chi2_minus_sig_KG[j] - 5.51871 < 0.0001) { cout << " (-) KG : " << KGs[j] << endl; }	  
		    
		    if(chi2_sig_KGp[j] - 6.90799 < 0.0001) { cout << " (+/-) KGp : " << KGps[j] << endl; }	 	    
		    if(chi2_plus_sig_KGp[j] - 1.39661 < 0.0001) { cout << " (+) KGp : " << KGps[j] << endl; }	 
		    if(chi2_minus_sig_KGp[j] - 5.45153 < 0.0001) { cout << " (-) KGp : " << KGps[j] << endl; }	
		    
		    /*
		    /// KG ///
		    // 3 sigma CL // 
		    // +- //
		    if( (chi2_sig_KG[j] - 7.97837 < 9.) && (chi2_sig_KG[j] - 7.97837 > 4.) ) 
		    { 
		    //cout << " (+/-) f : " << fs[j] << " d : " << ds[k] << endl;
		    }
		    */ 			  		  
		    // 2 sigma CL  //  
		    // +- // 
		    if( (chi2_sig_KG[j] - 7.97837 < 4.) && (chi2_sig_KG[j] - 7.97837 > 1.) ) 
		    {    
		      KGs_2sigma.push_back(KGs[j]);
		    }	
		    /*
		    // + //
		    if( (chi2_plus_sig_KG[j] - 1.39661 < 4.) && (chi2_plus_sig_KG[j] - 1.39661 > 1.) ) 		  
		    { 

		    }	 	
		    // - //	  		
		    if( (chi2_minus_sig_KG[j] - 5.45153 < 4.) && (chi2_minus_sig_KG[j] - 5.45153 > 1.) ) 		  
		    { 

		    }
		    */
		      		    	    
		    // 1 sigma CL//   
        // +- //
		    if(chi2_sig_KG[j] - 7.97837 < 1.)
		    { 
		      //cout << " KG : " << KGs[j] << endl;
		      KGs_1sigma.push_back(KGs[j]);
		    }	   
		    /*
		    // + //	 		     	  		  
		    if(chi2_plus_sig_KG[j] - 1.39661 < 1.) 		  
		    {
 
		    }	 		  	
        // - //
		    if(chi2_minus_sig_KG[j] - 5.45153 < 1.) 		  
		    { 

		    }			  		    	    
		    */
		    /*
		    /// KGp /// 
		    // 3 sigma CL // 
		    // +- //
		    if( (chi2_sig_KGp[j] - 6.90799 < 9.) && (chi2_sig_KGp[j] - 6.90799 > 4.) ) 
		    { 
		    //cout << " (+/-) f : " << fs[j] << " d : " << ds[k] << endl;
		    } 
		    */			  		  
		    // 2 sigma CL  //  
		    // +- // 
		    if( (chi2_sig_KGp[j] - 6.90799 < 4.) && (chi2_sig_KGp[j] - 6.90799 > 1.) ) 
		    {    
		      KGps_2sigma.push_back(KGps[j]);
		    }	
		    /*
		    // + //
		    if( (chi2_plus_sig_KGp[j] - 1.39661 < 4.) && (chi2_plus_sig_KGp[j] - 1.39661 > 1.) ) 		  
		    { 

		    }	 	
		    // - //	  		
		    if( (chi2_minus_sig_KGp[j] - 5.45153 < 4.) && (chi2_minus_sig_KGp[j] - 5.45153 > 1.) ) 		  
		    { 

		    }	 
		    */
		      		    	    
		    // 1 sigma CL//   
        // +- //
		    if(chi2_sig_KGp[j] - 6.90799 < 1.)
		    { 
		      //cout << " KGp : " << KGps[j] << endl;
		      KGps_1sigma.push_back(KGps[j]);
		    }	   
		    /*
		    // + //	 		     	  		  
		    if(chi2_plus_sig_KGp[j] - 1.39661 < 1.) 		  
		    {
 
		    }	 		  	
        // - //
		    if(chi2_minus_sig_KGp[j] - 5.45153 < 1.) 		  
		    { 

		    }	
		    */			    		  	    	        		            
      }
  }
  
  if (d0)
  {
	  TFile *outfile = new TFile("lamParamChi2MinZoomed_KGKGp_d0.root", "RECREATE");
	}
  else
  {
	  TFile *outfile = new TFile("lamParamChi2MinZoomed_KGKGp_d.root", "RECREATE");
	}	
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
  KGs_chi2_min->SetPoint(0,-0.0001, 0.);
  KGs_chi2_min->SetMarkerStyle(20);
  TGraph *KGps_chi2_min = new TGraph();
  KGps_chi2_min->SetPoint(0,0.0002, 0.);  
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
  
  if (d0) {can_chi2->SaveAs("images/theoryCompare/KGKGp_chi2_d0.png");}
  else {can_chi2->SaveAs("images/theoryCompare/KGKGp_chi2_d.png");}  

  TCanvas *can = new TCanvas();
  can->Divide(1,2);
  can->cd(1);
  KGs_1sig_graph->GetYaxis()->SetRangeUser(-1., 1.);
  KGs_1sig_graph->GetXaxis()->SetLimits(-0.002, 0.002);    
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
  KGps_1sig_graph->GetXaxis()->SetLimits(-0.002, 0.002);    
  KGps_1sig_graph->SetTitle("");
  KGps_1sig_graph->GetXaxis()->SetTitle("K_{G}'");  
  KGps_1sig_graph->GetXaxis()->SetTitleSize(0.06);  
  KGps_1sig_graph->GetXaxis()->SetLabelSize(0.045);    
  KGps_1sig_graph->GetXaxis()->SetTitleOffset(0.75);     
  KGps_1sig_graph->Draw("AP");
  KGps_2sig_graph->Draw("P same");
  KGps_chi2_min->Draw("P same");  
  
  if (d0) {can->SaveAs("images/theoryCompare/KGKGp_CL_d0.png");}
  else {can->SaveAs("images/theoryCompare/KGKGp_CL_d.png");}
 
	cout << " KG : chi2 min (+/-) : " << chi2_min_KG  << " chi2 min (+) : " << chi2_plus_min_KG << " -- chi2 min (-) : " << chi2_minus_min_KG << endl;  	
	cout << " KGp : chi2 min (+/-) : " << chi2_min_KGp  << " chi2 min (+) : " << chi2_plus_min_KGp << " -- chi2 min (-) : " << chi2_minus_min_KGp << endl;  	
	
  outfile->Write();
  outfile->Delete();
	
}
