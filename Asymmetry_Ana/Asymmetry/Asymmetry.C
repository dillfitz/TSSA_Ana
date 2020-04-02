#include "Constants.h"
#include "Asymmetry.h"

Asymmetry::Asymmetry()
{
  for( int i = 0; i < NUM_VALUE_BINS; i++ )
    accCorrection[i] = 1;

  for( int beam = 0; beam < NUM_BEAMS; beam++ )
    for( int f = 0; f < NUM_FILL_BINS; f++ )
      {
	polarization [ beam ][f] = 1;
	polError     [ beam ][f] = 0;
	relLuminosity[ beam ][f] = 1;

	for( int xing = 0; xing < NUM_XINGS; xing++ )
	  spinDirection[ beam ][f][ xing ] = 0;

	for( int i = 0; i < NUM_VALUE_BINS; i++ )
	  {
	    numUL[ beam ][f][i] = 0;
	    numDL[ beam ][f][i] = 0;
	    numUR[ beam ][f][i] = 0;
	    numDR[ beam ][f][i] = 0;

	    asymmetry[ beam ][f][i] = 0;
	    asymError[ beam ][f][i] = 1;
	  }
      }
}

bool Asymmetry::isBeamValid( const int beam )
{
  if( beam != YELLOW && beam != BLUE )
    {
      cout << "  Error: invalid beam " << beam << endl;
      return false;
    }
  else
    return true;
}

bool Asymmetry::isFillBinValid( const int f )
{
  if( f < 0 || f >= NUM_FILL_BINS )
    {
      cout << "  Error: Invalid bin " << f << " entered for fill " << endl;
      return false;
    }
  else
    return true;
}

bool Asymmetry::isValueBinValid( const int valueBin )
{
  if( valueBin < 0 || valueBin >= NUM_VALUE_BINS )
    {
      cout << "  Error: Invalid value bin " << valueBin << endl;
      return false;
    }
  else
    return true;
}

bool Asymmetry::setAccCorrection( const float inputAccCorr )
{
  if( inputAccCorr < 0 || inputAccCorr > 1 )
    {
      cout << "  Error: Acceptance Correction should be between 1 and 0" 
	   << endl;
      return false;
    }

  for( int i = 0; i < NUM_VALUE_BINS; i++ )
    accCorrection[i] = inputAccCorr;

  return true;
}

bool Asymmetry::setAccCorrection( const float* inputAccCorrArray )
{
  for( int i = 0; i < NUM_VALUE_BINS; i++ )
    {
      if( inputAccCorrArray[i] < 0 || inputAccCorrArray[i] > 1 )
	{
	  cout << "  Error: Acceptance Correction should be between 1 and 0" 
	       << endl;
	  return false;
	}
      accCorrection[i] = inputAccCorrArray[i];
    }
  return true;
}

bool Asymmetry::setPolarization( const int beam,
				 const int f, 
				 const float inputPolarization, 
				 const float inputPolErr )
{
  if( !isBeamValid( beam ) || !isFillBinValid( f ) )
    {
      cout << "-->Asymmetry::setPolarization()" << endl;;
      return false;
    }
   if( inputPolarization > 1 || inputPolarization < 0 )
     {
       cout << "Error: beam " << beam << " fill bin " << f 
	    << " invalid polarization value entered: " << inputPolarization 
	    << endl;
       return false;
     }

   if( inputPolErr > 1 || inputPolErr < 0 )
     {
       cout << "Error: beam " << beam << " fill bin " << f 
	    << " invalid polarization error value entered: " << inputPolErr
	    << endl;
       return false;
     }

  polarization[ beam ][f] = inputPolarization;
  polError    [ beam ][f] = inputPolErr;
  return true;

}

