#include <iostream>
using namespace std;

#include "TFile.h"
#include "TH1F.h"
#include "TAxis.h"

const int NUM_PARA = 7;
const double GUESS[ NUM_PARA ] = {  60000, 0.135, 0.02, 1000, 100, -100, -100 };
///*
//0.113-163 for signal, and 0.05-0.1 and 0.2-0.3 for the bkg
const double FIT_RANGE_MIN = 0.075;
const double FIT_RANGE_MAX = 0.200;
const float MIN_MASS = 0.04;
const float MAX_MASS = 0.440;
const int NUM_BINS = 80;
const float MIN_SIGNAL_MASS = 0.105;
const float MAX_SIGNAL_MASS = 0.165;
const float MIN_BG_MASS_1 = 0.05;
const float MAX_BG_MASS_1 = 0.1;
const float MIN_BG_MASS_2 = 0.2;
const float MAX_BG_MASS_2 = 0.3;
/*
const double FIT_RANGE_MIN = 0.4;
const double FIT_RANGE_MAX = 0.8;
const float MIN_MASS = 0.2;
const float MAX_MASS = 0.9;
const int NUM_BINS = 70;
const float MIN_SIGNAL_MASS = 0.480;
const float MAX_SIGNAL_MASS = 0.620;
const float MIN_BG_MASS_1 = 0.300;
const float MAX_BG_MASS_1 = 0.400;
const float MIN_BG_MASS_2 = 0.700;
const float MAX_BG_MASS_2 = 0.800;
*/
void plotPrettyInvMass()
{
  TFile *file = TFile::Open( "curated_pi0_file.root" );
  TH1F *histo = (TH1F*)file->Get( "invmassWest4to5" );

  TH1F *sig = new TH1F( "sig", "", NUM_BINS, MIN_MASS, MAX_MASS );
  TH1F *lilBG = new TH1F( "lilBG", "", NUM_BINS, MIN_MASS, MAX_MASS );
  TH1F *bigBG = new TH1F( "bigBG", "", NUM_BINS, MIN_MASS, MAX_MASS );
  for( int i = 1; i <= NUM_BINS; i++ )
    {
      double counts = histo->GetBinContent( i );
      double invmass = (i - 1 )* 0.005 + MIN_MASS;
   
      if( invmass > MIN_SIGNAL_MASS && invmass < MAX_SIGNAL_MASS )
	sig->Fill( invmass, counts );
      else if( invmass > MIN_BG_MASS_1 && invmass < MAX_BG_MASS_1 ) 
	lilBG->Fill( invmass, counts );
      else if( invmass > MIN_BG_MASS_2 && invmass < MAX_BG_MASS_2 )
	bigBG->Fill( invmass, counts );
    }

  sig->SetStats( false );
  sig->SetTitle( "; M_{#gamma#gamma} [GeV/c^{2}]; counts" );
  float titleOffset = 0.7;
  TAxis *xAxis = sig->GetXaxis();
  xAxis->CenterTitle( true );
  xAxis->SetTitleSize( 0.06 );
  xAxis->SetTitleOffset( titleOffset );
  TAxis *yAxis = sig->GetYaxis();
  yAxis->CenterTitle( true );
  yAxis->SetTitleSize( 0.06 );
  yAxis->SetTitleOffset( titleOffset );

  sig->SetFillColor( kBlue );
  sig->Draw( "B"  );

  lilBG->SetFillColor( kRed );
  lilBG->Draw( "BSAME" );
  bigBG->SetFillColor( kRed );
  bigBG->Draw( "BSAME" );

  histo->Draw( "SAME" );
  TF1 *fit = new TF1( "fit", "gaus+pol3(3)",  FIT_RANGE_MIN, FIT_RANGE_MAX );
  for( int para = 0; para < NUM_PARA; para++ )
    fit->SetParameter( para, GUESS[ para ] );
  histo->Fit( "fit"  );


}
