#include "TFile.h"
#include "TF1.h"
#include "TH1.h"

using namespace std;

void fit_fnp()
{
	//gStyle->SetOptStat(1);
    //gStyle->SetOptFit(1111);
	const int nbins = 4;
	double bins[nbins+1] = {1.5, 1.8, 2.1, 2.7, 5.0};

    TFile *infile = TFile::Open("normalizations.root");

	h_oldfnp_conveto->Rebin(nbins,"h_fnp_1p5to5",bins);

    TF1 *f1 = new TF1("f1","[0]+[1]*sqrt(x)+[2]*log(x)",1.,5.);
	TF1 *g1 = new TF1("g1","([0]*x)/(sqrt(1+[1]*x*x))",1.,5.);

    g1->SetLineColor(kBlue);
    
	h_fnp_1p5to5->Fit("f1");
	h_fnp_1p5to5->Fit("g1");

	cout << "f1 sys bin 1 : " << f1->Eval(1.161) << endl;
	cout << "f1 sys bin 2 : " << f1->Eval(1.398) << endl;

	cout << "g1 sys bin 1 : " << g1->Eval(1.161) << endl;
	-cout << "g1 sys bin 2 : " << g1->Eval(1.398) << endl;
}


