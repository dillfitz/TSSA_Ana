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

const int NUM_FILLS = 29;  // This corresponds to the number of fills used in the analysis (after bad runs are removed) -- 144 for the p+p pi0 and eta TSSA analysis  -- currently 28 for the pAl analysis based on Norbert's good run list and additional QA
const int NUM_FILLS_IN_FILE = 35;  // This corresponds to the number of lines in polarization_pp.txt (

void makeFillFile_pAl()
{
  ifstream textFile;
  // This needs to be replaced eventually! Although now I need to check if polarization.txt is actually used to calculate the asymmetry, or just to make the figure of polarization by fill.... 
  textFile.open( "polarization_pAl.txt" );

  int fillNumberArray[ NUM_FILLS_IN_FILE ];
  float polBArray[ NUM_FILLS_IN_FILE ], polBErrArray[ NUM_FILLS_IN_FILE ];
  for( int i = 0; i < NUM_FILLS_IN_FILE; i++ )
    {
      textFile >> fillNumberArray[i];
      textFile >> polBArray[i];
      textFile >> polBErrArray[i];
    }
  //want the fraction not the percentage
  for( int i = 0; i < NUM_FILLS_IN_FILE; i++ )
    {
      polBArray[i] /= 100;
      polBErrArray[i] /= 100;
    }

  int fillNumber, runNumber, xingShift;
  float polb;
  int spinBlue[ NUM_XINGS ];
  Long64_t triggerCounts[ NUM_XINGS ];
  for( int i = 0; i < NUM_XINGS; i++ )
    triggerCounts[i] = spinBlue[i] = 0;

  TFile *polFile = TFile::Open("../pAl_goodruns_ana664.root");
  TTree* pol_tree = (TTree*)polFile->Get("pol_tree");
  pol_tree->SetBranchAddress( "fillnumber",    &fillNumber );
  pol_tree->SetBranchAddress( "run",           &runNumber );
  pol_tree->SetBranchAddress( "xing_shift",    &xingShift ); 
  pol_tree->SetBranchAddress( "gl1pScalerCh0", triggerCounts );
  //  pol_tree->SetBranchAddress( "gl1pScalerCh1", triggerCounts );
  pol_tree->SetBranchAddress( "spin_blue",     spinBlue );
  pol_tree->SetBranchAddress( "polb",          &polb);
 

  TH1F *blue   = new TH1F( "blue_RelLumi", 
			   "Fill by Fill Rel Lumi of the Blue Beam",
			   120, 0.7, 1.3 );

  TH1F *polHistB = new TH1F( "blue_Pol", 
			     "Fill by Fill Polarization of the Blue Beam", 
			     50, 0.3, 0.8 );

  const int length = 25;
  int runsInFill[length];
  for( int i = 0; i < length; i++ )
    runsInFill[i] = 0;
  int runIndex = 0;

  int lastRunNumber = 0;
  int lastFillNumber = 0;

  TFile *outFile = new TFile( "fill_pAl.root", "RECREATE");
  float relLumB;
  float polarizationB, polarizationErrB;
  Long64_t countsUpB   = 0;
  Long64_t countsDownB = 0;
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
  fill->Branch( "countsUpB",        &countsUpB,        "countsUpB/L" );
  fill->Branch( "countsDownB",      &countsDownB,      "countsDownB/L" );
  fill->Branch( "triggerCounts",    totTriggerCounts,  "triggerCounts[120]/L" );
  fill->Branch( "spinDirB",         spinBSafeKeeping,  "spinDirB[120]/I" );

  //GetReady to make some lovely graphs <3 <3 <3 
  float fills[ NUM_FILLS ];
  float arrayRelLumB[ NUM_FILLS ];
  float arrayPolB[ NUM_FILLS ], arrayPolErrB[ NUM_FILLS ];
  for( int i = 0; i < NUM_FILLS; i++ )
    {
      fills[i] = arrayRelLumB[i] = 0;
      arrayPolB[i] = arrayPolErrB[i] = 0;
    }

  int fillIndex = 0;
  int numEntries = pol_tree->GetEntries();
  int skips = 0;
  for( int i = 0; i < numEntries; i++ )
    {
      pol_tree->GetEntry(i);

      //cout << polb << endl;

      // COME BACK AND REVISE THIS IF THERE IS NONZERO VALUES FROM THE CNI GROUP
       //if (polb==0) {continue;}
      // Actually, this should only apply for the pAu case ^^
      

      // See if any fills need to be removed based on QA.. what about the ones with the wrong spin patterns? What about the ones with zero polarization?

      // For some reason there is no data in the pair_tree corresponding to this fill (for pAu), so possibly there is a mistake in the taxi code? Or every candidate in the fill did not pass the cuts...
      //if (fillNumber==19110) {continue;}

      // this is just a test //
      //if (fillNumber==19237 || fillNumber==19238) {continue;}

      if( i == 0 && fillIndex == 0 )
	fills[ fillIndex ] = fillNumber;

      //we at a new fill, so we should load the info about the last fill 
      if( fillNumber != lastFillNumber && i!= 0 )
	{

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
	       << polBErrArray[ fillIndex + skips ]<<  endl;

	  polarizationB = polBArray[ fillIndex + skips ];
	  polarizationErrB = polBErrArray[ fillIndex + skips ];

	  relLumB = (float)countsUpB / (float)countsDownB;


	  blue  ->Fill( relLumB );
	  polHistB->Fill( polarizationB );


	  fill->Fill();

	  fills[ fillIndex + 1 ] = fillNumber;
	  arrayRelLumB[ fillIndex ] = relLumB;
	  arrayPolB[ fillIndex ]    = polarizationB;
	  arrayPolErrB[ fillIndex ] = polarizationErrB;

	  fillIndex++;

	  countsUpB = countsDownB = 0;
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

	      totTriggerCounts[ xing ] += triggerCounts[ xing ];
	    } //end loop through crossings in fill

	      
	  // I cannot think of any similar check to implement in the pA analysis?
	  // if( countsUpY + countsDownY != countsUpB + countsDownB )
	  //	cout << "~~~~~~Ack! pol tree entry " << i << " and run number " 
	  //	     << runNumber << " has a weird luminosity" << endl;

	}
	  cout << "New fill " << fillNumber << " with runs in fill: ";
	  for( int k = 0; k < length; k++ ) cout << runsInFill[k] << " ";
	  cout << endl;
      // Why for this specific run number? Is this the first one in p+p? Need to check because this condition will fail!! So... 432008 corresponds to the last run in the pp data, lets switch this to the last run in the pAu... (similarly for pAl)
      if( runNumber == 438422 && i != 0 )
	{
	  relLumB = (float)countsUpB / (float)countsDownB;

	  polarizationB = polBArray[ fillIndex + skips ];
	  polarizationErrB = polBErrArray[ fillIndex + skips ];

	  blue  ->Fill(relLumB);
	  // Is this a typo for pp? It appeared in another spot that I removed
	  //polHistY->Fill( polarizationB );
	  //polHistB->Fill( polarizationY ); 
	  polHistB->Fill(polarizationB);
	  fill->Fill();

	  //	  fills[fillIndex] = fillNumber;
	  arrayRelLumB[fillIndex] = relLumB;
	  arrayPolB[fillIndex]    = polarizationB;
	  arrayPolErrB[fillIndex] = polarizationErrB;
	}

      lastRunNumber  = runNumber;
      lastFillNumber = fillNumber;

      for( int xing = 0; xing < NUM_XINGS; xing++ )
	{
	  spinBSafeKeeping[ xing ] = spinBlue[ xing ];
	}

    }//end looping over entries of pol_tree

  //  polFile->Close();
  //  polFile->Delete();

  cout << "The fill index got to " << fillIndex << " with fill number " 
       << fillNumber << " and run number " << runNumber << endl;

  TGraph *blueGraph   = new TGraph( NUM_FILLS, fills, arrayRelLumB );
  blueGraph->SetTitle( "Fill by Fill Relative Luminosity of the Blue Beam; Fill Number; Relataive Luminosity" );
  blueGraph->SetMarkerColor( kBlue );

  TGraphErrors *bluePolGraph = new TGraphErrors( NUM_FILLS, fills, 
						 arrayPolB, 0, arrayPolErrB );
  bluePolGraph->SetTitle( "Fill by Fill Polarization of the Blue Beam; Fill Number; Polarization" );
  bluePolGraph->SetMarkerColor( kBlue );
  bluePolGraph->SetLineColor( kBlue );
 
  outFile->cd();

  fill  ->Write();
  blue  ->Write();
  blueGraph->SetName("blueRelLumiGraph");
  blueGraph  ->Write();
  polHistB->Write();
  bluePolGraph->SetName("bluePolGraph");
  bluePolGraph  ->Write();

  fill  ->Delete();
  blue  ->Delete();
  blueGraph  ->Delete();
  polHistB->Delete();
  bluePolGraph  ->Delete();

  outFile->Close();
  outFile->Delete();
}
