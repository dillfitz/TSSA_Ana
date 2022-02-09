#include <iostream>
#include <string>
#include <vector>
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
bool D0flag = 0;
bool D0barflag = 0;
bool Dplusflag = 0;
bool Dminusflag = 1;

void eANextraction_weightsLoopZoomed_KGKGp()
{
  gStyle->SetOptFit();
  const int ndptbins = 41;
  const int neptbins = 6;
  double eptbins[neptbins+1] = {1.0, 1.3, 1.5, 1.8, 2.1, 2.7, 5.0};
  double eptcenters[neptbins] = {1.161, 1.398, 1.639, 1.936, 2.349, 3.290};
  double dptbins[ndptbins+1] = {0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95, 1.05, 1.15, 1.25, 1.35, 1.45, 1.55, 1.65, 1.75, 1.85, 1.95, 2.05, 2.15, 2.25, 2.35, 2.45, 2.55, 2.65, 2.75, 2.85, 2.95, 3.05, 3.15, 3.25, 3.35, 3.45, 3.55, 3.65, 3.75, 3.85, 3.95, 4.05, 4.15, 4.25, 4.35, 4.45};
  double dptcenters[ndptbins] = {0.4, 0.5, 0.6,, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4};

  double AND0_noKG[ndptbins] = {-0.6274824000929276, -0.7817068208160842, -0.9176233774342479, -1.0383295182343715, -1.1614377365415864, -1.2719632267368592, -1.365848090415206, -1.4635185651217544, -1.5423645757732245, -1.621185301977502, -1.6781225866338163, -1.755490389864286, -1.8134261731389314, -1.842423989618348, -1.8910393232078682, -1.909882461534582, -1.9551374737742815, -1.956217187978704, -1.970901348571342, -1.9894661943412897, -2.0055426338105775, -2.01310324426068, -2.0154468980340785, -2.0140606493179507, -2.0140015684657904, -2.0006098801460004, -2.0055031688081333, -1.9986565149191142, -2.001215045903091, -1.9852186692649718, -1.9722096713547168, -1.958407448557239, -1.966900737185024, -1.9365534703477105, -1.9267895045891439, -1.9401859855271426, -1.921165628605535, -1.9058310906517422, -1.8975633922943356, -1.8799557813286518, -1.887982344283107};

  double AND0_noKGp[ndptbins] = {14.664000517139224, 17.731979902254313, 20.516193408406657, 22.967092215467076, 25.001432262376348, 26.677202792121467, 28.028213070458897, 28.951191615297773, 29.61248256174749, 29.982225265006146, 30.154998155994832, 30.00234174710597, 29.69732259410898, 29.35141531720642, 28.777203373850906, 28.174772819837365, 27.40163750549663, 26.723818770303897, 25.938149440621277, 25.10622182663265, 24.221540006795266, 23.37373064261309, 22.567466978322354, 21.745046270865217, 20.94561125929737, 20.186709173886836, 19.39393342069707, 18.64942885961422, 17.903161369594734, 17.216353550053046, 16.54581396045734, 15.908251427864064, 15.225655999372847, 14.693419178817408, 14.118147293496879, 13.504589876951393, 12.989662218495138, 12.50771876208015, 12.013844431782507, 11.556677529229606, 11.059718890923591};
  
  double ANDplus_noKG[ndptbins] = {-0.633445002301727, -0.788215184257882, -0.9258910109106822, -1.0480413689834749, -1.1722366571770184, -1.284220882397701, -1.3794201818081158, -1.4766399041215414, -1.5553021778851412, -1.6345332747310417, -1.6923920554386296, -1.7702169692102037, -1.8286181681908154, -1.8564579691776852, -1.9062718132655048, -1.927888234350968, -1.9734657530499577, -1.9758848930859, -1.9908219851162507, -2.0075767837171172, -2.0231349757486137, -2.0302326803537647, -2.033168538334925, -2.034065443806923, -2.0350589492604865, -2.0220845239403817, -2.0248224211241643, -2.016601895772738, -2.019548950100019, -2.003422037528232, -1.9901700232581092, -1.977453389006957, -1.9864285529093502, -1.9551467632900097, -1.9454099314825954, -1.9573976978429328, -1.9395960527439347, -1.924716999528432, -1.9154153438562604, -1.8968475558242242, -1.905226413502139};

  double ANDplus_noKGp[ndptbins] = {15.046465836260676, 18.204299512736497, 21.07011383056006, 23.594168472138676, 25.696918159412927, 27.428727945824587, 28.829492080961117, 29.799557052745808, 30.5003089324194, 30.896709457816446, 31.08774446410004, 30.948253304019808, 30.647461210445915, 30.305161761033247, 29.719349048213108, 29.09742734282163, 28.31070349523236, 27.61340281350978, 26.809428875889065, 25.964262372768594, 25.06381303650736, 24.196792910181678, 23.365594416227253, 22.5122134108699, 21.686849458713425, 20.905285433860236, 20.098796814160778, 19.33787686058397, 18.569097178149896, 17.86159834979566, 17.168175591018016, 16.507227013535772, 15.804544592168687, 15.256639224931615, 14.663720936118763, 14.035353379626917, 13.499598735054747, 13.000675583250567, 12.493267991874577, 12.024414403450193, 11.510697192786505};
  
  double AND0bar_noKG[ndptbins] = {-1.9835096174620346, -2.4372108947902387, -2.8626633575569085, -3.2573976175642554, -3.6112785333368422, -3.935015564043235, -4.2187065089842655, -4.469730417864148, -4.682041469139662, -4.862126190121375, -5.005815613265618, -5.12455694338949, -5.220890631029857, -5.287842313998718, -5.336434429531745, -5.359491559906778, -5.377017258651625, -5.3789136829000705, -5.363766888162092, -5.343033096607029, -5.313996997370368, -5.279715161748157, -5.239325169590213, -5.190388591345119, -5.143387667759557, -5.096692056688567, -5.0444352654435605, -4.990803137655573, -4.932541681176356, -4.876465549538863, -4.823393082156938, -4.771484807412024, -4.712038224942185, -4.657219255417658, -4.604781847430413, -4.5553134813440375, -4.505512524118659, -4.452407498548753, -4.403252881612244, -4.353889007271338, -4.303182083331609};

  double AND0bar_noKGp[ndptbins] = {-15.896666910879778, -19.391789416245253, -22.59009092661698, -25.46163183835282, -27.940762566023555, -30.09915308514587, -31.88535323790433, -33.34786010061567, -34.47270595692677, -35.31395549783692, -35.863055146237706, -36.19652038887394, -36.344338702502945, -36.28679848236905, -36.08801074160734, -35.731077002910574, -35.32748920886942, -34.83811404158516, -34.25033286512718, -33.63535321638859, -32.98095066464483, -32.3117740760194, -31.628321016166286, -30.915571421755512, -30.227341426181674, -29.55609389312398, -28.87260002840086, -28.196870000551705, -27.516888184368778, -26.86521378286051, -26.2426347904429, -25.6425261254134, -25.023783723088954, -24.44589892430114, -23.890168935233177, -23.359403099934617, -22.842910720138946, -22.327899712669108, -21.839140582018363, -21.362904687357634, -20.890370445639824};
  
  double ANDminus_noKG[ndptbins] = {-2.00386888568295, -2.4634624262480265, -2.895429032940172, -3.296657333151198, -3.657456154433885, -3.987912342799291, -4.2781812051213155, -4.5355308478884355, -4.753132780716358, -4.938094538532421, -5.0858629453546556, -5.208565486468753, -5.30774413709565, -5.377173944790739, -5.427814877022022, -5.452156863309067, -5.470232304171338, -5.472710395104902, -5.457658529773524, -5.436716316217272, -5.4072875026139595, -5.371992770875464, -5.330675046564382, -5.281203721076877, -5.233091893645654, -5.185056146745793, -5.131602251901805, -5.076321269121237, -5.016624590647145, -4.959253168218867, -4.904534273807386, -4.851026242177821, -4.790385863386832, -4.734115123803418, -4.680153347077257, -4.629178563965116, -4.5777808958949375, -4.523699255338875, -4.473014519371925, -4.422400095909339, -4.370365692136383};

  double ANDminus_noKGp[ndptbins] = {-16.226317983340564, -19.803061067412788, -23.08265217905688, -26.03064941089843, -28.583883729933063, -30.809631979661503, -32.65682390603426, -34.173614798244174, -35.341199636415666, -36.217887451153125, -36.79314395377731, -37.14857265278113, -37.308646496878104, -37.25834896674968, -37.06144015819155, -36.700291594561754, -36.287638248217455, -35.78805212268783, -35.186207289953195, -34.555557220276846, -33.8842696745963, -33.194818132553586, -32.49136037232465, -31.76051856848113, -31.051915683484317, -30.35963343810752, -29.65659495091731, -28.959171753788787, -28.25870655103152, -27.587740155712602, -26.944668839747006, -26.324904952274288, -25.688894583926114, -25.093231904955143, -24.519887042307957, -23.972049136568423, -23.438440527394313, -22.90937073508341, -22.4046091785157, -21.91401670541035, -21.427133981094762};
  
    // parameters to tune to data! //
  double K_G = 0.002;
  double K_Gp = 0.0005; 
  const int Npar = 101;
  double KGs[Npar] = {-0.005, -0.0049, -0.0048, -0.0047, -0.0046, -0.0045, -0.0044, -0.0043, -0.0042, -0.0041, -0.004, -0.0039, -0.0038, -0.0037, -0.0036, -0.0035, -0.0034, -0.0033, -0.0032, -0.0031, -0.003, -0.0029, -0.0028, -0.0027, -0.0026, -0.0025, -0.0024, -0.0023, -0.0022, -0.0021, -0.002, -0.0019, -0.0018, -0.0017, -0.0016, -0.0015, -0.0014, -0.0013, -0.0012, -0.0011, -0.0010, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0004, -0.0003, -0.0002, -0.0001, 0.0, 0.0001, 0.0002, 0.0003, 0.0004, 0.0005, 0.0006, 0.0007, 0.0008, 0.0009, 0.0010, 0.0011, 0.0012, 0.0013, 0.0014, 0.0015 , 0.0016, 0.0017, 0.0018, 0.0019, 0.002, 0.0021, 0.0022, 0.0023, 0.0024, 0.0025 , 0.0026, 0.0027, 0.0028, 0.0029, 0.003, 0.0031, 0.0032, 0.0033, 0.0034, 0.0035 , 0.0036, 0.0037, 0.0038, 0.0039, 0.004, 0.0041, 0.0042, 0.0043, 0.0044, 0.0045, 0.0046, 0.0047, 0.0048, 0.0049, 0.005};
  double KGps[Npar] = {-0.00025, -0.00024, -0.00023, -0.00022, -0.00021, -0.0002, -0.00019, -0.00018, -0.00017, -0.00016, -0.00015, -0.00014, -0.00013, -0.00012, -0.00011, -0.0001, -0.00009, -0.00008, -0.00007, -0.00006, -0.00005, -0.00004, -0.00003, -0.00002, -0.00001, 0.0, 0.00001, 0.00002, 0.00003, 0.00004, 0.00005, 0.00006, 0.00007, 0.00008, 0.00009, 0.0001, 0.00011, 0.00012, 0.00013, 0.00014, 0.00015, 0.00016, 0.00017, 0.00018, 0.00019, 0.0002, 0.00021, 0.00022, 0.00023, 0.00024, 0.00025, 0.00026, 0.00027, 0.00028, 0.00029, 0.0003, 0.00031, 0.00032, 0.00033, 0.00034, 0.00035, 0.00036, 0.00037, 0.00038, 0.00039 , 0.0004, 0.00041, 0.00042, 0.00043, 0.00044, 0.00045, 0.00046, 0.00047, 0.00048, 0.00049, 0.0005, 0.00051, 0.00052, 0.00053, 0.00054, 0.00055, 0.00056, 0.00057, 0.00058, 0.00059, 0.0006, 0.00061, 0.00062, 0.00063, 0.00064, 0.00065, 0.00066, 0.00067, 0.00068, 0.00069, 0.0007, 0.00071, 0.00072, 0.00073, 0.00074, 0.00075};

	double AN_model1[Npar][ndptbins]; 
  double AN_model2[Npar][ndptbins]; 
  if (D0flag && !(D0barflag || Dplusflag || Dminusflag) )
  {
    TFile *inFile = TFile::Open("dataFiles/d0DecayAsymmetry_10B_weightsLoopZoomed_KGKGp.root");  
	  TFile *outFile = new TFile("dataFiles/d0DecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root", "RECREATE"); 
    for (int j=0; j<Npar; ++j)
      {
        for (int i=0; i<ndptbins; ++i)
	        {	        
	          AN_model1[j][i] = KGs[j]*AND0_noKG[i];
	          AN_model2[j][i] = KGps[j]*AND0_noKGp[i];	         
	        } 
      }
  }
  else if (D0barflag && !(D0flag || Dplusflag || Dminusflag) )
  {
    TFile *inFile = TFile::Open("dataFiles/antid0DecayAsymmetry_10B_weightsLoopZoomed_KGKGp.root");   
	  TFile *outFile = new TFile("dataFiles/antid0DecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root", "RECREATE"); 
	  double AND0_model1[Npar][ndptbins]; 
    double AND0_model2[Npar][ndptbins]; 
    for (int j=0; j<Npar; ++j)
      {
        for (int i=0; i<ndptbins; ++i)
	        {
	          AN_model1[j][i] = KGs[j]*AND0bar_noKG[i];
	          AN_model2[j][i] = KGps[j]*AND0bar_noKGp[i];
	        } 
      }	  
  }
  else if (Dplusflag && !(D0flag || D0barflag || Dminusflag) )
  {
    TFile *inFile = TFile::Open("dataFiles/dplusDecayAsymmetry_10B_weightsLoopZoomed_KGKGp.root");   
	  TFile *outFile = new TFile("dataFiles/dplusDecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root", "RECREATE");
    for (int j=0; j<Npar; ++j)
      {
        for (int i=0; i<ndptbins; ++i)
	        {
	          AN_model1[j][i] = KGs[j]*ANDplus_noKG[i];
	          AN_model2[j][i] = KGps[j]*ANDplus_noKGp[i];
	        } 
      }	  	   
  }  
  else if (Dminusflag && !(D0flag || D0barflag || Dplusflag) )
  {
    TFile *inFile = TFile::Open("dataFiles/dminusDecayAsymmetry_10B_weightsLoopZoomed_KGKGp.root");   
	  TFile *outFile = new TFile("dataFiles/dminusDecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root", "RECREATE");
    for (int j=0; j<Npar; ++j)
      {
        for (int i=0; i<ndptbins; ++i)
	        {
	          AN_model1[j][i] = KGs[j]*ANDminus_noKG[i];
	          AN_model2[j][i] = KGps[j]*ANDminus_noKGp[i];
	        } 
      }	  	   
  }  
  else { cout << "Adjust the flags at the top to select an output file!" << endl; return;}

  cout << "Get histos" << endl;
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

	        ephi_model1[j][i] = (TH1F*)inFile->Get(histname_model1);
	        ephi_model2[j][i] = (TH1F*)inFile->Get(histname_model2);
	      }
    } 


  cout << "Fit histos" << endl;
  TF1 *eAN_model1[Npar][neptbins];
  TF1 *eAN_model2[Npar][neptbins];  
  double eANfit_model1[Npar][neptbins];
  double eANfitUC_model1[Npar][neptbins];  
  double eANfit_model2[Npar][neptbins];
  double eANfitUC_model2[Npar][neptbins];    
  double eANfitUCx[Npar][neptbins];  
  
  TString efitname = "";
  string efit_name_model1 = ""; 
  string efit_name_model2 = ""; 
  TString efitname_model1 = "";
  TString efitname_model2 = "";
  
  for (int j=0; j<Npar; ++j)
  {
	    for (int i=0; i<neptbins; ++i)
	    {
	      efitname =  "efit";
	      efitname += i;
	      ostringstream ss1;
	      ostringstream ss2;
	      ss1<<KGs[j];
	      ss2<<KGps[j];
	      efitname_model1 = efitname;
	      efitname_model1 += "_KG_";
	      efitname_model1 += ss1.str();
	      efitname_model2 = efitname; 
	      efitname_model2 += "_KGp_";
	      efitname_model2 += ss2.str();

	      efit_name_model1 = efitname_model1;
	      replace(efit_name_model1.begin(), efit_name_model1.end(), '.', 'p');
	      replace(efit_name_model1.begin(), efit_name_model1.end(), '-', 'm');
	      
	      efit_name_model2 = efitname_model2;
	      replace(efit_name_model2.begin(), efit_name_model2.end(), '.', 'p');
	      replace(efit_name_model2.begin(), efit_name_model2.end(), '-', 'm');	      
	      
	      efitname_model1 = efit_name_model1;
	      efitname_model2 = efit_name_model2;

	      eAN_model1[j][i] = new TF1(efitname,"[0]*(1+[1]*cos(x))", 0, 2*PI);
	      eAN_model2[j][i] = new TF1(efitname,"[0]*(1+[1]*cos(x))", 0, 2*PI);	      

	      ephi_model1[j][i]->Fit(eAN_model1[j][i],"q");	  
	      if ( eAN_model1[j][i]->GetChisquare()/eAN_model1[j][i]->GetNDF() > 8. )
	        {
	          cout << "take 1 : chisq/ndf : " << eAN_model1[j][i]->GetChisquare()/eAN_model1[j][i]->GetNDF() << endl;  
	          ephi_model1[j][i]->Fit(eAN_model1[j][i],"q");	  
	          cout << "take 2 : chisq/ndf : " << eAN_model1[j][i]->GetChisquare()/eAN_model1[j][i]->GetNDF() << endl;  
	        } 
	      eANfit_model1[j][i] = eAN_model1[j][i]->GetParameter(1);
	      eANfitUC_model1[j][i] = eAN_model1[j][i]->GetParError(1);	
	      
	      ephi_model2[j][i]->Fit(eAN_model2[j][i], "q");	  
	      if ( eAN_model2[j][i]->GetChisquare()/eAN_model2[j][i]->GetNDF() > 8. )
	        {
	          cout << "take 1 : chisq/ndf : " << eAN_model2[j][i]->GetChisquare()/eAN_model2[j][i]->GetNDF() << endl;  
	          ephi_model2[j][i]->Fit(eAN_model2[j][i],"q");	  
	          cout << "take 2 : chisq/ndf : " << eAN_model2[j][i]->GetChisquare()/eAN_model2[j][i]->GetNDF() << endl;  
	        } 
	      eANfit_model2[j][i] = eAN_model2[j][i]->GetParameter(1);
	      eANfitUC_model2[j][i] = eAN_model2[j][i]->GetParError(1);		      
	      
	      eANfitUCx[j][i] = 0;
	      	      	            
	      ephi_model1[j][i]->Write();
	      ephi_model1[j][i]->Delete();
	      
	      ephi_model2[j][i]->Write();
	      ephi_model2[j][i]->Delete();	      
	    }
  }
  
  cout << "Make graphs" << endl;
  TGraph *dANGraph_model1[Npar]; 
  TGraph *dANGraph_model2[Npar];   
  TGraphErrors *eANGraph_model1[Npar];
  TGraphErrors *eANGraph_model2[Npar];  
  TString graphname = "";
  string graph_name_model1 = ""; 
  string graph_name_model2 = ""; 
  TString graphname_model1 = "";
  TString graphname_model2 = "";  
  for (int j=0; j<Npar; ++j)
  {
	  eANGraph_model1[j] = new TGraphErrors(neptbins, eptcenters, eANfit_model1[j], eANfitUCx[j], eANfitUC_model1[j]);
	  eANGraph_model2[j] = new TGraphErrors(neptbins, eptcenters, eANfit_model2[j], eANfitUCx[j], eANfitUC_model2[j]);	  
	  graphname = "eAN";
	  ostringstream ss1;
	  ostringstream ss2;
	  ss1<<KGs[j];
	  ss2<<KGps[j];
	  graphname_model1 = graphname;
	  graphname_model1 += "_KG_";
	  graphname_model1 += ss1.str();
	  graphname_model2 = graphname; 
	  graphname_model2 += "_KGp_";
	  graphname_model2 += ss2.str();

	  string graph_name_model1 = graphname_model1;
	  replace(graph_name_model1.begin(), graph_name_model1.end(), '.', 'p');
	  replace(graph_name_model1.begin(), graph_name_model1.end(), '-', 'm');

	  string graph_name_model2 = graphname_model2;
	  replace(graph_name_model2.begin(), graph_name_model2.end(), '.', 'p');
	  replace(graph_name_model2.begin(), graph_name_model2.end(), '-', 'm');

	  graphname_model1 = graph_name_model1;
	  graphname_model2 = graph_name_model2;
	  
	  eANGraph_model1[j]->SetName(graphname_model1);
	  eANGraph_model1[j]->SetTitle("");
	  eANGraph_model1[j]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	  eANGraph_model1[j]->GetXaxis()->SetTitleOffset(1.4);
	  eANGraph_model1[j]->GetYaxis()->SetTitle("A_{N}");
	  eANGraph_model1[j]->GetYaxis()->SetTitleOffset(1.4);
	  eANGraph_model1[j]->SetMarkerStyle(23);
	  eANGraph_model1[j]->SetMarkerColor(kRed);
	  eANGraph_model1[j]->SetLineColor(kRed);
	  //eANGraph[j][k]->GetYaxis()->SetRangeUser(-0.5, 0.5);
	  eANGraph_model1[j]->GetXaxis()->SetLimits(0.0, 5.1);
	  eANGraph_model1[j]->Write();
	  eANGraph_model1[j]->Delete();
	  
	  eANGraph_model2[j]->SetName(graphname_model2);
	  eANGraph_model2[j]->SetTitle("");
	  eANGraph_model2[j]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	  eANGraph_model2[j]->GetXaxis()->SetTitleOffset(1.4);
	  eANGraph_model2[j]->GetYaxis()->SetTitle("A_{N}");
	  eANGraph_model2[j]->GetYaxis()->SetTitleOffset(1.4);
	  eANGraph_model2[j]->SetMarkerStyle(23);
	  eANGraph_model2[j]->SetMarkerColor(kRed);
	  eANGraph_model2[j]->SetLineColor(kRed);
	  eANGraph_model2[j]->GetXaxis()->SetLimits(0.0, 5.1);
	  eANGraph_model2[j]->Write();
	  eANGraph_model2[j]->Delete();	  
	  
	  dANGraph_model1[j] = new TGraph(ndptbins,dptcenters, AN_model1[j]);
	  dANGraph_model2[j] = new TGraph(ndptbins,dptcenters, AN_model2[j]);	  
	  graphname = "dAN";
	  ostringstream ss1;
	  ostringstream ss2;
	  ss1<<KGs[j];
	  ss2<<KGps[j];
	  graphname_model1 = graphname;
	  graphname_model1 += "_KG_";
	  graphname_model1 += ss1.str();
	  graphname_model2 = graphname; 
	  graphname_model2 += "_KGp_";
	  graphname_model2 += ss2.str();

	  string graph_name_model1 = graphname_model1;
	  replace(graph_name_model1.begin(), graph_name_model1.end(), '.', 'p');
	  replace(graph_name_model1.begin(), graph_name_model1.end(), '-', 'm');

	  string graph_name_model2 = graphname_model2;
	  replace(graph_name_model2.begin(), graph_name_model2.end(), '.', 'p');
	  replace(graph_name_model2.begin(), graph_name_model2.end(), '-', 'm');

	  graphname_model1 = graph_name_model1;
	  graphname_model2 = graph_name_model2;
	  
	  dANGraph_model1[j]->SetName(graphname_model1);
	  dANGraph_model1[j]->SetTitle("");
	  dANGraph_model1[j]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	  dANGraph_model1[j]->GetXaxis()->SetTitleOffset(1.4);
	  dANGraph_model1[j]->GetYaxis()->SetTitle("A_{N}");
	  dANGraph_model1[j]->GetYaxis()->SetTitleOffset(1.4);
	  dANGraph_model1[j]->SetMarkerStyle(23);
	  dANGraph_model1[j]->SetMarkerColor(kBlue);
	  dANGraph_model1[j]->SetLineColor(kBlue);
	  dANGraph_model1[j]->GetXaxis()->SetLimits(0.0, 5.1);
	  dANGraph_model1[j]->Write();
	  dANGraph_model1[j]->Delete();  
	  
	  dANGraph_model2[j]->SetName(graphname_model2);
	  dANGraph_model2[j]->SetTitle("");
	  dANGraph_model2[j]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	  dANGraph_model2[j]->GetXaxis()->SetTitleOffset(1.4);
	  dANGraph_model2[j]->GetYaxis()->SetTitle("A_{N}");
	  dANGraph_model2[j]->GetYaxis()->SetTitleOffset(1.4);
	  dANGraph_model2[j]->SetMarkerStyle(23);
	  dANGraph_model2[j]->SetMarkerColor(kBlue);
	  dANGraph_model2[j]->SetLineColor(kBlue);
	  dANGraph_model2[j]->GetXaxis()->SetLimits(0.0, 5.1);
	  dANGraph_model2[j]->Write();
	  dANGraph_model2[j]->Delete();  	  
  }
  

  outFile->cd();
  outFile->Write();
  outFile->Close();
}
