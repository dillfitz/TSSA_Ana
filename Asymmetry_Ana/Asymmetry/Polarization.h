//Class designed to keep track of the bream polarization and calculate the 
//wieghted average of the polarization of different  fills combined into the 
//same group. Weights are dertermined by the luminosity of that fill
#ifndef _POLARIZATION_H_
#define _POLARIZATION_H_

#include <cmath>
#include <iostream>
using namespace std;

#include "Constants.h"

class TGraphErrors;

class Polarization
{
 private:
  float polarization[ NUM_FILL_BINS ][ NUM_FILLS_IN_GROUP ];
  float polarizationErr[ NUM_FILL_BINS ][ NUM_FILLS_IN_GROUP ];
  double lumiWeight[ NUM_FILL_BINS ][ NUM_FILLS_IN_GROUP ];
  //weighting polarization by luminosity of the fill

 public:
  Polarization();
  virtual ~Polarization() {}

  bool updatePolarization( const int f, const int i,
			   const float inputPol, const float inputPolErr,
			   const double inputLumiWeight );
  //updates arrays above.  Divides all the lumi weights by 10,000 because 
  //doubles can only be so big

  bool averagePolarization( const int f, float &avePol, float &avePolErr );
  //finds the averag polarization for that fill bin weighted by luminosity

  bool getPolarization( const int f, const int i, float &pol, float &polErr );
  double getLumiWeight( const int f, const int i );

  void print();
  TGraphErrors* graph( const int color );

  bool isFillBinValid( const int f );
  bool isFillIndexValid( const int i );

};


#endif /* _POLARIZATION_H_ */
