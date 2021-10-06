//Inputs two TGraphAsymmErrors graphs and performs a T test between them for 
//either 0% or 100% correlation.  Graphs must have the same number of points
//Returns a TGraph that's ready to plot this results of this T test

#ifndef _TTESTNEW_H_
#define _TTESTNEW_H_

#include <iostream>
#include <cmath>
using namespace std;

#include "TGraphAsymmErrors.h"
#include "TGraph.h"

TGraph *ttestNew( TGraphAsymmErrors *graph1, TGraphAsymmErrors* graph2,
	       int correlation )
{
  int numPoints1 = graph1->GetN();
  int numPoints2 = graph2->GetN();
  if( numPoints1 != numPoints2 )
    cout << "ttest.h: Error: graphs should have same number of points! " 
	 << endl;

  double *pt = graph1->GetX();
  double *asymmetry1    = graph1->GetY();
  double *asymmetryErr1 = graph1->GetEYhigh();
  double *asymmetry2    = graph2->GetY();
  double *asymmetryErr2 = graph2->GetEYhigh();

  double *ttestValues = new double[ numPoints1 ];
  for( int i = 0; i < numPoints1; i++ )
    {
      ttestValues[i] = asymmetry1[i] - asymmetry2[i];
      ttestValues[i] /= sqrt( fabs( asymmetryErr1[i]*asymmetryErr1[i]
				  + asymmetryErr2[i]*asymmetryErr2[i] 
                                  - 2.*correlation*asymmetryErr1[i]*asymmetryErr2[i] ) );
      
      cout << asymmetry1[i] << " +/- " << asymmetryErr1[i] << ", " 
           << asymmetry2[i] << " +/- " << asymmetryErr2[i] << ", t: "
           << ttestValues[i] << endl;
    }
  //  cout << endl;

  //find range... because ROOT is tedious
  double *ptCenters = graph1->GetX();
  double xMin = ptCenters[0] - graph1->GetErrorXlow(0) - 1.5;
  double xMax = ptCenters[ numPoints1 - 1 ] 
                 + graph1->GetErrorXhigh( numPoints1 - 1 ) + 1.5;

  TGraph *ttestGraph = new TGraph( numPoints1, pt, ttestValues );
  ttestGraph->SetMarkerColor( kBlack );
  ttestGraph->SetMarkerStyle( kFullCircle );
  ttestGraph->GetXaxis()->SetLimits( xMin, xMax );
  ttestGraph->SetTitle( "; p_{T} [GeV];" );
  return ttestGraph;
}

#endif /* _TTESTNEW_H_ */
