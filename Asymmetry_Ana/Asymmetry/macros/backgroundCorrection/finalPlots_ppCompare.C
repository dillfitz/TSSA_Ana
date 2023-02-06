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


void finalPlots_ppCompare(const char* particle = "pi0")
{

  if (particle == "pi0")
  {
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
    TString label = "p^{#uparrow} + p #rightarrow #pi^{0} + X, #sqrt{s} = 200 GeV, |#eta|<0.35";    
  }
  if (particle == "eta")
  {
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
    TString label = "p^{#uparrow} + p #rightarrow #eta + X, #sqrt{s} = 200 GeV, |#eta|<0.35";          
     
  }
  double ptError[ NUM_PPG234_PT_BINS ];
  for( int b = 0; b < NUM_PPG234_PT_BINS; b++ )
  {
    ptError[b]  = 0.15;
  }  
  
  TCanvas *can = new TCanvas( "can", "", 200, 200, 800, 500 );
  gPad->SetBottomMargin( 0.15 );
  gPad->SetLeftMargin( 0.15 );  

  TH1 *frame = new TH2F( "frame", " ", 1, 1, 15, 1, -0.07, 0.1 );
 
 
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
  frame->Draw( " " );  
    
  TGraphErrors *PPG234Lines = new TGraphErrors( NUM_PPG234_PT_BINS, PPG234_PT, 
						PPG234, 0, PPG234_STAT_ERR );

  PPG234Lines->SetTitle("");
  PPG234Lines->GetXaxis()->SetLimits(1.,15.);
  PPG234Lines->GetYaxis()->SetRangeUser(-0.06,0.1);  
  PPG234Lines->SetLineColor( kBlack );//ue );
  PPG234Lines->SetMarkerColor( kBlack );//ue );
  PPG234Lines->SetMarkerStyle( kOpenCircle );
  PPG234Lines->Draw( "P" );
    
  TGraphErrors *PPG234Boxes = new TGraphErrors( NUM_PPG234_PT_BINS, PPG234_PT, 
					     PPG234, ptError, PPG234_SYST_ERR );
  PPG234Boxes->SetLineColor( 11 );//38 );//kBlue );
  PPG234Boxes->SetFillColor( 11 );//38 );//kWhite );
  PPG234Boxes->Draw( "5" );
 
  TF1 *zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlack );
  zeroLine->SetLineStyle( 2 );
  zeroLine->Draw( "same" ); 
  
  //if (particle == "pi0")
    TLegend *legend = new TLegend( 0.475, 0.6, 0.9, 0.8 );
  //else if (particle == "eta")
  //  TLegend *legend = new TLegend( 0.165, 0.6, 0.45, 0.8 );    
  //legend->AddEntry( PPG135Lines, "Phys. Rev. D 90, 012006", "p" );
  legend->AddEntry( PPG234Lines,       "PRD 103 052009 (2021)",             "p" );
  legend->SetBorderSize( 0 );
  legend->Draw();  

  TLatex text;
  text.SetTextSize( 0.065 );
  text.SetTextAlign( 13 );
  //if (particle == "pi0")
    text.DrawLatex( 1.6, 0.097, label );
  //else if (particle == "eta")
  //  text.DrawLatex( 1.6, 0.115, label );  
    
  TLatex bottomText;
  bottomText.SetTextSize( 0.1 );
  bottomText.SetTextAlign( 13 );
  bottomText.DrawLatex( 2.5, -0.035, "PHENIX" );    
 

    
  //if (particle == "pi0")
  //{
    TPad *subPad = new TPad( "subPad", "", 0.16, 0.57, 0.52, 0.81 ); 
    subPad->SetLeftMargin( 0.17 );
    subPad->SetBottomMargin( 0.15 );
    subPad->Draw(); 
    subPad->cd(); 

    if (particle == "pi0")
      TH1 *smallFrame = new TH2F( "small", " ", 1, 2, 6, 1, -0.004, 0.004 );
    else if (particle == "eta")
      TH1 *smallFrame = new TH2F( "small", " ", 1, 2, 6, 1, -0.007, 0.007 );
    smallFrame->SetStats(0);
    float labelSizeSmall = 0.15;
    TAxis *xAxisSmall = smallFrame->GetXaxis();
    xAxisSmall->SetLabelSize( labelSizeSmall );
    xAxisSmall->SetNdivisions( 703 );
    //  xAxisSmall->SetTickLength( 0.1 );
    xAxisSmall->SetTickSize( 0.1 );
    TAxis *yAxisSmall = smallFrame->GetYaxis();
    yAxisSmall->SetLabelSize( labelSizeSmall );
    yAxisSmall->SetNdivisions( 503 );
    smallFrame->Draw( " " );

    PPG234Boxes->Draw( "5" );
    PPG234Lines->Draw( "P" );
    zeroLine->Draw( "same" );
  //}  

  TString imgname = particle;
  imgname += "_pp_finalAN.png";
  can->SaveAs(imgname);  
    
}
