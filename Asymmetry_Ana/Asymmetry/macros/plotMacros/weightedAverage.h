//Takes in two inputs with statistical errors, and outputs the weighted average

#ifndef _WEIGHTED_AVERAGE_H_
#define _WEIGHTED_AVERAGE_H_

bool weightedAverage( const float input1, const float inputErr1, 
		      const float input2, const float inputErr2, 
		      float &output, float &outputErr )
{
  if( input1 == 0 || inputErr1 == 0 || input2 == 0 || inputErr2 == 0 ) 
    {
      cout << "Nope! " << endl;
      return false;
    }
  else
    {
      outputErr = 1/( inputErr1*inputErr1 ) + 1/( inputErr2*inputErr2 );
      outputErr = 1/sqrt( outputErr );

      output = input1/( inputErr1*inputErr1 ) + input2/( inputErr2*inputErr2 );
      output *= outputErr*outputErr;
      return true;
    }

}

bool weightedAverage( const double input1, const double inputErr1, 
		      const double input2, const double inputErr2, 
		      double &output, double &outputErr )
{
  if( input1 == 0 || inputErr1 == 0 || input2 == 0 || inputErr2 == 0 ) 
    {
      cout << "Nope! " << endl;
      return false;
    }
  else
    {
      outputErr = 1/( inputErr1*inputErr1 ) + 1/( inputErr2*inputErr2 );
      outputErr = 1/sqrt( outputErr );

      output = input1/( inputErr1*inputErr1 ) + input2/( inputErr2*inputErr2 );
      output *= outputErr*outputErr;
      return true;
    }

}

#endif /* _WEIGHTED_AVERAGE_H_ */
