void ANGraphs()
{
  bool large = 1;
  bool shortrange = 0;
  if (large)
    {
      TFile *D0lamfd07file = TFile::Open("dataFiles/d0DecayAsymmetry_10B_lamfdp07.root");
      TFile *D0lamfdpm07file = TFile::Open("dataFiles/d0DecayAsymmetry_10B_lamfdpm07.root");
      TFile *D0lamfd0file = TFile::Open("dataFiles/d0DecayAsymmetry_10B_lamfd0.root");
      TFile *antiD0lamfd07file = TFile::Open("dataFiles/antid0DecayAsymmetry_10B_lamfdp07.root");
      TFile *antiD0lamfdpm07file = TFile::Open("dataFiles/antid0DecayAsymmetry_10B_lamfdpm07.root");
      TFile *antiD0lamfd0file = TFile::Open("dataFiles/antid0DecayAsymmetry_10B_lamfd0.root");
    }
  else
    {
      TFile *D0lamfd07file = TFile::Open("dataFiles/d0DecayAsymmetry_100M_lamfdp07.root");
      TFile *D0lamfdpm07file = TFile::Open("dataFiles/d0DecayAsymmetry_100M_lamfdpm07.root");
      TFile *D0lamfd0file = TFile::Open("dataFiles/d0DecayAsymmetry_100M_lamfd0.root");
      TFile *antiD0lamfd07file = TFile::Open("dataFiles/antid0DecayAsymmetry_100M_lamfdp07.root");
      TFile *antiD0lamfdpm07file = TFile::Open("dataFiles/antid0DecayAsymmetry_100M_lamfdpm07.root");
      TFile *antiD0lamfd0file = TFile::Open("dataFiles/antid0DecayAsymmetry_100M_lamfd0.root");
    }

  // d0 and d0bar theory inputs //
  TGraph *d0fd07 = (TGraph*)D0lamfd07file->Get("d0AN");
  d0fd07->SetName("d0fd07");
  d0fd07->GetYaxis()->SetRangeUser(-0.03, 0.004);
  if (shortrange)
    d0fd07->GetXaxis()->SetLimits(1.0, 4.0);
  d0fd07->SetLineStyle(1);
  d0fd07->SetLineWidth(2);
  TGraph *d0fdpm07 = (TGraph*)D0lamfdpm07file->Get("d0AN");
  d0fdpm07->SetName("d0fdpm07");
  d0fdpm07->GetYaxis()->SetRangeUser(-0.03, 0.004);
  d0fdpm07->SetLineStyle(2);
  d0fdpm07->SetLineWidth(2);
  TGraph *d0fd0 = (TGraph*)D0lamfd0file->Get("d0AN");
  d0fd0->SetName("d0fd0");
  d0fd0->GetYaxis()->SetRangeUser(-0.03, 0.004);
  d0fd0->SetLineStyle(9);
  d0fd0->SetLineWidth(2);


  TGraph *antid0fd07 = (TGraph*)antiD0lamfd07file->Get("d0AN");
  antid0fd07->SetName("antid0fd07");
  antid0fd07->GetYaxis()->SetRangeUser(-0.03, 0.004);
  if (shortrange)
    antid0fd07->GetXaxis()->SetLimits(1.0, 4.0);
  antid0fd07->SetLineStyle(1);
  antid0fd07->SetLineWidth(2);
  TGraph *antid0fdpm07 = (TGraph*)antiD0lamfdpm07file->Get("d0AN");
  antid0fdpm07->SetName("antid0fdpm07");
  antid0fdpm07->GetYaxis()->SetRangeUser(-0.03, 0.004);
  antid0fdpm07->SetLineStyle(2);
  antid0fdpm07->SetLineWidth(2);
  TGraph *antid0fd0 = (TGraph*)antiD0lamfd0file->Get("d0AN");
  antid0fd0->SetName("antid0fd0");
  antid0fd0->GetYaxis()->SetRangeUser(-0.03, 0.004);
  antid0fd0->SetLineStyle(9);
  antid0fd0->SetLineWidth(2);

  // d0 and d0bar fits from simulation //
  TGraph *d0fitfd07 = (TGraph*)D0lamfd07file->Get("d0ANfit");
  d0fitfd07->SetName("d0fitfd07");
  d0fitfd07->GetYaxis()->SetRangeUser(-0.03, 0.004);
  if (shortrange)
    d0fitfd07->GetXaxis()->SetLimits(1.0, 4.0);
  d0fitfd07->SetLineStyle(1);
  d0fitfd07->SetLineWidth(2);
  d0fitfd07->SetLineColor(kBlue);
  TGraph *d0fitfdpm07 = (TGraph*)D0lamfdpm07file->Get("d0ANfit");
  d0fitfdpm07->SetName("d0fitfdpm07");
  d0fitfdpm07->GetYaxis()->SetRangeUser(-0.03, 0.004);
  d0fitfdpm07->SetLineStyle(2);
  d0fitfdpm07->SetLineWidth(2);
  d0fitfdpm07->SetLineColor(kBlue);
  TGraph *d0fitfd0 = (TGraph*)D0lamfd0file->Get("d0ANfit");
  d0fitfd0->SetName("d0fitfd0");
  d0fitfd0->GetYaxis()->SetRangeUser(-0.03, 0.004);
  d0fitfd0->SetLineStyle(9);
  d0fitfd0->SetLineWidth(2);
  d0fitfd0->SetLineColor(kBlue);


  TGraph *antid0fitfd07 = (TGraph*)antiD0lamfd07file->Get("d0ANfit");
  antid0fitfd07->SetName("antid0fitfd07");
  antid0fitfd07->GetYaxis()->SetRangeUser(-0.03, 0.004);
  if (shortrange)
    antid0fitfd07->GetXaxis()->SetLimits(1.0, 4.0);
  antid0fitfd07->SetLineStyle(1);
  antid0fitfd07->SetLineWidth(2);
  antid0fitfd07->SetLineColor(kBlue);
  TGraph *antid0fitfdpm07 = (TGraph*)antiD0lamfdpm07file->Get("d0ANfit");
  antid0fitfdpm07->SetName("antid0fitfdpm07");
  antid0fitfdpm07->GetYaxis()->SetRangeUser(-0.03, 0.004);
  antid0fitfdpm07->SetLineStyle(2);
  antid0fitfdpm07->SetLineWidth(2);
  antid0fitfdpm07->SetLineColor(kBlue);
  TGraph *antid0fitfd0 = (TGraph*)antiD0lamfd0file->Get("d0ANfit");
  antid0fitfd0->SetName("antid0fitfd0");
  antid0fitfd0->GetYaxis()->SetRangeUser(-0.03, 0.004);
  antid0fitfd0->SetLineStyle(9);
  antid0fitfd0->SetLineWidth(2);
  antid0fitfd0->SetLineColor(kBlue);

  // e+ and e- outputs //
  TGraph *d0efd07 = (TGraph*)D0lamfd07file->Get("eANfit");
  d0efd07->SetName("d0efd07");
  d0efd07->GetYaxis()->SetRangeUser(-0.035, 0.035);
  //if (shortrange)
    //d0efd07->GetXaxis()->SetLimits(0.0, 5.0);
  d0efd07->SetLineStyle(1);
  d0efd07->SetLineWidth(2);
  d0efd07->SetLineColor(kRed);
  TGraph *d0efdpm07 = (TGraph*)D0lamfdpm07file->Get("eANfit");
  d0efdpm07->SetName("d0efdpm07");
  d0efdpm07->SetLineStyle(2);
  d0efdpm07->SetLineWidth(2);
  d0efdpm07->SetLineColor(kRed);
  TGraph *d0efd0 = (TGraph*)D0lamfd0file->Get("eANfit");
  d0efd0->SetName("d0efd0");
  d0efd0->SetLineStyle(9);
  d0efd0->SetLineWidth(2);
  d0efd0->SetLineColor(kRed);

  TGraph *antid0efd07 = (TGraph*)antiD0lamfd07file->Get("eANfit");
  antid0efd07->SetName("antid0efd07");
  antid0efd07->GetYaxis()->SetRangeUser(-0.035, 0.035);
  //if (shortrange)
    //antid0efd07->GetXaxis()->SetLimits(1.0, 4.0);
  antid0efd07->SetLineStyle(1);
  antid0efd07->SetLineWidth(2);
  antid0efd07->SetLineColor(kRed);
  TGraph *antid0efdpm07 = (TGraph*)antiD0lamfdpm07file->Get("eANfit");
  antid0efdpm07->SetName("antid0efdpm07");
  antid0efdpm07->SetLineStyle(2);
  antid0efdpm07->SetLineWidth(2);
  antid0efdpm07->SetLineColor(kRed);
  TGraph *antid0efd0 = (TGraph*)antiD0lamfd0file->Get("eANfit");
  antid0efd0->SetName("antid0efd0");
  antid0efd0->SetLineStyle(9);
  antid0efd0->SetLineWidth(2);
  antid0efd0->SetLineColor(kRed);


  TCanvas c0("c0");
  c0.Divide(2,1);
  c0.cd(1);
  d0fd07->Draw("AL");
  d0fdpm07->Draw("SAME");
  d0fd0->Draw("SAME");
  c0.cd(2);
  antid0fd07->Draw("AL");
  antid0fdpm07->Draw("SAME");
  antid0fd0->Draw("SAME");
  if (shortrange)
    c0.SaveAs("d0ShortRange.png");
  else
    c0.SaveAs("d0FullRange.png");

  TCanvas c1("c1");
  c1.Divide(2,1);
  c1.cd(1);
  d0fitfd07->Draw("AL");
  d0fitfdpm07->Draw("SAME");
  d0fitfd0->Draw("SAME");
  c1.cd(2);
  antid0fitfd07->Draw("AL");
  antid0fitfdpm07->Draw("SAME");
  antid0fitfd0->Draw("SAME");
  if (shortrange)
    c1.SaveAs("d0fitShortRange.png");
  else
    c1.SaveAs("d0fitFullRange.png");

  TCanvas c2("c2");
  c2.Divide(2,1);
  c2.cd(1);
  d0efd07->Draw("AL");
  d0efdpm07->Draw("SAME");
  d0efd0->Draw("SAME");
  c2.cd(2);
  antid0efd07->Draw("AL");
  antid0efdpm07->Draw("SAME");
  antid0efd0->Draw("SAME");
  c2.SaveAs("d0eAN.png");

  TFile *outfile = new TFile("dataFiles/TheoryCurves.root","RECREATE");
  outfile->cd();

  d0fd07->Write();
  d0fdpm07->Write();
  d0fd0->Write();
  antid0fd07->Write();
  antid0fdpm07->Write();
  antid0fd0->Write();

  d0fitfd07->Write();
  d0fitfdpm07->Write();
  d0fitfd0->Write();
  antid0fitfd07->Write();
  antid0fitfdpm07->Write();
  antid0fitfd0->Write();

  d0efd07->Write();
  d0efdpm07->Write();
  d0efd0->Write();
  antid0efd07->Write();
  antid0efdpm07->Write();
  antid0efd0->Write();

  d0fd07->Delete();
  d0fdpm07->Delete();
  d0fd0->Delete();
  antid0fd07->Delete();
  antid0fdpm07->Delete();
  antid0fd0->Delete();

  d0fitfd07->Delete();
  d0fitfdpm07->Delete();
  d0fitfd0->Delete();
  antid0fitfd07->Delete();
  antid0fitfdpm07->Delete();
  antid0fitfd0->Delete();

  d0efd07->Delete();
  d0efdpm07->Delete();
  d0efd0->Delete();
  antid0efd07->Delete();
  antid0efdpm07->Delete();
  antid0efd0->Delete();

  outfile->Write();
  outfile->Close();
}
