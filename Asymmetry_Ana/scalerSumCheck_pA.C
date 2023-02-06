void scalerSumCheck_pA(const char* collisionSystem = "pAl", const bool oldCheck = false, const bool removeRuns = true)
{
  const int Nxings = 120;
  TString inputFileName = "../";
  inputFileName += collisionSystem;
  inputFileName += "_goodruns_ana664";
  
  if (oldCheck)
  {
    inputFileName += "_old";
  }
  inputFileName += ".root";
  
  int fillNumber, runNumber, xingShift;
  float polb;
  int spinBlue[ Nxings ];
  Long64_t triggerCounts[ Nxings ];
  for( int i = 0; i < Nxings; i++ )
    triggerCounts[i] = spinBlue[i] = 0;  
  
  TFile *infile = TFile::Open(inputFileName);
  TTree *pol_tree = (TTree*)infile->Get("pol_tree");
  pol_tree->SetBranchAddress( "fillnumber",    &fillNumber );
  pol_tree->SetBranchAddress( "run",           &runNumber );
  pol_tree->SetBranchAddress( "xing_shift",    &xingShift ); 
  pol_tree->SetBranchAddress( "gl1pScalerCh0", triggerCounts );
  //  pol_tree->SetBranchAddress( "gl1pScalerCh1", triggerCounts );
  pol_tree->SetBranchAddress( "spin_blue",     spinBlue );
  pol_tree->SetBranchAddress( "polb",          &polb);  
  

  int lastrun = -999;
  int Nentries = pol_tree->GetEntries();
  TGraph *countGraph = new TGraph(Nentries);
  for (int i=0; i<pol_tree->GetEntries(); ++i)
  { 
    pol_tree->GetEntry(i);
    if (removeRuns)
      if (runNumber == 435361 || runNumber == 438153) ) {continue;}    

    if (runNumber == lastrun)
      cout << "found one" << runNumber << " " << totCounts << endl;
      
    else
    {
      long totCounts = 0;
    }
    for (int xing=0; xing<Nxings; ++xing)
    {
      totCounts += triggerCounts[xing];    
    }
    
    //double run = runNumber;
    //double counts = totCounts;
    
    cout << " run : " << runNumber << " counts : " << totCounts << endl;
    countGraph->SetPoint(i, runNumber, totCounts);
    //countGraph->GetYaxis()->SetRangeUser(100000000, 100000000000);

      
    lastrun = runNumber;
  }
  countGraph->SetMarkerStyle(8);
  countGraph->SetMarkerSize(1.2);
  if (collisionSystem == "pAl")
    countGraph->GetXaxis()->SetLimits(436500,438500);  
  else if (collisionSystem == "pAu")
    countGraph->GetXaxis()->SetLimits(432500, 436500);
    
  countGraph->SetTitle("");  
  countGraph->GetXaxis()->SetTitle("run number");
  countGraph->GetYaxis()->SetTitle("gl1pscalerch0 sum");

  
  TCanvas *can = new TCanvas();
  can->SetLogy(1);
  countGraph->Draw("AP");
  TString imgName = collisionSystem;
  imgName += "_gl1pscalerch0Sums";
  if (removeRuns) 
  {
    imgName += "_removedRuns";
  }
  imgName += ".png";
    
  can->SaveAs(imgName);  
}
