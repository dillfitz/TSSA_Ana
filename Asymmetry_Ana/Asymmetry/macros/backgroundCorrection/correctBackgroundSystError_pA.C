#include <iostream>
using namespace std;

#include "TFile.h"
#include "TGraphAsymmErrors.h"

//#include "../../Constants.h"
#include "../plotMacros/weightedAverage.h"

const int NUM_ARMS = 2;
const int NUM_PT_BINS = 10;
const int NUM_BG_PT_BINS = 8;


//just to keep track of things in an arryay
const int NUM_THINGS = 3;
const int MINUS = 0;//minus r range
const int ORIG  = 1;
const int PLUS  = 2;//plus r range

// This will need to be adjusted... 
const int NUM_GRAPHS = 3;
const char* names[ NUM_GRAPHS ] = { "lumiBL","lumiBR", "sqrtB"};
  
// R_RANGE must be for systematics... 
void correctBackgroundSystError_pA(const char* particle = "eta", const char* collisionSystem = "pAu")
{
  if (particle == "pi0")
  {
    const int NUM_PT_BINS = 10;
    const double PT_BINS[ NUM_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 20 };    
    const int NUM_BG_PT_BINS = 8;
    const double BG_PT_BINS[ NUM_BG_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 9, 20 };          
    if (collisionSystem == "pAu")
    {
      const double BIN_CENTERS[ NUM_PT_BINS ] = { 2.711, 3.731, 4.311, 5.4, 6.414, 7.423, 8.431, 9.438, 10.78, 13.52 }; 
    }
    else if (collisionSystem == "pAl")
    {
      const double BIN_CENTERS[ NUM_PT_BINS ] = { 2.674, 3.467, 4.412, 5.41, 6.417, 7.424, 8.433, 9.439, 10.79, 13.55 };     
    }
  }
  else if (particle == "eta")
  {
    const int NUM_PT_BINS = 8;
    const double PT_BINS[ NUM_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 10, 20 };    
    const int NUM_BG_PT_BINS = 7;
    const double BG_PT_BINS[ NUM_BG_PT_BINS + 1 ] = { 2, 3, 4, 5, 6, 7, 8, 20 };   
    if (collisionSystem == "pAu")
    {
      const double BIN_CENTERS[ NUM_PT_BINS ] = { 2.64, 3.443, 4.406, 5.403, 6.413, 7.422, 8.74, 11.65 }; 
    }
    else if (collisionSystem == "pAl")
    {
      const double BIN_CENTERS[ NUM_PT_BINS ] = { 2.641, 3.461, 4.416, 5.408, 6.416, 7.425, 8.742, 11.7 };
    }     
  }
  
  TH1F* h_rError = new TH1F("rError",";#sigma_{A_{N}^{r}};", NUM_PT_BINS, PT_BINS);
  cout << "Subtracting off background asymmetry" << endl;
  TString sgString = "../dataFiles/";
  sgString += particle;
  sgString += "_";
  sgString += collisionSystem;
  sgString += "_AN.root";
  TFile *sgFile = TFile::Open( sgString );
  TGraphAsymmErrors* peakGraphs[ NUM_GRAPHS ];
  double *peak[ NUM_GRAPHS ], *peakErr[ NUM_GRAPHS ];
  for( int g = 0; g < NUM_GRAPHS; g++ )
    {
      peakGraphs[g] = (TGraphAsymmErrors*)sgFile->Get( names[g] );
      peak[g]    = peakGraphs[g]->GetY();
      peakErr[g] = peakGraphs[g]->GetEYhigh();
    }

  TString bgString = "../dataFiles/";
  bgString += particle;
  bgString += "_background_";
  bgString += collisionSystem;
  bgString += "_AN.root";
  TFile *bgFile = TFile::Open( bgString );
  TGraphAsymmErrors* backGraphs[ NUM_GRAPHS ];
  double *back[ NUM_GRAPHS ], *backErr[ NUM_GRAPHS ];
  for( int g = 0; g < NUM_GRAPHS; g++ )
    {
      backGraphs[g] = (TGraphAsymmErrors*)bgFile->Get( names[g] );
      back[g]    = backGraphs[g]->GetY();
      backErr[g] = backGraphs[g]->GetEYhigh();
    }
 
  TString bgFracString = "../dataFiles/";
  bgFracString += particle;
  bgFracString += "_bgfracs_wSys_";
  bgFracString += collisionSystem;
  bgFracString += ".root";
  TFile *bgFracFile = TFile::Open( bgFracString );
	TH1F* h_rEast = (TH1F*)bgFracFile->Get("rEast");  
	TH1F* h_rWest = (TH1F*)bgFracFile->Get("rWest");  
	TH1F* h_rSqrt = (TH1F*)bgFracFile->Get("rSqrt");  
	TH1F* h_rRange = (TH1F*)bgFracFile->Get("rRange");	

 	
  double r[ NUM_PT_BINS ];
  //3: minus r range, orig, plus r range
  double corrected[ NUM_THINGS ][ NUM_GRAPHS ][ NUM_PT_BINS ];
  double correctedErr[ NUM_THINGS ][ NUM_GRAPHS ][ NUM_PT_BINS ];
  for( int i = 0; i < NUM_THINGS; i++ )
    for( int g = 0; g < NUM_GRAPHS; g++ )
      for( int b = 0; b < NUM_PT_BINS; b++ )
	{
	  //     0        1        
	  //  { "lumiBL","lumiBR", "sqrtB" };
	  if( g == 0  ) //blue left
	    r[b] = h_rWest->GetBinContent(b+1);
	  else if(  g == 1  ) //blue left
	    r[b] = h_rEast->GetBinContent(b+1);
	  else if (g == 2) // blue sqrt
	    r[b] = h_rSqrt->GetBinContent(b+1);

	  if( i == MINUS )
	    r[b] = r[b] - h_rRange->GetBinContent(i+1);
	  else if( i == PLUS )
	    r[b] = r[b] + h_rRange->GetBinContent(i+1);

	  int a = b;//background asymmetry has less pt bins
	  if( b >= NUM_BG_PT_BINS ) a = NUM_BG_PT_BINS - 1;
	  //cout << a << endl;


	  corrected[i][g][b] = ( peak[g][b] - r[b]*back[g][a] ) / ( 1 - r[b] );
	  
	  correctedErr[i][g][b] = sqrt( peakErr[g][b]*peakErr[g][b] + 
					r[b]*r[b]*backErr[g][a]*backErr[g][a] );

	  correctedErr[i][g][b] /= 1 - r[b];

	  //cout << corrected[i][ graph ][ ptBin ] << endl;

	}

  double asymmetry[ NUM_THINGS ][ NUM_PT_BINS ];
  double asymError[ NUM_THINGS ][ NUM_PT_BINS ];
  double asymmetryLumi[ NUM_PT_BINS ];
  double asymLumiError[ NUM_PT_BINS ];  
  double asymmetrySqrt[ NUM_PT_BINS ];
  double asymSqrtError[ NUM_PT_BINS ];  
  double maxDiff[ NUM_PT_BINS ];
  for( int i = 0; i < NUM_THINGS; i++ )
    for( int b = 0; b < NUM_PT_BINS; b++ )
      {
	//     0        1        
	//  {"lumiBL","lumiBR", "sqrtB" };

	//first need to average left and right and then average blue and yellow
	weightedAverage( corrected[i][0][b], correctedErr[i][0][b],
			 corrected[i][1][b], correctedErr[i][1][b],
			 asymmetry[i][b], asymError[i][b] );


  if (i==1)
  {
    asymmetryLumi[b] = asymmetry[i][b];
    asymLumiError[b] = asymError[i][b];
    asymmetrySqrt[b] = corrected[i][2][b];
    asymSqrtError[b] = correctedErr[i][2][b];
  }
  
	if( i == ORIG )
	  cout << asymmetry[i][b] << " +/ - " << asymError[i][b] << endl;

      }


  for( int b = 0; b < NUM_PT_BINS; b++ )
    {
      double minusDiff = fabs( asymmetry[ MINUS ][b] - asymmetry[ ORIG ][b] );
      double plusDiff  = fabs( asymmetry[ PLUS ][b]  - asymmetry[ ORIG ][b] );

      if( minusDiff > plusDiff )
	maxDiff[b] = minusDiff;
      else
	maxDiff[b] = plusDiff;

  h_rError->SetBinContent(b+1, maxDiff[b]);
      cout << b << ": " << "max diff " << maxDiff[b] << " for: " 
	   << asymmetry[ MINUS ][b] << " " << asymmetry[ ORIG ][b] << " " 
	   << asymmetry[ PLUS ][b] << endl;
    }

  cout << endl;
  cout << "Or to print it more prettiful: " << endl;
  cout << "const double R_ERROR[ NUM_PT_BINS ] = { ";
  for( int b = 0; b < NUM_PT_BINS; b++ )
    {
      cout << maxDiff[b];
      if( b < NUM_PT_BINS - 1 )
	cout << ", ";
    }
  cout << " };" << endl;

  TString outfileString = "../dataFiles/";
  outfileString += particle;
  outfileString += "_bgCorrected_";
  outfileString += collisionSystem;
  outfileString += "_AN.root";
  TFile *outFile = new TFile( outfileString,"RECREATE" );
  outFile->cd(); 
  double ptLow[ NUM_PT_BINS ], ptHigh[ NUM_PT_BINS ];
  for( int b = 0; b < NUM_PT_BINS; b++ )
  {
    ptLow[b]  = fabs( PT_BINS[b]     - BIN_CENTERS[b] );
    ptHigh[b] = fabs( PT_BINS[b + 1] - BIN_CENTERS[b] );
    cout << asymmetryLumi[b] << " " << asymLumiError[b] << " " << endl;
  }  
 

  TGraphAsymmErrors *graphLumi = new TGraphAsymmErrors( NUM_PT_BINS, BIN_CENTERS, asymmetryLumi, ptLow, ptHigh, asymLumiError, asymLumiError );
  graphLumi->Write( "lumi" );
  graphLumi->SetMarkerStyle( kFullCircle );
  graphLumi->Delete();

  TGraphAsymmErrors *graphSqrt = new TGraphAsymmErrors( NUM_PT_BINS, BIN_CENTERS, asymmetrySqrt, ptLow, ptHigh, asymSqrtError, asymSqrtError );
  graphSqrt->Write( "sqrt" );
  graphSqrt->SetMarkerStyle( kFullCircle );
  graphSqrt->Delete();   
  
  h_rError->Write();
  h_rError->Delete();
  
}
