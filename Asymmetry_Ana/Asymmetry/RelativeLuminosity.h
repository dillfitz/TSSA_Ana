//Designed to keep track of relative luminosity and add up counts when there are
//multiple fills in a group
#ifndef _RELATIVELUMINOSITY_H_
#define _RELATIVELUMINOSITY_H_

#include <iostream>
using namespace std;

#include "Constants.h"

class TGraph;

class RelativeLuminosity
{
 private:
  long long countsUp[ NUM_FILL_BINS ];
  long long countsDown[ NUM_FILL_BINS ];

 public:
  RelativeLuminosity();
  ~RelativeLuminosity() { }

  bool incrementCounts( const int f, 
			int* spinPattern, 
			long long* triggerCounts );
  //increment counts using arrays of bunch by bunch information collected from 
  //the spin data base
 
  bool incrementCounts( const int f, 
			const long long inputCountsUp, 
			const long long inputCountsDown );
  //Used for when the counts have already been totalled for each fill, but we 
  //are combining fills into groups
 
  float calculateRelativeLuminosity( const int f );
  //L = counts up / counts down

  long long getCountsUp( const int f );
  long long getCountsDown( const int f );

  void print();
  TGraph* graph( const int color );

  bool isFillBinValid( const int f );
};


#endif /* _RELATIVELUMINOSITY_H_ */
