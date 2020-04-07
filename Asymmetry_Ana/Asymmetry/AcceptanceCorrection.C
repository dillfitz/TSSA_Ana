#include "Constants.h"
#include "AcceptanceCorrection.h"

#include "TGraph.h"

AcceptanceCorrection::AcceptanceCorrection()
{
  for( int a = 0; a < NUM_ARMS; a++ )
    for( int b = 0; b < NUM_VALUE_BINS; b++ )
      counts[a][b] = cosSum[a][b] = 0;
}

bool AcceptanceCorrection::isArmValid( const int arm )
{
  if( arm != WEST && arm != EAST )
    {
      cout << "   Error: invalid arm " << arm << endl;
      return false;
    }
  else
    return true;
}

bool AcceptanceCorrection::isOptionValid( const int option )
{
  if( option != WEST && option != EAST && option != SQRT )
    {
      cout << "   Error: invalid acceptance correction option " << option 
	   << endl;
      return false;
    }
  else
    return true;
}

bool AcceptanceCorrection::isValueBinValid( const int valueBin )
{
  if( valueBin < 0 || valueBin >= NUM_VALUE_BINS )
    {
      cout << "  Error: Invalid value bin " << valueBin << endl;
      return false;
    }
  else
    return true;
}


void AcceptanceCorrection::print( const int option, float* correction )
{
  if( option == WEST )      cout << "West Arm    ";
  else if( option == EAST ) cout << "East Arm    ";
  else if( option == SQRT ) cout << "Square Root ";

  for( int b= 0; b < NUM_VALUE_BINS; b++ )
    {
      cout << correction[b];
      if( b < NUM_VALUE_BINS - 1 ) cout << ", ";
    }
  cout << endl;
}

bool AcceptanceCorrection::increment( const int arm, const int valueBin, 
				      const float px, const float py )
{
  if( !isArmValid( arm ) || !isValueBinValid( valueBin ) )
    {
      cout << "-->AcceptanceCorrection::increment()" << endl;
      return false;
    }

  counts[ arm ][ valueBin ] += 1;

  float phi = atan( py / px );
  if( arm == EAST )
    phi = phi + PI;
  cosSum[ arm ][ valueBin ] += fabs( cos( phi ) );

  //cout << "ptbin " << valueBin << ", phi " << phi << endl;

  return true;

}

bool AcceptanceCorrection::calculate( const int option, 
				      float *acceptanceCorrection )
{
  if( !isOptionValid( option ) )
    {
      cout << "-->AcceptanceCorrection::caculate()" << endl;
      return false;
    }

  for( int b = 0; b < NUM_VALUE_BINS; b++ )
    if( ( option == WEST || option == EAST ) && counts[ option ][b] )
      acceptanceCorrection[b] = cosSum[ option ][b] 
	                          / (float)counts[ option ][b];
    else if( option == SQRT && ( counts[ WEST ][b] + counts[ EAST ][b] ) )
      {
	float totalCosSum = cosSum[ WEST ][b] + cosSum[ EAST ][b];
	float totalCounts = (float)counts[ WEST ][b] + (float)counts[ EAST ][b];
	acceptanceCorrection[b] =  totalCosSum / totalCounts;
      }
    else
      acceptanceCorrection[b] = -999;

  print( option, acceptanceCorrection );
}


bool AcceptanceCorrection::calculateConst( const int option, 
					   float *acceptanceCorrection )
{
  if( !isOptionValid( option ) )
    {
      cout << "-->AcceptanceCorrection::caculateConst()" << endl;
      return false;
    }

  float totalCosSum[ NUM_ARMS ] = { 0, 0 };
  float totalCounts[ NUM_ARMS ] = { 0, 0 };
  for( int a = 0; a < NUM_ARMS; a++ )
    for( int b = 0; b < NUM_VALUE_BINS; b++ )
      {
	totalCosSum[a] += cosSum[a][b];
	totalCounts[a] += (float)counts[a][b];
      }

  float cor;
  if( ( option == WEST || option == EAST ) && totalCounts[ option ] )
    cor = totalCosSum[ option ] / totalCounts[ option ];
  else if( option == SQRT && ( totalCounts[ WEST ] + totalCounts[ EAST ] ) )
    cor =   ( totalCosSum[ WEST ] + totalCosSum[ EAST ] )
                        / ( totalCounts[ WEST ] + totalCounts[ EAST ] );
  else
    cor = -999;
 
  for( int b = 0; b < NUM_VALUE_BINS; b++ )
    acceptanceCorrection[b] = cor;

  print( option, acceptanceCorrection );
}

