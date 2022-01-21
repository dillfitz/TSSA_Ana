#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TString.h"
#include "TGraph.h"


const float PI =  3.14159265358979323846;
bool verbosity = 1;
void decayAsymmetryD0bar_weightsLoopZoomed_KGKGp_condor(int process=0)
{
  TString infileName = "dataFiles/condor/antid0decay_";
  infileName += process;
  infileName += ".root";
  TString outfileName = "dataFiles/condor/antid0DecayAsymmetry_weightsLoopZoomed_KGKGp_";
  outfileName += process;
  outfileName += ".root";

  gStyle->SetOptFit();
  const int ndptbins = 41;
  const int neptbins = 6;
  double eptbins[neptbins+1] = {1.0, 1.3, 1.5, 1.8, 2.1, 2.7, 5.0};
  double eptcenters[neptbins] = {1.161, 1.398, 1.639, 1.936, 2.349, 3.290};
  double dptbins[ndptbins+1] = {0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95, 1.05, 1.15, 1.25, 1.35, 1.45, 1.55, 1.65, 1.75, 1.85, 1.95, 2.05, 2.15, 2.25, 2.35, 2.45, 2.55, 2.65, 2.75, 2.85, 2.95, 3.05, 3.15, 3.25, 3.35, 3.45, 3.55, 3.65, 3.75, 3.85, 3.95, 4.05, 4.15, 4.25, 4.35, 4.45};
  double dptcenters[ndptbins] = {0.4, 0.5, 0.6,, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4};

  double AND0bar_noKG[ndptbins] = {-1.9835096174620346, -2.4372108947902387, -2.8626633575569085, -3.2573976175642554, -3.6112785333368422, -3.935015564043235, -4.2187065089842655, -4.469730417864148, -4.682041469139662, -4.862126190121375, -5.005815613265618, -5.12455694338949, -5.220890631029857, -5.287842313998718, -5.336434429531745, -5.359491559906778, -5.377017258651625, -5.3789136829000705, -5.363766888162092, -5.343033096607029, -5.313996997370368, -5.279715161748157, -5.239325169590213, -5.190388591345119, -5.143387667759557, -5.096692056688567, -5.0444352654435605, -4.990803137655573, -4.932541681176356, -4.876465549538863, -4.823393082156938, -4.771484807412024, -4.712038224942185, -4.657219255417658, -4.604781847430413, -4.5553134813440375, -4.505512524118659, -4.452407498548753, -4.403252881612244, -4.353889007271338, -4.303182083331609};

  double AND0bar_noKGp[ndptbins] = {-15.896666910879778, -19.391789416245253, -22.59009092661698, -25.46163183835282, -27.940762566023555, -30.09915308514587, -31.88535323790433, -33.34786010061567, -34.47270595692677, -35.31395549783692, -35.863055146237706, -36.19652038887394, -36.344338702502945, -36.28679848236905, -36.08801074160734, -35.731077002910574, -35.32748920886942, -34.83811404158516, -34.25033286512718, -33.63535321638859, -32.98095066464483, -32.3117740760194, -31.628321016166286, -30.915571421755512, -30.227341426181674, -29.55609389312398, -28.87260002840086, -28.196870000551705, -27.516888184368778, -26.86521378286051, -26.2426347904429, -25.6425261254134, -25.023783723088954, -24.44589892430114, -23.890168935233177, -23.359403099934617, -22.842910720138946, -22.327899712669108, -21.839140582018363, -21.362904687357634, -20.890370445639824};

  // parameters to tune to data! //
  double K_G = 0.002;
  double K_Gp = 0.0005; 
  const int Npar = 41;
  double KGs[Npar] = {-0.002, -0.0019, -0.0018, -0.0017, -0.0016, -0.0015, -0.0014, -0.0013, -0.0012, -0.0011, -0.0010, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0004, -0.0003, -0.0002, -0.0001, 0.0, 0.0001, 0.0002, 0.0003, 0.0004, 0.0005, 0.0006, 0.0007, 0.0008, 0.0009, 0.0010, 0.0011, 0.0012, 0.0013, 0.0014, 0.0015 , 0.0016, 0.0017, 0.0018, 0.0019, 0.002};
  double KGps[Npar] = {-0.002, -0.0019, -0.0018, -0.0017, -0.0016, -0.0015, -0.0014, -0.0013, -0.0012, -0.0011, -0.0010, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0004, -0.0003, -0.0002, -0.0001, 0.0, 0.0001, 0.0002, 0.0003, 0.0004, 0.0005, 0.0006, 0.0007, 0.0008, 0.0009, 0.0010, 0.0011, 0.0012, 0.0013, 0.0014, 0.0015 , 0.0016, 0.0017, 0.0018, 0.0019, 0.002};

  double AND0bar_model1[Npar][ndptbins]; 
  double AND0bar_model2[Npar][ndptbins]; 
  for (int j=0; j<Npar; ++j)
    {
      for (int i=0; i<ndptbins; ++i)
	{
	  AND0bar_model1[j][i] = KGs[j]*AND0bar_noKG[i];
	  AND0bar_model2[j][i] = KGps[j]*AND0bar_noKGp[i];
	} 
    }
  float Pol = 1;
  TFile *inFile = TFile::Open(infileName);
  TTree *decayTree = (TTree*)inFile->Get( "d0bar" );
  float pt, pt1, phi, phi1;
  decayTree->SetBranchAddress("pt", &pt);
  decayTree->SetBranchAddress("pt1", &pt1);  
  decayTree->SetBranchAddress("phi", &phi);  
  decayTree->SetBranchAddress("phi1", &phi1);

  TFile *outFile = new TFile(outfileName, "RECREATE");

  TString histname = "";
  string hist_name_model1 = ""; 
  string hist_name_model2 = ""; 
  TString histname_model1 = "";
  TString histname_model2 = "";

  TH1F *ephi_model1[Npar][neptbins];
  TH1F *ephi_model2[Npar][neptbins];
  for (int j=0; j<Npar; ++j)
    {
      for (int i=0; i<neptbins; ++i)
	{
	  histname = "ephi";
	  histname += i;
	  ostringstream ss1;
	  ostringstream ss2;
	  ss1<<KGs[j];
	  ss2<<KGps[j];
	  histname_model1 = histname;
	  histname_model1 += "_KG_";
	  histname_model1 += ss1.str();
	  histname_model2 = histname; 
	  histname_model2 += "_KGp_";
	  histname_model2 += ss2.str();

	  string hist_name_model1 = histname_model1;
	  replace(hist_name_model1.begin(), hist_name_model1.end(), '.', 'p');
	  replace(hist_name_model1.begin(), hist_name_model1.end(), '-', 'm');

	  string hist_name_model2 = histname_model2;
	  replace(hist_name_model2.begin(), hist_name_model2.end(), '.', 'p');
	  replace(hist_name_model2.begin(), hist_name_model2.end(), '-', 'm');

	  histname_model1 = hist_name_model1;
	  histname_model2 = hist_name_model2;

	  ephi_model1[j][i] = new TH1F(histname_model1, ";#phi;", 12, 0, 2*PI);
	  ephi_model1[j][i]->Sumw2();

	  ephi_model2[j][i] = new TH1F(histname_model2, ";#phi;", 12, 0, 2*PI);
	  ephi_model2[j][i]->Sumw2();
	}
    } 
  
  int nEntries = decayTree->GetEntries();
  cout << "There are  " << nEntries << " in the original d0bar decay tree " << endl;
  for (int i=0; i<nEntries; ++i)
    {
      if (i%1000000==0) {cout << "Entry " << i << endl; }
      decayTree->GetEntry(i);
      if (pt < 0.35 || pt > 4.45) {continue;}
      if (pt1 < 1.0 || pt1 > 5.0) {continue;}
      for (int l=0; l<Npar; ++l)
	{
	  int dPtIndex = 0;
	  for (int j=0; j<ndptbins; ++j)
	    {
	      if ( pt > dptbins[j] && pt < dptbins[j+1] )
		{
		  dPtIndex = j;
		}
	    }
	  double weight_model1 = 1.0 + AND0bar_model1[l][dPtIndex]*std::cos(phi);
	  double weight_model2 = 1.0 + AND0bar_model2[l][dPtIndex]*std::cos(phi);
	  //cout << "weight : " << weight << endl;
	  for (int k=0; k<neptbins; ++k)
	    {
	      if (pt1 > eptbins[k] && pt1 < eptbins[k+1] )
		{
		  ephi_model1[l][k]->Fill(phi1, weight_model1);
		  ephi_model2[l][k]->Fill(phi1, weight_model2);
		}
	    }
	}
    }
  
  outFile->cd();
  for (int j=0; j<Npar; ++j)
    {
      for (int i=0; i<neptbins; ++i)
	{
	  ephi_model1[j][i]->Write();
	  ephi_model1[j][i]->Delete();

	  ephi_model2[j][i]->Write();
	  ephi_model2[j][i]->Delete();
	}
    }
  outFile->Write();
  outFile->Close();
}
