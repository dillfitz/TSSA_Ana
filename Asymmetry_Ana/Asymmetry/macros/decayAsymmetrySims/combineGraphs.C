void combineGraphs()
{
  const int numSecs = 10;
  const int N_iter = 100;
  TString name = "";
  // stat error only //
  TCanvas can, can_plus, can_minus;
  for (int i=0; i<numSecs; ++i)
  {
    name = "dataFiles/lamParamChi2Zoomed_variations_";
    name += N_iter;
    name += "_";
    name += i;
    name += ".root";
    TFile *infile = TFile::Open(name);
    
    TGraph *lamparam = (TGraph*)infile->Get("lamparam");
    TGraph *lamparam_plus = (TGraph*)infile->Get("lamparam_plus");
    TGraph *lamparam_minus = (TGraph*)infile->Get("lamparam_minus");        
    
    can.cd();
    if (i==0) { lamparam->Draw("AP"); }
    else { lamparam->Draw("P same"); }
    
    can_plus.cd();
    if (i==0) { lamparam_plus->Draw("AP"); }
    else { lamparam_plus->Draw("P same"); }   
    
    can_minus.cd();
    if (i==0) { lamparam_minus->Draw("AP"); }
    else { lamparam_minus->Draw("P same"); }        
  }  
  // stat + sys error in random sampling procedure //    
  TCanvas can_wSys, can_wSys_plus, can_wSys_minus;
  for (int i=0; i<numSecs; ++i)
  {
    name = "dataFiles/lamParamChi2Zoomed_variations_wSys_";
    name += N_iter;
    name += "_";
    name += i;
    name += ".root";
    TFile *infile = TFile::Open(name);
    
    TGraph *lamparam = (TGraph*)infile->Get("lamparam");
    TGraph *lamparam_plus = (TGraph*)infile->Get("lamparam_plus");
    TGraph *lamparam_minus = (TGraph*)infile->Get("lamparam_minus");        
    
    can_wSys.cd();
    if (i==0) { lamparam->Draw("AP"); }
    else { lamparam->Draw("P same"); }
    
    can_wSys_plus.cd();
    if (i==0) { lamparam_plus->Draw("AP"); }
    else { lamparam_plus->Draw("P same"); }   
    
    can_wSys_minus.cd();
    if (i==0) { lamparam_minus->Draw("AP"); }
    else { lamparam_minus->Draw("P same"); }        
  }  
  
  // stat + sys error in random sampling procedure and denominator of chi2 statistic //      
  TCanvas can_wSysDenom, can_wSysDenom_plus, can_wSysDenom_minus;
  for (int i=0; i<numSecs; ++i)
  {
    name = "dataFiles/lamParamChi2Zoomed_variations_wSysDenom_";
    name += N_iter;
    name += "_";
    name += i;
    name += ".root";
    TFile *infile = TFile::Open(name);
    
    TGraph *lamparam = (TGraph*)infile->Get("lamparam");
    TGraph *lamparam_plus = (TGraph*)infile->Get("lamparam_plus");
    TGraph *lamparam_minus = (TGraph*)infile->Get("lamparam_minus");        
    
    can_wSysDenom.cd();
    if (i==0) { lamparam->Draw("AP"); }
    else { lamparam->Draw("P same"); }
    
    can_wSysDenom_plus.cd();
    if (i==0) { lamparam_plus->Draw("AP"); }
    else { lamparam_plus->Draw("P same"); }   
    
    can_wSysDenom_minus.cd();
    if (i==0) { lamparam_minus->Draw("AP"); }
    else { lamparam_minus->Draw("P same"); }        
  }      
  name = "images/theoryCompare/lamParamChi2Zoomed_variations_";
  name += N_iter;
  name += ".png";  
  can.SaveAs(name);
 
  name = "images/theoryCompare/lamParamChi2Zoomed_variations_plus_";
  name += N_iter;
  name += ".png";
  can_plus.SaveAs(name);

  name = "images/theoryCompare/lamParamChi2Zoomed_variations_minus_";
  name += N_iter;
  name += ".png";  
  can_minus.SaveAs(name);   
  
  name = "images/theoryCompare/lamParamChi2Zoomed_variations_wSys_";
  name += N_iter;
  name += ".png";  
  can_wSys.SaveAs(name);
 
  name = "images/theoryCompare/lamParamChi2Zoomed_variations_wSys_plus_";
  name += N_iter;
  name += ".png";
  can_wSys_plus.SaveAs(name);

  name = "images/theoryCompare/lamParamChi2Zoomed_variations_wSys_minus_";
  name += N_iter;
  name += ".png";  
  can_wSys_minus.SaveAs(name);  
  
  name = "images/theoryCompare/lamParamChi2Zoomed_variations_wSysDenom_";
  name += N_iter;
  name += ".png";  
  can_wSysDenom.SaveAs(name);
 
  name = "images/theoryCompare/lamParamChi2Zoomed_variations_wSysDenom_plus_";
  name += N_iter;
  name += ".png";
  can_wSysDenom_plus.SaveAs(name);

  name = "images/theoryCompare/lamParamChi2Zoomed_variations_wSysDenom_minus_";
  name += N_iter;
  name += ".png";  
  can_wSysDenom_minus.SaveAs(name);       
}
