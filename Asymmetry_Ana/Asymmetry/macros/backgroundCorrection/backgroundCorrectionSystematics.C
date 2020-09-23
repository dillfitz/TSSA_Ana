#include <cmath>
#include "../../Constants.h"
#include "../plotMacros/ttest.h"

void backgroundCorrectionSystematics()
{
	bool prelim = 0;
	int verbosity = 1;
	bool SAVE_IMAGES = 1;
	bool jpsi = 1;
	const int nbins = NUM_VALUE_BINS;
	double x_lumi[nbins], y_lumi[nbins], y_sqrt[nbins];
	double y_corrected_lumi[nbins], y_corrected_lumi_np[nbins];
	double corrected_lumi_err[nbins], corrected_lumi_err_np[nbins];
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
	double jpsi_AN_val = -0.065; 
	double jpsi_AN_errVal = 0.158;
	double jpsi_AN[nbins] = {0.447*jpsi_AN_val, 0.688*jpsi_AN_val, 0.789*jpsi_AN_val, 0.871*jpsi_AN_val  };
	//double jpsi_AN_plus[nbins] = {0.435*(jpsi_AN_val+jpsi_AN_errVal), 0.701*(jpsi_AN_val+jpsi_AN_errVal), 0.794*(jpsi_AN_val+jpsi_AN_errVal), 0.871*(jpsi_AN_val+jpsi_AN_errVal)};
	//double jpsi_AN_minus[nbins] = {0.440*(jpsi_AN_val-jpsi_AN_errVal), 0.693*(jpsi_AN_val-jpsi_AN_errVal), 0.793*(jpsi_AN_val-jpsi_AN_errVal), 0.871*(jpsi_AN_val-jpsi_AN_errVal)};
	double jpsi_AN_err[nbins] = {0.447*jpsi_AN_errVal, 0.688*jpsi_AN_errVal, 0.789*jpsi_AN_errVal, 0.872*jpsi_AN_errVal  };
	
	double hadminus_AN[nbins] = {-0.0012, -0.0012, -0.021, -0.021};
	double hadminus_AN_err[nbins] = {0.0082, 0.0082, 0.027, 0.027};
	
	double hadplus_AN[nbins] = {-0.0054, -0.0054, -0.031, -0.031};
	double hadplus_AN_err[nbins] = {0.0078, 0.0078, 0.026, 0.026};
	
	//TGraphAsymmErrors *corrected_lumi = new TGraphAsymmErrors();
	
	TFile *infile1 = TFile::Open("inputFiles/normalizations.root");
	TFile *infile2 = TFile::Open("inputFiles/systematics.root");
	TFile *infile3 = TFile::Open("inputFiles/hadcontam.root");
	TFile *infile4 = TFile::Open("inputFiles/curated_ohfe.root");
	
	TTree* eTree = (TTree*)infile4->Get( "e_svx_tree" );
	
	TH1F* h_piz_n = (TH1F*)infile1->Get("h_piz_n");
	TH1F* h_eta_n = (TH1F*)infile1->Get("h_eta_n");
	TH1F* h_jpsi_n = (TH1F*)infile1->Get("h_jpsi_n");
	TH1F* h_ke3_n = (TH1F*)infile1->Get("h_ke3_n");
	TH1F* h_photon_n = (TH1F*)infile1->Get("h_photon_n");
	TH1F* h_hadcontam_n = (TH1F*)infile1->Get("h_hadcontam_n");
	TH1F* h_sys_piz = (TH1F*)infile2->Get("h_sys_piz");
	TH1F* h_sys_eta = (TH1F*)infile2->Get("h_sys_eta");
	TH1F* h_sys_ke3 = (TH1F*)infile2->Get("h_sys_ke3");
	TH1F* h_sys_jpsi = (TH1F*)infile2->Get("h_sys_jpsi");
	TH1F* h_sys_photon = (TH1F*)infile2->Get("h_sys_photon");
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

	TFile *infile0 = TFile::Open("inputFiles/ohfe_AN.root");

	for (int i=0; i<nbins; ++i)
	{
		had_AN[i] = minus_fraction[i]*hadminus_AN[i] + (1-minus_fraction[i])*hadplus_AN[i];
		had_AN_err[i] = minus_fraction[i]*hadminus_AN_err[i] + (1-minus_fraction[i])*hadplus_AN_err[i];
		
		lumi->GetPoint(i, x_lumi[i], y_lumi[i]);
		sqrt0->GetPoint(i, x_lumi[i], y_sqrt[i]);
		double sysFormulaDiff = fabs(y_lumi[i] - y_sqrt[i]);
		if ( verbosity == 1 )
		{
			//cout << "minus fraction : " << minus_fraction[i] << endl;
			//cout << "charged hadron AN : " << had_AN[i] << " err : " << had_AN_err[i] << endl;
			cout << "lumi : " << i << " "<< y_lumi[i] << endl;
			cout << "lumi err : " << i << " " << lumi->GetErrorYhigh(i) << endl;
		}
		//total_bg[i] = h_piz_n->GetBinContent(i+1) + h_eta_n->GetBinContent(i+1) + h_photon_n->GetBinContent(i+1) + h_jpsi_n->GetBinContent(i+1) + h_hadcontam_n->GetBinContent(i+1) + h_ke3_n->GetBinContent(i+1);
		total_bg[i] = h_piz_n->GetBinContent(i+1) + h_eta_n->GetBinContent(i+1) + h_photon_n->GetBinContent(i+1) + h_jpsi_n->GetBinContent(i+1) + h_hadcontam_n->GetBinContent(i+1);
		total_bg_plus[i] = (h_piz_n->GetBinContent(i+1)+h_sys_piz->GetBinContent(i+1)) + (h_eta_n->GetBinContent(i+1)+h_sys_eta->GetBinContent(i+1)) + (h_photon_n->GetBinContent(i+1)+h_sys_photon->GetBinContent(i+1)) + (h_jpsi_n->GetBinContent(i+1)+h_sys_jpsi->GetBinContent(i+1)) + h_sys_hadhigh->GetBinContent(i+1);
		total_bg_minus[i] = (h_piz_n->GetBinContent(i+1)-h_sys_piz->GetBinContent(i+1)) + (h_eta_n->GetBinContent(i+1)-h_sys_eta->GetBinContent(i+1)) + (h_photon_n->GetBinContent(i+1)-h_sys_photon->GetBinContent(i+1)) + (h_jpsi_n->GetBinContent(i+1)-h_sys_jpsi->GetBinContent(i+1)) + h_sys_hadlow->GetBinContent(i+1);

		total_bg_np[i] = h_piz_n->GetBinContent(i+1) + h_eta_n->GetBinContent(i+1) + h_photon_n->GetBinContent(i+1) +  h_hadcontam_n->GetBinContent(i+1);
		total_bg_np_plus[i] = (h_piz_n->GetBinContent(i+1)+h_sys_piz->GetBinContent(i+1)) + (h_eta_n->GetBinContent(i+1)+h_sys_eta->GetBinContent(i+1)) + (h_photon_n->GetBinContent(i+1)+h_sys_photon->GetBinContent(i+1)) + h_sys_hadhigh->GetBinContent(i+1);
		total_bg_np_minus[i] = (h_piz_n->GetBinContent(i+1)-h_sys_piz->GetBinContent(i+1)) + (h_eta_n->GetBinContent(i+1)-h_sys_eta->GetBinContent(i+1)) + (h_photon_n->GetBinContent(i+1)-h_sys_photon->GetBinContent(i+1)) + h_sys_hadlow->GetBinContent(i+1);

		y_corrected_lumi[i] = (y_lumi[i] - h_jpsi_n->GetBinContent(i+1)*jpsi_AN[i] - h_hadcontam_n->GetBinContent(i+1)*had_AN[i])/(1-total_bg[i]);
		corrected_lumi_err[i] = std::sqrt(std::pow(lumi->GetErrorYhigh(i), 2) + std::pow(h_jpsi_n->GetBinContent(i+1)*jpsi_AN_err[i],2) + std::pow(h_hadcontam_n->GetBinContent(i+1)*had_AN_err[i],2))/(1-total_bg[i]);
		bg_sys_plus[i] = fabs(y_corrected_lumi[i] - (y_lumi[i] - (h_jpsi_n->GetBinContent(i+1)+h_sys_jpsi->GetBinContent(i+1))*jpsi_AN[i] - h_sys_hadhigh->GetBinContent(i+1)*had_AN[i])/(1-total_bg_plus[i]));
		bg_sys_minus[i] = fabs(y_corrected_lumi[i] -(y_lumi[i] - (h_jpsi_n->GetBinContent(i+1)-h_sys_jpsi->GetBinContent(i+1))*jpsi_AN[i] - h_sys_hadlow->GetBinContent(i+1)*had_AN[i])/(1-total_bg_minus[i]));
		//jpsi_sys_plus[i] = fabs(y_corrected_lumi[i] - (y_lumi[i] - h_jpsi_n->GetBinContent(i+1)*jpsi_AN_plus[i] - h_sys_hadhigh->GetBinContent(i+1)*had_AN[i])/(1-total_bg_plus[i]));
		//jpsi_sys_minus[i] = fabs(y_corrected_lumi[i] -(y_lumi[i] - h_jpsi_n->GetBinContent(i+1)*jpsi_AN_minus[i] - h_sys_hadlow->GetBinContent(i+1)*had_AN[i])/(1-total_bg_minus[i]));

		cout << "sysFormulaDiff : " << sysFormulaDiff << endl;
		//sys_plus_tot[i] = std::sqrt(bg_sys_plus[i]*bg_sys_plus[i] + jpsi_sys_plus[i]*jpsi_sys_plus[i] + sysFormulaDiff*sysFormulaDiff);
		//sys_minus_tot[i] = std::sqrt(bg_sys_minus[i]*bg_sys_minus[i] + jpsi_sys_minus[i]*jpsi_sys_minus[i] + sysFormulaDiff*sysFormulaDiff);
		sys_plus_tot[i] = std::sqrt(bg_sys_plus[i]*bg_sys_plus[i]  + sysFormulaDiff*sysFormulaDiff);
		sys_minus_tot[i] = std::sqrt(bg_sys_minus[i]*bg_sys_minus[i] + sysFormulaDiff*sysFormulaDiff);

		y_corrected_lumi_np[i] = (y_lumi[i] - h_hadcontam_n->GetBinContent(i+1)*had_AN[i])/(1-total_bg_np[i]);
		corrected_lumi_err_np[i] = std::sqrt(std::pow(lumi->GetErrorYhigh(i), 2) + std::pow(h_hadcontam_n->GetBinContent(i+1)*had_AN_err[i],2))/(1-total_bg_np[i]);

		bg_sys_plus_np[i] = fabs(y_corrected_lumi_np[i] - (y_lumi[i] - h_sys_hadhigh->GetBinContent(i+1)*had_AN[i])/(1-total_bg_np_plus[i]));
		bg_sys_minus_np[i] = fabs(y_corrected_lumi_np[i] - (y_lumi[i] - h_sys_hadlow->GetBinContent(i+1)*had_AN[i])/(1-total_bg_np_minus[i]));
		sys_plus_tot_np[i] = std::sqrt(bg_sys_plus_np[i]*bg_sys_plus_np[i] + sysFormulaDiff*sysFormulaDiff);
		sys_minus_tot_np[i] = std::sqrt(bg_sys_minus_np[i]*bg_sys_minus_np[i] + sysFormulaDiff*sysFormulaDiff);

		cout << "total_bg_frac : " << total_bg[i] << " + " << total_bg_plus[i] << " - " << total_bg_minus[i] << endl;
		cout << "total_bg_frac_np : " << total_bg_np[i] << " + " << total_bg_np_plus[i] << " - " << total_bg_np_minus[i] << endl;

		cout << "sysBGfrac (ohfe) : " << " + " << bg_sys_plus[i] << " - " << bg_sys_minus[i] << endl;
		cout << "sysBGfrac (np) : " << " + " << bg_sys_plus_np[i] << " - " << bg_sys_minus_np[i] << endl;


		if ( verbosity == 1 )
		{	
			cout << "corrected lumi " << i << " "<< y_corrected_lumi[i] << endl;
			cout << "corrected lumi error " << i << " "<< corrected_lumi_err[i] << endl;
		}
		//corrected_lumi->SetPoint(x_lumi[i], y_corrected_lumi[i]);
	} 
 
	double ptLow[nbins];
	double ptHigh[nbins];
	double x_low[nbins], x_high[nbins];
	TCanvas *can[2];
	for( int i = 0; i < NUM_VALUE_BINS; ++i )
	{
		ptLow[i]  = fabs( VALUE_BINS[i] - x_lumi[i] );
		ptHigh[i] = fabs( VALUE_BINS[i + 1] - x_lumi[i] );
		x_low[i] = x_lumi[i] - 0.05; x_high[i] = x_lumi[i] + 0.05;

	}

	//gStyle->SetErrorX(0.05);
   gStyle->SetEndErrorSize(4);
	can[0] = new TCanvas( "c", "Corrected A_{N}" );

	TGraphAsymmErrors *corrected_lumi = new TGraphAsymmErrors( nbins, x_high, y_corrected_lumi, ptLow, ptHigh, corrected_lumi_err, corrected_lumi_err); 
	TGraphAsymmErrors *corrected_lumi_sys = new TGraphAsymmErrors( nbins, x_high, y_corrected_lumi, ptLow, ptHigh, sys_minus_tot, sys_plus_tot); 

	TGraphAsymmErrors *corrected_lumi_np = new TGraphAsymmErrors( nbins, x_low, y_corrected_lumi_np, ptLow, ptHigh, corrected_lumi_err_np, corrected_lumi_err_np); 
	TGraphAsymmErrors *corrected_lumi_np_sys = new TGraphAsymmErrors( nbins, x_low, y_corrected_lumi_np, ptLow, ptHigh, sys_minus_tot_np, sys_plus_tot_np); 
	//TGraphAsymmErrors *corrected_lumi = new TGraphAsymmErrors( nbins, x_lumi, y_lumi, ptLow, ptHigh, lumi->GetErrorY(), lumi->GetErrorY()); 

	corrected_lumi->GetXaxis()->SetRangeUser(0.0,5.0);
	for (int i=0; i<nbins; ++i)
	{
		corrected_lumi->SetPointEXhigh(i,0.0); corrected_lumi->SetPointEXlow(i,0.0);
		corrected_lumi_sys->SetPointEXhigh(i,0.075); corrected_lumi_sys->SetPointEXlow(i,0.075);
		corrected_lumi_np->SetPointEXhigh(i,0.0); corrected_lumi_np->SetPointEXlow(i,0.0);
		corrected_lumi_np_sys->SetPointEXhigh(i,0.075); corrected_lumi_np_sys->SetPointEXlow(i,0.075);
		lumi->SetPointEXhigh(i,0.0); lumi->SetPointEXlow(i,0.0);
	}

	TF1 *line = new TF1( "line", "0", 0, 20 );
	line->SetLineColor( kBlack );
	line->SetLineStyle( 2 );

	TLegend *legend = new TLegend( 0.50, 0.15, 0.86, 0.35 );
	legend->SetLineColor(0);
	legend->AddEntry(corrected_lumi, "Open Heavy Flavor e", "lep" );
	legend->AddEntry(corrected_lumi_np, "Nonphotonic e", "lep" );

	TLegend *legend2 = new TLegend( 0.1275, 0.6210, 0.4756, 0.850 );
	legend2->SetLineColor(0);
	legend2->AddEntry((TObject*)0, "p^{#uparrow}+p, #sqrt{s} = 200 GeV", "");
	legend2->AddEntry((TObject*)0, "PHENIX Preliminary", "");


	legend->AddEntry(lumi, "Before BG Correction", "lep" );

	corrected_lumi->SetMarkerStyle(20);
	corrected_lumi->SetLineColor(kBlue); corrected_lumi_sys->SetLineColor(kBlue);
	corrected_lumi->SetMarkerColor(kBlue);
	corrected_lumi->SetLineWidth(1.5); 
	
	corrected_lumi_sys->SetFillColor(kBlue);
	corrected_lumi_sys->SetFillStyle(3002);
	corrected_lumi->SetTitle("; p_{T} [GeV]; A_{N}");
	corrected_lumi->GetXaxis()->SetTitleOffset(1.2);
	corrected_lumi->GetYaxis()->SetTitleSize(0.045);
	corrected_lumi->Draw("AP");
	if (prelim)
		corrected_lumi_sys->Draw("5");

	corrected_lumi_np->SetMarkerStyle(21);
	corrected_lumi_np->SetLineColor(kGreen+2); corrected_lumi_np_sys->SetLineColor(kGreen+2);
	corrected_lumi_np->SetMarkerColor(kGreen+2);
	corrected_lumi_np->SetLineWidth(1.5); 

	corrected_lumi_np_sys->SetFillColor(kGreen+2);
	corrected_lumi_np_sys->SetFillStyle(3002);
	corrected_lumi_np->Draw("P");
	if (prelim)
		corrected_lumi_np_sys->Draw("5");

	lumi->SetMarkerStyle(22);
	lumi->SetMarkerSize(1.25);
	lumi->SetLineWidth(1.5);
	if (!prelim)
		lumi->Draw("P");

	line->Draw( "same" );
	legend->Draw();
	if (prelim)
		legend2->Draw();

	if( SAVE_IMAGES )
	{
		TString name = "./images/";
		if (prelim)
			name += "prelim_AN";
		else
			name += "bgCorrected_AN";
 
		name += ".png";
		can[0]->SaveAs( name );

	}

}