//make a list of events were there are multiple photons claiming to be the ERT
//trigger photon.  Good for removing random benefit in low p_T (below 4 GeV) 
//photon pairs
#include <iostream>
#include <fstream>
using namespace std;

#include "TFile.h"
#include "TTree.h"

const char *outputFileName = "multiesPair_pAl.txt";

void oneTriggerPerEvent_pAl()
{
  cout << "Opening output file " << outputFileName << endl;
  ofstream outputFile( outputFileName );

  TFile *inputFile = TFile::Open( "../pAl_goodruns_ana664.root" );
  TTree* dataTree = (TTree*)inputFile->Get( "pair_tree" );
  int eventNumber;
  float energy1;
  dataTree->SetBranchAddress( "event",  &eventNumber );
  dataTree->SetBranchAddress( "energy1", &energy1 );

  int lastEventNumber = -99;
  vector<int> eventVector;
  int numEntries = dataTree->GetEntries();
  cout << "There are " << numEntries << " to loop over " << endl;
  for( int i = 0; i < numEntries + 1; i++ )
    {
      if( i < numEntries )
	dataTree->GetEntry(i);
      else
	eventNumber = -99;

      if( i%10000000 == 0 ) 
	cout << "Processed " << i << " photon pairs " << endl;

      if( eventNumber != lastEventNumber &&  i != 0 )
	{
	  if( eventVector.size() > 1 )
	    {
	      //find highest energy trigger photon in the event
	      float maxEnergy = 0;
	      int size = eventVector.size();
	      for( int j = 0; j < size; j++ )
		{
		  int index = eventVector.at( j );
		  dataTree->GetEntry( index );
		  if( energy1 > maxEnergy ) 
		    maxEnergy = energy1;
		}
	      
	      //only want to use info from pairs that have this highest 
	      //energy trigger photon
	      for( int j = 0; j < size; j++ )
		{
		  int index = eventVector.at( j );
		  dataTree->GetEntry( index );
		  if( energy1 != maxEnergy )
		    outputFile << index << endl;
		}
	    }//more than one photon pair in event

	  //resetting event vector
	  eventVector.clear();

	}//end if( end of event ) 
      //make sure you're using the correct info for last event!
      if( i < numEntries )
	dataTree->GetEntry(i);
      else
	eventNumber = -999;

      lastEventNumber = eventNumber;
      eventVector.push_back(i);
 
    }//end looping over original tree

  cout << "Got to the end of the loop!" << endl;

}
