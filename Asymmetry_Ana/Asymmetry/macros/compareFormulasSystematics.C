#include <cmath>
#include "../../Constants.h"

void compareFormulasSystematics()
{
  const int nbins = NUM_VALUE_BINS;
  double x_lumi[nbins], y_lumi[nbins], x_sqrt[nbins], y_sqrt[nbins], diff[nbins];
  TFile *file = TFile::Open( "dataFiles/ohfe_AN.root");
  for (int i=0; i<nbins; i++)
  {
    lumi->GetPoint(i, x_lumi[i], y_lumi[i]);
    sqrt0->GetPoint(i, x_sqrt[i], y_sqrt[i]);
    diff[i] = y_lumi[i] - y_sqrt[i];
    cout << "<pT> : " << x_lumi[i] << endl; 
    cout << "lumi result : " << y_lumi[i] << endl;
    cout << "sqrt result : " << y_sqrt[i] << endl;
    cout << "lumi - sqrt sys error :" << diff[i] << endl;
    cout << endl;
  }  
}
