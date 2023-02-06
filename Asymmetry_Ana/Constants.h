//File constaining all the global constants for Caluculating asymmetries...
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <string>
using namespace std;

const float PI = 3.1415926;
const int NUM_XINGS = 120;
const int SPIN_UP   = -1;
const int SPIN_DOWN = +1;

const int NUM_BEAMS = 2;
const int YELLOW = 0;
const int BLUE   = 1;
const string BEAM_NAMES[ NUM_BEAMS ] = { "Yellow", "Blue" };
//yellow beam travels counter-clock wise through RHIC and South through PHENIX

//=====Options for the asymmetry formula:
const int NUM_OPTIONS = 3;
const int LEFT_LUMINOSITY   = 99;
//Cancels out acceptance effects, but not luminosity. Only looks at counts that
//are found in the detector to left of the polarized-proton-going direction
const int RIGHT_LUMINOSITY  = 102;
//same as above but for the dectector to the right of the beam direction
//has a minus sign to keep A_N a left right asymmetry
const int SQUARE_ROOT       = 104;
//approximate and only for transverse single-spin asymmetries but where both 
//acceptance and luminosity effects cancel to several orders.  Uses both sides 
//of the detector
const  int OPTIONS[ NUM_OPTIONS ] = 
    { LEFT_LUMINOSITY, RIGHT_LUMINOSITY, SQUARE_ROOT };
const string OPTION_NAMES[ NUM_OPTIONS ] = { "Left", "Right", "Sqrt" };

/*
const int NUM_ARMS = 2;
const int LEFT = 0;
const int RIGHT = 1;
const int EAST = 0;
const int WEST = 1;
const int SQRT = 2;
const string ARM_NAMES[ NUM_ARMS ] = { "East", "West" };
*/


// Note that the mapping to east and west is opposite for an old EMCAL analysis (i.e. use below for dp, eta, pi0)

const int NUM_ARMS = 2;
const int LEFT = 0;
const int RIGHT = 1;
const int WEST = 0;
const int EAST = 1;
const int SQRT = 2;
const string ARM_NAMES[ NUM_ARMS ] = { "West", "East" };


const int NUM_CUT_OFF = 10;
//Error formulas are based off of Poison Statistics, so there needs to be "many"
//counts in each bin for those formulas to be valid. 
//Asymmetry::caculateAsymmetry( const int beam, const int fillBin, int option ) 
//requires there to be at least NUM_CUT_OFF counts in each bin being used.  
//Set this number to 10 if you're doing things properly and set to 1 if you just
//want your error bars not to blow up

/*
//Binning for open heavy flavor electron analysis //
//const int NUM_FILLS_IN_GROUP = 1;
//const int NUM_FILL_BINS = 140;
const int NUM_FILLS_IN_GROUP = 2;
const int NUM_FILL_BINS = 70;
//const int NUM_FILLS_IN_GROUP = 3;
//const int NUM_FILL_BINS = 47;
const int NUM_VALUE_BINS = 4;
const float VALUE_BINS[ NUM_VALUE_BINS + 1 ] = 
  { 1.5, 1.8, 2.1, 2.7, 5.0 };
const float BIN_CENTERS[ NUM_VALUE_BINS ] = 
  { 1.639, 1.936, 2.349, 3.29 };
*/

//Extending the pT range down to 1 GeV for the OHFe analysis //
/*
const int NUM_FILLS_IN_GROUP = 2;
const int NUM_FILL_BINS = 70;
const int NUM_VALUE_BINS = 6;
const float VALUE_BINS[ NUM_VALUE_BINS + 1 ] = 
  { 1.0, 1.3, 1.5, 1.8, 2.1, 2.7, 5.0 };
const float BIN_CENTERS[ NUM_VALUE_BINS ] = 
  { 1.161, 1.398, 1.639, 1.936, 2.349, 3.29 };
*/

/*
//////////////
// pi0 pAu //
////////////
// 1 fill per group
//const int NUM_FILLS_IN_GROUP = 1;
//const int NUM_FILL_BINS = 78;
// 2 fills per group
const int NUM_FILLS_IN_GROUP = 2;
const int NUM_FILL_BINS = 39;

const int NUM_VALUE_BINS = 10;
const float VALUE_BINS[ NUM_VALUE_BINS + 1 ] = 
  { 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 20 };
const float BIN_CENTERS[ NUM_VALUE_BINS ] = 
  { 2.711, 3.731, 4.311, 5.4, 6.414, 7.423, 8.431, 9.438, 10.78, 13.52 }; 
/**/

/*
//////////////
// pi0 pAl //
////////////
// 1 fill per group
//const int NUM_FILLS_IN_GROUP = 1;
//const int NUM_FILL_BINS = 29;
// 2 fills per group
const int NUM_FILLS_IN_GROUP = 2;
const int NUM_FILL_BINS = 15;

const int NUM_VALUE_BINS = 10;
const float VALUE_BINS[ NUM_VALUE_BINS + 1 ] = 
  { 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 20 };
const float BIN_CENTERS[ NUM_VALUE_BINS ] = 
  { 2.676, 3.468, 4.412, 5.41, 6.417, 7.424, 8.433, 9.439, 10.79, 13.56 };
/**/

