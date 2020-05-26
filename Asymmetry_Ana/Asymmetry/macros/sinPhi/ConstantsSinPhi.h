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
const int NUM_PT_BINS = 6;
const float PT_BINS[ NUM_VALUE_BINS + 1 ] = 
  { 1.0, 1.25, 1.5, 1.8, 2.1, 2.7, 6.0 };

const float PT_BIN_CENTERS[ NUM_VALUE_BINS ] = 
  { 1.134, 1.374, 1.64, 1.936, 2.351, 3.325 }; 


const int NUM_PHI_BINS = 3;
const float PHI_BINS[ NUM_PHI_BINS + 1 ] = { 0.589, 1.1127, 1.6364, 2.1598 };

const float PHI_BIN_CENTERS[ NUM_ARMS ][ NUM_PT_BINS ][ NUM_PHI_BINS ] = 
   {
    {//West arm 
      { 0.8679, 1.275, 1.913 }, //1-1.25
      { 0.8979, 1.263, 1.928 }, //1.25-1.5
      { 0.9189, 1.27, 1.945 }, //1.5-1.8
      { 0.935, 1.288, 1.954 }, //1.8-2.1
      { 0.9573, 1.308, 1.962 }, //2.1-2.7
      { 0.9863, 1.323, 1.965 }  //2.7-6
    },
    {//East arm 
      { 0.862, 1.399, 1.859 }, //1-1.25
      { 0.8674, 1.429, 1.875 }, //1.25-1.5
      { 0.866, 1.431, 1.884 }, //1.5-1.8
      { 0.8763, 1.433, 1.879 }, //1.8-2.1
      { 0.878, 1.438, 1.881 }, //2.1-2.7
      { 0.8954, 1.44, 1.88 }  //2.7-6
    }
  };




#endif  /*_CONSTANTS_SIN_PHI_H_*/