bool Asymmetry::setRelLuminosity( const int beam,
				  const int f, 
				  const float inputRelLuminosity )
{
  if( !isBeamValid( beam ) || !isFillBinValid( f ) )
    {
      cout << "-->Assymetry::setRelLuminosity()" << endl;
      return false;
    }
  else if( inputRelLuminosity < 0 )
    {
      cout << "Error: beam " << beam << " fill bin " << f 
	   << " invald relative luminosity enetered: " << inputRelLuminosity 
	   << endl;
      return false;
    }

  relLuminosity[ beam ][f] = inputRelLuminosity;
  return true;
}

bool Asymmetry::setSpinDirectionArrays( const int f,
					const int* yellowSpinDirection,
					const int* blueSpinDirection )
{
  if( !isFillBinValid( f ) )
  //caution! this will need to be changed if you ever decide to start combining
  //fills.  i.e. NUM_FILL_BINS and NUM_FILLS is different
    {
      cout << "-->Asymmetry::setSpinDirectionArrays()" << endl;
      return false;
    }

  for( int xing = 0; xing < NUM_XINGS; xing++ )
    {
      spinDirection[ YELLOW ][f][ xing ] = yellowSpinDirection[ xing ];
      spinDirection[ BLUE   ][f][ xing ] =   blueSpinDirection[ xing ];
    }
  return true;
}

bool Asymmetry::incrementCountsByXing( const int f,
				       const int valueBin,
				       const int arm,
				       const int xing )
{
  if( !isFillBinValid( f ) || !isValueBinValid( valueBin ) )
    {
      cout << "-->Asymmetry::incrementCountsByXing()" << endl;
      return false;
    }
  else if( xing < 0 || xing >= NUM_XINGS )
    {
      cout << "Error: invalid input xing " << xing << endl;
      return false;
    }

  if( arm == 0 )//west arm: yellow right, blue left
    {
      if( spinDirection[ YELLOW ][f][ xing ] == SPIN_UP )
	numUR[ YELLOW ][f][ valueBin ]++;
      else if( spinDirection[ YELLOW ][f][ xing ] == SPIN_DOWN )
	numDR[ YELLOW ][f][ valueBin ]++;

      if( spinDirection[ BLUE ][f][ xing ] == SPIN_UP )
	numUL[ BLUE ][f][ valueBin ]++;
      else if( spinDirection[ BLUE ][f][ xing ] == SPIN_DOWN )
	numDL[ BLUE ][f][ valueBin ]++;
    }
  else if( arm == 1 )//east arm: yellow left, blue right
    {
      if( spinDirection[ YELLOW ][f][ xing ] == SPIN_UP )
	numUL[ YELLOW ][f][ valueBin ]++;
      else if( spinDirection[ YELLOW ][f][ xing ] == SPIN_DOWN )
	numDL[ YELLOW ][f][ valueBin ]++;

      if( spinDirection[ BLUE ][f][ xing ] == SPIN_UP )
	numUR[ BLUE ][f][ valueBin ]++;
      else if( spinDirection[ BLUE ][f][ xing ] == SPIN_DOWN )
	numDR[ BLUE ][f][ valueBin ]++;
    }

  return true;
}

