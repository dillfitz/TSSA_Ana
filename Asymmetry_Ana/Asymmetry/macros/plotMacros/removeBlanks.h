//In the Asymmetry classis an asymmetry did not have enough counts itis set to 
//0 +/- 1.  This function removes these blank asymmetries from the average 
//asymmetry calculations

#ifndef _REMOVE_BANKS_H_
#define _REMOVE_BANKS_H_

TGraphErrors *removeBlanks( TGraphErrors *graph )
{
  int numGroups = graph->GetN();
  double *asymmetry = graph->GetY();
  double *asymmetryErr = graph->GetEY();

  int numBlanks = 0;
  for( int groupIndex = 0; groupIndex < numGroups; groupIndex++ )
    if( asymmetryErr[ groupIndex ] == 1 ) 
      numBlanks++;

  //make arrays without these the blanks. 
  int num = numGroups - numBlanks;
  double *newGroupIndexes = new double[ num ];
  double *newAsymmetry    = new double[ num ];
  double *newAsymmetryErr = new double[ num ];

  int skips = 0;
  for( int k = 0; k < numGroups; k++ )
  //loop through the old arrays
    {
      if( asymmetryErr[k] == 1 )
	skips++;
      else
	{
	  newGroupIndexes[ k - skips ] = k  - skips + 1;
	  newAsymmetry[ k - skips ] = asymmetry[k];
	  newAsymmetryErr[ k - skips ] = asymmetryErr[k];
	}

    }
  graph = new TGraphErrors( num, newGroupIndexes, newAsymmetry,
			    0,newAsymmetryErr );

  return graph;
}


#endif /* _REMOVE_BANKS_H_ */
