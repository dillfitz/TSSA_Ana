//Designed to keep track of relative luminosity and add up counts when there are
//multiple fills in a group
#include "Constants.h"
#include "RelativeLuminosity.h"

#include "TGraph.h"

RelativeLuminosity::RelativeLuminosity()
{
  for( int f = 0; f < NUM_FILL_BINS; f++ )
    countsUp[f] = countsDown[f]  = 0;

}

bool RelativeLuminosity::isFillBinValid( const int f )
{
  if( f < 0 || f >= NUM_FILL_BINS )
    {
      cout << "Error: Invalid bin " << f << " entered for fill " << endl;
      return false;
    }
  else
    return true;
}

bool RelativeLuminosity::incrementCounts( const int f, 
					  int* spinPattern, 
					  long long* triggerCounts )
{
  if( !isFillBinValid( f ) )
    {
      cout << "RelativelLuminosity::incrementCounts()" << endl;
      return false;
    }

  for( int xing = 0; xing < NUM_XINGS; xing++ )
    {
      if( spinPattern[ xing ] == SPIN_UP )
	countsUp[f]   += triggerCounts[ xing ];
      else if( spinPattern[ xing ] == SPIN_DOWN )
	countsDown[f] += triggerCounts[ xing ];
    }

  return true;
}

bool RelativeLuminosity::incrementCounts( const int f, 
					  const long long inputCountsUp, 
					  const long long inputCountsDown )
{
  if( !isFillBinValid( f ) )
    {
      cout << "RelativelLuminosity::incrementCounts()" << endl;
      return false;
    }
  if( inputCountsUp < 0 || inputCountsDown < 0 )
    {
      cout << "Error: invalid input counts fill bin " << f << ": " 
	   << inputCountsUp << ", " << inputCountsDown << endl;
      return false;
    }  
  countsUp[f]   += inputCountsUp;
  countsDown[f] += inputCountsDown;

  return true;
}

float RelativeLuminosity::calculateRelativeLuminosity( const int f )
{
  if( !isFillBinValid( f ) )
    {
      cout << "RelativelLuminosity::calculateRelativeLuminosity()" << endl;
      return false;
    }
  else if( countsDown[f] == 0 )
    return 0;
  else 
    return (float)countsUp[f] / (float)countsDown[f];
}

long long RelativeLuminosity::getCountsUp( const int f )
{
  if( !isFillBinValid( f ) )
    {
      cout << "RelativelLuminosity::getCountsUp()" << endl;
      return false;
    }
  else
    return countsUp[f];
}

long long RelativeLuminosity::getCountsDown( const int f )
{
  if( !isFillBinValid( f ) )
    {
      cout << "RelativelLuminosity::getCountsDown()" << endl;
      return false;
    }
  else
    return countsDown[f];
}

void RelativeLuminosity::print()
{
  for( int f = 0; f < NUM_FILL_BINS; f++ )
    { 
      cout << "Bin " << f << " has counts up " << countsUp[f] 
	   << " and counts down " << countsDown[f] 
	   << " making for a relative luminosity of " 
	   << calculateRelativeLuminosity( f ) << endl;
    }
}

TGraph* RelativeLuminosity::graph( const int color )
{
  float fillBinArray[ NUM_FILL_BINS ], relLumiArray[ NUM_FILL_BINS ];
  for( int f = 0; f < NUM_FILL_BINS; f++ )
    {
      fillBinArray[f] = f + 1;
      relLumiArray[f] = calculateRelativeLuminosity( f );
    }

  TGraph *graph = new TGraph( NUM_FILL_BINS, fillBinArray, relLumiArray );
  TString title;
  title += "Relatvie Luminosity of the ";
  if( color == YELLOW )
    title += "Yellow ";
  else if( color == BLUE )
    title += "Blue ";
  title+= "Beam; fill bin; relative luminosity ";
  graph->SetTitle( title );

  if( color == YELLOW )
    graph->SetMarkerColor( kOrange );
  else if( color == BLUE )
    graph->SetMarkerColor( kBlue );

  return graph;
}
