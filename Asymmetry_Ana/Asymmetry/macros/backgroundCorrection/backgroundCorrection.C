#include <cmath>
#include "../../Constants.h"
#include "../plotMacros/ttest.h"

void backgroundCorrection()
{
    int verbosity = 1;
    bool SAVE_IMAGES = 1;
    bool jpsi = 0;
    bool hadron = 0;
    const int nbins = NUM_VALUE_BINS;
    double x_lumi[nbins], y_lumi[nbins], x_sqrt[nbins], y_sqrt[nbins];
    double y_corrected_lumi[nbins], y_corrected_sqrt[nbins];
    double corrected_lumi_err[nbins], corrected_sqrt_err[nbins];
    double total_bg[nbins], had_AN[nbins], had_AN_err[nbins];

    // Taken from previous 200 GeV PHENIX measurements (Run 6) //
    double jpsi_AN_val = -0.065;
    double jpsi_AN_errVal = 0.158;
    double jpsi_AN[nbins] = {0.437*jpsi_AN_val, 0.671*jpsi_AN_val, 0.785*jpsi_AN_val, 0.868*jpsi_AN_val  };
    double jpsi_AN_err[nbins] = {0.437*jpsi_AN_errVal, 0.671*jpsi_AN_errVal, 0.785*jpsi_AN_errVal, 0.868*jpsi_AN_errVal  };

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
        sqrt0->GetPoint(i, x_sqrt[i], y_sqrt[i]);
        if ( verbosity == 1 )
        {
            //cout << "minus fraction : " << minus_fraction[i] << endl;
            //cout << "charged hadron AN : " << had_AN[i] << " err : " << had_AN_err[i] << endl;
            cout << "lumi : " << i << " "<< y_lumi[i] << endl;
            cout << "lumi err : " << i << " " << lumi->GetErrorYhigh(i) << endl;
        }
	    //total_bg[i] = h_piz_n->GetBinContent(i+1) + h_eta_n->GetBinContent(i+1) + h_photon_n->GetBinContent(i+1) + h_jpsi_n->GetBinContent(i+1) + h_hadcontam_n->GetBinContent(i+1) + h_ke3_n->GetBinContent(i+1);
	    total_bg[i] = h_piz_n->GetBinContent(i+1) + h_eta_n->GetBinContent(i+1) + h_photon_n->GetBinContent(i+1) + h_jpsi_n->GetBinContent(i+1) + h_hadcontam_n->GetBinContent(i+1);
	    if ( hadron && jpsi )
		{
	        y_corrected_lumi[i] = (y_lumi[i] - h_jpsi_n->GetBinContent(i+1)*jpsi_AN[i] - h_hadcontam_n->GetBinContent(i+1)*had_AN[i])/(1-total_bg[i]);
	        y_corrected_sqrt[i] = (y_sqrt[i] - h_jpsi_n->GetBinContent(i+1)*jpsi_AN[i] - h_hadcontam_n->GetBinContent(i+1)*had_AN[i])/(1-total_bg[i]);	
			corrected_lumi_err[i] = std::sqrt(std::pow(lumi->GetErrorYhigh(i), 2) + std::pow(h_jpsi_n->GetBinContent(i+1)*jpsi_AN_err[i],2) + std::pow(h_hadcontam_n->GetBinContent(i+1)*had_AN_err[i],2))/(1-total_bg[i]);
			corrected_sqrt_err[i] = std::sqrt(std::pow(sqrt0->GetErrorYhigh(i), 2) + std::pow(h_jpsi_n->GetBinContent(i+1)*jpsi_AN_err[i],2) + std::pow(h_hadcontam_n->GetBinContent(i+1)*had_AN_err[i],2))/(1-total_bg[i]);
		}
	    else if (hadron)
		{
	        y_corrected_lumi[i] = (y_lumi[i] - h_hadcontam_n->GetBinContent(i+1)*had_AN[i])/(1-total_bg[i]);
	        y_corrected_sqrt[i] = (y_sqrt[i] - h_hadcontam_n->GetBinContent(i+1)*had_AN[i])/(1-total_bg[i]);
			corrected_lumi_err[i] = std::sqrt(std::pow(lumi->GetErrorYhigh(i), 2) + std::pow(h_hadcontam_n->GetBinContent(i+1)*had_AN_err[i],2))/(1-total_bg[i]);
			corrected_sqrt_err[i] = std::sqrt(std::pow(sqrt0->GetErrorYhigh(i), 2) +  std::pow(h_hadcontam_n->GetBinContent(i+1)*had_AN_err[i],2))/(1-total_bg[i]);
		}	
	    else if (jpsi)
		{
	        y_corrected_lumi[i] = (y_lumi[i] - h_jpsi_n->GetBinContent(i+1)*jpsi_AN[i])/(1-total_bg[i]);
	        y_corrected_sqrt[i] = (y_sqrt[i] - h_jpsi_n->GetBinContent(i+1)*jpsi_AN[i])/(1-total_bg[i]);
			corrected_lumi_err[i] = std::sqrt(std::pow(lumi->GetErrorYhigh(i), 2) + std::pow(h_jpsi_n->GetBinContent(i+1)*jpsi_AN_err[i],2))/(1-total_bg[i]);
			corrected_sqrt_err[i] = std::sqrt(std::pow(sqrt0->GetErrorYhigh(i), 2) + std::pow(h_jpsi_n->GetBinContent(i+1)*jpsi_AN_err[i],2))/(1-total_bg[i]);
		}	
	    else
		{
            y_corrected_lumi[i] = (y_lumi[i])/(1-total_bg[i]);
            y_corrected_sqrt[i] = (y_sqrt[i])/(1-total_bg[i]);
			corrected_lumi_err[i] = lumi->GetErrorYhigh(i)/(1-total_bg[i]);
			corrected_sqrt_err[i] = sqrt0->GetErrorYhigh(i)/(1-total_bg[i]);
		}

        if ( verbosity == 1 )
		{	
	        	cout << "corrected lumi " << i << " "<< y_corrected_lumi[i] << endl;
			cout << "corrected lumi error " << i << " "<< corrected_lumi_err[i] << endl;
		}

        //corrected_lumi->SetPoint(x_lumi[i], y_corrected_lumi[i]);
    } 
 
	double ptLow[nbins];
	double ptHigh[nbins];
	TCanvas *can[2];
    for( int i = 0; i < NUM_VALUE_BINS; ++i )
    {
        ptLow[i]  = fabs( VALUE_BINS[i] - x_lumi[i] );
        ptHigh[i] = fabs( VALUE_BINS[i + 1] - x_lumi[i] );
    }

	can[0] = new TCanvas( "c", "Corrected A_{N}" );
	TGraphAsymmErrors *corrected_lumi = new TGraphAsymmErrors( nbins, x_lumi, y_corrected_lumi, ptLow, ptHigh, corrected_lumi_err, corrected_lumi_err); 
	TGraphAsymmErrors *corrected_sqrt = new TGraphAsymmErrors( nbins, x_sqrt, y_corrected_sqrt, ptLow, ptHigh, corrected_sqrt_err, corrected_sqrt_err); 

	TF1 *line = new TF1( "line", "0", 0, 20 );
	line->SetLineColor( kBlack );
	line->SetLineStyle( 2 );

	if (hadron && jpsi )
		corrected_sqrt->SetTitle("Background Corrected A_{N}");
	else if (hadron)
		corrected_sqrt->SetTitle("Background Corrected A_{N} with A_{N}^{J/#psi} = 0 ");
	else if (jpsi)
		corrected_sqrt->SetTitle("Background Corrected A_{N} with A_{N}^{h^{#pm}} = 0 ");
	else
		corrected_sqrt->SetTitle("Background Corrected A_{N} with A_{N}^{J/#psi} = A_{N}^{h^{#pm}} = 0 ");

    TLegend *legend = new TLegend( 0.55, 0.9, 0.95, 0.7 );
    legend->AddEntry( corrected_sqrt, "Square Root Formula", "lep" );
    legend->AddEntry( corrected_lumi, "Relative Luminosity Formula", "lep" );

    corrected_lumi->SetMarkerStyle( kFullDiamond );
    corrected_lumi->SetMarkerColor( kViolet - 6 );
    corrected_lumi->SetMarkerSize( 2 );

    corrected_sqrt->SetMarkerStyle( kFullSquare );
    corrected_sqrt->SetMarkerColor( kRed );
    corrected_sqrt->SetMarkerSize( 0.7* 2 );

    corrected_sqrt->SetTitle(";p_{T} [GeV]; A_{N}");
    corrected_sqrt->Draw("AP");
    corrected_lumi->Draw("P");
    line->Draw( "same" );

	legend->Draw();

    if( SAVE_IMAGES )
	{
		TString name = "./images/";
		name += "compareCorrectedSqrtLumi";
		if (hadron && jpsi )
			name += "HadJPsiCorr";
		else if (hadron)
			name += "HadCorr";
		else if (jpsi)
			name += "JPsiCorr";
		else
			name += "DilutionOnly";

		TString ttestName = name;
		name += ".png";
		can[0]->SaveAs( name );

	}

    can[1] = new TCanvas( "c0", "Corrected A_{N} tTest" );
    TGraph *tTest; 
    tTest = ttest( corrected_sqrt, corrected_lumi, 1 );
    tTest->SetMarkerColor( kBlack );
    tTest->SetMarkerStyle( kFullCircle );
    tTest->SetTitle( "; p_{T}[GeV];T" );
    tTest->GetYaxis()->SetRangeUser( -3, 3 );
    tTest->Draw( "AP" );
    line->Draw( "same" );
	//corrected_sqrt->Draw("SAME");

    if( SAVE_IMAGES )
	{
		ttestName += "TTest.png";
		can[1]->SaveAs( ttestName );
	}

}
