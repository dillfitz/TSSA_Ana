#include <iostream>
#include <fstream>
using namespace std;

#include "TFile.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TString.h"

const int NUM_SECTORS = 8;
const int PBGL_WIDTH  = 48;
const int PBGL_HEIGHT = 96;

const bool doingLowEnergy = true;


 
void checkHotTowerList(const char *collisionSystem = "pAl")
{

  TString inputFileName = "";
  if (doingLowEnergy)
    inputFileName += "lowHotTowers_";
  else
    inputFileName += "highHotTowers_";
  
  inputFileName += collisionSystem;
  inputFileName += ".txt";

  int hotMap[ NUM_SECTORS ][ PBGL_WIDTH ][ PBGL_HEIGHT ];
  for( int sec  = 0; sec < NUM_SECTORS; sec++ )
    for( int y = 0; y < PBGL_WIDTH; y++ )
      for( int z = 0; z < PBGL_HEIGHT; z++ )
	hotMap[ sec ][y][z] = 0;

  //load old files
  ifstream inputFile;
  int sector, ypos, zpos;
  inputFile.open( inputFileName );
  if( inputFile.is_open() )
    while( !inputFile.eof() )
      {
	inputFile >> sector >> ypos >> zpos;
	hotMap[ sector ][ ypos ][ zpos ] = 1;
      }
  else
    cout << "Couldn't find file " << inputFileName << endl;


  //load histos
  //TFile *dataFile = TFile::Open( "../../13620/data/AllRuns.root" );
  TString infilename = "../../";
  infilename += collisionSystem;
  infilename += "_goodruns_ana664.root";
  TFile *dataFile = TFile::Open( infilename);
  TCanvas *cOld[ NUM_SECTORS ];
  TH2I *oldHistos[ NUM_SECTORS ];
  for( int i = 0; i < NUM_SECTORS; i++ )
    { 
      TString name = "hotSector";
      name += i;
      if( doingLowEnergy )
	name += "LowBefore";
      else
	name += "HighBefore";

      oldHistos[i] = (TH2I*)dataFile->Get( name );

      TString thing = "old";
      thing += i;
      cOld[i] = new TCanvas( thing, thing );
      oldHistos[i]->SetStats( false );
      oldHistos[i]->Draw( "COLZ" );
      TString imgname = "images/";
      imgname += collisionSystem;
      imgname += "HotTowers_";
      imgname += thing;
      if (doingLowEnergy)
        imgname += "_low";
      else
        imgname += "_high";
      imgname += ".png";  
      
      cOld[i]->SaveAs(imgname);    
    }

  TH2I *newHistos[ NUM_SECTORS ];
  TCanvas *cNew[ NUM_SECTORS ];
  for( int sec = 0; sec < NUM_SECTORS; sec++ )
    {
      int numY = oldHistos[ sec ]->GetNbinsX();
      int numZ = oldHistos[ sec ]->GetNbinsY();
      TString name = "NewSector";
      name += sec; 

      newHistos[ sec ] = new TH2I( name, name, numY, 0, numY, numZ, 0, numZ );

      double sumSquared = 0;
      int towersCounted = 0;
      for( int y = 0; y < numY; y++ )
	for( int z = 0; z < numZ; z++ )
	  {
	    if( hotMap[ sec ][y][z] ) continue;
	    double counts = oldHistos[ sec ]->GetBinContent( y + 1, z + 1 );

	    sumSquared += counts*counts;
	    towersCounted++;
	    newHistos[ sec ]->Fill( y, z, counts );
	  }
      cout << "Sector " << sec << ": RMS = " << sqrt( sumSquared/towersCounted )
	   << " = sqrt( " << sumSquared << " / " << towersCounted << " ) " 
	   << endl;

      //name += 0;
      cNew[ sec ] = new TCanvas( name, name );
      newHistos[ sec ]->Draw( "COLZ" );
      TString imgname = "images/";
      imgname += collisionSystem;
      imgname += "HotTowers_";
      imgname += name;
      if (doingLowEnergy)
        imgname += "_low";
      else
        imgname += "_high";
      imgname += ".png";
      
      cNew[ sec ] ->SaveAs(imgname);
    }
}