bool Asymmetry::incrementCounts( const int f, 
				 const int valueBin, 
				 const int arm, 
				 const int spinPattern )
{
  if( !isFillBinValid( f ) || !isValueBinValid( valueBin ) )
    {
      cout << "-->Asymmetry::incrementCounts()" << endl;
      return false;
    }

  if( arm == 0 )//west arm: yellow right, blue left
    {
      if( spinPattern == 0 )//yellow down, blue down
	{
	  numDR[ YELLOW ][f][ valueBin ]++;
	  numDL[ BLUE   ][f][ valueBin ]++;
	}
      else if( spinPattern == 1 )//yellow up, blue down
	{
	  numUR[ YELLOW ][f][ valueBin ]++;
	  numDL[ BLUE   ][f][ valueBin ]++;
	}
      else if( spinPattern == 2 )//yellow down, blue up
	{
	  numDR[ YELLOW ][f][ valueBin ]++;
	  numUL[ BLUE   ][f][ valueBin ]++;
	}
      else if( spinPattern == 3 )//yellow up, blue up
	{
	  numUR[ YELLOW ][f][ valueBin ]++;
	  numUL[ BLUE   ][f][ valueBin ]++;
	}
    }
  else if( arm == 1 )//east arm: yellow left, blue right
    {
      if( spinPattern == 0 )//yellow down, blue down
	{
	  numDL[ YELLOW ][f][ valueBin ]++;
	  numDR[ BLUE   ][f][ valueBin ]++;
	}
      else if( spinPattern == 1 )//yellow up, blue down
	{
	  numUL[ YELLOW ][f][ valueBin ]++;
	  numDR[ BLUE   ][f][ valueBin ]++;
	}
      else if( spinPattern == 2 )//yellow down, blue up
	{
	  numDL[ YELLOW ][f][ valueBin ]++;
	  numUR[ BLUE   ][f][ valueBin ]++;
	}
      else if( spinPattern == 3 )//yellow up, blue up
	{
	  numUL[ YELLOW ][f][ valueBin ]++;
	  numUR[ BLUE   ][f][ valueBin ]++;
	}
    }

  return true;
}

void Asymmetry::clearCounts()
{
  for( int beam = 0; beam < NUM_BEAMS; beam++ )
    for( int f = 0; f < NUM_FILL_BINS; f++ )
      for( int i = 0; i < NUM_VALUE_BINS; i++ )
	{
	  numUL[ beam ][f][i] = numUR[ beam ][f][i] = 0;
	  numDL[ beam ][f][i] = numDR[ beam ][f][i] = 0;
	}
}

bool Asymmetry::calculateAsymmetry( const int beam,
				    const int f,
				    const int option )
{
  if( ! isBeamValid( beam ) || !isFillBinValid( f ) )
    {
      cout << "-->Asymmetry::calculateAsymmetry()" << endl;
      return false;
    }
  float p    = polarization [ beam ][f];
  float pErr = polError     [ beam ][f];
  float R    = relLuminosity[ beam ][f];

  for( int i = 0; i < NUM_VALUE_BINS; i++ )
    {
      float UL = (float)numUL[ beam ][f][i];
      float DL = (float)numDL[ beam ][f][i];
      float UR = (float)numUR[ beam ][f][i];
      float DR = (float)numDR[ beam ][f][i];

      if( UL + DL + UR + DR == 0 ) continue;

      if( option == LEFT_LUMINOSITY && UL >= NUM_CUT_OFF && DL >= NUM_CUT_OFF )
	{
	  asymmetry[ beam ][f][i] = ( UL - R * DL ) / ( UL + R * DL );
	  asymError[ beam ][f][i] = 2 * R * UL * DL * sqrt( 1/UL + 1/DL )
	                            / ( ( UL + R * DL ) * ( UL + R * DL ) );
	}
      else if( option == RIGHT_LUMINOSITY 
	       && UR >= NUM_CUT_OFF && DR >= NUM_CUT_OFF )
	{
	  asymmetry[ beam ][f][i] = -( UR - R * DR ) / ( UR + R * DR );
	  asymError[ beam ][f][i] = 2 * R * UR * DR * sqrt( 1/UR + 1/DR )
	                             / ( ( UR + R * DR ) * ( UR + R * DR ) );
	}
      else if( option == SQUARE_ROOT && UL >= NUM_CUT_OFF && DL >= NUM_CUT_OFF 
	       && UR >= NUM_CUT_OFF && DR >= NUM_CUT_OFF )
	{
	  float denominator = sqrt( 1.0 * UL * DR ) + sqrt( 1.0 * DL * UR );

	  asymmetry[ beam ][f][i] = sqrt( UL * DR ) - sqrt( DL * UR );
	  asymmetry[ beam ][f][i] /= denominator;

	  asymError[ beam ][f][i] = sqrt( UL * DR * DL * UR )
	                             * sqrt( 1.0/UL + 1.0/DL + 1.0/UR + 1.0/DR )
	                             / ( denominator * denominator );
	}
      else
	{
	  cout << "Beam " << beam << " fill bin " << f << ", value bin " << i 
	       << " was empty: " << UL << " " << DL << " " << UR << " " << DR 
	       << endl;
	  
	  asymmetry[ beam ][f][i] = 0;
	  asymError[ beam ][f][i] = 1;
	}


      float rawAs = asymmetry[ beam ][f][i];
      asymmetry[ beam ][f][i] /= ( p * accCorrection[i] );
      if( rawAs!= 0 )
	{
	  float error = asymError[ beam ][f][i];
	  asymError[ beam ][f][i] = sqrt( error*error / (rawAs*rawAs)
					    + pErr*pErr / (p*p) );
	  asymError[ beam ][f][i] *= abs( asymmetry[ beam ][f][i] ); 
	}
    }//end looping over value bins

  return true;
}


