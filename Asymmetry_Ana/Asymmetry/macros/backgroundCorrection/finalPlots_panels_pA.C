#include <iostream>
using namespace std;

#include "TF1.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TLatex.h"
#include "TH1.h"
#include "TH2.h"


void finalPlots_panels_pA( const char* particle = "eta" )
{

  if (particle == "pi0")
  {
    const int NUM_PT_BINS = 10;
    const float PT_BINS[ NUM_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 20 };
    //Table IX of PPG234 (PhysRevD.90.012006)
    const int NUM_PPG234_PT_BINS = 10;
    const double PPG234_PT[ NUM_PPG234_PT_BINS ] = 
    { 2.58, 3.42, 4.4, 5.4, 6.41, 7.42, 8.43, 9.43, 10.79, 13.53 };
    const double PPG234[ NUM_PPG234_PT_BINS ] = 
    {  0.000143,  -0.000343, 0.000335, 0.00233, -0.000689, 
    0.00193, -0.00238, 0.000404, 0.00734, -0.0105 };
    const double PPG234_STAT_ERR[ NUM_PPG234_PT_BINS ] = 
    {  0.000281,  0.000321, 0.000571, 0.00106, 0.00187,  
    0.00311,  0.00488, 0.00703,  0.00799, 0.0127 };
    const double PPG234_SYST_ERR[ NUM_PPG234_PT_BINS ] = 
    {  0.00012,  0.0000177, 0.0000657, 0.0000961, 0.000114,  
    0.00035,  0.000469, 0.000351, 0.000328, 0.000686 };
    TString label_pAu = "p^{#uparrow} + Au #rightarrow #pi^{0} + X, #sqrt{s} = 200 GeV, |#eta|<0.35";  
    TString label_pAl = "p^{#uparrow} + Al #rightarrow #pi^{0} + X, #sqrt{s} = 200 GeV, |#eta|<0.35";        
    TString label_pp = "p^{#uparrow} + p #rightarrow #pi^{0} + X, #sqrt{s} = 200 GeV, |#eta|<0.35";                      
  }
  
  if (particle == "eta")
  {
    const int NUM_PT_BINS = 8;
    const float PT_BINS[ NUM_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 10, 20 };
    //Table IX of PPG234 (PhysRevD.90.012006)
    const int NUM_PPG234_PT_BINS = 8;
    const double PPG234_PT[ NUM_PPG234_PT_BINS ] = 
    { 2.39, 3.53, 4.39, 5.4, 6.41, 7.42, 8.75, 11.76 };
    const double PPG234[ NUM_PPG234_PT_BINS ] = 
    {  0.00244,  -0.00199, -0.00331, -0.00139, 0.00222,
    0.0103, 0.00790, 0.0168 };
    const double PPG234_STAT_ERR[ NUM_PPG234_PT_BINS ] = 
    {  0.00183,  0.00159, 0.00248, 0.00421, 0.00709,  
    0.0115,  0.0137, 0.0219 };
    const double PPG234_SYST_ERR[ NUM_PPG234_PT_BINS ] = 
    {  0.000809,  0.0000899, 0.000151, 0.000244, 0.00112,  
    0.000720,  0.00125, 0.00426 }; 
    TString label_pAu = "p^{#uparrow} + Au #rightarrow #eta + X, #sqrt{s} = 200 GeV, |#eta|<0.35";  
    TString label_pAl = "p^{#uparrow} + Al #rightarrow #eta + X, #sqrt{s} = 200 GeV, |#eta|<0.35";        
    TString label_pp = "p^{#uparrow} + p #rightarrow #eta + X, #sqrt{s} = 200 GeV, |#eta|<0.35";    
  }  
  

  TString infileName = "../dataFiles/";
  infileName += particle;
  infileName += "_bgCorrected_pAu_AN.root";
  TFile *file_pAu = TFile::Open( infileName );
  infileName = "../dataFiles/";
  infileName += particle;
  infileName += "_bgCorrected_pAl_AN.root";
  TFile *file_pAl = TFile::Open( infileName );
  
  TGraphAsymmErrors *sqrtGraph_pAu = (TGraphAsymmErrors*)file_pAu->Get( "sqrt" );
  TGraphAsymmErrors *lumiGraph_pAu = (TGraphAsymmErrors*)file_pAu->Get( "lumi" );
	TH1F* h_rError_pAu = (TH1F*)file_pAu->Get("rError");	  
  double *ptCenters_pAu = lumiGraph_pAu->GetX();
  double *lumi_pAu      = lumiGraph_pAu->GetY();
  double *lumiErr_pAu   = lumiGraph_pAu->GetEYhigh();
  double *sqrtAs_pAu    = sqrtGraph_pAu->GetY();
  double *sqrtErr_pAu   = sqrtGraph_pAu->GetEYhigh();
  	
  TGraphAsymmErrors *sqrtGraph_pAl = (TGraphAsymmErrors*)file_pAl->Get( "sqrt" );
  TGraphAsymmErrors *lumiGraph_pAl = (TGraphAsymmErrors*)file_pAl->Get( "lumi" );
	TH1F* h_rError_pAl = (TH1F*)file_pAl->Get("rError");	
  double *ptCenters_pAl = lumiGraph_pAl->GetX();
  double *lumi_pAl      = lumiGraph_pAl->GetY();
  double *lumiErr_pAl   = lumiGraph_pAl->GetEYhigh();
  double *sqrtAs_pAl    = sqrtGraph_pAl->GetY();
  double *sqrtErr_pAl   = sqrtGraph_pAl->GetEYhigh();	 	

  
  TString shuffleSysString = "../dataFiles/";
  shuffleSysString += particle;
  shuffleSysString += "_pAu_shufflingSys.root";
  TFile *shufflingSysFile_pAu = TFile::Open(shuffleSysString);	
  
  shuffleSysString = "../dataFiles/";
  shuffleSysString += particle;
  shuffleSysString += "_pAl_shufflingSys.root";
  TFile *shufflingSysFile_pAl = TFile::Open(shuffleSysString);	  
  
  TH1F* h_shufflingSys_pAu = (TH1F*)shufflingSysFile_pAu->Get("sys");    
  TH1F* h_shufflingSys_pAl = (TH1F*)shufflingSysFile_pAl->Get("sys");  

  double ptError[ NUM_PT_BINS ];
  
  double boxError_pAu[ NUM_PT_BINS ], formulaDiff_pAu[ NUM_PT_BINS ];
  double boxError_pAl[ NUM_PT_BINS ], formulaDiff_pAl[ NUM_PT_BINS ];  
  for( int b = 0; b < NUM_PT_BINS; b++ )
  {
    ptError[b]  = 0.15;
    
    formulaDiff_pAu[b] =  lumi_pAu[b] - sqrtAs_pAu[b];
    boxError_pAu[b] = sqrt( formulaDiff_pAu[b]*formulaDiff_pAu[b] + h_rError_pAu->GetBinContent(b+1)*h_rError_pAu->GetBinContent(b+1) + h_shufflingSys_pAu->GetBinContent(b+1)*h_shufflingSys_pAu->GetBinContent(b+1) );
    
    formulaDiff_pAl[b] =  lumi_pAl[b] - sqrtAs_pAl[b];
    boxError_pAl[b] = sqrt( formulaDiff_pAl[b]*formulaDiff_pAl[b] + h_rError_pAl->GetBinContent(b+1)*h_rError_pAl->GetBinContent(b+1) + h_shufflingSys_pAl->GetBinContent(b+1)*h_shufflingSys_pAl->GetBinContent(b+1) );      
  }

  TCanvas c1("c1", "",200, 200, 800, 1500);
  gPad->SetBottomMargin(0.125);  
  c1.Divide(1,3,0,0);


  if (particle == "pi0")
    TH1 *frame = new TH2F( "frame", " ", 1, 1, 15, 1, -0.07, 0.12 );  
  if (particle == "eta")
    TH1 *frame = new TH2F( "frame", " ", 1, 1, 15, 1, -0.07, 0.12 );    
 
  frame->SetStats(0);
  frame->SetTitle( " ; p_{T} [GeV/c]; A_{N} " );

  float labelSize   = 0.055;
  TAxis *xAxis = frame->GetXaxis();
  xAxis->SetTitleSize( 0.05 );
  xAxis->SetLabelSize( labelSize );
  xAxis->SetTitleOffset( 1.2 );
  TAxis *yAxis = frame->GetYaxis();
  yAxis->CenterTitle( true );
  yAxis->SetTitleSize( 0.07 );
  yAxis->SetLabelSize( labelSize );
  yAxis->SetNdivisions( 504 );//310 );
  yAxis->SetTitleOffset( 0.7 );
  
  TGraphErrors *boxes_pAu = new TGraphErrors( NUM_PT_BINS, ptCenters_pAu, lumi_pAu, ptError, boxError_pAu );
  TGraphErrors *boxes_pAl = new TGraphErrors( NUM_PT_BINS, ptCenters_pAl, lumi_pAl, ptError, boxError_pAl );  

  boxes_pAu->SetLineColor( 38 );
  boxes_pAu->SetFillColor( 38 );
  boxes_pAl->SetLineColor( 30 );
  boxes_pAl->SetFillColor( 30 );    


  TGraphErrors *lines_pAu = new TGraphErrors( NUM_PT_BINS, ptCenters_pAu, lumi_pAu, 0, lumiErr_pAu );
  TGraphErrors *lines_pAl = new TGraphErrors( NUM_PT_BINS, ptCenters_pAl, lumi_pAl, 0, lumiErr_pAl );  

  lines_pAu->SetLineColor( kBlue );
  lines_pAu->SetMarkerColor( kBlue );
  lines_pAu->SetMarkerStyle( kFullCircle );   
     
  lines_pAl->SetLineColor( kGreen+2 );
  lines_pAl->SetMarkerColor( kGreen+2 );
  lines_pAl->SetMarkerStyle( kFullSquare );       
      
  TGraphErrors *PPG234Lines = new TGraphErrors( NUM_PPG234_PT_BINS, PPG234_PT, 
						PPG234, 0, PPG234_STAT_ERR );

  PPG234Lines->SetTitle("");
  PPG234Lines->GetXaxis()->SetLimits(1.,15.);
  PPG234Lines->GetYaxis()->SetRangeUser(-0.06,0.1);  
  PPG234Lines->SetLineColor( kBlack );//ue );
  PPG234Lines->SetMarkerColor( kBlack );//ue );
  PPG234Lines->SetMarkerStyle( 33 );
  PPG234Lines->SetMarkerSize(1.5);

    
  TGraphErrors *PPG234Boxes = new TGraphErrors( NUM_PPG234_PT_BINS, PPG234_PT, 
					     PPG234, ptError, PPG234_SYST_ERR );
  PPG234Boxes->SetLineColor( 11 );//38 );//kBlue );
  PPG234Boxes->SetFillColor( 11 );//38 );//kWhite );

  
  if (particle == "pi0")
  {
    TLegend *legend_pAu = new TLegend( 0.51, 0.6, 0.9, 0.8 );
    legend_pAu->SetMargin(0.25);
    TLegend *legend_pAl = new TLegend( 0.51, 0.6, 0.9, 0.8 );
    legend_pAl->SetMargin(0.25);   
    TLegend *legend_pp = new TLegend( 0.51, 0.6, 0.9, 0.8 );
    legend_pp->SetMargin(0.25);    
  }
  else if (particle == "eta")
  {
    TLegend *legend_pAu = new TLegend( 0.165, 0.5, 0.48, 0.8 );    
    legend_pAu->SetMargin(0.075);
    TLegend *legend_pAl = new TLegend( 0.165, 0.5, 0.48, 0.8 );    
    legend_pAl->SetMargin(0.075);
    TLegend *legend_pp = new TLegend( 0.165, 0.5, 0.48, 0.8 );    
    legend_pp->SetMargin(0.075);        
  }
  legend_pAu->AddEntry( lines_pAu, "(p+Au) This Result", "p" );
  legend_pAl->AddEntry( lines_pAl, "(p+Al) This Result", "p" );
  legend_pp->AddEntry( PPG234Lines, "(p+p) PRD103 052009 (2021)", "p" );
  legend_pAu->SetBorderSize( 0 );
  //legend_pAu->SetTextSize(0.0375);
  legend_pAl->SetBorderSize( 0 );
  //legend_pAl->SetTextSize(0.0375); 
  legend_pp->SetBorderSize( 0 );
  //legend_pp->SetTextSize(0.0375);   
  //legend->SetMargin(0.075);
  //legend->Draw();
 
  TF1 *zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlack );
  zeroLine->SetLineStyle( 2 );

  c1.cd(1);
  gPad->SetRightMargin(0.01);
  frame->Draw("");       
  boxes_pAu->Draw( "5" );
  lines_pAu->Draw( "P" );
  zeroLine->Draw( "same" );   
  legend_pAu->Draw();  
  
  TLatex text;
  text.SetTextSize( 0.065 );
  text.SetTextAlign( 13 );
  text.DrawLatex( 2, 0.115, label_pAu );
  
  text.SetTextSize(0.045);
  text.DrawLatex(2, 0.095, "3% Polarization scale uncertainty not included");

    
  TLatex bottomText;
  bottomText.SetTextSize( 0.1 );
  bottomText.SetTextAlign( 13 );
  bottomText.DrawLatex( 2.5, -0.035, "PHENIX" );    
      
  if (particle == "pi0")
  {
    TPad *subPad1 = new TPad( "subPad", "", 0.15, 0.55, 0.55, 0.8 ); 
    subPad1->SetLeftMargin( 0.17 );
    subPad1->SetBottomMargin( 0.15 );
    subPad1->Draw(); 
    subPad1->cd(); 

    TH1 *smallFrame = new TH2F( "small", " ", 1, 2, 6, 1, -0.004, 0.004 );
  
    smallFrame->SetStats(0);
    float labelSizeSmall = 0.13;
    TAxis *xAxisSmall = smallFrame->GetXaxis();
    xAxisSmall->SetLabelSize( labelSizeSmall );
    xAxisSmall->SetNdivisions( 703 );
    //  xAxisSmall->SetTickLength( 0.1 );
    xAxisSmall->SetTickSize( 0.1 );
    TAxis *yAxisSmall = smallFrame->GetYaxis();
    yAxisSmall->SetLabelSize( labelSizeSmall );
    yAxisSmall->SetNdivisions( 801 );
    smallFrame->Draw( " " );

    boxes_pAu->Draw( "5" );
    lines_pAu->Draw( "P" );         
    zeroLine->Draw( "same" );
  }  
  
  c1.cd(2);  
  gPad->SetRightMargin(0.01);
  frame->Draw("");
  boxes_pAl->Draw( "5" );  
  lines_pAl->Draw( "P" );  
  zeroLine->Draw( "same" );
  legend_pAl->Draw();
  
  text.SetTextSize( 0.065 );
  text.SetTextAlign( 13 );
  text.DrawLatex( 2, 0.115, label_pAl );    
    
  bottomText.SetTextSize( 0.1 );
  bottomText.SetTextAlign( 13 );
  bottomText.DrawLatex( 2.5, -0.035, "PHENIX" );    
  
  text.SetTextSize(0.045);
  text.DrawLatex(2, 0.095, "3% Polarization scale uncertainty not included");  
           
  if (particle == "pi0")
  {
    TPad *subPad2 = new TPad( "subPad2", "", 0.15, 0.55, 0.55, 0.8 ); 
    subPad2->SetLeftMargin( 0.17 );
    subPad2->SetBottomMargin( 0.15 );
    subPad2->Draw(); 
    subPad2->cd();   
    smallFrame->Draw();
    boxes_pAl->Draw("5");
    lines_pAl->Draw("P");
    zeroLine->Draw( "same" );    
  }

      

  
  c1.cd(3);
  gPad->SetRightMargin(0.01);  
  frame->Draw("");
  PPG234Boxes->Draw( "5" );  
  PPG234Lines->Draw( "P" );  
  zeroLine->Draw( "same" );   
  legend_pp->Draw();
  
  text.SetTextSize( 0.065 );
  text.SetTextAlign( 13 );
  text.DrawLatex( 2, 0.115, label_pp );

    
  bottomText.SetTextSize( 0.1 );
  bottomText.SetTextAlign( 13 );
  bottomText.DrawLatex( 2.5, -0.035, "PHENIX" );   
  
  text.SetTextSize(0.045);
  text.DrawLatex(2, 0.095, "3.4% Polarization scale uncertainty not included");   
        
  if (particle == "pi0")
  {
    TPad *subPad3 = new TPad( "subPad3", "", 0.15, 0.55, 0.55, 0.8 ); 
    subPad3->SetLeftMargin( 0.17 );
    subPad3->SetBottomMargin( 0.15 );
    subPad3->Draw(); 
    subPad3->cd();   
    smallFrame->Draw();
    PPG234Boxes->Draw("5");
    PPG234Lines->Draw("P");
    zeroLine->Draw( "same" );    
  }  
    
  TString imgname = particle;
  imgname += "_panels_finalAN.png";
  c1.SaveAs(imgname);

}
