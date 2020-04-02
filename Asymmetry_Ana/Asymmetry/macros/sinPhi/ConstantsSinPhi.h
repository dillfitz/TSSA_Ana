//Constants just for sin phi calculation i.e. PI and different pt bins

#ifndef _CONSTANTS_SIN_PHI_H_
#define _CONSTANTS_SIN_PHI_H_

#include "../../../Constants.h"

const int NUM_OPTIONS_PHI = 2;
const  char *OPTION_NAMES_PHI[ NUM_OPTIONS_PHI ] = { "Left", "Right" };

const int NUM_PT_BINS = 4;
const float PT_BINS[ NUM_PT_BINS + 1 ] = { 5, 6, 8, 10, 18 };
const double PT_BIN_CENTERS[ NUM_PT_BINS ] =  
{ 5.39, 6.69, 8.77, 11.88 };  

const int NUM_PHI_BINS = 3;
const float PHI_BINS[ NUM_PHI_BINS + 1 ] = { 0.589, 1.1127, 1.6364, 2.1598 };
const float PHI_BIN_CENTERS[ NUM_ARMS ][ NUM_PT_BINS ][ NUM_PHI_BINS ] = 
  {
    {//West arm 
      { 0.839, 1.377, 1.896 }, //5-6
      { 0.840, 1.378, 1.895 }, //6-8
      { 0.842, 1.378, 1.897 }, //8-10
      { 0.840, 1.377, 1.895 }  //10-18
    },
    {//East arm 
      { 0.829, 1.393, 1.905 }, //5-6
      { 0.829, 1.394, 1.905 }, //6-8
      { 0.828, 1.396, 1.903 }, //8-10
      { 0.827, 1.393, 1.902 }  //10-18
    }
  };



#endif  /*_CONSTANTS_SIN_PHI_H_*/
