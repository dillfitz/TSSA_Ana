//load all the information about the fills
#include <iostream>
#include <fstream>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TGraph.h"
#include "TGraphErrors.h"

#include "Constants.h"

const int NUM_FILLS = 142;
const int NUM_FILLS_IN_FILE = 165;

void makeFillFile()
{
  ifstream textFile;
  textFile.open( "polarization.txt" );

  int fillNumberArray[ NUM_FILLS_IN_FILE ];
  float polBArray[ NUM_FILLS_IN_FILE ], polBErrArray[ NUM_FILLS_IN_FILE ];
  float polYArray[ NUM_FILLS_IN_FILE ], polYErrArray[ NUM_FILLS_IN_FILE ];
  for( int i = 0; i < NUM_FILLS_IN_FILE; i++ )
    {
      textFile >> fillNumberArray[i];
      textFile >> polBArray[i];
      textFile >> polBErrArray[i];
      textFile >> polYArray[i];
      textFile >> polYErrArray[i];
    }
  //want the fraction not the percentage
  for( int i = 0; i < NUM_FILLS_IN_FILE; i++ )
    {
      polBArray[i] /= 100;
      polBErrArray[i] /= 100;
      polYArray[i] /= 100;
      polYErrArray[i] /= 100;
    }

  int fillNumber, runNumber, xingShift;
  int spinBlue[ NUM_XINGS ];
  int spinYellow[ NUM_XINGS ];
  Long64_t triggerCounts[ NUM_XINGS ];
  for( int i = 0; i < NUM_XINGS; i++ )
    triggerCounts[i] = spinBlue[i] = spinYellow[i] = 0;

  TFile *polFile = TFile::Open("../AllRuns_736_ana644.root");
  //TFile *polFile = TFile::Open("../AllRuns_754_ana640.root");
  TTree* pol_tree = (TTree*)polFile->Get("pol_tree");
  pol_tree->SetBranchAddress( "fillnumber",    &fillNumber );
  pol_tree->SetBranchAddress( "run",           &runNumber );
  pol_tree->SetBranchAddress( "xing_shift",    &xingShift ); 
  pol_tree->SetBranchAddress( "gl1pScalerCh0", triggerCounts );
  //  pol_tree->SetBranchAddress( "gl1pScalerCh1", triggerCounts );
  pol_tree->SetBranchAddress( "spin_blue",     spinBlue );
  pol_tree->SetBranchAddress( "spin_yellow",   spinYellow );
 

  TH1F *blue   = new TH1F( "blue_RelLumi", 
			   "Fill by Fill Rel Lumi of the Blue Beam",
			   120, 0.7, 1.3 );
  TH1F *yellow = new TH1F( "yellow_RelLumi", 
			   "Fill by Fill Re Lumi of the Yellow Beam",
			   120, 0.7, 1.3 );

  TH1F *polHistB = new TH1F( "blue_Pol", 
			     "Fill by Fill Polarization of the Blue Beam", 
			     50, 0.3, 0.8 );
  TH1F *polHistY = new TH1F( "yellow_Pol", 
			     "Fill by Fill Polarization of the Yellow Beam", 
			     50, 0.3, 0.8 );

  const int length = 14;
  int runsInFill[length];
  for( int i = 0; i < length; i++ )
    runsInFill[i] = 0;
  int runIndex = 0;

  int lastRunNumber = 0;
  int lastFillNumber = 0;

  TFile *outFile = new TFile( "fill.root", "RECREATE");
  float relLumB, relLumY;
  float polarizationB, polarizationErrB, polarizationY, polarizationErrY;
  Long64_t countsUpB   = 0;
  Long64_t countsDownB = 0;
  Long64_t countsUpY   = 0;
  Long64_t countsDownY = 0;
  Long64_t totTriggerCounts[ NUM_XINGS ];
  for( int xing = 0; xing < NUM_XINGS; xing++ )
    totTriggerCounts[ xing ] = 0;
  int spinBSafeKeeping[ NUM_XINGS ];
  int spinYSafeKeeping[ NUM_XINGS ]; 
  TTree *fill = new TTree("fill_tree", "Fill By Fill information ");
  fill->Branch( "fillNumber",       &lastFillNumber,   "fillNumber/I" );
  fill->Branch( "runsInFill",       runsInFill,        "runsInFill[14]/I");
  fill->Branch( "polarizationB",    &polarizationB,    "polarizationB/F" );
  fill->Branch( "polarizationErrB", &polarizationErrB, "polarizationErrB/F" );
  fill->Branch( "polarizationY",    &polarizationY,    "polarizationY/F" );
  fill->Branch( "polarizationErrY", &polarizationErrY, "polarizationErrY/F" );
  fill->Branch( "countsUpB",        &countsUpB,        "countsUpB/L" );
  fill->Branch( "countsDownB",      &countsDownB,      "countsDownB/L" );
  fill->Branch( "countsUpY",        &countsUpY,        "countsUpY/L" );
  fill->Branch( "countsDownY",      &countsDownY,      "countsDownY/L" );
  fill->Branch( "triggerCounts",    totTriggerCounts,  "triggerCounts[120]/L" );
  fill->Branch( "spinDirB",         spinBSafeKeeping,  "spinDirB[120]/I" );
  fill->Branch( "spinDirY",         spinYSafeKeeping,  "spinDirY[120]/I" );

  //GetReady to make some lovely graphs <3 <3 <3 
  float fills[ NUM_FILLS ];
  float arrayRelLumB[ NUM_FILLS ], arrayRelLumY[ NUM_FILLS ];
  float arrayPolB[ NUM_FILLS ], arrayPolErrB[ NUM_FILLS ];
  float arrayPolY[ NUM_FILLS ], arrayPolErrY[ NUM_FILLS ];
  for( int i = 0; i < NUM_FILLS; i++ )
    {
      fills[i] = arrayRelLumB[i] = arrayRelLumY[i] = 0;
      arrayPolB[i] = arrayPolErrB[i] = arrayPolY[i]  = arrayPolErrY[i] = 0;
    }

  int fillIndex = 0;
  int numEntries = pol_tree->GetEntries();
  int skips = 0;
  for( int i = 0; i < numEntries; i++ )
    {
      pol_tree->GetEntry(i);

      if( fillNumber==18777 || fillNumber==18758 || fillNumber==18778) continue;

      //forgot about run 426319 before I merged files

      if( i == 0 && fillIndex == 0 )
	fills[ fillIndex ] = fillNumber;

      //we at a new fill, so we should load the info about the last fill 
      if( fillNumber != lastFillNumber && i!= 0 )
	{
	  /*
	  cout << "New fill " << fillNumber << " with runs in fill: ";
	  for( int k = 0; k < length; k++ ) cout << runsInFill[k] << " ";
	  cout << endl;
	  */

	  while( lastFillNumber != fillNumberArray[ fillIndex + skips ] )
	    {
	      cout << skips << ". looking for info on "<< lastFillNumber 
		   << " Skipping fill " 
		   << fillNumberArray[ fillIndex + skips ] << endl;
	      skips++;
	    }

	  cout << lastFillNumber << " ?=? " 
	       << fillNumberArray[ fillIndex + skips ]
	       << " B: " << polBArray[ fillIndex + skips ] << " +/- "
	       << polBErrArray[ fillIndex + skips ] << " Y: " 
	       << polYArray[ fillIndex + skips ] << " +/- " 
	       << polYErrArray[ fillIndex + skips ] << endl;

	  polarizationB = polBArray[ fillIndex + skips ];
	  polarizationErrB = polBErrArray[ fillIndex + skips ];
	  polarizationY = polYArray[ fillIndex + skips ];
	  polarizationErrY = polYErrArray[ fillIndex + skips ];

	  relLumB = (float)countsUpB / (float)countsDownB;
	  relLumY = (float)countsUpY / (float)countsDownY;	  


	  blue  ->Fill( relLumB );
	  yellow->Fill( relLumY );
	  polHistY->Fill( polarizationB );
	  polHistB->Fill( polarizationY ); 


	  if( fills[ fillIndex ] != 18758//funky ERTA efficiency
	      && fills[ fillIndex ] != 18777 && fills[ fillIndex ] != 18778 )
	      //funky yellow beam relative luminosity
	    fill->Fill();

	  fills[ fillIndex + 1 ] = fillNumber;
	  arrayRelLumB[ fillIndex ] = relLumB;
	  arrayRelLumY[ fillIndex ] = relLumY;
	  arrayPolB[ fillIndex ]    = polarizationB;
	  arrayPolErrB[ fillIndex ] = polarizationErrB;
	  arrayPolY[ fillIndex ]    = polarizationY;
	  arrayPolErrY[ fillIndex ] = polarizationErrY;

	  fillIndex++;

	  countsUpB = countsDownB = countsUpY = countsDownY = 0;
	  runIndex = 0;
	  for( int k = 0; k < length; k++ )
	    runsInFill[k] = 0;
	  for( int xing = 0; xing < NUM_XINGS; xing++ )
	    totTriggerCounts[ xing ] = 0;
	}
      if( runNumber != lastRunNumber )
	{
	  runsInFill[ runIndex ] = runNumber;
	  runIndex++; 
	  /*
	  if( triggerCounts[0] == 0 )
	    cout << "Entry " << i << " Fill number " << fillNumber 
		 << ", run number " << runNumber 
		 << " has zero trigger counts " << endl;
	  */
	  for( int xing = 0; xing < NUM_XINGS; xing++ )
	    {
	      if( spinBlue[ xing ] == SPIN_UP )
		countsUpB += triggerCounts[ xing ];
	      else if( spinBlue[ xing ] == SPIN_DOWN )
		countsDownB += triggerCounts[ xing ];
	      
	      if( spinYellow[ xing ] == SPIN_UP )
		countsUpY += triggerCounts[ xing ];
	      else if( spinYellow[ xing ] == SPIN_DOWN )
		countsDownY += triggerCounts[ xing ];

	      totTriggerCounts[ xing ] += triggerCounts[ xing ];
	    } //end loop through crossings in fill

	      
	      if( countsUpY + countsDownY != countsUpB + countsDownB )
		cout << "~~~~~~Ack! pol tree entry " << i << " and run number " 
		     << runNumber << " has a weird luminosity" << endl;

	}
	
      if( runNumber == 432008 && i != 0 )
	{
	  relLumB = (float)countsUpB / (float)countsDownB;
	  relLumY = (float)countsUpY / (float)countsDownY;	  

	  polarizationB = polBArray[ fillIndex + skips ];
	  polarizationErrB = polBErrArray[ fillIndex + skips ];
	  polarizationY = polYArray[ fillIndex + skips ];
	  polarizationErrY = polYErrArray[ fillIndex + skips ];

	  blue  ->Fill(relLumB);
	  yellow->Fill(relLumY);
	  polHistY->Fill( polarizationB );
	  polHistB->Fill( polarizationY ); 
	  fill->Fill();

	  //	  fills[fillIndex] = fillNumber;
	  arrayRelLumB[fillIndex] = relLumB;
	  arrayRelLumY[fillIndex] = relLumY;
	  arrayPolB[fillIndex]    = polarizationB;
	  arrayPolErrB[fillIndex] = polarizationErrB;
	  arrayPolY[fillIndex]    = polarizationY;
	  arrayPolErrY[fillIndex] = polarizationErrY;
	}

      lastRunNumber  = runNumber;
      lastFillNumber = fillNumber;

      for( int xing = 0; xing < NUM_XINGS; xing++ )
	{
	  spinBSafeKeeping[ xing ] = spinBlue[ xing ];
	  spinYSafeKeeping[ xing ] = spinYellow[ xing ];
	}

    }//end looping over entries of pol_tree

  //  polFile->Close();
  //  polFile->Delete();

  cout << "The fill index got to " << fillIndex << " with fill number " 
       << fillNumber << " and run number " << runNumber << endl;

  TGraph *blueGraph   = new TGraph( NUM_FILLS, fills, arrayRelLumB );
  TGraph *yellowGraph = new TGraph( NUM_FILLS, fills, arrayRelLumY );
  blueGraph->SetTitle( "Fill by Fill Relative Luminosity of the Blue Beam; Fill Number; Relataive Luminosity" );
  blueGraph->SetMarkerColor( kBlue );
  yellowGraph->SetTitle( "Fill by Fill Relative Luminosity of the Yellow Beam; Fill Number; Relataive Luminosity" );
  yellowGraph->SetMarkerColor( kOrange );

  TGraphErrors *bluePolGraph = new TGraphErrors( NUM_FILLS, fills, 
						 arrayPolB, 0, arrayPolErrB );
  TGraphErrors *yellowPolGraph = new TGraphErrors( NUM_FILLS, fills, 
						   arrayPolY, 0, arrayPolErrY );
  bluePolGraph->SetTitle( "Fill by Fill Polarization of the Blue Beam; Fill Number; Polarization" );
  bluePolGraph->SetMarkerColor( kBlue );
  bluePolGraph->SetLineColor( kBlue );
  yellowPolGraph->SetTitle( "Fill by Fill Polarization of the Yellow Beam; Fill Number; Polarization" );
  yellowPolGraph->SetMarkerColor( kOrange );
  yellowPolGraph->SetLineColor( kOrange );

  outFile->cd();

  fill  ->Write();
  blue  ->Write();
  yellow->Write();
  blueGraph  ->Write("blueRelLumiGraph");
  yellowGraph->Write("yellowRelLumiGraph");
  polHistB->Write();
  polHistY->Write();
  bluePolGraph  ->Write( "bluePolGraph" );
  yellowPolGraph->Write( "yellowPolGraph" );

  fill  ->Delete();
  blue  ->Delete();
  yellow->Delete();
  blueGraph  ->Delete();
  yellowGraph->Delete();
  polHistB->Delete();
  polHistY->Delete();
  bluePolGraph  ->Delete();
  yellowPolGraph->Delete();

  outFile->Close();
  outFile->Delete();
}
