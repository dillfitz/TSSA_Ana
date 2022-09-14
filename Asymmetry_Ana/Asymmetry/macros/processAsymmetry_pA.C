#include <iostream>
using namespace std;

#include "TFile.h"
#include "TGraphErrors.h"
#include "TString.h" 
#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TGraphAsymmErrors.h"

#include "../../Constants.h"

#include "./plotMacros/removeBlanks.h"
#include "./plotMacros/drawGraph.h"
#include "./plotMacros/weightedAverage.h"

const int width = 900;
const int heightSqrt = 450;
const int heightLumi = 450;

//const int SQRT = 2;
void processAsymmetry_pA( const char* particle = "pi0", const char* collisionSystem = "pAu", const bool background = false, const bool dpBackground = false, const bool isolated = false )

//both must be set to false for dp
{
 // if( particle != "ohfe" && && particle != "dp" && particle != "pi0" && particle != "eta" )
 //   cout << "Error! particle can only be dp, pi0, or eta!  You are calculating nonsense " << endl;

  gStyle->SetOptFit();

  char *directory = "./dataFiles/";
  TString inputFileName = directory;
  inputFileName += particle;
  inputFileName += "_";
  if (background)
    inputFileName += "background_";
  inputFileName += collisionSystem;
  inputFileName += "_AN_fills";
  inputFileName += ".root";


  TString outputFileName = directory;
  outputFileName += particle;
  outputFileName += "_";
  if (background)
    outputFileName += "background_";
  outputFileName += collisionSystem;
  outputFileName += "_AN";
  outputFileName += ".root";

  cout << "Opening file " << inputFileName << " to fit asymmetries " << endl; 
  cout << "Putting graphs in " << outputFileName << endl;

  TFile *file = TFile::Open( inputFileName );
  TGraphErrors *graphs[ NUM_VALUE_BINS ][ NUM_BEAMS ][ NUM_OPTIONS ];
  for( int ptBin = 0; ptBin < NUM_VALUE_BINS; ptBin++ )
    for( int i = 0; i < NUM_BEAMS; i++ )// beam: yellow, blue
      for( int j = 0; j < NUM_OPTIONS; j++ )//options: left, right, sqrt
	    {
	      if (i==YELLOW) {continue;}
	      ostringstream names;
	      names << BEAM_NAMES[ i ] << OPTION_NAMES[j] << VALUE_BINS[ ptBin ]
	            << "to" << VALUE_BINS[ ptBin + 1 ];
	      cout << names.str().c_str() << endl;
	
	      graphs[ ptBin ][i][j] = (TGraphErrors*)file->Get( names.str().c_str() );

	      //get rid of asymmetries that did not have enough counts
	      graphs[ ptBin ][i][j]->GetN();
	      graphs[ ptBin ][i][j] = removeBlanks( graphs[ ptBin ][i][j] );
 
	    }// end looping through graphs

  float fitValues[ NUM_BEAMS ][ NUM_OPTIONS ][ NUM_VALUE_BINS ];
  float fitValuesErr[ NUM_BEAMS ][ NUM_OPTIONS ][ NUM_VALUE_BINS ];
  TString canvasTitle[ NUM_VALUE_BINS ];
  for( int i = 0; i < NUM_VALUE_BINS; i++ )
  {
    canvasTitle[i] = " Asymmetry from pT ";
    canvasTitle[i] += VALUE_BINS[i];
    canvasTitle[i] += " to ";
    canvasTitle[i] += VALUE_BINS[i + 1];
    canvasTitle[i] += " GeV";
  }

  TH1D *lumiChi2Histo = new TH1D( "lumiChi2Histo", 
			      "Reduced #chi^{2} Distribution for Lumi Formula ",
			      20, 0.0, 10.0 );
  TCanvas *lumiCanvases[ NUM_VALUE_BINS ];
  for( int ptBin = 0; ptBin < NUM_VALUE_BINS; ptBin++ )
  {
    TString name = "c";
    name += ptBin;
    lumiCanvases[ ptBin ] = new TCanvas( name, canvasTitle[ ptBin ], width, heightLumi );
    lumiCanvases[ ptBin ]->Divide( 2, 1 );

    int index = 1;
    for( int beam = 0; beam < NUM_BEAMS; beam++ )
	    for( int option = 0; option < SQRT; option++ )
	    //lumi left then lumi right
	    {
	      if (beam==YELLOW) {continue;}
	      lumiCanvases[ ptBin ]->cd( index );
	      TGraphErrors *graph = graphs[ ptBin ][ beam ][ option ];
	      double fitValueInput = -999;
	      double fitValueErrInput = -99;
	      drawGraph( graph, ptBin, beam, option, fitValueInput, fitValueErrInput, lumiChi2Histo );

	      fitValues[ beam ][ option ][ ptBin ]    = fitValueInput;
	      fitValuesErr[ beam ][ option ][ ptBin ] = fitValueErrInput;

	      index++;
	    }//end looping over direction and beam

  }//end looping over pt bin

  TCanvas *c399 = new TCanvas( "c399", "Lumi Chi 2 dist", 800, 0, 700, 500 );
  lumiChi2Histo->Draw();

  TH1D *sqrtChi2Histo = new TH1D( "sqrtChi2Histo", "Reduced #chi^{2} Distribution for Sqrt Formula ", 20, 0.0, 2 );
  TCanvas *sqrtCanvases[ NUM_VALUE_BINS ];
  for( int ptBin = 0; ptBin < NUM_VALUE_BINS; ptBin++ )
  {
    TString name = "c";
    name += 10 + ptBin;
    sqrtCanvases[ ptBin ] = new TCanvas( name, canvasTitle[ ptBin ], width/2, heightSqrt );
    //sqrtCanvases[ ptBin ]->Divide( 2, 1 );
    for( int beam = 0; beam < NUM_BEAMS; beam++ )
	  {
	    if (beam==YELLOW) {continue;}
	    //sqrtCanvases[ ptBin ]->cd( beam + 1 );
	    double fitValueInput = -999;
	    double fitValueErrInput = -99;
	    TGraphErrors* graph = graphs[ ptBin ][ beam ][ SQRT ];
	    drawGraph( graph, ptBin, beam, SQRT, fitValueInput, fitValueErrInput, sqrtChi2Histo );

	    fitValues[ beam ][ SQRT ][ ptBin ]    = fitValueInput;
	    fitValuesErr[ beam ][ SQRT ][ ptBin ] = fitValueErrInput;

	  }//end looping through beams
 
  }// end looping through pt bins

  TCanvas *c398 = new TCanvas( "c398", "Sqrt Chi 2 dist" );
  sqrtChi2Histo->Draw();

  TFile *outFile = new TFile( outputFileName, "RECREATE" );
  outFile->cd();//save the graphs we already have
  float ptLow[ NUM_VALUE_BINS ], ptHigh[ NUM_VALUE_BINS ];
  for( int i = 0; i < NUM_VALUE_BINS; i++ )
  {
    ptLow[i]  = fabs( VALUE_BINS[i] - BIN_CENTERS[i] );
    ptHigh[i] = fabs( VALUE_BINS[i + 1] - BIN_CENTERS[i] );
  }

  for( int o = 0; o < NUM_OPTIONS; o++ )//left right sqrt
    for( int b= 0; b < NUM_BEAMS; b++ )
    {
      if (b==YELLOW) {continue;}
	    TString graphName;
	    if( o < NUM_OPTIONS - 1 )  
	      graphName =  "lumi";
	    else
	      graphName = "sqrt";

	    if( b == YELLOW )    graphName += "Y";
	    else if( b == BLUE ) graphName += "B";
  
	    if( o == LEFT )       graphName += "L";
	    else if( o == RIGHT ) graphName += "R";
	    TGraphAsymmErrors *outGraph =  new TGraphAsymmErrors( NUM_VALUE_BINS, BIN_CENTERS, fitValues[b][o], ptLow, ptHigh, fitValuesErr[b][o], fitValuesErr[b][o]);
	    outGraph->SetName( graphName );
	    outGraph->SetTitle( graphName );
	    outGraph->Write( graphName );
	    outGraph->Delete();
    }

  //find all the other graphs I need
  //double fitValues[ NUM_BEAMS ][ NUM_OPTIONS ][ NUM_VALUE_BINS ];
  //double fitValuesErr[ NUM_BEAMS ][ NUM_OPTIONS ][ NUM_VALUE_BINS ];
  float lumiY[ NUM_VALUE_BINS ], lumiYErr[ NUM_VALUE_BINS ];
  float lumiB[ NUM_VALUE_BINS ], lumiBErr[ NUM_VALUE_BINS ];
  float asymmetrySqrt[ NUM_VALUE_BINS ], asymmetrySqrtErr[ NUM_VALUE_BINS ];
  float asymmetryLumi[ NUM_VALUE_BINS ], asymmetryLumiErr[ NUM_VALUE_BINS ];
  for( int i = 0; i < NUM_VALUE_BINS; i++ )
  {
    //weightedAverage( fitValues[ YELLOW ][ LEFT  ][i], fitValuesErr[ YELLOW ][ LEFT  ][i], fitValues[ YELLOW ][ RIGHT ][i], fitValuesErr[ YELLOW ][ RIGHT ][i], lumiY[i], lumiYErr[i] );
    weightedAverage(fitValues[ BLUE ][ LEFT  ][i], fitValuesErr[ BLUE ][ LEFT  ][i], fitValues[ BLUE ][ RIGHT ][i], fitValuesErr[ BLUE ][ RIGHT ][i], lumiB[i], lumiBErr[i] );
    //weightedAverage( lumiY[i], lumiYErr[i], lumiB[i], lumiBErr[i], asymmetryLumi[i], asymmetryLumiErr[i] );
    //weightedAverage( fitValues[ YELLOW ][ SQRT ][i], fitValuesErr[ YELLOW ][ SQRT ][i], fitValues[ BLUE   ][ SQRT ][i], fitValuesErr[ BLUE   ][ SQRT ][i], asymmetrySqrt[i], asymmetrySqrtErr[i] );
  }

  TGraphAsymmErrors *graphLumiB = new TGraphAsymmErrors( NUM_VALUE_BINS, BIN_CENTERS, lumiB, ptLow, ptHigh, lumiBErr, lumiBErr );
  graphLumiB->SetName( "lumiB" );
  graphLumiB->SetTitle( "lumiB" );
  graphLumiB->Write( "lumiB" );
  graphLumiB->Delete();

  outFile->Close();
  outFile->Delete();
}

