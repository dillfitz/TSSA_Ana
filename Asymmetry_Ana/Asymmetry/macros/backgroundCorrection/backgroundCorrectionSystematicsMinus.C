#include <cmath>
#include "../../Constants.h"
#include "../plotMacros/ttest.h"

void backgroundCorrectionSystematicsMinus()
{
	bool prelim = 1;
 	bool theory_compare = 1;
    bool sysTables = 1;
	int verbosity = -99;
	bool SAVE_IMAGES = 1;
	bool jpsi = 1;
	const int nbins = NUM_VALUE_BINS;
    //    const int nbins = 4;
	double x_lumi[nbins], y_lumi[nbins], y_sqrt[nbins];
	double y_corrected_lumi[nbins], y_corrected_lumi_np[nbins];
	double corrected_lumi_err[nbins], corrected_lumi_err_np[nbins], lumi_err[nbins];
	double sysDiff[nbins];
	double total_bg[nbins], total_bg_plus[nbins], total_bg_minus[nbins];
	double total_bg_np[nbins], total_bg_np_plus[nbins], total_bg_np_minus[nbins];
	double had_AN[nbins], had_AN_err[nbins];

	// for systematics //
	double bg_sys_plus[nbins], bg_sys_minus[nbins];
	double bg_sys_plus_np[nbins], bg_sys_minus_np[nbins];
	double jpsi_sys_plus[nbins], jpsi_sys_minus[nbins];
	double sys_plus_tot[nbins], sys_minus_tot[nbins];
	double sys_plus_tot_np[nbins], sys_minus_tot_np[nbins];
	
	// Taken from previous 200 GeV PHENIX measurements (Run 6) //
	double jpsi_AN_val = -0.064; 
	double jpsi_AN_errVal = 0.106;
	//double jpsi_dilutions[nbins] = {0.447, 0.688, 0.789, 0.871};
        // recalculated with corrected detector accpetance // 
    if (nbins==6)
    {
	  double jpsi_dilutions[nbins] = {-0.376, -0.0804, 0.489, 0.714, 0.808, 0.886};
	  double jpsi_AN[nbins] = {jpsi_dilutions[0]*jpsi_AN_val, jpsi_dilutions[1]*jpsi_AN_val, jpsi_dilutions[2]*jpsi_AN_val, jpsi_dilutions[3]*jpsi_AN_val, jpsi_dilutions[4]*jpsi_AN_val, jpsi_dilutions[5]*jpsi_AN_val   };
	  double jpsi_AN_err[nbins] = {fabs(jpsi_dilutions[0])*jpsi_AN_errVal, fabs(jpsi_dilutions[1])*jpsi_AN_errVal, fabs(jpsi_dilutions[2])*jpsi_AN_errVal, fabs(jpsi_dilutions[3])*jpsi_AN_errVal, fabs(jpsi_dilutions[4])*jpsi_AN_errVal, fabs(jpsi_dilutions[5])*jpsi_AN_errVal  };

	  double hadminus_AN[nbins] = {-0.0012, -0.0012, -0.0012, -0.0012, -0.021, -0.021};
	  double hadminus_AN_err[nbins] = {0.0082, 0.0082, 0.0082, 0.0082, 0.027, 0.027};

	  double hadplus_AN[nbins] = {-0.0054, -0.0054,-0.0054, -0.0054, -0.031, -0.031};
	  double hadplus_AN_err[nbins] = {0.0078, 0.0078,0.0078, 0.0078, 0.026, 0.026};
    }
    else if (nbins == 4)
    {
	  double jpsi_dilutions[nbins] = {0.489, 0.714, 0.808, 0.886};
	  double jpsi_AN[nbins] = {jpsi_dilutions[0]*jpsi_AN_val, jpsi_dilutions[1]*jpsi_AN_val, jpsi_dilutions[2]*jpsi_AN_val, jpsi_dilutions[3]*jpsi_AN_val   };
	  double jpsi_AN_err[nbins] = {jpsi_dilutions[0]*jpsi_AN_errVal, jpsi_dilutions[1]*jpsi_AN_errVal, jpsi_dilutions[2]*jpsi_AN_errVal, jpsi_dilutions[3]*jpsi_AN_errVal  };

	  double hadminus_AN[nbins] = {-0.0012, -0.0012, -0.021, -0.021};
	  double hadminus_AN_err[nbins] = {0.0082, 0.0082, 0.027, 0.027};

	  double hadplus_AN[nbins] = {-0.0054, -0.0054, -0.031, -0.031};
	  double hadplus_AN_err[nbins] = {0.0078, 0.0078, 0.026, 0.026};
    }
	
	//TGraphAsymmErrors *corrected_lumi = new TGraphAsymmErrors();
	
	TFile *infile1 = TFile::Open("inputFiles/normalizations_minus.root");
	TFile *infile2 = TFile::Open("inputFiles/systematics_minus.root");
	TFile *infile3 = TFile::Open("inputFiles/hadcontam_minus.root");
	TFile *infile4 = TFile::Open("inputFiles/curated_ohfe.root");
	
	TTree* eTree = (TTree*)infile4->Get( "e_svx_tree" );
	
	TH1F* h_piz_n = (TH1F*)infile1->Get("h_piz_n");
	TH1F* h_eta_n = (TH1F*)infile1->Get("h_eta_n");
	TH1F* h_jpsi_n = (TH1F*)infile1->Get("h_jpsi_n");
	TH1F* h_ke3_n = (TH1F*)infile1->Get("h_ke3_n");
	TH1F* h_photon_n = (TH1F*)infile1->Get("h_photon_n");
	TH1F* h_hadcontam_n = (TH1F*)infile1->Get("h_hadcontam_n");
	TH1F* h_sys_fnp = (TH1F*)infile2->Get("h_sys_fnp");
	TH1F* h_sys2_fnp = (TH1F*)infile2->Get("fnp_sys2");
	TH1F* h_sys_piz = (TH1F*)infile2->Get("h_sys_piz");
	TH1F* h_sys_eta = (TH1F*)infile2->Get("h_sys_eta");
	TH1F* h_sys_ke3 = (TH1F*)infile2->Get("h_sys_ke3");
	TH1F* h_sys_jpsi = (TH1F*)infile2->Get("h_sys_jpsi");
	TH1F* h_sys_photon = (TH1F*)infile2->Get("h_sys_photon");
	TH1F* h_sys2_piz = (TH1F*)infile2->Get("piz_sys2u");
	TH1F* h_sys2_eta = (TH1F*)infile2->Get("eta_sys2u");
	TH1F* h_sys2_photon = (TH1F*)infile2->Get("gam_sys2u");
	TH1F* h_sys2_jpsi = (TH1F*)infile2->Get("jpsi_sys2u");
	TH1F* h_sys_hadlow = (TH1F*)infile3->Get("h_depfit")->Clone("h_sys_hadlow");
	TH1F* h_sys_hadhigh = (TH1F*)infile3->Get("h_algebraic")->Clone("h_sys_hadhigh");
	TH1F* h_sys_hadnom = (TH1F*)infile3->Get("h_hadcontam")->Clone("h_sys_hadnom");
	TH1I* h_charge1 = new TH1I("ch1", "", 4, -1, 2);
	TH1I* h_charge2 = new TH1I("ch2", "", 4, -1, 2);
	TH1I* h_charge3 = new TH1I("ch3", "", 4, -1, 2);
	TH1I* h_charge4 = new TH1I("ch4", "", 4, -1, 2);
	
	eTree->Draw("charge>>ch1","pt>1.5 && pt<1.8");
	eTree->Draw("charge>>ch2","pt>1.8 && pt<2.1");
	eTree->Draw("charge>>ch3","pt>2.1 && pt<2.7");
	eTree->Draw("charge>>ch4","pt>2.7 && pt<5.0");
	
	double minus_fraction[nbins] = 
	{ 
		h_charge1->GetBinContent(1)/(h_charge1->GetBinContent(1) + h_charge1->GetBinContent(3)),
		h_charge2->GetBinContent(1)/(h_charge2->GetBinContent(1) + h_charge2->GetBinContent(3)),
		h_charge3->GetBinContent(1)/(h_charge3->GetBinContent(1) + h_charge3->GetBinContent(3)),
		h_charge4->GetBinContent(1)/(h_charge4->GetBinContent(1) + h_charge4->GetBinContent(3)) 
	};
        if (nbins == 6)
	    TFile *infile0 = TFile::Open("inputFiles/ohfe_AN_minus.root");
        else if (nbins == 4)
	    TFile *infile0 = TFile::Open("inputFiles/4binsBackup/ohfe_AN_minus.root");



	for (int i=0; i<nbins; ++i)
	{
		had_AN[i] = hadminus_AN[i] ;
		had_AN_err[i] = hadminus_AN_err[i];
		
		lumi_minus->GetPoint(i, x_lumi[i], y_lumi[i]);
		sqrt_minus->GetPoint(i, x_lumi[i], y_sqrt[i]);
		lumi_err[i] = lumi_minus->GetErrorYhigh(i);
		double sysFormulaDiff = fabs(y_lumi[i] - y_sqrt[i]);
		if ( verbosity == 1 )
		{
			//cout << "minus fraction : " << minus_fraction[i] << endl;
			//cout << "charged hadron AN : " << had_AN[i] << " err : " << had_AN_err[i] << endl;
			cout << "lumi : " << i << " "<< y_lumi[i] << endl;
			cout << "lumi err : " << i << " " << lumi_minus->GetErrorYhigh(i) << endl;
			cout << "sqrt : " << i << " "<< y_sqrt[i] << endl;
			cout << "sqrt err : " << i << " " << sqrt_minus->GetErrorYhigh(i) << endl;
		}
		//total_bg[i] = h_piz_n->GetBinContent(i+1) + h_eta_n->GetBinContent(i+1) + h_photon_n->GetBinContent(i+1) + h_jpsi_n->GetBinContent(i+1) + h_hadcontam_n->GetBinContent(i+1) + h_ke3_n->GetBinContent(i+1);
		total_bg[i] = h_piz_n->GetBinContent(i+1) + h_eta_n->GetBinContent(i+1) + h_photon_n->GetBinContent(i+1) + h_jpsi_n->GetBinContent(i+1) + h_hadcontam_n->GetBinContent(i+1);
		total_bg_plus[i] = (h_piz_n->GetBinContent(i+1)+h_sys_piz->GetBinContent(i+1)+h_sys2_piz->GetBinContent(i+1)) + (h_eta_n->GetBinContent(i+1)+h_sys_eta->GetBinContent(i+1)+h_sys2_eta->GetBinContent(i+1)) + (h_photon_n->GetBinContent(i+1)+h_sys_photon->GetBinContent(i+1)+h_sys2_photon->GetBinContent(i+1)) + (h_jpsi_n->GetBinContent(i+1)+h_sys_jpsi->GetBinContent(i+1)+h_sys2_jpsi->GetBinContent(i+1)) + h_sys_hadhigh->GetBinContent(i+1);
		total_bg_minus[i] = (h_piz_n->GetBinContent(i+1)-h_sys_piz->GetBinContent(i+1)-h_sys2_piz->GetBinContent(i+1)) + (h_eta_n->GetBinContent(i+1)-h_sys_eta->GetBinContent(i+1)-h_sys2_eta->GetBinContent(i+1)) + (h_photon_n->GetBinContent(i+1)-h_sys_photon->GetBinContent(i+1)-h_sys2_photon->GetBinContent(i+1)) + (h_jpsi_n->GetBinContent(i+1)-h_sys_jpsi->GetBinContent(i+1)-h_sys2_jpsi->GetBinContent(i+1)) + h_sys_hadlow->GetBinContent(i+1);

		total_bg_np[i] = h_piz_n->GetBinContent(i+1) + h_eta_n->GetBinContent(i+1) + h_photon_n->GetBinContent(i+1) +  h_hadcontam_n->GetBinContent(i+1);
		total_bg_np_plus[i] = (h_piz_n->GetBinContent(i+1)+h_sys_piz->GetBinContent(i+1)+h_sys2_piz->GetBinContent(i+1)) + (h_eta_n->GetBinContent(i+1)+h_sys_eta->GetBinContent(i+1)+h_sys2_eta->GetBinContent(i+1)) + (h_photon_n->GetBinContent(i+1)+h_sys_photon->GetBinContent(i+1)+h_sys2_photon->GetBinContent(i+1)) + h_sys_hadhigh->GetBinContent(i+1);
		total_bg_np_minus[i] = (h_piz_n->GetBinContent(i+1)-h_sys_piz->GetBinContent(i+1)-h_sys2_piz->GetBinContent(i+1)) + (h_eta_n->GetBinContent(i+1)-h_sys_eta->GetBinContent(i+1)-h_sys2_eta->GetBinContent(i+1)) + (h_photon_n->GetBinContent(i+1)-h_sys_photon->GetBinContent(i+1)-h_sys2_photon->GetBinContent(i+1)) + h_sys_hadlow->GetBinContent(i+1);

		y_corrected_lumi[i] = (y_lumi[i] - h_jpsi_n->GetBinContent(i+1)*jpsi_AN[i] - h_hadcontam_n->GetBinContent(i+1)*had_AN[i])/(1-total_bg[i]);
		corrected_lumi_err[i] = std::sqrt(std::pow(lumi_minus->GetErrorYhigh(i), 2) + std::pow(h_jpsi_n->GetBinContent(i+1)*jpsi_AN_err[i],2) + std::pow(h_hadcontam_n->GetBinContent(i+1)*had_AN_err[i],2))/(1-total_bg[i]);
		bg_sys_plus[i] = fabs(y_corrected_lumi[i] - (y_lumi[i] - (h_jpsi_n->GetBinContent(i+1)+h_sys_jpsi->GetBinContent(i+1)+h_sys2_jpsi->GetBinContent(i+1))*jpsi_AN[i] - h_sys_hadhigh->GetBinContent(i+1)*had_AN[i])/(1-total_bg_plus[i]));
		bg_sys_minus[i] = fabs(y_corrected_lumi[i] -(y_lumi[i] - (h_jpsi_n->GetBinContent(i+1)-h_sys_jpsi->GetBinContent(i+1)-h_sys2_jpsi->GetBinContent(i+1))*jpsi_AN[i] - h_sys_hadlow->GetBinContent(i+1)*had_AN[i])/(1-total_bg_minus[i]));
		//jpsi_sys_minus[i] = fabs(y_corrected_lumi[i] -(y_lumi[i] - h_jpsi_n->GetBinContent(i+1)*jpsi_AN_minus[i] - h_sys_hadlow->GetBinContent(i+1)*had_AN[i])/(1-total_bg_minus[i]));

		//sys_plus_tot[i] = std::sqrt(bg_sys_plus[i]*bg_sys_plus[i] + jpsi_sys_plus[i]*jpsi_sys_plus[i] + sysFormulaDiff*sysFormulaDiff);
		//sys_minus_tot[i] = std::sqrt(bg_sys_minus[i]*bg_sys_minus[i] + jpsi_sys_minus[i]*jpsi_sys_minus[i] + sysFormulaDiff*sysFormulaDiff);
		sys_plus_tot[i] = std::sqrt(bg_sys_plus[i]*bg_sys_plus[i]  + sysFormulaDiff*sysFormulaDiff);
		sys_minus_tot[i] = std::sqrt(bg_sys_minus[i]*bg_sys_minus[i] + sysFormulaDiff*sysFormulaDiff);

		y_corrected_lumi_np[i] = (y_lumi[i] - h_hadcontam_n->GetBinContent(i+1)*had_AN[i])/(1-total_bg_np[i]);
		corrected_lumi_err_np[i] = std::sqrt(std::pow(lumi_minus->GetErrorYhigh(i), 2) + std::pow(h_hadcontam_n->GetBinContent(i+1)*had_AN_err[i],2))/(1-total_bg_np[i]);

		bg_sys_plus_np[i] = fabs(y_corrected_lumi_np[i] - (y_lumi[i] - h_sys_hadhigh->GetBinContent(i+1)*had_AN[i])/(1-total_bg_np_plus[i]));
		bg_sys_minus_np[i] = fabs(y_corrected_lumi_np[i] - (y_lumi[i] - h_sys_hadlow->GetBinContent(i+1)*had_AN[i])/(1-total_bg_np_minus[i]));
		sys_plus_tot_np[i] = std::sqrt(bg_sys_plus_np[i]*bg_sys_plus_np[i] + sysFormulaDiff*sysFormulaDiff);
		sys_minus_tot_np[i] = std::sqrt(bg_sys_minus_np[i]*bg_sys_minus_np[i] + sysFormulaDiff*sysFormulaDiff);

		sysDiff[i] = sysFormulaDiff;

		//cout << "total_bg_frac : " << total_bg[i] << " + " << total_bg_plus[i] << " - " << total_bg_minus[i] << endl;
		//cout << "total_bg_frac_np : " << total_bg_np[i] << " + " << total_bg_np_plus[i] << " - " << total_bg_np_minus[i] << endl;

		if ( verbosity == -1 )
		{	
			cout << "sysFormulaDiff : " << sysFormulaDiff << endl;
        }
		if ( verbosity == 1 )
		{	
			cout << " AN values and statistical errors : " << endl;
			cout << "corrected lumi " << i << " "<< y_corrected_lumi[i] << endl;
			cout << "corrected lumi error " << i << " "<< corrected_lumi_err[i] << endl;
			cout << "corrected lumi np  " << i << " "<< y_corrected_lumi_np[i] << endl;
			cout << "corrected lumi np error " << i << " "<< corrected_lumi_err_np[i] << endl;

			cout << " Systematic errors : " << endl;
			cout << "sysFormulaDiff : " << sysFormulaDiff << endl;
			cout << "sysBGfrac (ohfe) : " << " + " << bg_sys_plus[i] << " - " << bg_sys_minus[i] << endl;
			cout << "sysBGfrac (np) : " << " + " << bg_sys_plus_np[i] << " - " << bg_sys_minus_np[i] << endl;
			cout << "totalSys (ohfe) : " << " + " << sys_plus_tot[i] << " - " << sys_minus_tot[i] << endl;
			cout << "totalSys (np) : " << " + " << sys_plus_tot_np[i] << " - " << sys_minus_tot_np[i] << endl;

		}

		if (verbosity == 2)
		{
			
			cout << "...............bin " << i << " ................." << endl;
			cout << "hadcontam bg frac : " << h_hadcontam_n->GetBinContent(i+1) << endl;
			cout << "pi0 bg frac : " << h_piz_n->GetBinContent(i+1) << endl;
			cout << "eta bg frac : " << h_eta_n->GetBinContent(i+1) << endl;
			cout << "gamma bg frac : " << h_photon_n->GetBinContent(i+1) << endl;
			cout << "jpsi bg frac : " << h_jpsi_n->GetBinContent(i+1) << endl;
		}
		//corrected_lumi->SetPoint(x_lumi[i], y_corrected_lumi[i]);

		if ( verbosity == 3 )
		{	
        	cout << "piz sys 2 : " << h_sys2_piz->GetBinContent(i+1) << endl;
        	cout << "jpsi sys 2 : " << h_sys2_jpsi->GetBinContent(i+1) << endl;
			if (i<2)
            {
				cout <<  "tot_bg_p " << total_bg_plus[i] << " tot_bg_m " << total_bg_minus[i] <<  endl;
				cout << "corrected lumi error " << i << " "<< corrected_lumi_err[i] << endl;
			    cout << "Systematic errors : " << endl;
			    cout << "sysFormulaDiff : " << sysFormulaDiff << endl;
			    cout << "sysBGfrac (ohfe) : " << " + " << bg_sys_plus[i] << " - " << bg_sys_minus[i] << endl;
			    cout << "sysBGfrac (np) : " << " + " << bg_sys_plus_np[i] << " - " << bg_sys_minus_np[i] << endl;
			    cout << "totalSys (ohfe) : " << " + " << sys_plus_tot[i] << " - " << sys_minus_tot[i] << endl;
			    cout << "totalSys (np) : " << " + " << sys_plus_tot_np[i] << " - " << sys_minus_tot_np[i] << endl;
		    }
		}
	} 
 
	double ptLow[nbins];
	double ptHigh[nbins];
	double x_low[nbins], x_high[nbins];
	TCanvas *can[2];
	for( int i = 0; i < nbins; ++i )
	{
		ptLow[i]  = fabs( VALUE_BINS[i] - x_lumi[i] );
		ptHigh[i] = fabs( VALUE_BINS[i + 1] - x_lumi[i] );
		x_low[i] = x_lumi[i] - 0.03; x_high[i] = x_lumi[i] + 0.03;

	}

    if (!prelim&&!sysTables)
	{

        cout << std::setprecision(3) << "        \\hline" << endl;
        cout << "        1.0 -- 1.3 & 1.161  & " << y_corrected_lumi[0] << " & "  << corrected_lumi_err[0] << " & " << y_corrected_lumi_np[0] << " & "  << corrected_lumi_err_np[0] << " & "  << y_lumi[0] <<  " & " << lumi_err[0] << " \\\\ " << endl;
        cout << "        1.3 -- 1.5 & 1.398  & " << y_corrected_lumi[1] << " & "  << corrected_lumi_err[1] << " & " << y_corrected_lumi_np[1] << " & "  << corrected_lumi_err_np[1] << " & "  << y_lumi[1] <<  " & " << lumi_err[1] << " \\\\ " << endl;
        cout << "        1.5 -- 1.8 & 1.639  & " << y_corrected_lumi[2] << " & "  << corrected_lumi_err[2] << " & " << y_corrected_lumi_np[2] << " & "  << corrected_lumi_err_np[2] << " & "  << y_lumi[2] <<  " & " << lumi_err[2] << " \\\\ " << endl;
        cout << "        1.8 -- 2.1 & 1.936  & " << y_corrected_lumi[3] << " & "  << corrected_lumi_err[3] << " & " << y_corrected_lumi_np[3] << " & "  << corrected_lumi_err_np[3] << " & "  << y_lumi[3] <<  " & " << lumi_err[3] << " \\\\ " << endl;
        cout << "        2.1 -- 2.7 & 2.349  & " << y_corrected_lumi[4] << " & "  << corrected_lumi_err[4] << " & " << y_corrected_lumi_np[4] << " & "  << corrected_lumi_err_np[4] << " & "  << y_lumi[4] <<  " & " << lumi_err[4] << " \\\\ " << endl;
        cout << "        2.7 -- 5.0 & 3.290  & " << y_corrected_lumi[5] << " & "  << corrected_lumi_err[5] << " & " << y_corrected_lumi_np[5] << " & "  << corrected_lumi_err_np[5] << " & "  << y_lumi[5] <<  " & " << lumi_err[5] << " \\\\ " << endl;
        cout << "        \\hline" << endl;
	}

    if (prelim&&!sysTables)
	{

        cout << std::setprecision(3) << "        \\hline" << endl;
        cout << "        1.0 -- 1.3 & 1.161  & " << y_corrected_lumi[0] << " & "  << corrected_lumi_err[0] << " & " << sys_plus_tot[0] << " & "  << sys_minus_tot[0] << " \\\\ " << endl;
        cout << "        1.3 -- 1.5 & 1.398  & " << y_corrected_lumi[1] << " & "  << corrected_lumi_err[1] << " & " << sys_plus_tot[1] << " & "  << sys_minus_tot[1] << " \\\\ " << endl;
        cout << "        1.5 -- 1.8 & 1.639  & " << y_corrected_lumi[2] << " & "  << corrected_lumi_err[2] << " & " << sys_plus_tot[2] << " & "  << sys_minus_tot[2] << " \\\\ " << endl;
        cout << "        1.8 -- 2.1 & 1.936  & " << y_corrected_lumi[3] << " & "  << corrected_lumi_err[3] << " & " << sys_plus_tot[3] << " & "  << sys_minus_tot[3] << " \\\\ " << endl;
        cout << "        2.1 -- 2.7 & 2.349  & " << y_corrected_lumi[4] << " & "  << corrected_lumi_err[4] << " & " << sys_plus_tot[4] << " & "  << sys_minus_tot[4] << " \\\\ " << endl;
        cout << "        2.7 -- 5.0 & 3.290  & " << y_corrected_lumi[5] << " & "  << corrected_lumi_err[5] << " & " << sys_plus_tot[5] << " & "  << sys_minus_tot[5] << " \\\\ " << endl;
        cout << "        \\hline" << endl;

        cout << std::setprecision(3) << "        \\hline" << endl;
        cout << "        1.0 -- 1.3 & 1.161  & " << y_corrected_lumi_np[0] << " & "  << corrected_lumi_err_np[0] << " & " <<sys_plus_tot_np[0] << " & "  << sys_minus_tot_np[0] << " \\\\ " << endl;
        cout << "        1.3 -- 1.5 & 1.398  & " << y_corrected_lumi_np[1] << " & "  << corrected_lumi_err_np[1] << " & " <<sys_plus_tot_np[1] << " & "  << sys_minus_tot_np[1] << " \\\\ " << endl;
        cout << "        1.5 -- 1.8 & 1.639  & " << y_corrected_lumi_np[2] << " & "  << corrected_lumi_err_np[2] << " & " <<sys_plus_tot_np[2] << " & "  << sys_minus_tot_np[2] << " \\\\ " << endl;
        cout << "        1.8 -- 2.1 & 1.936  & " << y_corrected_lumi_np[3] << " & "  << corrected_lumi_err_np[3] << " & " <<sys_plus_tot_np[3] << " & "  << sys_minus_tot_np[3] << " \\\\ " << endl;
        cout << "        2.1 -- 2.7 & 2.349  & " << y_corrected_lumi_np[4] << " & "  << corrected_lumi_err_np[4] << " & " <<sys_plus_tot_np[4] << " & "  << sys_minus_tot_np[4] << " \\\\ " << endl;
        cout << "        2.7 -- 5.0 & 3.290  & " << y_corrected_lumi_np[5] << " & "  << corrected_lumi_err_np[5] << " & " <<sys_plus_tot_np[5] << " & "  << sys_minus_tot_np[5] << " \\\\ " << endl;
        cout << "        \\hline" << endl;
	}
	if (sysTables)
	{
        cout << std::setprecision(3) << "        \\hline" << endl;
        cout << "        1.0 -- 1.3 & 1.161  & " << bg_sys_plus[0] << " & "  <<  bg_sys_minus[0] << " \\\\ " << endl;
        cout << "        1.3 -- 1.5 & 1.398  & " << bg_sys_plus[1] << " & "  <<  bg_sys_minus[1] << " \\\\ " << endl;
        cout << "        1.5 -- 1.8 & 1.639  & " << bg_sys_plus[2] << " & "  <<  bg_sys_minus[2] << " \\\\ " << endl;
        cout << "        1.8 -- 2.1 & 1.936  & " << bg_sys_plus[3] << " & "  <<  bg_sys_minus[3] << " \\\\ " << endl;
        cout << "        2.1 -- 2.7 & 2.349  & " << bg_sys_plus[4] << " & "  <<  bg_sys_minus[4] << " \\\\ " << endl;
        cout << "        2.7 -- 5.0 & 3.290  & " << bg_sys_plus[5] << " & "  <<  bg_sys_minus[5] << " \\\\ " << endl;
        cout << "        \\hline" << endl;	

        cout << std::setprecision(3) << "        \\hline" << endl;
        cout << "        1.0 -- 1.3 & 1.161  & " << bg_sys_plus_np[0] << " & "  <<  bg_sys_minus_np[0] << " \\\\ " << endl;
        cout << "        1.3 -- 1.5 & 1.398  & " << bg_sys_plus_np[1] << " & "  <<  bg_sys_minus_np[1] << " \\\\ " << endl;
        cout << "        1.5 -- 1.8 & 1.639  & " << bg_sys_plus_np[2] << " & "  <<  bg_sys_minus_np[2] << " \\\\ " << endl;
        cout << "        1.8 -- 2.1 & 1.936  & " << bg_sys_plus_np[3] << " & "  <<  bg_sys_minus_np[3] << " \\\\ " << endl;
        cout << "        2.1 -- 2.7 & 2.349  & " << bg_sys_plus_np[4] << " & "  <<  bg_sys_minus_np[4] << " \\\\ " << endl;
        cout << "        2.7 -- 5.0 & 3.290  & " << bg_sys_plus_np[5] << " & "  <<  bg_sys_minus_np[5] << " \\\\ " << endl;
        cout << "        \\hline" << endl;	

        cout << std::setprecision(3) << "        \\hline" << endl;
        cout << "        1.0 -- 1.3 & 1.161  & " << bg_sys_plus[0] << " & "  <<  bg_sys_minus[0] << " & " <<  sysDiff[0] << " & " << sys_plus_tot[0] << " & " << sys_minus_tot[0] << " \\\\ " << endl;
        cout << "        1.3 -- 1.5 & 1.398  & " << bg_sys_plus[1] << " & "  <<  bg_sys_minus[1] << " & " <<  sysDiff[1] << " & " << sys_plus_tot[1] << " & " << sys_minus_tot[1] << " \\\\ " << endl;
        cout << "        1.5 -- 1.8 & 1.639  & " << bg_sys_plus[2] << " & "  <<  bg_sys_minus[2] << " & " <<  sysDiff[2] << " & " << sys_plus_tot[2] << " & " << sys_minus_tot[2] << " \\\\ " << endl;
        cout << "        1.8 -- 2.1 & 1.936  & " << bg_sys_plus[3] << " & "  <<  bg_sys_minus[3] << " & " <<  sysDiff[3] << " & " << sys_plus_tot[3] << " & " << sys_minus_tot[3] << " \\\\ " << endl;
        cout << "        2.1 -- 2.7 & 2.349  & " << bg_sys_plus[4] << " & "  <<  bg_sys_minus[4] << " & " <<  sysDiff[4] << " & " << sys_plus_tot[4] << " & " << sys_minus_tot[4] << " \\\\ " << endl;
        cout << "        2.7 -- 5.0 & 3.290  & " << bg_sys_plus[5] << " & "  <<  bg_sys_minus[5] << " & " <<  sysDiff[5] << " & " << sys_plus_tot[5] << " & " << sys_minus_tot[5] << " \\\\ " << endl;
        cout << "        \\hline" << endl;

        cout << std::setprecision(3) << "        \\hline" << endl;
        cout << "        1.0 -- 1.3 & 1.161  & " << bg_sys_plus_np[0] << " & "  <<  bg_sys_minus_np[0] << " & " <<  sysDiff[0] << " & " << sys_plus_tot_np[0] << " & " << sys_minus_tot_np[0] << " \\\\ " << endl;
        cout << "        1.3 -- 1.5 & 1.398  & " << bg_sys_plus_np[1] << " & "  <<  bg_sys_minus_np[1] << " & " <<  sysDiff[1] << " & " << sys_plus_tot_np[1] << " & " << sys_minus_tot_np[1] << " \\\\ " << endl;
        cout << "        1.5 -- 1.8 & 1.639  & " << bg_sys_plus_np[2] << " & "  <<  bg_sys_minus_np[2] << " & " <<  sysDiff[2] << " & " << sys_plus_tot_np[2] << " & " << sys_minus_tot_np[2] << " \\\\ " << endl;
        cout << "        1.8 -- 2.1 & 1.936  & " << bg_sys_plus_np[3] << " & "  <<  bg_sys_minus_np[3] << " & " <<  sysDiff[3] << " & " << sys_plus_tot_np[3] << " & " << sys_minus_tot_np[3] << " \\\\ " << endl;
        cout << "        2.1 -- 2.7 & 2.349  & " << bg_sys_plus_np[4] << " & "  <<  bg_sys_minus_np[4] << " & " <<  sysDiff[4] << " & " << sys_plus_tot_np[4] << " & " << sys_minus_tot_np[4] << " \\\\ " << endl;
        cout << "        2.7 -- 5.0 & 3.290  & " << bg_sys_plus_np[5] << " & "  <<  bg_sys_minus_np[5] << " & " <<  sysDiff[5] << " & " << sys_plus_tot_np[5] << " & " << sys_minus_tot_np[5] << " \\\\ " << endl;
        cout << "        \\hline" << endl;	
	}


	//gStyle->SetErrorX(0.05);
   gStyle->SetEndErrorSize(4);
	can[0] = new TCanvas( "c", "Corrected A_{N}" );

	TGraphAsymmErrors *corrected_lumi = new TGraphAsymmErrors( nbins, x_high, y_corrected_lumi, ptLow, ptHigh, corrected_lumi_err, corrected_lumi_err); 
	TGraphAsymmErrors *corrected_lumi_sys = new TGraphAsymmErrors( nbins, x_high, y_corrected_lumi, ptLow, ptHigh, sys_minus_tot, sys_plus_tot); 

	TGraphAsymmErrors *corrected_lumi_np = new TGraphAsymmErrors( nbins, x_low, y_corrected_lumi_np, ptLow, ptHigh, corrected_lumi_err_np, corrected_lumi_err_np); 
	TGraphAsymmErrors *corrected_lumi_np_sys = new TGraphAsymmErrors( nbins, x_low, y_corrected_lumi_np, ptLow, ptHigh, sys_minus_tot_np, sys_plus_tot_np); 
	//TGraphAsymmErrors *corrected_lumi = new TGraphAsymmErrors( nbins, x_lumi, y_lumi, ptLow, ptHigh, lumi_minus->GetErrorY(), lumi_minus->GetErrorY()); 

	corrected_lumi->GetXaxis()->SetLimits(0.5,6.5);
	corrected_lumi_np->GetXaxis()->SetLimits(0.5,6.5);
	corrected_lumi->GetYaxis()->SetRangeUser(-0.06,0.085);
	corrected_lumi_np->GetYaxis()->SetRangeUser(-0.06,0.085);
	for (int i=0; i<nbins; ++i)
	{
		corrected_lumi->SetPointEXhigh(i,0.0); corrected_lumi->SetPointEXlow(i,0.0);
		corrected_lumi_sys->SetPointEXhigh(i,0.075); corrected_lumi_sys->SetPointEXlow(i,0.075);
		corrected_lumi_np->SetPointEXhigh(i,0.0); corrected_lumi_np->SetPointEXlow(i,0.0);
		corrected_lumi_np_sys->SetPointEXhigh(i,0.075); corrected_lumi_np_sys->SetPointEXlow(i,0.075);
		lumi_minus->SetPointEXhigh(i,0.0); lumi_minus->SetPointEXlow(i,0.0);
	}

	TF1 *line = new TF1( "line", "0", 0, 20 );
	line->SetLineColor( kBlack );
	line->SetLineStyle( 2 );

	TLegend *legend = new TLegend( 0.515, 0.55, 0.85, 0.75 );
	legend->SetLineColor(0);
	legend->SetTextSize(0.035);
	legend->AddEntry(corrected_lumi, "Open Heavy Flavor e^{-}", "lep" );
	if (!theory_compare || !prelim)
		legend->AddEntry(corrected_lumi_np, "Nonphotonic e^{-}", "lep" );

  if (prelim)
  {
		TLegend *legend2 = new TLegend( 0.13, 0.65, 0.425, 0.895 );
		legend2->SetLineColor(0);
		legend2->AddEntry((TObject*)0, "p^{#uparrow}+p #rightarrow e^{-} + X, #sqrt{s} = 200 GeV, |#eta| < 0.35", "");
		legend2->AddEntry((TObject*)0, "", "");
		legend2->SetTextSize(0.035);
		legend2->SetMargin(0.05);

		TLegend *legend3 = new TLegend( 0.362, 0.155, 0.895, 0.315 );
		legend3->SetLineColor(0);
		legend3->AddEntry((TObject*)0, "3.4% polarization scale uncertainty not included", "");
		legend3->SetTextSize(0.035);
		legend3->SetMargin(0.05);
  }

	if (!prelim)
		legend->AddEntry(lumi_minus, "Before BG Correction", "lep" );

	corrected_lumi->SetMarkerStyle(20);
	corrected_lumi->SetLineColor(kBlue); 
	corrected_lumi_sys->SetLineColor(kBlue);
	corrected_lumi->SetMarkerColor(kBlue);
	corrected_lumi->SetLineWidth(1.5); 
	
	corrected_lumi_sys->SetFillColor(kBlue);
	corrected_lumi_sys->SetFillStyle(3002);
	corrected_lumi->SetTitle("; p_{T} [GeV/c]; A_{N}");
	corrected_lumi->GetXaxis()->SetTitleOffset(1.05);
	corrected_lumi->GetXaxis()->SetTitleSize(0.042);
	corrected_lumi->GetYaxis()->SetTitleSize(0.05);
        corrected_lumi->GetXaxis()->SetLabelSize(0.0415);
        corrected_lumi->GetYaxis()->SetLabelSize(0.0415);
	corrected_lumi->Draw("AP");

	if (prelim)
		corrected_lumi_sys->Draw("5");

	corrected_lumi_np->SetMarkerStyle(21);
	corrected_lumi_np->SetLineColor(kGreen+2); 
	corrected_lumi_np_sys->SetLineColor(kGreen+2);
	corrected_lumi_np->SetMarkerColor(kGreen+2);
	corrected_lumi_np->SetLineWidth(1.5); 

	corrected_lumi_np_sys->SetFillColor(kGreen+2);
	corrected_lumi_np_sys->SetFillStyle(3002);
	if (!theory_compare || !prelim)
		corrected_lumi_np->Draw("P");

	if (prelim && !theory_compare)
		corrected_lumi_np_sys->Draw("5");

	lumi_minus->SetMarkerStyle(22);
	lumi_minus->SetMarkerSize(1.25);
	lumi_minus->SetLineWidth(1.5);

	if (!prelim)
		lumi_minus->Draw("P");

	line->Draw( "same" );
	legend->Draw();

	if (prelim)
  {
		legend2->Draw();
    legend3->Draw();
  }
	if (prelim && theory_compare)
	{
	  TFile *theoryFile = TFile::Open("inputFiles/antid0DecayAsymmetry_fitted_10B_weightsLoopZoomed.root");	 
	  TGraph *d0efdbestfit = (TGraphErrors*)theoryFile->Get("eAN_m0p01_0p11");
	  //TGraph *d0efdp1sig = (TGraphErrors*)theoryFile->Get("eAN_0p02_0p2");
	  //TGraph *d0efdm1sig = (TGraphErrors*)theoryFile->Get("eAN_m0p04_0p02");
	
	  TFile *theoryFileKGKGp = TFile::Open("inputFiles/antidcombinedDecayAsymmetry_fitted_10B_weightsLoopZoomed_KGKGp.root");	 	  
	  TGraph *deKGbestfit = (TGraphErrors*)theoryFileKGKGp->Get("eAN_KG_0p0006");
	  TGraph *deKGpbestfit = (TGraphErrors*)theoryFileKGKGp->Get("eAN_KGp_0p0002");	  	  	  

    d0efdbestfit->SetLineWidth(2);
	  d0efdbestfit->Draw("SAME");
	  
    deKGbestfit->SetLineWidth(2);
    deKGbestfit->SetLineColor(kGreen-6);   	  
	  deKGbestfit->Draw("SAME");
	  
    deKGpbestfit->SetLineWidth(2);
    deKGpbestfit->SetLineColor(kViolet-3);   	  
	  deKGpbestfit->Draw("SAME");	
	  
	  legend->AddEntry((TObject*)0, "", "");
	  legend->AddEntry((TObject*)0, "PRD78, 114013 (#bar{D}^{0} #rightarrow e^{-})",""); 
	  legend->AddEntry(d0efdbestfit, "#lambda_{f} = -0.01 GeV, #\lambda_{d} = 0.11 GeV","l");	
	  
	  legend->AddEntry((TObject*)0, "", "");
	  legend->AddEntry((TObject*)0, "PRD84, 014026 ((#bar{D}^{0}+D^{-}) #rightarrow e^{+})","");
	  legend->AddEntry(deKGbestfit, "K_{G} = 0.0006","l");	
	  legend->AddEntry(deKGpbestfit, "K_{G}' = 0.00025","l");		
	}

	if( SAVE_IMAGES )
	{
		TString name = "./images/";
		if (prelim && theory_compare)
			name += "AN_minus_sys_theory";
		else if (prelim && !theory_compare)
			name += "AN_minus_sys";
		else
			name += "bgCorrected_AN_minus";
 
		name += ".png";
		can[0]->SaveAs( name );

		TString name = "./images/";
		if (prelim && theory_compare)
			name += "AN_minus_sys_theory";
		else if (prelim && !theory_compare)
			name += "AN_minus_sys";
		else
			name += "bgCorrected_AN_minus";
 
		name += ".pdf";
		can[0]->SaveAs( name );

	}

  TFile *outfile = new TFile("../dataFiles/bgCorrected_ohfe_AN_minus.root","RECREATE");
  outfile->cd();
  corrected_lumi->SetName("ohfeAN");
  corrected_lumi_np->SetName("npeAN");
  corrected_lumi->Write();
  corrected_lumi_np->Write();
  corrected_lumi->Delete();
  corrected_lumi_np->Delete();
  outfile->Write();
  outfile->Close();

}
