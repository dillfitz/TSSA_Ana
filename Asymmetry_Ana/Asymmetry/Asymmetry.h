//Calculates the transverse single spin as a function of fill bin and another 
//variable, usually pt
#ifndef _ASYMMETRY_H_
#define _ASYMMETRY_H_

#include <iostream>
#include <cmath>
using namespace std;

#include "Constants.h"

class Asymmetry
{
 private:
  //All constants (variables in ALL_CAPS) are defined in Constants.h

  float accCorrection[ NUM_VALUE_BINS ]; //default 1

  float polarization[ NUM_BEAMS ][ NUM_FILL_BINS ];  //default 1
  float polError[ NUM_BEAMS ][ NUM_FILL_BINS ];      //default 0
  float relLuminosity[ NUM_BEAMS ][ NUM_FILL_BINS ]; //default 1

  int numUL[ NUM_BEAMS ][ NUM_FILL_BINS ][ NUM_VALUE_BINS ];
  int numDL[ NUM_BEAMS ][ NUM_FILL_BINS ][ NUM_VALUE_BINS ];
  int numUR[ NUM_BEAMS ][ NUM_FILL_BINS ][ NUM_VALUE_BINS ];
  int numDR[ NUM_BEAMS ][ NUM_FILL_BINS ][ NUM_VALUE_BINS ];
  //Up(U) or down(D) refers to the polarization of the beam.  Left(L) or 
  //right(R) is the side that arm that detected the count is on in reference to
  //the polarized beam going direction.  

  float asymmetry[ NUM_BEAMS ][ NUM_FILL_BINS ][ NUM_VALUE_BINS ];
  float asymError[ NUM_BEAMS ][ NUM_FILL_BINS ][ NUM_VALUE_BINS ];

  bool isBeamValid( const int beam );
  bool isFillBinValid( const int f );
  bool isValueBinValid( const int valueBin );
  int spinDirection[ NUM_BEAMS ][ NUM_FILL_BINS ][ NUM_XINGS ];
  //caution! this will need to be changed if you ever decide to start combining
  //fills. i.e. NUM_FILL_BINS and NUM_FILLS is different

 public:
  Asymmetry();
  virtual ~Asymmetry() {}

  bool setAccCorrection( const float inputAccCorr );
  bool setAccCorrection( const float* inputAccCorrArray );
  //Used if the acceptance correction depends on the value that you are binning 
  //in.  i.e. The eta meson acceptance is p_T dependent

  bool setPolarization( const int beam, 
			const int f, 
			const float inputPolarization, 
			const float inputPolErr );

  bool setRelLuminosity( const int beam,
			 const int f, 
			 const float inputRelLuminosity );

  bool setSpinDirectionArrays( const int f,
			       const int* yellowSpinDirection,
			       const int* blueSpinDirection );

  bool incrementCountsByXing( const int f,
			      const int valueBin,
			      const int arm,
			      const int xing );
  //Uses the xing number to figure out the polarization direction
  //need to set the spin direction arrays from the fill info first

  bool incrementCounts( const int f, 
			const int valueBin,
			const int arm, 
			const int spinPattern );
  //Uses the "spin pattern" to figure out the polarization direction
  //"spin pattern" is stored in data trees

  void clearCounts();

  bool calculateAsymmetry( const int beam,
			   const int f,
			   const int option );
  //The options are: LEFT_LUMINOSITY, RIGHT_LUMINOSITY, SQUARE_ROOT
  //Explanation for these formulas located in Constants.h

  bool averageOverFills( const int beam,  
			 float *aveAsymmetry,
			 float *aveAsymError );

  void averageOverBeamsAndFills( float *aveAsymmetry,
				 float *aveAsymError );

  float getAccCorrection( const int valueBin );
  float getPolarization ( const int beam, const int f );
  float getPolError     ( const int beam, const int f );
  float getRelLuminosity( const int beam, const int f );

  int getNumUL( const int beam, const int f, const int valueBin );
  int getNumUR( const int beam, const int f, const int valueBin );
  int getNumDL( const int beam, const int f, const int valueBin );
  int getNumDR( const int beam, const int f, const int valueBin );

  float getAsymmetry( const int beam, const int f, const int valueBin );
  float getAsymError( const int beam, const int f, const int valueBin );

  void printCounts();
  //prints total number of counts per beam and p_T bin in a table the can 
  //hopefully be copied and pasted into latex

  void printNumbersPretty( const int beam, const int f, const int valueBin );
  //For when you are debugging and it's not going well.  Trying to put numbers 
  //into a format that is simple to copy and paste into excel.  Prints:
  //<beam> <fill bin> <value bin> <polarization> <polarization error> 
  //<relative luminosity> <num up left> <num up right> <num down left> 
  //<num down right>

  void printNumbersDetailed( const int beam, const int f, const int valueBin );
  //Same thing but with strings identifying which numbers are which
};

#endif /* _ASYMMETRY_H_ */
