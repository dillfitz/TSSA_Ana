#ifndef _FIND_BIN_H_
#define _FIND_BIN_H_

#include "Constants.h"

int findBin( const float value )
{
  if( value < VALUE_BINS[0] )
    return -99;
  else if( value > VALUE_BINS[ NUM_VALUE_BINS ] ) 
    return -33;

  int bin = 0;
  bool foundBin = false;
  while( !foundBin && bin < NUM_VALUE_BINS )
    {
      if( value <= VALUE_BINS[ bin + 1 ] )
	foundBin = true;
      else
	bin++;
    }
  return bin;
}

int findBin( const int numBins, const float* bins, const float value )
{
  if( value < bins[0] )
    return -99;
  else if( value > bins[ numBins ] ) 
    return -33;

  int bin = 0;
  bool foundBin = false;
  while( !foundBin && bin < numBins  )
    {
      if( value <= bins[ bin + 1 ] )
	foundBin = true;
      else
	bin++;
    }
  return bin;
}


#endif /* _FIND_BIN_H */