TGraph* AcceptanceCorrection::graph( const int option )
{
  if( !isOptionValid( option ) )
    {
      cout << "-->AcceptanceCorrection::graph()" << endl;
      return 0;
    }
  float correctionArray[ NUM_VALUE_BINS ];
  calculate( option, correctionArray );
  TGraph *graph = new TGraph( NUM_VALUE_BINS, BIN_CENTERS, correctionArray );

  TString title = "Acceptance Correction for the ";
  TString name = "AccCorr";
  if( option == WEST )      { title += "West Arm "; name += "W"; }
  else if( option == EAST ) { title += "East Arm "; name += "E";}
  else if( option == SQRT ) { title += "Square Root Formula "; name += "Sqrt";}
  graph->SetTitle( title );
  graph->SetName( name );

  return graph;
}


TGraph* AcceptanceCorrection::graphConst( const int option )
{
  if( !isOptionValid( option ) )
    {
      cout << "-->AcceptanceCorrection::graphConst()" << endl;
      return 0;
    }
  float correctionArray[ NUM_VALUE_BINS ];
  calculateConst( option, correctionArray );
  TGraph *graph = new TGraph( NUM_VALUE_BINS, BIN_CENTERS, correctionArray );

  TString title = "Acceptance Correction for the ";
  TString name = "AccCorr";
  if( option == WEST )      { title += "West Arm "; name += "W"; }
  else if( option == EAST ) { title += "East Arm "; name += "E";}
  else if( option == SQRT ) { title += "Square Root Formula "; name += "Sqrt";}
  graph->SetTitle( title );
  graph->SetName( name );

  return graph;
}


void AcceptanceCorrection::printNumbers()
{
  for( int b = 0; b < NUM_VALUE_BINS; b++ )
    {
      cout << "~~~~" << VALUE_BINS[b] << " to " << VALUE_BINS[b + 1] << "~~~~" 
	   << endl;
      for( int op = 0; op < 3; op++ )
	{
	  if( op == WEST )      cout << "West: ";
	  else if( op == EAST ) cout << "East: ";
	  else if( op == SQRT ) cout << "Sqrt: ";

	  if( op < 2 )
	    cout << cosSum[op][b] / (float)counts[op][b] << " = " 
		 << cosSum[op][b] << " / " << counts[op][b] << endl;
	  else
	    cout << ( cosSum[ WEST ][b] + cosSum[ EAST ][b] )
	      / ( (float)counts[ WEST ][b] + (float)counts[ EAST ][b] ) 
		 << " = " << cosSum[ WEST ][b] + cosSum[ EAST ][b] << " / " 
		 << counts[ WEST ][b] + counts[ EAST ][b] << endl;
	}
      cout << endl;
    }
}

int AcceptanceCorrection::getCounts( const int arm, const int valueBin )
{
  if( !isArmValid( arm ) || !isValueBinValid( valueBin ) )
    {
      cout << "-->AcceptanceCorrection::getCounts()" << endl;
      return -999;
    }
  return counts[ arm ][ valueBin ];
}

float AcceptanceCorrection::getCosSum( const int arm, const int valueBin )
{
  if( !isArmValid( arm ) || !isValueBinValid( valueBin ) )
    {
      cout << "-->AcceptanceCorrection::getCosSum()" << endl;
      return -999;
    }
  return cosSum[ arm ][ valueBin ];
}
