#include <iostream>
#include <sstream>
using namespace std;

#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"

#include "../../Constants.h"

const bool SAVE_PICTURES = true;
const bool peak = false;//true;
//if false than looking at background

void plotBunchShuffling_local()
{
  gStyle->SetOptFit();
  TFile *file = TFile::Open( "shuffled_ohfe.root" );
  const char *particle = "ohfe";

  TCanvas *canvases[ NUM_VALUE_BINS ];
  TH1F *histos[ NUM_VALUE_BINS ];
  TF1 *fits[ NUM_VALUE_BINS ];
  for( int i = 0; i < NUM_VALUE_BINS; i++ )
    {
      ostringstream name;
      name << "shuffled" << i;
      histos[i] = (TH1F*)file->Get( name.str().c_str() );

      ostringstream title;
      title << "Bunch Shuffling for ";
      /* 
     if( peak )
	title << "Peak ";
      else
      title << "Background ";*/
      title << VALUE_BINS[i] << " < p_{T} < " << VALUE_BINS[i + 1] 
	    << " GeV; A_{N}/#sigma_{A}; ";
      name << i; 
      canvases[i] = new TCanvas( name.str().c_str(), title.str().c_str() );

      if( !SAVE_PICTURES )
	histos[i]->SetTitle( title.str().c_str() );
      else
	histos[i]->SetTitle( " ;A_{N}/#sigma_{A}; " );

      fits[i] = new TF1( name.str().c_str(), "gaus", -5, 5 );
      histos[i]->Fit( fits[i], "Q" );

      if( SAVE_PICTURES )
	{
	  ostringstream name;
	  name << "images/" << particle;
	  name << "/shuffles" << VALUE_BINS[i] << "to" << VALUE_BINS[i + 1] 
	       << ".png";
	  canvases[i]->SaveAs( name.str().c_str() );
	}
    }
}