bool Asymmetry::averageOverFills( const int beam,
				  float *aveAsymmetry,
				  float *aveAsymError )
{
  //Asymmetries that did not have enough counts were set to 0+/-1.  We don't 
  //want to include those blank asymmetries in the average asymmetry calculation

  if( !isBeamValid( beam ) )
    {
      cout << "-->Asymmetry::averageOverFills()" << endl;
      return false;
    }
  for( int i = 0; i < NUM_VALUE_BINS; i++ )//one value (pt) bin at a time
    {
      //First need to find an asymmetry to start with that's not blank
      bool foundStart = false;
      int start = 0;
      while( !foundStart && start < NUM_FILL_BINS )
	if( asymmetry[ beam ][ start ][i] == 0 
	    && asymError[ beam ][ start ][i] == 1 )//blank asymmetry
	  start++;
	else
	  {
	    foundStart = true;
	    aveAsymmetry[i] = asymmetry[ beam ][ start ][i];
	    aveAsymError[i] = asymError[ beam ][ start ][i];
	  }

      for( int f = start + 1; f < NUM_FILL_BINS; f++ )
	{
	  if( asymmetry[ beam ][f][i] == 0  && asymError[ beam ][f][i] == 1 )
	    continue; //skipping blank asymmetry

	  float newErr = 1/( aveAsymError[i]*aveAsymError[i] )
	                 + 1/(asymError[ beam ][f][i]*asymError[ beam ][f][i] );
	  newErr = 1 / sqrt( newErr );

	  aveAsymmetry[i] = aveAsymmetry[i]/( aveAsymError[i]*aveAsymError[i] )
                           + asymmetry[ beam ][f][i]
                             /(asymError[ beam ][f][i]*asymError[ beam ][f][i]);
	  aveAsymmetry[i] *= newErr * newErr;

	  aveAsymError[i] = newErr;
	}// end looping over fills
    }// end looping over value bins

   return true;
}

void Asymmetry::averageOverBeamsAndFills( float *aveAsymmetry,
					  float *aveAsymError )
{
  float yAs[ NUM_VALUE_BINS ], yAsErr[ NUM_VALUE_BINS ];
  float bAs[ NUM_VALUE_BINS ], bAsErr[ NUM_VALUE_BINS ];

  averageOverFills( YELLOW, yAs, yAsErr );
  averageOverFills( BLUE,   bAs, bAsErr );

  for( int i = 0; i < NUM_VALUE_BINS; i++ )
    {

      aveAsymError[i] = 1/( yAsErr[i]*yAsErr[i] ) + 1/( bAsErr[i] * bAsErr[i] );
      aveAsymError[i] = 1 / sqrt( aveAsymError[i] );

      aveAsymmetry[i] = yAs[i] / ( yAsErr[i]*yAsErr[i] ) 
	                   + bAs[i] / ( bAsErr[i] * bAsErr[i] );
      aveAsymmetry[i] *= aveAsymError[i] * aveAsymError[i];
    }
}

