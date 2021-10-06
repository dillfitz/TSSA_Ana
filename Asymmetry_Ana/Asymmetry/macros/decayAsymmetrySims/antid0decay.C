#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif


// This macro is to generate decay posirons and electrons from D0 and D0bar.
// Study effects of pt distribution and detector acceptance.      

bool do_smear = false;
int nAccEvents = 10000000;
void antid0decay(int group=0, int process=0) {


  TFile* outfile = new TFile("dataFiles/antid0decay.root","recreate");

  TNtuple* d0tree = new TNtuple("d0bar", "D0bar to e", "event:pnum:xvtx:yvtx:zvtx:px1:pt1:pz1:E1:theta1:phi1:rap1:eta1:ePID:pt:phi");

  TPythia6Decayer* mydec = TPythia6Decayer::Instance();
  mydec->SetDecayTableFile("d0decay.dec");
  mydec->ReadDecayTable();
  //mydec->WriteDecayTable();

  TRandom2* rnd = new TRandom2();
  rnd->SetSeed();
  gRandom->SetSeed();

  TClonesArray* a = new TClonesArray("TParticle", 100);
  float ntpD0[99];
  float ntpD0bar[99];
  int inacccount=0;
  int evtcount=0;

//---------------------------------------------------------------------
  TF1* f_d0_spectrum = new TF1("f_d0_spectrum", "2*TMath::Pi()*x*([0]/TMath::Power((TMath::Exp(-[1]*x-[2]*x*x)+(x/[3])),[4]))", 0.0, 25.0);
  f_d0_spectrum->SetParameters(.0238357, 0.398803, 0.0784526, 1.86030, 7.56442); // From PPG223 Figure X -- may need an additional norm factor for lumi?? probs not since we can simulate arbitrary number of evets.

  /* 
   * fermi fn for ERT efficiency, applied to dielectrons that end up in detector acceptance.
   * accept or reject 
   */
  TF1* ffermi = new TF1("ffermi","[0]/(exp(-((x-[2])/[1]))+1.)",0.,20.0);
    ffermi->SetParameters(0.99,0.3,1.4);

while(inacccount<nAccEvents){
  // cout << "---------------------- gen event # " << evtcount << " ---------------------" <<  endl;
  ++evtcount;
  if (inacccount%1000 == 1) cout<<"inacc event "<<inacccount<<endl;
  float evtvtxz = rnd->Uniform(-30.,30.);  // uniform vertex distribution
  evtvtxz = evtvtxz *1.0e+13; // convert to Fermi

  float raplim = 0.5;
  //float ptlim1 = 0.0;
  //float ptlim2 = 15.0;
  float pt;
  pt = f_d0_spectrum->GetRandom();
  // else {pt = rnd->Uniform(ptlim1,ptlim2);}
  float eta = rnd->Uniform(-raplim,raplim);
  float theta = 3.141592654/2. + (exp(2.*eta)-1.)/(exp(2.*eta)+1.);
  float phi = rnd->Uniform(0.,2.*3.141592654);

  float px = pt*cos(phi);
  float py = pt*sin(phi);
  float pp = pt/sin(theta);
  float pz = pp*cos(theta);
  float mass = 1.86484;
  float ee = sqrt(mass*mass+px*px+py*py+pz*pz);
  TLorentzVector* vec = new TLorentzVector(px, py, pz, ee);

  mydec->Decay(-421,vec);

  int nnn = mydec->ImportParticles(a);
  delete vec;
  float px1; float py1; float pz1;
  float prob; float pt1; 
  float ert1; 
  float smear = 0.02;
  bool ok1=false;
  int partcount=0;

  prob = 0.; ert1 = false;
  int epCount = 0;
  for(int i=0; i<nnn; i++) {
    TParticle* part = (TParticle*)a->UncheckedAt(i);
    // cout << part->Pt() << " " << part->GetPdgCode() << endl;
    if(part->GetPdgCode()==11) { // electron -- corresponds to antiD0 tree
      epCount++;
      // cout << " posironix " << epCount << endl;
      if (epCount!=1) {continue;}
      // cout << "apparently the positron " << part->Pt() << " " << part->GetPdgCode() << endl;
      ntpD0[0] = (float)evtcount;
      ntpD0[1] = 2;
        partcount++;
        px1 = part->Px();
        py1 = part->Py();
        pz1 = part->Pz();
      if(do_smear){
        px1 = px1*(1.0+rnd->Gaus(0.0,smear));
        py1 = py1*(1.0+rnd->Gaus(0.0,smear));
        pz1 = pz1*(1.0+rnd->Gaus(0.0,smear));
      }
      ntpD0[5] = px1;
      ntpD0[6] = py1;
      pt1 = sqrt(px1*px1 + py1*py1);
      ntpD0[6]= pt1; 
      prob = ffermi->Eval(pt1);
      ert1 = ( prob > rnd->Uniform(0.,1.));
      ntpD0[7] = pz1;
      ntpD0[8] = sqrt(px1*px1 + py1*py1 + pz1*pz1 + mass*mass);
      ntpD0[2] = 0.;
      ntpD0[3] = 0.;
      ntpD0[4] = evtvtxz;
      ntpD0[9] = part->Theta();
      ntpD0[10] = part->Phi();
      ntpD0[11] = part->Y();
      ntpD0[12] = part->Eta();
      ntpD0[13] = part->GetPdgCode();
      //ok1 = false;
      if(fabs(part->Eta())<0.35) {
	ok1=true;
      }
    } 
  }

  if(ok1) { // if detector acceptance
 
    float MASS = 0.000511;
    ntpD0[14] = pt;
    ntpD0[15] = phi;

    inacccount++;
    d0tree->Fill(ntpD0);

  }
}

cout << "total generated: "<< evtcount << endl;
cout << "in acceptance: " << inacccount << endl;

d0tree->Write();
outfile->Close();


}
