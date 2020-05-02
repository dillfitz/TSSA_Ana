#include <cmath>
#include "../../Constants.h"

void CompareFormulas_Systematics()
{
  const int nbins = NUM_VALUE_BINS;
  double x_lumi[nbins], y_lumi[nbins], x_sqrt[nbins], y_sqrt[nbins], diff[nbins];
  TFile *file = TFile::Open( "dataFiles/ohfe_AN.root");
  for (int i=0; i<nbins; i++)
  {
    lumi->GetPoint(i, x_lumi[i], y_lumi[i]);
    sqrt0->GetPoint(i, x_sqrt[i], y_sqrt[i]);
    diff[i] = y_lumi[i] - y_sqrt[i];
    cout << diff[i] << endl;
  }  
}
