#include <iostream>
using namespace std;

#include "TFile.h"
#include "TTree.h"

#include "ConstantsSinPhi.h"
#include "../../../Constants.h"
#include "../../../findBin.h"

const bool ISOLATED = false;

// Use this for the open heavy flavor electron measurement //
void findPhiBinCenter_pA( const char* particle = "eta", const char* collisionSystem = "pAu", const bool background = true  )
{
  if( particle != "ohfe" && particle != "e" && particle != "dp" && particle != "pi0" && particle != "eta" )
    cout << "Error! particle can only be dp, pi0, or eta!  You are calculating nonsense " << endl;

  TString inputDataFileName = "../../../curated_";
  inputDataFileName += particle;
  inputDataFileName += "_";
  inputDataFileName += collisionSystem;
  inputDataFileName += ".root";

  TString inputTreeNameInFile = "";
  if (background)
    inputTreeNameInFile += "background_";
  inputTreeNameInFile += particle;
  inputTreeNameInFile += "_tree";

  TString outputFileName = "../dataFiles/";
  outputFileName += particle;
  outputFileName += "_";
  if (background)
    outputFileName += "background_";
  outputFileName += collisionSystem;  
  outputFileName += ".root";
  cout << "~~~~~~~~~~~Gonna output everything into " << outputFileName 
       << "~~~~~~~~~~~" << endl;


  TFile *dataFile = TFile::Open( inputDataFileName );
  TTree* dataTree = (TTree*)dataFile->Get( inputTreeNameInFile );
  cout << "Loading " << inputTreeNameInFile << " from " << inputDataFileName << endl;
  int arm;
  float pt, phi, px, py, px1, px2, py1, py2, energy1, energy2, coneEnergy1;
  dataTree->SetBranchAddress( "arm",         &arm );
  if( particle == "dp" )
  {
    dataTree->SetBranchAddress( "px",         &px );
    dataTree->SetBranchAddress( "py",         &py );
  }
  else if (particle == "ohfe" )
  {
    dataTree->SetBranchAddress( "phi", &phi );
    dataTree->SetBranchAddress( "pt",         &pt );
  }
  else
  {
    cout << " here" << endl;
    dataTree->SetBranchAddress( "px1",         &px1 );
    dataTree->SetBranchAddress( "px2",         &px2 );
    dataTree->SetBranchAddress( "py1",         &py1 );
    dataTree->SetBranchAddress( "py2",         &py2 );
  }

  float phiTotal[ NUM_ARMS ][ NUM_PT_BINS ][ NUM_PHI_BINS ];
  int counts[ NUM_ARMS ][ NUM_PT_BINS ][ NUM_PHI_BINS ];
  for( int a = 0; a < NUM_ARMS; a++ )
    for( int ptBin = 0; ptBin < NUM_PT_BINS; ptBin++ )
      for( int phiBin = 0; phiBin < NUM_PHI_BINS; phiBin++ )
	      phiTotal[a][ ptBin ][ phiBin ] = counts[a][ ptBin ][ phiBin ] = 0;
	      
  float ptTotal[ NUM_PT_BINS ];
  for( int i = 0; i < NUM_PT_BINS; i++ )
    ptTotal[i] = 0;

  int numEntries = dataTree->GetEntries();
  cout << "There are " << numEntries << " in tree " << endl;
  for( int i = 0; i < numEntries; i++ )
  {
    //cout << phi << " " << pt << endl;
    dataTree->GetEntry(i);
    if( i%10000000 == 0 ) 
	    cout << "Processed " << i << " entries " << endl;
 
    if( particle == "pi0" || particle == "eta" )
	  {
	    px = px1 + px2;
	    py = py1 + py2;
	    //cout << "px : " << px << endl;
	    //cout << "py : " << py << endl;
	  }
    float pt = sqrt( px*px + py*py );
    //cout << "pt : " << pt << endl;

    float phi = atan( py / px );

   // cout << "arm : " << arm << endl;
   // cout << "phi_before : " << phi << endl;
    int ptBin = findBin( NUM_PT_BINS, PT_BINS, pt );
    //cout << "ptBin : " << ptBin << endl;
    ptTotal[ ptBin ] += pt;

      //Nicoles pp analysis used the same (- for arm 0, + for arm 1) //
      if( arm == 0 )  
        phi =  ( PI/2 - phi );
      else if( arm == 1 )
        phi = (PI/2 + phi);
   // cout << "phi : " << phi << endl;          
/*
    if( arm == 0 )  
      phi =  - ( PI/2 - phi );
    else if( arm == 1 )
      phi = PI/2 - phi;
*/      




    int phiBin = findBin( NUM_PHI_BINS, PHI_BINS, phi );
    

    //cout <<"phiBin : " << phiBin << endl; 

    //cout << "arm : " << arm << " pt : " << pt << " phi : " << phi << endl;
    if( ptBin >= 0 && phiBin >= 0 )//only acceptable bin values allowed!
	  {
	    phiTotal[ arm ][ ptBin ][ phiBin ] += phi;
	    counts[ arm ][ ptBin ][ phiBin ]++;
      //cout << counts[arm][ptBin][phiBin] << endl;
	  }
  }

  for( int a = 0; a < NUM_ARMS; a++ )
  {
    for( int ptBin = 0; ptBin < NUM_PT_BINS; ptBin++ )
	  {
	    for( int phiBin = 0; phiBin < NUM_PHI_BINS; phiBin++ )
      {
        //cout << counts[a][ptBin][phiBin] << endl;
      }
    }
  }
            

  cout << setprecision(4);
  cout << "const double PT_BIN_CENTERS[ NUM_PT_BINS ] = " << endl << "  { ";
  for( int ptBin = 0; ptBin < NUM_PT_BINS; ptBin++ )
  {
    int totalCounts = 0;
    for( int a = 0; a < NUM_ARMS; a++ )
	    for( int phiBin = 0; phiBin < NUM_PHI_BINS; phiBin ++ )
	      totalCounts += counts[a][ ptBin ][ phiBin ];
	      
    cout << ptTotal[ ptBin ] / totalCounts;
    if( ptBin < NUM_PT_BINS  - 1)
	    cout << ", ";
  }
  cout << " };" << endl;

  cout << "const float PHI_BIN_CENTERS[ NUM_ARMS ][ NUM_PT_BINS ]"
       << "[ NUM_PHI_BINS ] = " << endl << "   {" << endl;
       
  for( int a = 0; a < NUM_ARMS; a++ )
  {
    cout << "    {//" << ARM_NAMES[a] << " arm " << endl; 
    for( int ptBin = 0; ptBin < NUM_PT_BINS; ptBin++ )
	  {
	    cout << "      { ";
	    for( int phiBin = 0; phiBin < NUM_PHI_BINS; phiBin++ )
	    {
        //cout << counts[a][ptBin][phiBin];
	      cout << phiTotal[a][ ptBin ][ phiBin ] / counts[a][ ptBin ][ phiBin ];
	      if( phiBin < NUM_PHI_BINS - 1 )
		      cout << ", ";
	    }
	    cout << " }";
	    if( ptBin < NUM_PT_BINS - 1 )
	      cout << ", //";
	    else
	      cout << "  //";
	    cout << PT_BINS[ ptBin ] << "-" << PT_BINS[ ptBin + 1 ]<< endl;
	}
  if( a == WEST )
	  cout << "    }," << endl;
  else
	  cout << "    }" << endl;
  }
  cout << "  };" << endl;

}
