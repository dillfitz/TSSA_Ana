//Outputs a root file with 6 graphs ( yellow left, yellow right, yellow sqrt, 
//blue left, blue right, blue sqrt ) for each pt bin of the asymmetry as a 
//function of fill group.  
//Inputting this output file into fitAsymmetry.C willfind the actual asymmetry 
//values averaged over fill group and output them into a .h file, which can then
//be inputted into makeGraphs.C to make them into graphs that can be easily 
//plotted
#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;

#include "../../Constants.h"
#include "../../findBin.h"

#include "TGraphErrors.h"
#include "TTree.h"
#include "TFile.h"

//dpBackground = true allows the "pi0" and "eta" asymmetries to be evaluated as 
//a function of photon pt instead of hadron pt
//isolated  = true performs a pair isolation cut of "pi0" and "eta" photons:
//E_gamma & 10% > E_cone - E_pair
//

void asymmetry( const char* particle = "ohfe",
		const bool dpBackground = false, 
		const bool isolated = false )//both must be set to false for dp
{
   if( particle != "ohfe" && particle != "dp" && particle != "pi0" && particle != "eta" )
    cout << "Error! particle can only be ohfe, dp, pi0, or eta!  "
	 << "You are calculating nonsense " << endl;

  gSystem->Load( "libppAsymmetry.so" );
  cout << "Num cut off set to " << NUM_CUT_OFF << " and there are " 
       << NUM_FILL_BINS << " groups of " << NUM_FILLS_IN_GROUP << " fills,"
       << " with " << NUM_VALUE_BINS << " pt bins " << endl;

  // matc tis filename structure
  TString inputDataFileName = "../../";
  inputDataFileName += "curated_";
  inputDataFileName += particle;
  inputDataFileName += ".root";
  TString inputTreeNameInFile = "e_svx_tree";

  TString outputFileName = "./dataFiles/";
  outputFileName += particle;
  outputFileName += "_AN_fills.root";

  // TString outputFileName = "/direct/phenix+u/workarea/nialewis/Run15ppPhotons/Asymmetry/macros/dataFiles/";
  // outputFileName += particle;
  // outputFileName += ".root";
  //outputFileName += "_one_pt_bin.root";
  cout << "~~~~~~~~~~~Gonna output everything into " << outputFileName 
       << "~~~~~~~~~~~" << endl;

  TFile *fillFile = TFile::Open( "fill.root" );
  TTree* fillTree = (TTree*)fillFile->Get( "fill_tree" );
  int fillNumberInFillTree;
  float polB, polErrB, polY, polErrY;
  Long64_t countsUpB, countsDownB, countsUpY, countsDownY;
  fillTree->SetBranchAddress( "fillNumber",       &fillNumberInFillTree );
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
  /*
  cout << "Yellow beam polarization: " << endl;
  polarizationY.print();
  cout << endl;
  cout << "Blue beam polarization: " << endl;
  polarizationB.print();
  cout << endl;
  cout << endl;
  cout << "Yellow beam relative luminosity: " << endl;
  relLumiY.print();
  cout << endl;
  cout << "Blue beam relative luminosity: " << endl;
  relLumiB.print();
  */

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

  Asymmetry asymmetry;
  float fillBinArray[ NUM_FILL_BINS ]; //make array for xaxis of later graphs
  for( int f = 0; f < NUM_FILL_BINS; f++ )
    {
      fillBinArray[f] = f + 1;  
      fillTree->GetEntry(f);
 
      float avePolY, avePolErrY, avePolB, avePolErrB;
      polarizationY.averagePolarization( f, avePolY, avePolErrY );
      polarizationB.averagePolarization( f, avePolB, avePolErrB );
      asymmetry.setPolarization( YELLOW, f, avePolY, avePolErrY );
      asymmetry.setPolarization( BLUE,   f, avePolB, avePolErrB );

      float relativeLuminosityY = relLumiY.calculateRelativeLuminosity( f );
      asymmetry.setRelLuminosity( YELLOW, f, relativeLuminosityY );
      float relativeLuminosityB = relLumiB.calculateRelativeLuminosity( f );
      asymmetry.setRelLuminosity( BLUE,   f, relativeLuminosityB );
    }

  TFile *dataFile = TFile::Open( inputDataFileName );
  TTree* dataTree = (TTree*)dataFile->Get( inputTreeNameInFile );
  cout << "~~~~~~~~~~~Loading " << inputTreeNameInFile << " from " 
       << inputDataFileName << "~~~~~~~~~~" << endl;
  int fillNumber, arm, spinPattern, ptBin, pt1Bin;
  float energy1, energy2, coneEnergy1;
  dataTree->SetBranchAddress( "fillNumber",  &fillNumber );
  dataTree->SetBranchAddress( "arm",         &arm );
  dataTree->SetBranchAddress( "spinPattern", &spinPattern );
  dataTree->SetBranchAddress( "ptBin",  &ptBin );

  float px, py, px1, py1, px2, py2, pt, phi;
  if( particle == "ohfe" )
    {    
      dataTree->SetBranchAddress( "pt", &pt );
      dataTree->SetBranchAddress( "phi", &phi );
      cout << "px : " << px << " py : " << py << endl;
      //dataTree->SetBranchAddress( "px", &px );
      //dataTree->SetBranchAddress( "py", &py );
    }
  else
    {
      dataTree->SetBranchAddress( "px1", &px1 );
      dataTree->SetBranchAddress( "py1", &py1 );
      dataTree->SetBranchAddress( "px2", &px2 );
      dataTree->SetBranchAddress( "py2", &py2 );
    }


  AcceptanceCorrection accCorr;

  int numEntries = dataTree->GetEntries();
  int lastFillNumber = 0;
  int fillBin = 0;
  int fillIndex = 0;
  cout << "There are " << numEntries << " entries in tree " << endl;
  for( int i = 0; i < numEntries; i++ )
    {
      dataTree->GetEntry(i);
      if( i%50000 == 0 ) cout << "Processed " << i << " entries " << endl;

      if( fillNumber != lastFillNumber )
	{
	  fillBin = fillIndex / NUM_FILLS_IN_GROUP;
	  //cout << "Fill " << fillNumber << " fill index " << fillIndex
	  //     << " fill bin " << fillBin << endl;
	  fillIndex++;
	}
      lastFillNumber = fillNumber;

      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      /*used for direct photon systematic error
      float pt1 = sqrt( px1*px1 + py1*py1 );
      if( pt1 > 5 && pt1 < 18 && (energy1 * 0.1 > (coneEnergy1 - energy2)) )
	{
	  accCorr.increment( arm, 0, px1, py1 );
	  asymmetry.incrementCounts( fillBin, 0, arm, spinPattern );
	}
      */
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if( particle == "ohfe" && ptBin >= 0  )//direct photon regular old asymmetry
	{
           px = pt*cos(phi); 
           py = pt*sin(phi); 
	  accCorr.increment( arm, ptBin, px, py );
	  asymmetry.incrementCounts( fillBin, ptBin, arm, spinPattern );
	}
      /*
      else if( !isolated && !dpBackground && particle != "dp"
	       && ptBin >= 0 ) //regular old asymmetry pi0 or eta
	{
	  asymmetry.incrementCounts( fillBin, ptBin, arm, spinPattern );
	  accCorr.increment( arm, ptBin, px1 + px2, py1 + py2 );
	}
      else if( isolated && dpBackground && particle != "dp" 
	  && pt1Bin >= 0 && (energy1 * 0.1 > (coneEnergy1 - energy2)) )
	{
	  accCorr.increment( arm, pt1Bin, px1, py1 );
	  asymmetry.incrementCounts( fillBin, pt1Bin, arm, spinPattern );

	  //cout << "Entry " << i << " pt1bin " << pt1Bin << endl;
	}
      */
      /**/
    }//end looping through pairs
  cout << "The last fill number was " << fillNumber << " with fill bin "
       << fillBin << endl;

  outFile->cd();
  float asArray[ NUM_VALUE_BINS ][ NUM_BEAMS ][ NUM_OPTIONS ][ NUM_FILL_BINS ];
  float erArray[ NUM_VALUE_BINS ][ NUM_BEAMS ][ NUM_OPTIONS ][ NUM_FILL_BINS ];
  for( int beam = 0; beam < NUM_BEAMS; beam++ )
    for( int o = 0; o < NUM_OPTIONS; o++ )
      {
	cout << BEAM_NAMES[ beam ] << ", " << OPTION_NAMES[o] 
	     << " Acceptance Correction: " ;

	TString accCorrGraphName  = "accCorr";
	int accCorrOption;//west, east, or sqrt
	if( ( beam == YELLOW && OPTIONS[o] == LEFT_LUMINOSITY )
	    || ( beam == BLUE && OPTIONS[o] == RIGHT_LUMINOSITY ) )
	  {
	    accCorrOption = EAST;
	    accCorrGraphName += "East";
	  }
	else if( ( beam == YELLOW && OPTIONS[o] == RIGHT_LUMINOSITY )
		 || ( beam == BLUE && OPTIONS[o] == LEFT_LUMINOSITY ) )
	  {
	    accCorrOption = WEST;
	    accCorrGraphName += "West";
	  }
	else if( OPTIONS[o] == SQUARE_ROOT )
	  {
	    accCorrOption = SQRT;
	    accCorrGraphName += "Sqrt";
	  }
	TGraph *accCorrGraph;
	float acceptanceCorrection[ NUM_VALUE_BINS ];
	if( particle != "ohfe" ) // check tis
 	  {
	    accCorr.calculate( accCorrOption, acceptanceCorrection );
	    accCorrGraph = accCorr.graph( accCorrOption );
	  }
	else
	  {
	     accCorr.calculateConst( accCorrOption, acceptanceCorrection );
	    accCorrGraph = accCorr.graphConst( accCorrOption );
	  }

	asymmetry.setAccCorrection( acceptanceCorrection );
	accCorrGraph->Write( accCorrGraphName );

	for( int f = 0; f < NUM_FILL_BINS; f++ )
	  {
	    asymmetry.calculateAsymmetry( beam, f, OPTIONS[o] );
	    for( int i = 0; i < NUM_VALUE_BINS; i++ )
	      {
		asArray[i][ beam ][o][f] = asymmetry.getAsymmetry( beam, f, i );
		erArray[i][ beam ][o][f] = asymmetry.getAsymError( beam, f, i );
	      }//end looping through pt bins

	  }//end looping through fills

      }//end looping through options and beams

  outFile->cd();
  for( int ptBin = 0; ptBin < NUM_VALUE_BINS; ptBin++ )
    for( int beam = 0; beam < NUM_BEAMS; beam++ )
      for( int o = 0; o < NUM_OPTIONS; o++ )
	{
	  ostringstream title;
	  title << BEAM_NAMES[ beam ] << OPTION_NAMES[o] 
		<< " Asymmetry from p_{T} " << VALUE_BINS[ ptBin ] << " to "
		<< VALUE_BINS[ ptBin + 1 ] << " GeV; Fill Group Index; ";

	  ostringstream name;
	  name << BEAM_NAMES[ beam ] << OPTION_NAMES[o] << VALUE_BINS[ ptBin ]
	       << "to" << VALUE_BINS[ ptBin + 1 ];

	  float *asPtr = asArray[ ptBin ][ beam ][o];
	  float *erPtr = erArray[ ptBin ][ beam ][o];
	  TGraphErrors *graph = 
	    new TGraphErrors( NUM_FILL_BINS, fillBinArray, asPtr, 0, erPtr );
	  graph->SetTitle( title.str().c_str() );
          graph->SetName( name.str().c_str() );
	  graph->Write( name.str().c_str() );
	  graph->Delete();
	}
 
  outFile->Close();
  outFile->Delete();

  asymmetry.printCounts();

  accCorr.printNumbers();

  /*
  //for when the debugging gets super fun
  cout << endl;
  for( int f = 0; f < NUM_FILL_BINS; f++ )
    {
      asymmetry.printNumbersDetailed( YELLOW, f, 0 );
      asymmetry.printNumbersDetailed( BLUE,   f, 0 );
    } 

  cout << "Yellow beam, fill bin 46, pt bins: " << endl;
  for( int b = 0; b < NUM_VALUE_BINS; b++ )
    asymmetry.printNumbersPretty( YELLOW, 46, b );

  cout << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << endl;

  cout << "Blue  beam, fill bin 46, pt bins: " << endl;
  for( int b = 0; b < NUM_VALUE_BINS; b++ )
    asymmetry.printNumbersPretty( BLUE,   46, b );
  */
}