/*
//////////////
// eta pAu //
////////////
// 1 fill per group
//const int NUM_FILLS_IN_GROUP = 1;
//const int NUM_FILL_BINS = 78;
// 2 fills per group
const int NUM_FILLS_IN_GROUP = 2;
const int NUM_FILL_BINS = 39;

const int NUM_VALUE_BINS = 8;
const float VALUE_BINS[ NUM_VALUE_BINS + 1 ] = 
  { 2, 3, 4, 5, 6, 7, 8, 10, 20 };
const float BIN_CENTERS[ NUM_VALUE_BINS ] = 
  { 2.64, 3.443, 4.406, 5.403, 6.413, 7.422, 8.74, 11.65 };
/**/

/*
//////////////  
// eta pAl //
////////////
// 1 fill per group
//const int NUM_FILLS_IN_GROUP = 1;
//const int NUM_FILL_BINS = 29;
// 2 fills per group
const int NUM_FILLS_IN_GROUP = 2;
const int NUM_FILL_BINS = 15;

const int NUM_VALUE_BINS = 8;
const float VALUE_BINS[ NUM_VALUE_BINS + 1 ] = 
  { 2, 3, 4, 5, 6, 7, 8, 10, 20 };
const float BIN_CENTERS[ NUM_VALUE_BINS ] = 
  { 2.642, 3.461, 4.416, 5.408, 6.417, 7.425, 8.742, 11.7 };
/**/

/*
/////////////////////////
// pi0 background pAu //
///////////////////////
// 1 fill per group
//const int NUM_FILLS_IN_GROUP = 1;
//const int NUM_FILL_BINS = 78;
// 2 fills per group
const int NUM_FILLS_IN_GROUP = 2;
const int NUM_FILL_BINS = 39;

const int NUM_VALUE_BINS = 8;
const float VALUE_BINS[ NUM_VALUE_BINS + 1 ] = 
  { 2, 3, 4, 5, 6, 7, 8, 9, 20 };
const float BIN_CENTERS[ NUM_VALUE_BINS ] = 
  { 2.629, 3.447, 4.406, 5.398, 6.408, 7.418, 8.427, 10.41 };
/**/

/*
/////////////////////////
// pi0 background pAl //
///////////////////////
// 1 fill per group
//const int NUM_FILLS_IN_GROUP = 1;
//const int NUM_FILL_BINS = 29;
// 2 fills per group
const int NUM_FILLS_IN_GROUP = 2;
const int NUM_FILL_BINS = 15;

const int NUM_VALUE_BINS = 8;
const float VALUE_BINS[ NUM_VALUE_BINS + 1 ] = 
  { 2, 3, 4, 5, 6, 7, 8, 9, 20 };
const float BIN_CENTERS[ NUM_VALUE_BINS ] = 
  { 2.634, 3.458, 4.412, 5.401, 6.408, 7.42, 8.429, 10.45 };
/**/

/*
/////////////////////////
// eta background pAu //
///////////////////////
// 1 fill per group
//const int NUM_FILLS_IN_GROUP = 1;
//const int NUM_FILL_BINS = 78;
// 2 fills per group
const int NUM_FILLS_IN_GROUP = 2;
const int NUM_FILL_BINS = 39;

const int NUM_VALUE_BINS = 7;
const float VALUE_BINS[ NUM_VALUE_BINS + 1 ] = 
  { 2, 3, 4, 5, 6, 7, 8, 20 };
const float BIN_CENTERS[ NUM_VALUE_BINS ] = 
  { 2.631, 3.431, 4.397, 5.394, 6.408, 7.419, 9.385 };
/**/

/**/
/////////////////////////
// eta background pAl //
///////////////////////
// 1 fill per group
//const int NUM_FILLS_IN_GROUP = 1;
//const int NUM_FILL_BINS = 29;
// 2 fills per group
const int NUM_FILLS_IN_GROUP = 2;
const int NUM_FILL_BINS = 15;

const int NUM_VALUE_BINS = 7;
const float VALUE_BINS[ NUM_VALUE_BINS + 1 ] = 
  { 2, 3, 4, 5, 6, 7, 8, 20 };
const float BIN_CENTERS[ NUM_VALUE_BINS ] = 
  { 2.631, 3.451, 4.409, 5.4, 6.413, 7.422, 9.432 };
/**/


/*
//pi0 and eta as a function of photon pt
const int NUM_FILLS_IN_GROUP = 3;
const int NUM_FILL_BINS = 48;
const int NUM_VALUE_BINS = 6;
const float VALUE_BINS[ NUM_VALUE_BINS + 1 ] = 
  { 2, 3, 4, 5, 6, 8, 18 };
const float BIN_CENTERS[ NUM_VALUE_BINS ] = 
  { 2.29, 3.34, 4.38, 5.4, 6.69, 9.48 };
/**/
/*
//just one pt bin for dir photon systematic uncertainty from background purposes
const int NUM_FILLS_IN_GROUP = 2;
const int NUM_FILL_BINS = 72;
const int NUM_VALUE_BINS = 1;
const float BIN_CENTERS[ NUM_VALUE_BINS ] = { 6 };
const float VALUE_BINS[ NUM_VALUE_BINS + 1 ] = 
  { 5, 18 };
/**/


#endif /* _CONSTANTS_H_ */
