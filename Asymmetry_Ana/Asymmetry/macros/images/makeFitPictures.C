//Does the same thins a fitAsymmetry.C, but just makes images instead

#include "TFile.h"
#include "TGraphErrors.h"
#include "TString.h" 
#include "TCanvas.h"
#include "TF1.h"

#include "../../Constants.h"
#include "../plotMacros/removeBlanks.h"

//const bool PEAK = false;//true;//false for background

// Use this for the open heavy flavor electron measurement //
void makeFitPictures( const char* particle = "ohfe" )
{
  gStyle->SetOptFit();

  TString inputFileName = "../dataFiles/";
  inputFileName += particle;
  inputFileName += "_AN_fills";
  inputFileName += ".root";
  TFile *file = TFile::Open( inputFileName );
  /*
  TFile *file;
  if( PEAK )
    file = TFile::Open( "../dataFiles/eta.root" );
  else
    file = TFile::Open( "../dataFiles/background.root" );
  */

  TCanvas *canvases[ NUM_VALUE_BINS ][ NUM_BEAMS ][ NUM_OPTIONS ];
  for( int ptBin = 0; ptBin < NUM_VALUE_BINS; ptBin++ )
    for( int beam = 0; beam < NUM_BEAMS; beam++ )
      for( int option = 0; option < NUM_OPTIONS; option++ )
	{
	  //TString name =  BEAM_NAMES[ beam ];
	  //name += OPTION_NAMES[ option ];
 	  //name += VALUE_BINS[ ptBin ];
	  //name += "to";
	  //name += VALUE_BINS[ ptBin + 1 ];

	  ostringstream name;
	  name << BEAM_NAMES[ beam ] << OPTION_NAMES[option] << VALUE_BINS[ ptBin ]
	       << "to" << VALUE_BINS[ ptBin + 1 ];
	  cout << name.str().c_str() << endl;
	
	
	  TGraphErrors *graph = (TGraphErrors*)file->Get( name.str().c_str() );
	  //going to get rid of blank asymetries
	  graph = removeBlanks( graph );
 
	  canvases[ ptBin ][ beam ][ option ] = new TCanvas( name.str().c_str(), name.str().c_str() );
	  TF1 *fit = new TF1( "fit", "[0]" );
	  graph->Fit( "fit", "Q" );
	  TString graphTitle = name.str().c_str();
	  graphTitle += "GeV; Fill Index; " ;
	  graph->SetTitle( ";FIll Group  Index;" );
	  graph->Draw("A*" );

	  TString pictureName = particle;
	  pictureName += "/";
	  /*
	  if( PEAK )
	    pictureName += "peakAsymmetryVSFill/";
	  else
	    pictureName += "backgroundAsymmetryVSFill/";
	  */
	  //pictureName += VALUE_BINS[ ptBin ];
	  //pictureName += "to";
	  //pictureName += VALUE_BINS[ ptBin + 1 ];
	  //pictureName +=  BEAM_NAMES[ beam ];
	  //pictureName += OPTION_NAMES[ option ];
	  pictureName += name.str().c_str();
	  pictureName += ".png";

	  canvases[ ptBin ][ beam ][ option ]->SaveAs( pictureName );

	}// end looping through graphs

}
