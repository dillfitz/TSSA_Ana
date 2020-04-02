//Class designed to keep track of the bream polarization and calculate the 
//wieghted average of the polarization of different  fills combined into the 
//same group. Weights are dertermined by the luminosity of that fill
#include <cmath>
#include <iostream>
using namespace std;

#include "Polarization.h"
#include "Constants.h"

#include "TGraphErrors.h"

Polarization::Polarization()
{
  for( int f = 0; f < NUM_FILL_BINS; f++ )
    for( int i = 0; i < NUM_FILLS_IN_GROUP; i++ )
      {
	polarization[f][i] =  lumiWeight[f][i] = 0;
	polarizationErr[f][i] = 1;
      }
}

bool Polarization::isFillBinValid( const int f )
{
  if( f < 0 || f >= NUM_FILL_BINS )
    {
      cout << "Error: Invalid bin " << f << " entered for fill " << endl;
      return false;
    }
  else
    return true;
}

bool Polarization::isFillIndexValid( const int i )
{
  if( i < 0 || i > NUM_FILLS_IN_GROUP )
    {
      cout << "Error: Invalid index " << i << " entered for fill " << endl;
      return false;
    }
  else 
    return true;
}

bool Polarization::updatePolarization( const int f, const int i,
				       const float inputPol,
				       const float inputPolErr,
				       const double inputLumiWeight )
{
  if( !isFillBinValid( f ) || !isFillIndexValid(i) )
    {
      cout << "Polarization::updatePolarization( const int f, const int i, " 
	   << "const float inputPol, const float inputPolErr, "
	   << "const double inputLumiWeight )" << endl;
      return false;
    }

  else if( inputPol < 0 || inputPolErr < 0 || inputPol > 1 || inputPolErr > 1 )
    {
      cout << "Error: Invalid input polarization or polarization error: " 
	   << "fill bin: " << f << ": " << inputPol << " +/- " << inputPolErr 
	   << endl;
      return false;
    }
  else if( inputLumiWeight < 0 )
    {
      cout << "Error: Invalid input luminosity weight: fill bin" << f << ": " 
	   << inputLumiWeight << endl;
      return false;
    }

  polarization[f][i]    = inputPol;
  polarizationErr[f][i] = inputPolErr;
  lumiWeight[f][i]      = inputLumiWeight / 10000.0;
  //doubles can only be so big
  return true; 
}

//Weighting just by lumi
bool Polarization::averagePolarization( const int f, 
					float &avePol, float &avePolErr )
{
  if( !isFillBinValid( f )  )
    {
      cout << "Polarization::averagePolarization( const int f, "
	   << "float &avePol, float &avePolErr )" << endl;
      return false;
    }

  float polNumerator = 0;
  float errNumerator = 0;
  float denominator = 0; 
  for( int i = 0; i < NUM_FILLS_IN_GROUP; i++ )
    {
      polNumerator += lumiWeight[f][i]*polarization[f][i];
      errNumerator += lumiWeight[f][i]*lumiWeight[f][i]
	                 *polarizationErr[f][i]*polarizationErr[f][i];
      denominator += lumiWeight[f][i];
    }

  avePol    = polNumerator / denominator;
  avePolErr = sqrt( errNumerator ) / denominator;
  return true;
}

bool Polarization::getPolarization( const int f, const int i, 
				    float &pol, float &polErr )
{
  if( !isFillBinValid( f ) || !isFillIndexValid(i))
    {
      cout << "Polarization::getPolarization( )" << endl;
      return false;
    }
  else 
    {
      pol    = polarization[f][i];
      polErr = polarizationErr[f][i];
      return true;
    }
}

double Polarization::getLumiWeight( const int f, const int i )
{
  if( !isFillBinValid( f ) || !isFillIndexValid(i))
    {
      cout << "Polarization::getLumiWeight( )" << endl;
      return -999;
    }
  else 
    return lumiWeight[f][i];
}

void Polarization::print()
{
  for( int f = 0; f < NUM_FILL_BINS; f++ )
    {
      float pol, polErr;
      averagePolarization( f, pol, polErr );
      cout << "Polarization bin " << f << " has value: " << pol << " +/- " 
	   << polErr << endl;
    }
}

TGraphErrors* Polarization::graph( const int color )
{
  float fillBinArray[ NUM_FILL_BINS ];
  float polArray[ NUM_FILL_BINS ], polErrArray[ NUM_FILL_BINS ];
  for( int f = 0; f < NUM_FILL_BINS; f++ )
    {
      fillBinArray[f] = f + 1;
      averagePolarization( f, polArray[f], polErrArray[f] );
    }

  TGraphErrors *graph = new TGraphErrors( NUM_FILL_BINS, fillBinArray, polArray,
					  0, polErrArray );
  TString title;
  title += "Polarization of the ";
  if( color == YELLOW )
    title += "Yellow ";
  else if( color == BLUE )
    title += "Blue ";
  title+= "Beam; fill bin; polarization ";
  graph->SetTitle( title );

  if( color == YELLOW )
    {
      graph->SetMarkerColor( kOrange );
      graph->SetLineColor( kOrange );
    }
  else if( color == BLUE )
    {
      graph->SetMarkerColor( kBlue );
      graph->SetLineColor( kBlue );
    }

  return graph;
}
