#include <iostream>
using namespace std;

#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TAxis.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TLegend.h"

#include "ttest.h"

const int SIZE = 2;
const float MIN = -0.06;
const float MAX =  0.09;
const float TTEST_MIN = -3;
const float TTEST_MAX =  3;

const bool LUMI = true;
//if lumi false that doing SQRT

const bool SAVE_IMAGES = true;//false;
void plotPi0IsoWithInclYellowBlue()
{
  if( LUMI )
    cout << "Doing lumi graphs" << endl;
  else
    cout << "Doing sqrt graphs" << endl;

  TFile *isoFile = TFile::Open(  "/direct/phenix+u/workarea/nialewis/Run15ppPhotons/Asymmetry/macros/dataFiles/graphs_pi0_iso.root" );
  TGraphAsymmErrors *isoY, *isoB;
  if( LUMI )
    {
      isoY = (TGraphAsymmErrors*)isoFile->Get( "lumiY" );
      isoB = (TGraphAsymmErrors*)isoFile->Get( "lumiB" );
    }
  else
    {
      isoY = (TGraphAsymmErrors*)isoFile->Get( "sqrtY" );
      isoB = (TGraphAsymmErrors*)isoFile->Get( "sqrtB" );
    }
  isoY->SetMarkerStyle( kFullSquare );
  isoY->SetMarkerColor( kOrange );
  isoY->SetMarkerSize( 0.7*SIZE );
  isoY->GetYaxis()->SetRangeUser( MIN, MAX );
  isoB->SetMarkerStyle( kFullDiamond );
  isoB->SetMarkerColor( kBlue );
  isoB->SetMarkerSize( SIZE );
  isoB->GetYaxis()->SetRangeUser( MIN, MAX );
  if( LUMI )
    {
      isoY->SetTitle( "Relative Luminosity Yellow; p_{T}^{#gamma} [GeV];" );
      isoB->SetTitle( "Relative Luminosity Blue; p_{T}^{#gamma} [GeV];" );
    }
  else
    {
      isoY->SetTitle( "Square Root Yellow; p_{T}^{#gamma} [GeV];" );
      isoB->SetTitle( "Square Root Blue; p_{T}^{#gamma} [GeV];" );
    }
  TFile *inclFile = TFile::Open(  "/direct/phenix+u/workarea/nialewis/Run15ppPhotons/Asymmetry/macros/dataFiles/graphs_pi0_iso_isolation_cut_commented_out.root" );
  TGraphAsymmErrors *inclY, *inclB;
  if( LUMI )
    {
      inclY = (TGraphAsymmErrors*)inclFile->Get( "lumiY" );
      inclB = (TGraphAsymmErrors*)inclFile->Get( "lumiB" );
    }
  else
    {
      inclY = (TGraphAsymmErrors*)inclFile->Get( "sqrtY" );
      inclB = (TGraphAsymmErrors*)inclFile->Get( "sqrtB" );
    }
  inclY->SetMarkerStyle( kOpenSquare );
  inclY->SetMarkerColor( kOrange );
  inclY->SetMarkerSize( 0.7*SIZE );
  inclB->SetMarkerStyle( kOpenDiamond );
  inclB->SetMarkerColor( kBlue );
  inclB->SetMarkerSize( SIZE );

  TF1* zeroLine = new TF1( "zeroLine", "0", 0, 20 );
  zeroLine->SetLineColor( kBlack );
  zeroLine->SetLineStyle( 2 );

  TCanvas *c1 = new TCanvas( "c1", "Iso with Incl Yellow" );
  isoY->Draw( "AP" );
  inclY->Draw( "P" );
  zeroLine->Draw( "same" );
  TLegend *yLegend = new TLegend( 0.6, 0.9, 0.9, 0.7 );
  yLegend->AddEntry( isoY, "Isolated #pi^{0}s", "lep" );
  yLegend->AddEntry( inclY, "Inclusive #pi^{0}s", "lep" );
  yLegend->Draw();
  if( SAVE_IMAGES )
    {
      if( LUMI )
	c1->SaveAs( "images/isoWithIncl/lumiYellow.png" );
      else
	c1->SaveAs( "images/isoWithIncl/sqrtYellow.png" );
    }
  TCanvas *c10 = new TCanvas( "c10", "Yellow iso incl", 800, 0, 700, 500 );
  c10->SetTicky();
  TGraph *ttestY = ttest( isoY, inclY, 1 );
  ttestY->SetMarkerStyle( kFullCircle );
  ttestY->SetTitle( "Yellow: iso - incl; p_{T}^{#gamma} [GeV]; T" );
  ttestY->GetYaxis()->SetRangeUser( TTEST_MIN, TTEST_MAX );
  ttestY->Draw( "AP" );
  zeroLine->Draw( "same" );
  if( SAVE_IMAGES )
    {
      if( LUMI )
	c10->SaveAs( "images/isoWithIncl/lumiYellowTTest.png" );
      else
	c10->SaveAs( "images/isoWithIncl/sqrtYellowTTest.png" );
    }

  TCanvas *c2 = new TCanvas( "c2", "Iso with Incl Blue" );
  isoB->Draw( "AP" );
  inclB->Draw( "P" );
  zeroLine->Draw( "same" );
  TLegend *bLegend = new TLegend( 0.6, 0.9, 0.9, 0.7 );
  bLegend->AddEntry( isoB, "Isolated #pi^{0}s", "lep" );
  bLegend->AddEntry( inclB, "Inclusive #pi^{0}s", "lep" );
  bLegend->Draw();
  if( SAVE_IMAGES )
    {
      if( LUMI )
	c2->SaveAs( "images/isoWithIncl/lumiBlue.png" );
      else
	c2->SaveAs( "images/isoWithIncl/lumiBlue.png" );
    }

  TCanvas *c20 = new TCanvas( "c20", "Blue iso incl", 800, 0, 700, 500 );
  c20->SetTicky();
  TGraph *ttestB = ttest( isoB, inclB, 1 );
  ttestB->SetMarkerStyle( kFullCircle );
  ttestB->SetTitle( "Blue: iso - incl; p_{T}^{#gamma} [GeV]; T" );
  ttestB->GetYaxis()->SetRangeUser( TTEST_MIN, TTEST_MAX );
  ttestB->Draw( "AP" );
  zeroLine->Draw( "same" );
  if( SAVE_IMAGES )
    {
      if( LUMI )
	c20->SaveAs( "images/isoWithIncl/lumiBlueTTest.png" );
      else
	c20->SaveAs( "images/isoWithIncl/lumiBlueTTest.png" );
    }
}
