#include <vector>
#include <pair>
#include "TGraph"

void ERTeff_QA_pA(const char* particle = "eta", const char* collisionSystem = "pAu")
{

  
  if (particle == "eta")
  {
    double lowermass = 0.480;  
    double uppermass = 0.620;
  }
  if (particle == "pi0")
  {
    double lowermass = 0.112;
    double uppermass = 0.162;
  }
  TString inputFileName = "../";
  inputFileName += collisionSystem;
  inputFileName += "_goodruns_ana664.root";
  
  
  int runEventTree, numERTCEvents, numERTAEvents, numERTBEvents;
  float invmass;
  
  TFile *infile = TFile::Open(inputFileName);
  TTree *event_tree = (TTree*)infile->Get("events_tree");
  event_tree->SetBranchAddress( "run",           &runEventTree );
  event_tree->SetBranchAddress( "numERTCEvents",    &numERTCEvents );  
  event_tree->SetBranchAddress( "numERTAEvents",    &numERTAEvents );    
  event_tree->SetBranchAddress( "numERTBEvents",    &numERTBEvents );  
  
  //const int numRuns = event_tree->GetEntries();
  std::vector<std::pair<int, int>> ERTCEventVec; 
  std::vector<std::pair<int, int>> ERTAEventVec;   
  std::vector<std::pair<int, int>> ERTBEventVec;     
  int lastrun = -999;
  int lastNumERTBEvents = -999;
  int lastNumERTAEvents = -999;
  int lastNumERTCEvents = -999;    
  int Nentries = pol_tree->GetEntries();
  TGraph *countGraph = new TGraph(Nentries);
  cout << "loop 1" << endl;
  for (int i=0; i<event_tree->GetEntries(); ++i)
  { 
    event_tree->GetEntry(i); 
    if (runEventTree == lastrun)
    {
      cout << "found one " << runEventTree << endl;
      numERTBEvents += lastNumERTBEvents;
      numERTAEvents += lastNumERTAEvents;
      numERTCEvents += lastNumERTCEvents;            
    } 
    else
    {      
      ERTBEventVec.push_back(make_pair(numERTBEvents, runEventTree));         
      ERTAEventVec.push_back(make_pair(numERTAEvents, runEventTree));
      ERTCEventVec.push_back(make_pair(numERTCEvents, runEventTree));
    }
    
    lastrun = runEventTree;
    lastNumERTBEvents = numERTBEvents;
    lastNumERTAEvents = numERTAEvents;
    lastNumERTCEvents = numERTCEvents;        
  }
  
  int runDataTree, scaledERT_4x4c, scaledERT_4x4a, scaledERT_4x4b;
  float invmass;
  TTree *data_tree = (TTree*)infile->Get("pair_tree");
  data_tree->SetBranchAddress( "run",           &runDataTree);
  data_tree->SetBranchAddress( "scaledERT_4x4c",    &scaledERT_4x4c );  
  data_tree->SetBranchAddress( "scaledERT_4x4a",    &scaledERT_4x4a );  
  data_tree->SetBranchAddress( "scaledERT_4x4b",    &scaledERT_4x4b );  
  data_tree->SetBranchAddress( "invmass",           &invmass );
 
  std::vector<std::pair<int, int>> ERTCPairVec; 
  std::vector<std::pair<int, int>> ERTAPairVec;   
  std::vector<std::pair<int, int>> ERTBPairVec;      
  int ERTC_pairs, ERTA_pairs, ERTB_pairs;
  int lastRunData = -999;  
  
  cout << "loop 2" << endl;
  for (int i=0; i<data_tree->GetEntries(); ++i)
  {
    data_tree->GetEntry(i);
    if (lastRunData != runDataTree && i !=0)
    {
        //cout << " lastRunData : " << lastRunData << " runDataTree : " << runDataTree << endl;
        ERTBPairVec.push_back(make_pair(ERTB_pairs, lastRunData));                
        ERTAPairVec.push_back(make_pair(ERTA_pairs, lastRunData));        
        ERTCPairVec.push_back(make_pair(ERTC_pairs, lastRunData));        
        ERTC_pairs = 0;
        ERTA_pairs = 0;
        ERTB_pairs = 0;       
    }
    if ( invmass > lowermass && invmass < uppermass ) 
    {   
      //cout << "scaledERT_4x4b : " << scaledERT_4x4b << endl;
      if (scaledERT_4x4b)
      {
        
        ERTB_pairs++;
        //cout << "made it here... " << endl;
      }
      if (scaledERT_4x4a && !scaledERT_4x4b)
        ERTA_pairs++;
      if (scaledERT_4x4c && !scaledERT_4x4b && !scaledERT_4x4a)
        ERTC_pairs++;
    } 
    
    lastRunData = runDataTree;
  }
  

  cout << "ERTCEventVec size : " << ERTCEventVec.size() << endl;
  cout << "ERTCPairVec size : " << ERTCPairVec.size() << endl;  
  
  std::vector<std::pair<float, int>> ratioBVec;
  std::vector<std::pair<float, int>> ratioAVec;
  std::vector<std::pair<float, int>> ratioCVec;  
  cout << "nested loop 3" << endl;  
  for (int i=0; i<ERTBEventVec.size(); ++i)
  {
    for (int j=0; j<ERTBPairVec.size(); ++j)
    {
      if (ERTBPairVec.at(j).second == ERTBEventVec.at(i).second)
      {
        cout << "ERTB Num : " << ERTBPairVec.at(j).first  << " ERTB Denom : " << ERTBEventVec.at(i).first << endl;      
        float ratioB = ERTBPairVec.at(j).first/(float)ERTBEventVec.at(i).first;
        ratioBVec.push_back(make_pair(ratioB, ERTBEventVec.at(i).second));
      }
    }  
  }  
  
  cout << "nested loop 4" << endl; 
  for (int i=0; i<ERTAEventVec.size(); ++i)
  {
    for (int j=0; j<ERTAPairVec.size(); ++j)
    {
      if (ERTAPairVec.at(j).second == ERTAEventVec.at(i).second)
      {
        float ratioA = ERTAPairVec.at(j).first/(float)ERTAEventVec.at(i).first;
        ratioAVec.push_back(make_pair(ratioA, ERTAEventVec.at(i).second));
      }
    }  
  }
  
  cout << "nested loop 5" << endl;  
  for (int i=0; i<ERTCEventVec.size(); ++i)
  {
    for (int j=0; j<ERTCPairVec.size(); ++j)
    {
      if (ERTCPairVec.at(j).second == ERTCEventVec.at(i).second)
      {
        float ratioC = ERTCPairVec.at(j).first/(float)ERTCEventVec.at(i).first;
        //cout << "ratioC : " << ratioC << endl;
        ratioCVec.push_back(make_pair(ratioC, ERTCEventVec.at(i).second));
      }
    }  
  }    
  
  //int numRuns = ERTBEventVec.size();
  //float runs[numRuns];
  //float ratioCArr[numruns];
  //float ratioAArr[numruns];
  //float ratioCArr[numruns];
  TGraph *ratioBGraph = new TGraph(ERTBEventVec.size());  
  TGraph *ratioAGraph = new TGraph(ERTAEventVec.size());
  TGraph *ratioCGraph = new TGraph(ERTCEventVec.size());
  cout << "loop 6" << endl;  
  

  for (int i=0; i<ratioBVec.size(); ++i)
  {
    ratioBGraph->SetPoint(i, ratioBVec.at(i).second, ratioBVec.at(i).first);           
  }     
  cout << "loop 7" << endl;    
  for (int i=0; i<ratioAVec.size(); ++i)
  {
    ratioAGraph->SetPoint(i, ratioAVec.at(i).second, ratioAVec.at(i).first);
  }
 
  cout << "loop 8" << endl;  
  for (int i=0; i<ratioCVec.size(); ++i)
  {
    ratioCGraph->SetPoint(i, ratioCVec.at(i).second, ratioCVec.at(i).first);   
    //cout << "ratio C " << ratioCVec.at(i).first << " run : " << ratioCVec.at(i).second << endl;   

  } 
  
  if (collisionSystem == "pAl")
  {
    ratioCGraph->GetXaxis()->SetLimits(436500,438500); 
    ratioAGraph->GetXaxis()->SetLimits(436500,438500);  
    ratioBGraph->GetXaxis()->SetLimits(436500,438500);           
  }
  else if (collisionSystem == "pAu")
  {
    ratioCGraph->GetXaxis()->SetLimits(432500, 436500);  
    ratioAGraph->GetXaxis()->SetLimits(432500, 436500);  
    ratioBGraph->GetXaxis()->SetLimits(432500, 436500);          
  }
  
  ratioBGraph->SetTitle("");
  TCanvas *can0 = new TCanvas();
  ratioBGraph->SetMarkerStyle(8);  
  ratioBGraph->SetMarkerSize(1.1);
  if (particle == "pi0")
    ratioBGraph->GetYaxis()->SetRangeUser(0.,1.5);
  else if (particle == "eta")  
    ratioBGraph->GetYaxis()->SetRangeUser(0.,1.);
  
  ratioBGraph->Draw("AP");
  ratioAGraph->SetMarkerStyle(8);    
  ratioAGraph->SetMarkerSize(1.1);
  ratioAGraph->SetMarkerColor(kRed);
  ratioAGraph->Draw("P");
  ratioCGraph->SetMarkerStyle(8);    
  ratioCGraph->SetMarkerSize(1.1);
  ratioCGraph->SetMarkerColor(kBlue);
  ratioCGraph->Draw("P");
  
  TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
  leg->AddEntry(ratioBGraph, "ERTB", "p");
  leg->AddEntry(ratioAGraph, "ERTA", "p");
  leg->AddEntry(ratioCGraph, "ERTC", "p");
  leg->Draw();
   
  TString imgname = "";
  imgname += particle;
  imgname += "_";
  imgname += collisionSystem;
  imgname += "_ERTefficiency_zoomedOut.png";
  can0->SaveAs(imgname);
  
  TCanvas *can1 = new TCanvas();
  if (particle == "pi0")
    ratioBGraph->GetYaxis()->SetRangeUser(0.,0.2);
  else if (particle == "eta")  
    ratioBGraph->GetYaxis()->SetRangeUser(0.,0.15);
  ratioBGraph->Draw("AP");
  ratioAGraph->Draw("P");
  ratioCGraph->Draw("P");
  
  leg->Draw();
  
  imgname = "";
  imgname += particle;
  imgname += "_";
  imgname += collisionSystem;
  imgname += "_ERTefficiency.png";
  can1->SaveAs(imgname);  
  
/*  
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
  imgName += "_gl1pscalerch0Sums.png";
    
  can->SaveAs(imgName);  
*/
}
