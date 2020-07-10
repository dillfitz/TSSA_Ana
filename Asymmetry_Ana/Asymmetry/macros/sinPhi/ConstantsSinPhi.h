//Constants just for sin phi calculation i.e. PI and different pt bins

#ifndef _CONSTANTS_SIN_PHI_H_
#define _CONSTANTS_SIN_PHI_H_

#include "../../../Constants.h"

const int NUM_OPTIONS_PHI = 2;
const  char *OPTION_NAMES_PHI[ NUM_OPTIONS_PHI ] = { "Left", "Right" };
/*
const int NUM_PT_BINS = 4;
const float PT_BINS[ NUM_VALUE_BINS + 1 ] = 
  {1.5, 1.8, 2.1, 2.7, 6.0 };
const float PT_BIN_CENTERS[ NUM_VALUE_BINS ] = 
  { 1.65, 1.95, 2.4, 4.35 }; 
*/
const int NUM_PT_BINS = 4;
const float PT_BINS[ NUM_VALUE_BINS + 1 ] = 
  {1.5, 1.8, 2.1, 2.7, 5.0 };

const double PT_BIN_CENTERS[ NUM_PT_BINS ] = 
  { 1.639, 1.936, 2.349, 3.29 };

const int NUM_PHI_BINS = 3;
const float PHI_BINS[ NUM_PHI_BINS + 1 ] = { 0.589, 1.1127, 1.6364, 2.1598 };

const float PHI_BIN_CENTERS[ NUM_ARMS ][ NUM_PT_BINS ][ NUM_PHI_BINS ] = 
   {
    {//East arm 
      { 0.919, 1.272, 1.945 }, //1.5-1.8
      { 0.9358, 1.289, 1.955 }, //1.8-2.1
      { 0.957, 1.308, 1.961 }, //2.1-2.7
      { 0.9842, 1.32, 1.964 }  //2.7-5
    }
    {//West arm 
      { 0.8664, 1.431, 1.883 }, //1.5-1.8
      { 0.8775, 1.433, 1.879 }, //1.8-2.1
      { 0.8792, 1.438, 1.879 }, //2.1-2.7
      { 0.893, 1.44, 1.88 }  //2.7-5
    },
  };




#endif  /*_CONSTANTS_SIN_PHI_H_*/
