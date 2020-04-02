//Draws the asymmetry vs fill graphs, fits it to a constant, outputs the fit 
//value and also updates the chi squared histogram

#ifndef _DRAW_GRAPH_H_
#define _DRAW_GRAPH_H_

void drawGraph( TGraphErrors *graph, 
		const int ptBin, const int beam, const int option,
		double &fitValue, double &fitValueErr, TH1D* chi2Histo )
{
  TString graphTitle = BEAM_NAMES[ beam ];
  graphTitle += OPTION_NAMES[ option ];
  graphTitle += "; Fill Index; ";
  graph->SetTitle( graphTitle );

  TF1 *fit = new TF1( "fit", "[0]" );
  graph->Fit( "fit", "Q" );
  graph->Draw("A*" );

  fitValue = fit->GetParameter( 0 );
  fitValueErr = fit->GetParError( 0 );

  double chi2 = fit->GetChisquare();
  int ndf = fit->GetNDF();
  double chi2Reduce = chi2/ndf;
  chi2Histo->Fill( chi2Reduce );
}

#endif /* _DRAW_GRAPH_H_ */
