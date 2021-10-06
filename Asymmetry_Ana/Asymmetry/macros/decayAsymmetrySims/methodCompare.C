void methodCompare()
{
  TFile *infileOld = TFile::Open("dataFiles/TheoryCurves.root");
  TFile *infileD0 = TFile::Open("dataFiles/d0DecayAsymmetry_fitted_10B_weightsLoop.root");
  //TFile *infileD0 = TFile::Open("dataFiles/d0DecayAsymmetry_weightsLoop_100M.root");  
  TFile *infileD0bar = TFile::Open("dataFiles/antid0DecayAsymmetry_fitted_10B_weightsLoop.root"); 
  //TFile *infileD0bar = TFile::Open("dataFiles/antid0DecayAsymmetry_fitted_7B_weightsLoop.root");     
  
  TGraph *antid0efd0 = (TGraphErrors*)infileOld->Get("antid0efd0");
  TGraph *antid0efd07 = (TGraphErrors*)infileOld->Get("antid0efd07");
  TGraph *antid0efdpm07 = (TGraphErrors*)infileOld->Get("antid0efdpm07");
    
  TGraph *d0efd0 = (TGraphErrors*)infileOld->Get("d0efd0");
  TGraph *d0efd07 = (TGraphErrors*)infileOld->Get("d0efd07");
  TGraph *d0efdpm07 = (TGraphErrors*)infileOld->Get("d0efdpm07");
    
  TGraph *d0eAN_0_0 = (TGraphErrors*)infileD0->Get("eAN_0_0");
  TGraph *d0eAN_0p07_0p07 = (TGraphErrors*)infileD0->Get("eAN_0p07_0p07");
  TGraph *d0eAN_0p07_m0p07 = (TGraphErrors*)infileD0->Get("eAN_0p07_m0p07");  

  TGraph *antid0eAN_0_0 = (TGraphErrors*)infileD0bar->Get("eAN_0_0");
  TGraph *antid0eAN_0p07_0p07 = (TGraphErrors*)infileD0bar->Get("eAN_0p07_0p07");
  TGraph *antid0eAN_0p07_m0p07 = (TGraphErrors*)infileD0bar->Get("eAN_0p07_m0p07");  
      
  TCanvas *can = new TCanvas(); 
  d0efd0->SetTitle("D^{0} #rightarrow e^{+}");     
  d0efd0->Draw();
  d0eAN_0_0->SetMarkerColor(kBlue);  
  d0eAN_0_0->SetLineColor(kBlue);
  d0eAN_0_0->SetLineWidth(2);
  d0eAN_0_0->SetLineStyle(9);
  d0eAN_0_0->Draw("PL same");
  
  d0efd07->Draw("PL same");
  d0eAN_0p07_0p07->SetMarkerColor(kBlue);  
  d0eAN_0p07_0p07->SetLineColor(kBlue);
  d0eAN_0p07_0p07->SetLineWidth(2);
  //d0eAN_0p07_0p07->SetLineStyle(9);
  d0eAN_0p07_0p07->Draw("PL same");  
  
  d0efdpm07->Draw("PL same");
  d0eAN_0p07_m0p07->SetMarkerColor(kBlue);    
  d0eAN_0p07_m0p07->SetLineColor(kBlue);
  d0eAN_0p07_m0p07->SetLineWidth(2);
  d0eAN_0p07_m0p07->SetLineStyle(2);
  d0eAN_0p07_m0p07->Draw("PL same");  
  
  TLegend *leg = new TLegend(0.6, 0.6, 0.9, 0.9);
  leg->AddEntry(d0efd0, "#lambda_{f} = #lambda_{d} = 0.0 GeV (old)", "epl");
  leg->AddEntry(d0eAN_0_0, "#lambda_{f} = #lambda_{d} = 0.0 GeV (new)", "epl");  
  leg->AddEntry(d0efd07, "#lambda_{f} = #lambda_{d} = 0.07 GeV (old)", "epl");
  leg->AddEntry(d0eAN_0p07_0p07, "#lambda_{f} = #lambda_{d} = 0.07 GeV (new)", "epl");       
  leg->AddEntry(d0efdpm07, "#lambda_{f} = -#lambda_{d} = 0.07 GeV (old)", "epl");   
  leg->AddEntry(d0eAN_0p07_m0p07, "#lambda_{f} = -#lambda_{d} = 0.07 GeV (new)", "epl");     
  leg->Draw("same");   
  
  can->SaveAs("images/theoryCompare/D0theoryCurveMethods.png");
    
  
  TCanvas *can1 = new TCanvas();   
  antid0efd0->SetTitle("#bar{D}^{0} #rightarrow e^{-}");          
  antid0efd0->Draw();
  antid0eAN_0_0->SetMarkerColor(kBlue);  
  antid0eAN_0_0->SetLineColor(kBlue);
  antid0eAN_0_0->SetLineWidth(2);
  antid0eAN_0_0->SetLineStyle(9);
  antid0eAN_0_0->Draw("PL same");
  
  antid0efd07->Draw("PL same");
  antid0eAN_0p07_0p07->SetMarkerColor(kBlue);
  antid0eAN_0p07_0p07->SetLineColor(kBlue);
  antid0eAN_0p07_0p07->SetLineWidth(2);
  //d0eAN_0p07_0p07->SetLineStyle(9);
  antid0eAN_0p07_0p07->Draw("PL same");  
  
  antid0efdpm07->Draw("PL same");
  antid0eAN_0p07_m0p07->SetMarkerColor(kBlue);  
  antid0eAN_0p07_m0p07->SetLineColor(kBlue);
  antid0eAN_0p07_m0p07->SetLineWidth(2);
  antid0eAN_0p07_m0p07->SetLineStyle(2);
  antid0eAN_0p07_m0p07->Draw("PL same");    
  
  TLegend *leg = new TLegend(0.6, 0.6, 0.9, 0.9);
  leg->AddEntry(antid0efd0, "#lambda_{f} = #lambda_{d} = 0.0 GeV (old)", "epl");
  leg->AddEntry(antid0eAN_0_0, "#lambda_{f} = #lambda_{d} = 0.0 GeV (new)", "epl");  
  leg->AddEntry(antid0efd07, "#lambda_{f} = #lambda_{d} = 0.07 GeV (old)", "epl");  
  leg->AddEntry(antid0eAN_0p07_0p07, "#lambda_{f} = #lambda_{d} = 0.07 GeV (new)", "epl");   
  leg->AddEntry(antid0efdpm07, "#lambda_{f} = -#lambda_{d} = 0.07 GeV (old)", "epl");   
  leg->AddEntry(antid0eAN_0p07_m0p07, "#lambda_{f} = -#lambda_{d} = 0.07 GeV (new)", "epl");     
  leg->Draw("same");  
  
  can1->SaveAs("images/theoryCompare/antiD0theoryCurveMethods.png");
    
}
