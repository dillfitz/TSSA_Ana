#include <algorithm>
using namespace std;

void backgroundCorrectionSystematicsPlusMinus()
{

  const int nbins = 6;
  
	TFile *infile_data_plus = TFile::Open("../dataFiles/bgCorrected_ohfe_AN_plus.root");
	TFile *infile_data_minus = TFile::Open("../dataFiles/bgCorrected_ohfe_AN_minus.root");
	TGraphAsymmErrors *ohfeAN_plus = (TGraphAsymmErrors*)infile_data_plus->Get("ohfeAN");
	TGraphAsymmErrors *ohfeAN_minus = (TGraphAsymmErrors*)infile_data_minus->Get("ohfeAN");
	TGraphAsymmErrors *ohfeAN_plus_sys = (TGraphAsymmErrors*)infile_data_plus->Get("ohfeAN");
	TGraphAsymmErrors *ohfeAN_minus_sys = (TGraphAsymmErrors*)infile_data_minus->Get("ohfeAN");	
	
  double sys_tot_plus_upper[nbins] = {0.0033, 0.00211, 0.00134, 0.0071, 0.00473, 0.00688};
	double sys_tot_plus_lower[nbins] = {0.00281, 0.00189, 0.00132, 0.00708, 0.00465, 0.00501};
	double sys_tot_minus_upper[nbins] = {0.00474, 0.00502, 0.00209, 0.00176, 0.00269, 0.00541};
	double sys_tot_minus_lower[nbins] = {0.00343, 0.00384, 0.00191, 0.00149, 0.00269, 0.00363};

    cout << "sys diff table plus" << endl;
    cout << std::setprecision(3) << "        \\hline" << endl;
    cout << "        1.0 -- 1.3 & 1.161  & " << ohfeAN_plus_sys->GetErrorYhigh(0) << " & "  <<  sqrt(pow(ohfeAN_plus_sys->GetErrorYhigh(0),2) + pow(max(sys_tot_plus_upper[0], sys_tot_plus_lower[0]),2)) <<  " \\\\ " << endl;
    cout << "        1.3 -- 1.5 & 1.398  & " << ohfeAN_plus_sys->GetErrorYhigh(1) << " & "  <<  sqrt(pow(ohfeAN_plus_sys->GetErrorYhigh(1),2) + pow(max(sys_tot_plus_upper[1], sys_tot_plus_lower[1]),2)) <<  " \\\\ " << endl;
    cout << "        1.5 -- 1.8 & 1.639  & " << ohfeAN_plus_sys->GetErrorYhigh(2) << " & "  <<  sqrt(pow(ohfeAN_plus_sys->GetErrorYhigh(2),2) + pow(max(sys_tot_plus_upper[2], sys_tot_plus_lower[2]),2)) <<  " \\\\ " << endl;
    cout << "        1.8 -- 2.1 & 1.936  & " << ohfeAN_plus_sys->GetErrorYhigh(3) << " & "  <<  sqrt(pow(ohfeAN_plus_sys->GetErrorYhigh(3),2) + pow(max(sys_tot_plus_upper[3], sys_tot_plus_lower[3]),2)) <<  " \\\\ " << endl;
    cout << "        2.1 -- 2.7 & 2.349  & " << ohfeAN_plus_sys->GetErrorYhigh(4) << " & "  <<  sqrt(pow(ohfeAN_plus_sys->GetErrorYhigh(4),2) + pow(max(sys_tot_plus_upper[4], sys_tot_plus_lower[4]),2)) <<  " \\\\ " << endl;
    cout << "        2.7 -- 5.0 & 3.290  & " << ohfeAN_plus_sys->GetErrorYhigh(5) << " & "  <<  sqrt(pow(ohfeAN_plus_sys->GetErrorYhigh(5),2) + pow(max(sys_tot_plus_upper[5], sys_tot_plus_lower[5]),2)) <<  " \\\\ " << endl;
    cout << "        \\hline" << endl;		
    
    cout << "sys diff table minus" << endl;
    cout << std::setprecision(3) << "        \\hline" << endl;
    cout << "        1.0 -- 1.3 & 1.161  & " << ohfeAN_minus_sys->GetErrorYhigh(0) << " & "  <<  sqrt(pow(ohfeAN_minus_sys->GetErrorYhigh(0),2) + pow(max(sys_tot_minus_upper[0], sys_tot_minus_lower[0]),2)) <<  " \\\\ " << endl;
    cout << "        1.3 -- 1.5 & 1.398  & " << ohfeAN_minus_sys->GetErrorYhigh(1) << " & "  <<  sqrt(pow(ohfeAN_minus_sys->GetErrorYhigh(1),2) + pow(max(sys_tot_minus_upper[1], sys_tot_minus_lower[1]),2)) <<  " \\\\ " << endl;
    cout << "        1.5 -- 1.8 & 1.639  & " << ohfeAN_minus_sys->GetErrorYhigh(2) << " & "  <<  sqrt(pow(ohfeAN_minus_sys->GetErrorYhigh(2),2) + pow(max(sys_tot_minus_upper[2], sys_tot_minus_lower[2]),2)) <<  " \\\\ " << endl;
    cout << "        1.8 -- 2.1 & 1.936  & " << ohfeAN_minus_sys->GetErrorYhigh(3) << " & "  <<  sqrt(pow(ohfeAN_minus_sys->GetErrorYhigh(3),2) + pow(max(sys_tot_minus_upper[3], sys_tot_minus_lower[3]),2)) <<  " \\\\ " << endl;
    cout << "        2.1 -- 2.7 & 2.349  & " << ohfeAN_minus_sys->GetErrorYhigh(4) << " & "  <<  sqrt(pow(ohfeAN_minus_sys->GetErrorYhigh(4),2) + pow(max(sys_tot_minus_upper[4], sys_tot_minus_lower[4]),2)) <<  " \\\\ " << endl;
    cout << "        2.7 -- 5.0 & 3.290  & " << ohfeAN_minus_sys->GetErrorYhigh(5) << " & "  <<  sqrt(pow(ohfeAN_minus_sys->GetErrorYhigh(5),2) + pow(max(sys_tot_minus_upper[5], sys_tot_minus_lower[5]),2)) <<  " \\\\ " << endl;
    cout << "        \\hline" << endl;		      
    
    

	for (int i=0; i<nbins; ++i)
	{
		cout << "plus stat: " << ohfeAN_plus_sys->GetErrorYhigh(i) << " plus sqrt(stat^2 + sys^2): " << sqrt(pow(ohfeAN_plus_sys->GetErrorYhigh(i),2) + pow(max(sys_tot_plus_upper[i], sys_tot_plus_lower[i]),2)) << endl;
		cout << "minus stat: " << ohfeAN_minus_sys->GetErrorYhigh(i) << " minus sqrt(stat^2 + sys^2): " << sqrt(pow(ohfeAN_minus_sys->GetErrorYhigh(i),2) + pow(max(sys_tot_minus_upper[i], sys_tot_minus_lower[i]),2)) << endl;	
		ohfeAN_plus_sys->SetPointError(i,0.075, 0.075, sys_tot_plus_lower[i], sys_tot_plus_upper[i]); 
		ohfeAN_minus_sys->SetPointError(i,0.075, 0.075, sys_tot_minus_lower[i], sys_tot_minus_upper[i]); 		
	}
	
	TCanvas *can = new TCanvas("can");
	can->SetLeftMargin(0.125);
	can->SetRightMargin(0.01);	
	can->SetBottomMargin(0.15);
	can->SetTopMargin(0.01);		
	ohfeAN_plus->SetMarkerColor(kRed);	
	ohfeAN_plus->SetLineColor(kRed);	
	ohfeAN_plus->SetMarkerStyle(20);	
	ohfeAN_plus->SetMarkerSize(1.1);
  ohfeAN_plus->GetYaxis()->SetRangeUser(-0.068,0.084);
	ohfeAN_plus->GetXaxis()->SetLimits(0.8,8.);
	ohfeAN_plus->GetXaxis()->SetLabelSize(0.06);
	ohfeAN_plus->GetXaxis()->SetTitleSize(0.06);	
	ohfeAN_plus->GetYaxis()->SetLabelSize(0.06);
	ohfeAN_plus->GetYaxis()->SetTitleSize(0.06);
	ohfeAN_plus->GetYaxis()->SetTitleOffset(1.05);							
	ohfeAN_plus->Draw("AP");	
	
	ohfeAN_plus_sys->SetMarkerColor(kRed);	
	ohfeAN_plus_sys->SetLineColor(kRed);	
	ohfeAN_plus_sys->SetMarkerStyle(20);	
	ohfeAN_plus_sys->SetMarkerSize(1.1);
	ohfeAN_plus_sys->SetFillColor(kRed);	
	ohfeAN_plus_sys->SetFillStyle(3004);	
	ohfeAN_plus_sys->Draw("5");

	ohfeAN_minus->SetMarkerStyle(21);	
	ohfeAN_minus->SetMarkerSize(1.1);	
	ohfeAN_minus->Draw("P same");
	
	ohfeAN_minus_sys->SetMarkerStyle(21);	
	ohfeAN_minus_sys->SetMarkerSize(1.1);	
	ohfeAN_minus_sys->SetFillStyle(3005);		
	ohfeAN_minus_sys->SetFillColor(kBlue);		
	ohfeAN_minus_sys->Draw("5");	
	
	TF1 *line = new TF1( "line", "0", 0, 20 );
	line->SetLineColor( kBlack );
	line->SetLineStyle( 3 );

	TLegend *legend = new TLegend( 0.45, 0.5, 0.82, 0.95 );
	legend->SetLineColor(0);
	legend->SetTextSize(0.06);
	legend->AddEntry(ohfeAN_plus, "Open Heavy Flavor e^{+}", "p" );
	legend->AddEntry((TObject*)0, "", "");	
  legend->AddEntry(ohfeAN_minus, "Open Heavy Flavor e^{-}", "p" );
	legend->AddEntry((TObject*)0, "", "");  



	TLegend *legend2 = new TLegend( 0.16, 0.7, 0.325, 0.98 );
	legend2->SetLineColor(0);
	legend2->AddEntry((TObject*)0, "p^{#uparrow}+p #rightarrow e^{+/-} + X", "");
	legend2->AddEntry((TObject*)0, "#sqrt{s} = 200 GeV", "");	
	legend2->AddEntry((TObject*)0, "|#eta| < 0.35", "");		
	legend2->AddEntry((TObject*)0, "#bf{PHENIX}", "");			
	legend2->SetMargin(0.1);
	legend2->SetTextSize(0.06);
	legend2->SetMargin(0.05);


	TLegend *legend3 = new TLegend( 0.2, 0.16, 0.98, 0.25 );
	legend3->SetLineColor(0);
	legend3->AddEntry((TObject*)0, "3.4% polarization scale uncertainty not included", "");	
	//legend3->AddEntry((TObject*)0, "uncertainty not included", "");
	
	legend3->SetTextSize(0.055);
	legend3->SetMargin(0.05);

	line->Draw( "same" );
	legend->Draw();
  legend2->Draw();
	legend3->Draw();


	  TFile *theoryFile_plus = TFile::Open("inputFiles/d0DecayAsymmetry_fitted_10B_weightsLoopZoomed.root");	 
	  TGraph *d0efdbestfit = (TGraphErrors*)theoryFile_plus->Get("eAN_m0p01_0p11");
	  //TGraph *d0efdp1sig = (TGraphErrors*)theoryFile->Get("eAN_0p02_0p2");
	  //TGraph *d0efdm1sig = (TGraphErrors*)theoryFile->Get("eAN_m0p04_0p02");
	
	  TFile *theoryFileKGKGp_plus = TFile::Open("inputFiles/dcombinedDecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");	 	  
	  TGraph *deKGbestfit = (TGraphErrors*)theoryFileKGKGp_plus->Get("eAN_KG_0p0006");
	  TGraph *deKGpbestfit = (TGraphErrors*)theoryFileKGKGp_plus->Get("eAN_KGp_0p00025");	  	
	  
	  TFile *theoryFile_minus = TFile::Open("inputFiles/antid0DecayAsymmetry_fitted_10B_weightsLoopZoomed.root");	 
	  TGraph *antid0efdbestfit = (TGraphErrors*)theoryFile_minus->Get("eAN_m0p01_0p11");
	  //TGraph *d0efdp1sig = (TGraphErrors*)theoryFile->Get("eAN_0p02_0p2");
	  //TGraph *d0efdm1sig = (TGraphErrors*)theoryFile->Get("eAN_m0p04_0p02");
	
	  TFile *theoryFileKGKGp_minus = TFile::Open("inputFiles/antidcombinedDecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");	 	  
	  TGraph *antideKGbestfit = (TGraphErrors*)theoryFileKGKGp_minus->Get("eAN_KG_0p0006");
	  TGraph *antideKGpbestfit = (TGraphErrors*)theoryFileKGKGp_minus->Get("eAN_KGp_0p0002");	    	  

    d0efdbestfit->SetLineWidth(2);
    d0efdbestfit->SetLineColor(kRed);
	  d0efdbestfit->Draw("SAME");
	  
    deKGbestfit->SetLineWidth(2);
    deKGbestfit->SetLineColor(kRed);  
    deKGbestfit->SetLineStyle(7);   	      	  
	  deKGbestfit->Draw("SAME");
	  
    deKGpbestfit->SetLineWidth(2);
    deKGpbestfit->SetLineColor(kRed);   
    deKGpbestfit->SetLineStyle(3);   	      	  
	  deKGpbestfit->Draw("SAME");	

	  legend->AddEntry((TObject*)0, "", "");	
	  legend->AddEntry((TObject*)0, "", "");	 
	  legend->AddEntry((TObject*)0, "", "");	  	   	    	   	   	   	    
	  legend->AddEntry((TObject*)0, "PRD78, 114013",""); 
	  legend->AddEntry((TObject*)0, "", "");
	  //legend->AddEntry((TObject*)0, "D^{0} #rightarrow e^{+}",""); 	  
	  legend->AddEntry(d0efdbestfit, "(#lambda_{f}, #lambda_{d}) = (-0.01, 0.11) GeV","l");	
	  legend->AddEntry((TObject*)0, "", "");	  	  
	  //legend->AddEntry((TObject*)0, "#bar{D}^{0} #rightarrow e^{-}",""); 	 
	  legend->AddEntry(antid0efdbestfit, "(#lambda_{f}, #lambda_{d}) = (-0.01, 0.11) GeV","l");		  
	  legend->AddEntry((TObject*)0, "", "");	 
	  legend->AddEntry((TObject*)0, "", "");	
	  legend->AddEntry((TObject*)0, "", "");	
	  legend->AddEntry((TObject*)0, "", "");	  	   	    	   	    	   	    	   
	  legend->AddEntry((TObject*)0, "PRD84, 014026", "") ;
	  //legend->AddEntry((TObject*)0, "(D^{0}+D^{+}) #rightarrow e^{+}","");
	  TLegend *legend_continued = new TLegend(0.37, 0.3, 0.98, 0.5);
	  legend_continued->SetBorderSize(0);
	  legend_continued->SetTextSize(0.06);
	  legend_continued->SetNColumns(2);
	  legend_continued->AddEntry(deKGbestfit, "K_{G} = 6.0x10^{-4}","l");	
	  legend_continued->AddEntry(deKGpbestfit, "K_{G}' = 2.5x10^{-4}","l");	
	  //legend->AddEntry((TObject*)0, "(#bar{D}^{0}+D^{-}) #rightarrow e^{-}","");	
	  legend_continued->AddEntry(antideKGbestfit, "K_{G} = 6.0x10^{-4}","l");
	  legend_continued->AddEntry(antideKGpbestfit, "K_{G}' = 2.5x10^{-4}","l");			  
	  legend_continued->Draw();	  	  

    antid0efdbestfit->SetLineWidth(2);
    antid0efdbestfit->SetLineColor(kBlue);
	  antid0efdbestfit->Draw("SAME");
	  
    antideKGbestfit->SetLineWidth(2);
    antideKGbestfit->SetLineColor(kBlue); 
    antideKGbestfit->SetLineStyle(7);   	      	      
	  antideKGbestfit->Draw("SAME");
	  
    antideKGpbestfit->SetLineWidth(2);
    antideKGpbestfit->SetLineColor(kBlue); 
    antideKGpbestfit->SetLineStyle(3);   	      	        	        	  
	  antideKGpbestfit->Draw("SAME");	
	  
	  can->SaveAs("./images/AN_sys_theory.png");
	  can->SaveAs("./images/AN_sys_theory.pdf");
	  	  	 	
}