float Asymmetry::getAccCorrection( const int valueBin )
{
  if( !isValueBinValid( valueBin ) )
    {
      cout << "-->Asymmetry::getAccCorrection()" << endl;
      return -999;
    }
  else
    return accCorrection[ valueBin ];
}

float Asymmetry::getPolarization ( const int beam, const int f )
{
  if( !isBeamValid( beam ) || !isFillBinValid( f ) )
    {
      cout << "-->Asymmetry:getPolarization()" << endl;
      return -999;
    }
  return polarization[ beam ][f];
}

float Asymmetry::getPolError( const int beam, const int f )
{
  if( !isBeamValid( beam ) || !isFillBinValid( f ) )
    {
      cout << "-->Asymmetry::getPolError()" << endl;
      return -999;
    }
  return polError[ beam ][f];
}

float Asymmetry::getRelLuminosity( const int beam, const int f )
{
  if( !isBeamValid( beam ) || !isFillBinValid( f ) )
    {
      cout << "-->Asymmetry::getRelLuminosity()" << endl;
      return -999;
    }
  return relLuminosity[ beam ][f];
}

int Asymmetry::getNumUL( const int beam, const int f, const int valueBin )
{
  if( !isBeamValid( beam ) || !isFillBinValid( f )
      || !isValueBinValid( valueBin ) )
    {
      cout << "-->Asymmetry::getNumUL()" << endl;
      return -999;
    }
  return numUL[ beam ][f][ valueBin ];
}

int Asymmetry::getNumUR( const int beam, const int f, const int valueBin )
{
  if( !isBeamValid( beam ) || !isFillBinValid( f )
      || !isValueBinValid( valueBin ) )
    {
      cout << "-->Asymmetry::getNumUR()" << endl;
      return -999;
    }
  return numUR[ beam ][f][ valueBin ];
}

int Asymmetry::getNumDL( const int beam, const int f, const int valueBin )
{
  if( !isBeamValid( beam ) || !isFillBinValid( f )
      || !isValueBinValid( valueBin ) )
    {
      cout << "-->Asymmetry::getNumDL()" << endl;
      return -999;
    }
  return numDL[ beam ][f][ valueBin ];
}

int Asymmetry::getNumDR( const int beam, const int f, const int valueBin )
{
  if( !isBeamValid( beam ) || !isFillBinValid( f )
      || !isValueBinValid( valueBin ) )
    {
      cout << "-->Asymmetry::getNumDR()" << endl;
      return -999;
    }
  return numDR[ beam ][f][ valueBin ];
}

float Asymmetry::getAsymmetry( const int beam, const int f, const int valueBin )
{
  if( !isBeamValid( beam ) || !isFillBinValid( f )
      || !isValueBinValid( valueBin ) )
    {
      cout << "-->Asymmetry::getAsymmetry()" << endl;
      return -999;
    }
  return asymmetry[ beam ][f][ valueBin ];
}

float Asymmetry::getAsymError( const int beam, const int f, const int valueBin )
{
  if( !isBeamValid( beam ) || !isFillBinValid( f )
      || !isValueBinValid( valueBin ) )
    {
      cout << "-->Asymmetry::getAsymError()" << endl;
      return -999;
    }
  return asymError[ beam ][f][ valueBin ];
}

