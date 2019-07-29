
void makepng() {

	TFile *f1 = TFile::Open("ohfe.root");
	TIter next(f1->GetListOfKeys());
	TKey *key;
	TCanvas c1;
	char name[500];
	bool log = 0;

	while ((key = (TKey*)next())) {
		TClass *cl = gROOT->GetClass(key->GetClassName());
		if (!cl->InheritsFrom("TH1")) continue;
		TH1 *h = (TH1*)key->ReadObj();
		//*name = ("%s", h->GetName());
		sprintf(name, "%s.png", h->GetName() );
		printf("name is %s ", name);
		//cout << name << endl;
		gStyle->SetStatX(0.9);
		gStyle->SetStatY(0.9);
		//h->SetTitle("");
		if (cl->InheritsFrom("TH2")) {   
			h->Draw("colz");
			c1.Print(name);
		}
		else {
			if (log) {
				c1.SetLogy();
				h->Draw();
				c1.Print(name);
			}
			else {
				h->Draw();
				c1.Print(name);
			}
		}
	}	
}
