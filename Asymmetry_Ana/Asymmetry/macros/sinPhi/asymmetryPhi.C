
//This code is currently structured to calculated the asymmetry for only one 
//fill at a time
#include <iostream>
#include <cmath>
using namespace std;

#include "TGraphAsymmErrors.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

#include "../../../Constants.h"
#include "../../../findBin.h"
#include "ConstantsSinPhi.h"


// Use this for the open heavy flavor electron measurement //
void asymmetryPhi( const char* particle = "ohfe" )
{
   if( particle != "ohfe" && particle != "e" && particle != "dp" && particle != "pi0" && particle != "eta" )
    cout << "Error! particle can only be dp, pi0, or eta!  You are calculating nonsense " << endl;

  gSystem->Load( "libppAsymmetry.so" );
  cout << "Num cut off set to " << NUM_CUT_OFF << " and there are " 
       << NUM_FILL_BINS << " groups of " << NUM_FILLS_IN_GROUP << " fills " 
       << endl;

  TString inputDataFileName = "../../../curated_";
  inputDataFileName += particle;
  inputDataFileName += ".root";

  TString inputTreeNameInFile = "e_svx_tree";

  TString outputFileName = particle;
  outputFileName += "_phi.root";
  cout << "~~~~~~~~~~~Gonna output everything into " << outputFileName 
       << "~~~~~~~~~~~" << endl;

  TFile *fillFile = TFile::Open( "../../../fill.root" );
  TTree* fillTree = (TTree*)fillFile->Get( "fill_tree" );
  float polB, polErrB, polY, polErrY;
  Long64_t countsUpB, countsDownB, countsUpY, countsDownY;
  fillTree->SetBranchAddress( "polarizationB",    &polB );
  fillTree->SetBranchAddress( "polarizationErrB", &polErrB );
  fillTree->SetBranchAddress( "polarizationY",    &polY );
  fillTree->SetBranchAddress( "polarizationErrY", &polErrY );
  fillTree->SetBranchAddress( "countsUpB",        &countsUpB );
  fillTree->SetBranchAddress( "countsDownB",      &countsDownB );
  fillTree->SetBranchAddress( "countsUpY",        &countsUpY );
  fillTree->SetBranchAddress( "countsDownY",      &countsDownY );

  int numFills = fillTree->GetEntries();
  Polarization polarizationY, polarizationB;
  RelativeLuminosity relLumiY, relLumiB;
  int fillIndexInit = 0;
  int fillBinInit = -1;
  for( int i = 0; i < numFills; i++ )
    {
      fillTree->GetEntry(i);
      if( i%NUM_FILLS_IN_GROUP == 0 )
	{
	  fillBinInit++;
	  fillIndexInit = 0;
	}
      double d_upY   = (double)countsUpY;
      double d_downY = (double)countsDownY;
      double d_upB   = (double)countsUpB;
      double d_downB = (double)countsDownB;
      polarizationY.updatePolarization( fillBinInit, fillIndexInit, 
					polY, polErrY, d_upY + d_downY );
      polarizationB.updatePolarization( fillBinInit, fillIndexInit, 
					polB, polErrB, d_upB + d_downB );
 
      relLumiY.incrementCounts( fillBinInit, countsUpY, countsDownY );
      relLumiB.incrementCounts( fillBinInit, countsUpB, countsDownB );
 
      fillIndexInit++;
    }
  TFile *outFile = new TFile( outputFileName, "RECREATE");
  outFile->cd();
  TGraphErrors *polGraphY = polarizationY.graph( YELLOW );
  polGraphY->Write( "polGraphY" );
  polGraphY->Delete();
  TGraphErrors *polGraphB = polarizationB.graph( BLUE );
  polGraphB->Write( "polGraphB" );
  polGraphB->Delete();
  TGraph *relLumiGraphY = relLumiY.graph( YELLOW );
  relLumiGraphY->Write( "relLumiGraphY" );
  relLumiGraphY->Delete();
  TGraph *relLumiGraphB = relLumiB.graph( BLUE );
  relLumiGraphB->Write( "relLumiGraphB" );
  relLumiGraphB->Delete();

  Asymmetry asymmetry[ NUM_PHI_BINS ];
  for( int f = 0; f < NUM_FILL_BINS; f++ )
    {
      fillTree->GetEntry(f);
      float avePolY, avePolErrY, avePolB, avePolErrB;
      polarizationY.averagePolarization( f, avePolY, avePolErrY );
      polarizationB.averagePolarization( f, avePolB, avePolErrB );
      float relativeLuminosityY = relLumiY.calculateRelativeLuminosity( f );
      float relativeLuminosityB = relLumiB.calculateRelativeLuminosity( f );
      for( int phBin = 0; phBin < NUM_PHI_BINS; phBin++ ) 
	{
	  asymmetry[ phBin ].setPolarization( YELLOW, f, avePolY, avePolErrY );
	  asymmetry[ phBin ].setPolarization( BLUE,   f, avePolB, avePolErrB );
	  asymmetry[ phBin ].setRelLuminosity( YELLOW, f, relativeLuminosityY );
	  asymmetry[ phBin ].setRelLuminosity( BLUE,   f, relativeLuminosityB );
	}
    }

  TFile *dataFile = TFile::Open( inputDataFileName );
  TTree* dataTree = (TTree*)dataFile->Get( inputTreeNameInFile );
  cout << "Loading " << inputTreeNameInFile << " from " << inputDataFileName 
       << endl;
  int fillNumber, arm, spinPattern;
  float px, py, px1, px2, py1, py2, pt, phi;
  dataTree->SetBranchAddress( "fillNumber",  &fillNumber );
  dataTree->SetBranchAddress( "arm",         &arm );
  dataTree->SetBranchAddress( "spinPattern", &spinPattern );
  if( particle == "ohfe" || particle == "e" )
    {
      dataTree->SetBranchAddress( "pt",           &pt );
      dataTree->SetBranchAddress( "phi",          &phi );
    }
  else if( particle == "dp" )
    {
      dataTree->SetBranchAddress( "px",           &px );
      dataTree->SetBranchAddress( "py",          &py );
    }
  else
    {
      dataTree->SetBranchAddress( "px1",         &px1 );
      dataTree->SetBranchAddress( "px2",         &px2 );
      dataTree->SetBranchAddress( "py1",         &py1 );
      dataTree->SetBranchAddress( "py2",         &py2 );
    }
  int numEntries = dataTree->GetEntries();
  int lastFillNumber = 0;
  int fillBin = 0;
  int fillIndex = 0;
  cout << "There are " << numEntries << " entries in tree " << endl;
  for( int i = 0; i < numEntries; i++ )
    {
      dataTree->GetEntry(i);
      if( i%10000000 == 0 ) 
	cout << "Processed " << i << " peak pairs" << endl;

      if( fillNumber != lastFillNumber ) 
	{
	  fillBin = fillIndex / NUM_FILLS_IN_GROUP;
	  //cout << "Last fill " << lastFillNumber << " current fill " 
	  //     << fillNumber << " fill index " << fillIndex
	  //     << " fill bin " << fillBin << endl;

	  fillIndex++;
	}

      lastFillNumber = fillNumber;

      if( particle != "dp" )
	{
	  px = px1 + px2;
	  py = py1 + py2;
	}
      //float pt = sqrt( px*px + py*py );
      int ptBin = findBin( NUM_PT_BINS, PT_BINS, pt );

      //float phi = atan( py / px );
      /*
      if( arm == 0 )  
	phi = PI/2 - phi;
      else if( arm == 1 )
	phi = PI/2 + phi;
      */
      // As before, use the mapping from the findPhiBinCenter // 
      if( arm == 0 )  
	phi = -(PI/2 - phi);
      else if( arm == 1 )
	phi = PI/2 - phi;
      int phiBin = findBin( NUM_PHI_BINS, PHI_BINS, phi );

      if( ptBin >= 0 && phiBin >= 0 )//only acceptable bin values allowed!
	asymmetry[ phiBin ].incrementCounts( fillBin, ptBin, arm, spinPattern );

    }//end looping through candidates

  float asArr[ NUM_BEAMS ][ NUM_OPTIONS_PHI ][ NUM_PHI_BINS ][ NUM_PT_BINS ];
  float asErrArr[ NUM_BEAMS ][ NUM_OPTIONS_PHI ][ NUM_PHI_BINS ][ NUM_PT_BINS ];
  for( int phiBin = 0; phiBin < NUM_PHI_BINS; phiBin++ )
    for( int b = 0; b < NUM_BEAMS; b++ )
      {
	for( int f = 0; f < NUM_FILL_BINS; f++ )
	  asymmetry[ phiBin ].calculateAsymmetry( b, f, LEFT_LUMINOSITY );
	asymmetry[ phiBin ].averageOverFills( b, 
					      asArr[b][ LEFT ][ phiBin ],
					      asErrArr[b][ LEFT ][ phiBin ] );

	for( int f = 0; f < NUM_FILL_BINS; f++ )
	  asymmetry[ phiBin ].calculateAsymmetry( b, f, RIGHT_LUMINOSITY );
	asymmetry[ phiBin ].averageOverFills( b, 
					      asArr[b][ RIGHT ][ phiBin ],
					      asErrArr[b][ RIGHT ][ phiBin ] );
      }

  outFile->cd();
  for( int b = 0; b < NUM_BEAMS; b++ )
    for( int option = 0; option < NUM_OPTIONS_PHI; option++ )
      for( int ptBin = 0; ptBin < NUM_PT_BINS; ptBin++ )
	{

/*
	  TString title = BEAM_NAMES[b]; 
	  title += OPTION_NAMES_PHI[ option ];
	  title += " Asymmetry from p_{T} ";
	  title += PT_BINS[ ptBin ];
	  title += " to ";
	  title += PT_BINS[ ptBin + 1 ];
	  title += " GeV; #phi; ";

	  TString name = BEAM_NAMES[b];
	  name += OPTION_NAMES_PHI[ option ];
	  name += PT_BINS[ ptBin ];
	  name += "to";
	  name += PT_BINS[ ptBin + 1 ];
*/
	  ostringstream title;
	  title << BEAM_NAMES[ b ] << OPTION_NAMES[ option ] << PT_BINS[ ptBin ]
	       << "to" << PT_BINS[ ptBin + 1 ] << " GeV; #phi; ";
	  //cout << title.str().c_str() << endl;

	  ostringstream name;
	  name << BEAM_NAMES[ b ] << OPTION_NAMES[ option ] << PT_BINS[ ptBin ]
	       << "to" << PT_BINS[ ptBin + 1 ];
	  //cout << name.str().c_str() << endl;

	  int arm = 0;
	  if( ( b == YELLOW && option == 0 ) || ( b == BLUE && option == 1 ) )
	    arm = 1;

	  float phiLow[ NUM_PHI_BINS ], phiHigh[ NUM_PHI_BINS ];
	  float phiArray[ NUM_PHI_BINS ];
	  float asymmetryArray[ NUM_PHI_BINS ], asymmeErrArray[ NUM_PHI_BINS ];
	  for( int phiBin = 0; phiBin < NUM_PHI_BINS; phiBin++ )
	    {
	      asymmetryArray[ phiBin ] = asArr[b][ option ][ phiBin ][ ptBin ];
	      asymmeErrArray[ phiBin ] = asErrArr[b][ option ][ phiBin ][ ptBin ];

	      phiArray[ phiBin ] = PHI_BIN_CENTERS[ arm ][ ptBin ][ phiBin ];
	      phiLow[ phiBin ] = phiArray[ phiBin ] - PHI_BINS[ phiBin ];
	      phiHigh[ phiBin ] = PHI_BINS[ phiBin + 1 ] - phiArray[ phiBin ];
	    }

	  TGraphAsymmErrors *graph 
	    = new TGraphAsymmErrors( NUM_PHI_BINS, phiArray, asymmetryArray,
				     phiLow, phiHigh, 
				     asymmeErrArray, asymmeErrArray );
	  graph->SetTitle( title.str().c_str() );
	  graph->Write( name.str().c_str() );
	  graph->Delete();
	}
 
  outFile->Close();
  outFile->Delete();

  for( int phiBin = 0; phiBin < NUM_PHI_BINS; phiBin++ )
    {
      cout << "For phi bin " << phiBin << ": " << endl;
      asymmetry[ phiBin ].printCounts();
      cout << endl;
    }

}