void Asymmetry::printCounts()
{
  int ULY[ NUM_VALUE_BINS ], URY[ NUM_VALUE_BINS ];
  int DLY[ NUM_VALUE_BINS ], DRY[ NUM_VALUE_BINS ];
  int ULB[ NUM_VALUE_BINS ], URB[ NUM_VALUE_BINS ];
  int DLB[ NUM_VALUE_BINS ], DRB[ NUM_VALUE_BINS ];

  for( int i = 0; i < NUM_VALUE_BINS; i++ )
    {
      ULY[i] = URY[i] = DLY[i] = DRY[i] = 0;
      ULB[i] = URB[i] = DLB[i] = DRB[i] = 0;

      for( int f = 0; f < NUM_FILL_BINS; f++ )
	{
	  ULY[i] += numUL[ YELLOW ][f][i];
	  URY[i] += numUR[ YELLOW ][f][i];
	  DLY[i] += numDL[ YELLOW ][f][i];
	  DRY[i] += numDR[ YELLOW ][f][i];
	  
	  ULB[i] += numUL[ BLUE ][f][i];
	  URB[i] += numUR[ BLUE ][f][i];
	  DLB[i] += numDL[ BLUE ][f][i];
	  DRB[i] += numDR[ BLUE ][f][i];

	  //check that the counts makes sense
	  if( ULY[i] + DLY[i] != URB[i] + DRB[i] )
	    cout << "Fill bin " << f << " value bin " << i 
		 << " counts in east arm don't make sense " << endl;
	  if( URY[i] + DRY[i] != ULB[i] + DLB[i] )
	    cout << "Fill bin " << f << " value bin " << i 
		 << " counts in east arm don't make sense " << endl;
	}

    }
  //formate is easily copied and pasted into a latex table (in theory)
  cout <<  endl 
       << "p_T[GeV] & ULY  & DLY &  URY &  DRY & ULB & DLB & URB & DRB \\" 
       << endl;
  for( int i = 0; i < NUM_VALUE_BINS; i++ )
    cout << VALUE_BINS[i] << " - " << VALUE_BINS[i + 1] << " & "
	 << ULY[i] << " & " << DLY[i] << " & " 
	 << URY[i] << " & " << DRY[i] << " & " 
	 << ULB[i] << " & " << DLB[i] << " & " 
	 << URB[i] << " & " << DRB[i] << " \\" << endl;
}

void Asymmetry::printNumbersPretty( const int beam, 
				    const int f, 
				    const int valueBin )
{
  if( ! isBeamValid( beam ) || !isFillBinValid( f ) 
      || !isValueBinValid( valueBin ) )
    cout << "-->Asymmetry::printNumbersPretty()" << endl;

  if( beam == YELLOW )
    cout << "Yellow ";
  else if( beam == BLUE )
    cout << "Blue ";
  cout << " " << f << " " << valueBin << " " << polarization[ beam ][f] << " " 
       << polError[ beam ][f] << " " << relLuminosity[ beam ][f] << " " 
       << numUL[ beam ][f][ valueBin ] << " " << numUR[ beam ][f][ valueBin ] 
       << " " << numDL[ beam ][f][ valueBin ]<< " " 
       << numDR[ beam ][f][ valueBin ] << endl;

  cout << endl;
}

void Asymmetry::printNumbersDetailed( const int beam, 
				    const int f, 
				    const int valueBin )
{
  if( ! isBeamValid( beam ) || !isFillBinValid( f ) 
      || !isValueBinValid( valueBin ) )
    cout << "-->Asymmetry::printNumbersDetailed()" << endl;

  if( beam == YELLOW )
    cout << "Yellow, ";
  else if( beam == BLUE )
    cout << "Blue,   ";
  cout << " fill bin = " << f << ", value bin = " << valueBin 
       << " polarization = " << polarization[ beam ][f] << " +/- " 
       << polError[ beam ][f] << ", relative luminosity =  " 
       << relLuminosity[ beam ][f] << ", UL = " << numUL[ beam ][f][ valueBin ] 
       << ", UR = " << numUR[ beam ][f][ valueBin ] << ", DL = " 
       << numDL[ beam ][f][ valueBin ] << ", DR " 
       << numDR[ beam ][f][ valueBin ] << endl;

  cout << endl;
}
