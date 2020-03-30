void yields_633()
{

	const int nPt_bins = 10;

	TFile* in1 = new TFile( "ohfe.root", "READ" );
	TFile* in2 = new TFile( "forfnp.root", "READ" );
	//TFile* in2 = new TFile( "1p5_6_n03_weightedcombination.root", "READ" );

	TString hist_labels_df[nPt_bins] = { "_15_18","_18_21", "_21_24", "_24_27", "_27_30", "_30_35", "_35_40", "_40_45", "_45_50", "_50_60"};
	TString hist_labels_tr[nPt_bins] = { "1","2", "3", "4", "5", "6", "7", "8", "9", "10"};

        double pt[nPt_bins] = {1.65, 1.95, 2.25, 2.55, 2.85, 3.25, 3.75, 4.25, 4.75, 5.5}; 
        double pt_e[nPt_bins] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}; 
	double N_df[nPt_bins], N_tr[nPt_bins], ratio[nPt_bins];
	double N_df_e[nPt_bins], N_tr_e[nPt_bins], ratio_e[nPt_bins];
	TH1F *dcat_df[nPt_bins], *dcat_tr[nPt_bins]; 
	std::cout << "pt bin :" << " my yields " <<": Tim's yields" << std::endl;
	for (int i=0; i<nPt_bins; i++) {
		hist_name_df = "dcat" + hist_labels_df[i];
		//hist_name_tr = "run15ppdca" + hist_labels_tr[i];
		dcat_df[i] = (TH1F*) in1->Get(hist_name_df);
		//dcat_tr[i] = (TH1F*) in2->Get(hist_name_tr);
		N_df[i] = dcat_df[i]->Integral();
		//N_tr[i] = dcat_tr[i]->Integral();
		N_tr[i] = h_ept_conveto->GetBinContent(i+1);
		N_df_e[i] = sqrt(N_df[i]);
		N_tr_e[i] = sqrt(N_tr[i]);
		ratio[i] = N_df[i]/N_tr[i];
		ratio_e[i] = ratio[i]*sqrt((N_df_e[i]/N_df[i])*(N_df_e[i]/N_df[i]) + (N_tr_e[i]/N_tr[i])*(N_tr_e[i]/N_tr[i]));
		std::cout <<"   " << i << "   :   " << N_df[i] <<"   :  " <<  N_tr[i] << std::endl;

	}

	TGraph *gr_df     = new TGraph(nPt_bins,pt,N_df);
	TGraph *gr_tr     = new TGraph(nPt_bins,pt,N_tr);
	TGraph *gr_ratio  = new TGraph(nPt_bins,pt,ratio);
	//TGraphErrors *gr_df     = new TGraphErrors(nPt_bins,pt,N_df, pt_e, N_df_e);
	//TGraphErrors *gr_tr     = new TGraphErrors(nPt_bins,pt,N_tr, pt_e, N_tr_e);
	//TGraphErrors *gr_ratio  = new TGraphErrors(nPt_bins,pt,ratio, pt_e, ratio_e);
        TLegend *legend   = new TLegend(0.6,0.6,0.9,0.9);


        gr_tr->SetMarkerStyle(22);
	gr_tr->SetMarkerColor(4);
	gr_tr->SetMarkerSize(1.25);


        gr_df->SetMarkerStyle(21);
	gr_df->SetMarkerColor(3);
	gr_df->SetMarkerSize(1.25);
	gr_df->SetMaximum(60000.);
	gr_df->SetTitle("electron yields (PPG223 ana.633)");
	gr_df->GetYaxis()->SetTitle("N_{e}");
	gr_df->GetYaxis()->SetTitleOffset(1.25);
	gr_df->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	gr_df->GetXaxis()->SetTitleOffset(1.25);

        gr_ratio->SetMarkerStyle(21);
        gr_ratio->SetMarkerSize(1);
	gr_ratio->SetTitle("electron yield ratios (PPG223 ana.633)");
	gr_ratio->GetYaxis()->SetTitle("N_{meas}/N_{PPG223}");
	gr_ratio->GetYaxis()->SetTitleOffset(1.25);
	gr_ratio->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	gr_ratio->GetXaxis()->SetTitleOffset(1.25);
	gr_ratio->SetMinimum(0);
	//gr_ratio->SetMaximum(2);

	legend->AddEntry(gr_tr,"yields from PPG223", "p");
	legend->AddEntry(gr_df,"measured yields -- with momrecal", "p");

	TCanvas *c1 = new TCanvas("c1","c1");
	c1->SetLogy();
	gr_df->Draw("AP");
	gr_tr->Draw("P");
	legend->Draw();

	TCanvas *c2 = new TCanvas("c2","c2");
	gr_ratio->Draw("AP");

}
