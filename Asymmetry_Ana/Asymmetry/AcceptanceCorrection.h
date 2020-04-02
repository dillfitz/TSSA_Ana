//Calculates the acceptance correction as the asymmetry is being calculated

#ifndef _ACCEPTANCE_CORRECTION_H_
#define _ACCEPTANCE_CORRECTION_H_

#include <iostream>
#include <cmath>
using namespace std;

#include "Constants.h"

class TGraph;

class AcceptanceCorrection
{
 private:
  int   counts[ NUM_ARMS ][ NUM_VALUE_BINS ];
  float cosSum[ NUM_ARMS ][ NUM_VALUE_BINS ];

  bool isArmValid( const int arm );
  bool isValueBinValid( const int valueBin );
  bool isOptionValid( const int option );
  //WEST, EAST, or SQRT

  void print( const int option, float* correction );

 public:
  AcceptanceCorrection();
  virtual ~AcceptanceCorrection() {}

  bool increment( const int arm, const int valueBin, 
		  const float px, const float py );

  bool calculate( const int option, float *acceptanceCorrection );
  //make acceptanceCorrection into an array that depends on the value bin
  //can be WEST, EAST, or SQRT

  bool calculateConst( const int option, float *acceptanceCorrection );
  //make acceptanceCorrection into an array that is constant for value bin
  //This is for the direct photon asymmetry in my case because it is only 
  //measuring one particle and doesn't have decay angle shenanigans
  //can be WEST, EAST, or SQRT

  TGraph* graph( const int option );
  //Store acceptance correction values in output file

  TGraph* graphConst( const int option );
  //Store acceptance correction value in output file


  //for all your debugging needs:
  void printNumbers();
  int  getCounts( const int arm, const int valueBin );
  float getCosSum( const int arm, const int valueBin );

};

#endif /* _ACCEPTANCE_CORRECTION_H_ */
