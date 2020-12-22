#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif


// This macro is to generate decay dielectrons from J/psi w/o polarization.
// Study effects of pt distribution, ERT efficiency and detector acceptance.      

bool do_kaplan= true;
bool do_smear = false;
bool save_root = true;
int nAccEvents = 10000000;
void jpsidecay(int group=0, int process=0) {


  TFile* outfile = new TFile("jpsidecay.root","recreate");

  TNtuple* particle = new TNtuple("particle", "J/psi to e+e-", "event:pnum:xvtx:yvtx:zvtx:px1:pt1:pz1:E1:theta1:phi1:rap1:eta1:px2:pt2:pz2:E2:theta2:phi2:rap2:eta2:openang:pt:phi");

  TPythia6Decayer* mydec = TPythia6Decayer::Instance();
  mydec->SetDecayTableFile("jpsidecay.dec");
  mydec->ReadDecayTable();
  //mydec->WriteDecayTable();

  TRandom2* rnd = new TRandom2();
  rnd->SetSeed();
  gRandom->SetSeed();

  TClonesArray* a = new TClonesArray("TParticle", 100);
  float ntp[99];
  int inacccount=0;
  int evtcount=0;

  ofstream outTextFile;
  char fname[256];
  sprintf(fname,"oscar_%d%d.txt",group,process);	
  outTextFile.open(fname, ios::out);

  if (!outTextFile) {
    cerr << "ERROR: Can't open output text file." << endl;
    exit(1);
  }

  outTextFile << "# OSC1999A" << endl;
  outTextFile << "# final_id_p_x" << endl;
  outTextFile << "# J/psi with flat pT distribution 20000 events" << endl;
  outTextFile << "#" << endl;

//---------------------------------------------------------------------
  TF1* fkaplan = new TF1("fkaplan","[0]*x*pow(1.+(x/[1])*(x/[1]),[2])",0.,20.);
    fkaplan->SetParameters(28.7, 3.41, -4.6); // ppg097 -- looks like this was from 200 GeV data

  /* 
   * fermi fn for ERT efficiency, applied to dielectrons that end up in detector acceptance.
   * accept or reject 
   */
  TF1* ffermi = new TF1("ffermi","[0]/(exp(-((x-[2])/[1]))+1.)",0.,20.0);
    ffermi->SetParameters(0.99,0.3,1.4);

//for(int j=0; j<100000; j++) { // phase space study
//for(int j=0; j<20000; j++) { // pisa input
//for(int j=0; j<200; j++) { // test
while(inacccount<nAccEvents){
  ++evtcount;
//  if (evtcount%200==0) cout<<"event " <<evtcount<<endl;
  if (inacccount%1000 == 1) cout<<"inacc event "<<inacccount<<endl;
  //float evtvtxz = hdiamond->GetRandom();
  float evtvtxz = rnd->Uniform(-30.,30.);  // uniform vertex distribution
  evtvtxz = evtvtxz *1.0e+13; // convert to Fermi

  float raplim = 0.5;
  float ptlim1 = 0.;
  float ptlim2 = 13.;
  float pt;
  if (do_kaplan){ pt = fkaplan->GetRandom();}
  else {pt = rnd->Uniform(ptlim1,ptlim2);}
  float eta = rnd->Uniform(-raplim,raplim);
  float theta = 3.141592654/2. + (exp(2.*eta)-1.)/(exp(2.*eta)+1.);
  float phi = rnd->Uniform(0.,2.*3.141592654);
  //cout << "RAND: " << pt << " " << eta << " " << phi << " " << theta << endl;

  float px = pt*cos(phi);
  float py = pt*sin(phi);
  float pp = pt/sin(theta);
  float pz = pp*cos(theta);
  float mass = 3.09688;
  float ee = sqrt(mass*mass+px*px+py*py+pz*pz);
  TLorentzVector* vec = new TLorentzVector(px, py, pz, ee);
  //cout << "PP: " << px << " " << py << " " << pz << " " << ee << " " << sqrt(px*px+py*py) << endl;

  mydec->Decay(443,vec);

  int nnn = mydec->ImportParticles(a);
  delete vec;
  float px1; float py1; float pz1;
  float px2; float py2; float pz2;
  float prob; float pt1; float pt2;
  float ert1; float ert2;
  float smear = 0.02;
  bool ok1=false;
  bool ok2=false;
  int partcount=0;

  prob = 0.; ert1 = false; ert2 = false;
  for(int i=0; i<nnn; i++) {
    TParticle* part = (TParticle*)a->UncheckedAt(i);
    //cout << part->Pt() << " " << part->GetPdgCode() << endl;
    if(part->GetPdgCode()==-11) { // positron
      ntp[0] = (float)evtcount;
      ntp[1] = 2;
        partcount++;
        px1 = part->Px();
        py1 = part->Py();
        pz1 = part->Pz();
      if(do_smear){
        px1 = px1*(1.0+rnd->Gaus(0.0,smear));
        py1 = py1*(1.0+rnd->Gaus(0.0,smear));
        pz1 = pz1*(1.0+rnd->Gaus(0.0,smear));
      }
      ntp[5] = px1;
      ntp[6] = py1;
      pt1 = sqrt(px1*px1 + py1*py1);
      if (save_root) ntp[6]= pt1; 
      prob = ffermi->Eval(pt1);
      ert1 = ( prob > rnd->Uniform(0.,1.));
// 	cout<<"prob "<<prob<<" ert " <<ert1<<endl;
      ntp[7] = pz1;
      ntp[8] = sqrt(px1*px1 + py1*py1 + pz1*pz1 + mass*mass);
      ntp[2] = 0.;
      ntp[3] = 0.;
      ntp[4] = evtvtxz;
      ntp[9] = part->Theta();
      ntp[10] = part->Phi();
      ntp[11] = part->Y();
      ntp[12] = part->Eta();
      if(fabs(part->Eta())<0.35) {
	ok1=true;
//	if (in_phi_acceptance(ntp[10]))
//	ok1=true;
//	else ok1=false;
      }
//	ok1=true;
    } 

    if(part->GetPdgCode()==11) { // electron
        partcount++;
	px2 = part->Px();
	py2 = part->Py();
	pz2 = part->Pz();
      if(do_smear){
        px2 = px2*(1.0+rnd->Gaus(0.0,smear));
        py2 = py2*(1.0+rnd->Gaus(0.0,smear));
        pz2 = pz2*(1.0+rnd->Gaus(0.0,smear));
      }
      ntp[13] = px2;
      ntp[14] = py2;
      pt2 =  sqrt(px2*px2 + py2*py2);
      if (save_root) ntp[14] = pt2;
      prob = ffermi->Eval(pt2);
      ert2 = (prob > rnd->Uniform(0.,1.));
      ntp[15] = pz2;
      ntp[16] = sqrt(px2*px2 + py2*py2 + pz2*pz2 +mass*mass);
      ntp[17] = part->Theta();
      ntp[18] = part->Phi();
      ntp[19] = part->Y();
      ntp[20] = part->Eta();
      if(fabs(part->Eta())<0.35) {
	ok2=true;	
//	if (in_phi_acceptance(ntp[18]))
//	ok2=true;
//	else ok2=false;
      }
//	ok2=true;
    }
  }
//cout<<"ert1 "<<ert1<<" ert2 "<<ert2<<endl; 
  // bool samearm = !do_same_arm || (do_same_arm && in_same_arm(ntp[10],ntp[18])); 
  //bool erttrig = !do_ert || (do_ert &&ert1&&ert2 );
//cout<<"in same arm "<<samearm<<endl;
//  if(ok1 && ok2 && samearm && erttrig) { // if detector acceptance
  if(ok1 || ok2) { // if detector acceptance
 
    float MASS = 0.000511;
    float P1[3]; P1[0]=ntp[5]; P1[1]=ntp[6]; P1[2]=ntp[7];
    float P2[3]; P2[0]=ntp[13]; P2[1]=ntp[14]; P2[2]=ntp[15];
    float beamP[3]; beamP[0] = 0.; beamP[1] = 0., beamP[2] = 255.;
    float P1sq = P1[0]*P1[0]+P1[1]*P1[1]+P1[2]*P1[2];
    float P2sq = P2[0]*P2[0]+P2[1]*P2[1]+P2[2]*P2[2];
    float P1dotP2= P1[0]*P2[0]+P1[1]*P2[1]+P1[2]*P2[2];
    if(P1sq>0. && P2sq>0.) 
      ntp[21] = P1dotP2/(sqrt(P1sq)*sqrt(P2sq));// openang cos(beta)
    ntp[22] = pt; 
    ntp[23] = phi;

    //ntp[22] = sqrt(pow(px1+px2,2)+pow(py1+py2,2));
    //float phi = atan2((py1+py2),(px1+px2));
    //if (phi>0)
    //  ntp[23] = phi;
    //if (phi<0)
    //  ntp[23] = phi + 2*M_PI;
    //	cout<<"pt before "<<pt<<" after "<<ntp[28]<<endl;
    inacccount++;
    particle->Fill(ntp);

    //outTextFile << "2 0" << endl;
    outTextFile << "0 2" << endl;
    outTextFile << " 0 " << 
      -11 << 
      " 0 " << 
      ntp[5] << " " <<    
      ntp[6] << " " <<    
      ntp[7] << " " <<    
      ntp[8] << " " <<    
      " 0.000511 0 0 " << evtvtxz << " 0" <<    
      endl;
    outTextFile << " 1 " <<
      11 <<
      " 0 " <<
      ntp[13] << " " <<
      ntp[14] << " " <<
      ntp[15] << " " <<
      ntp[16] << " " <<
      " 0.000511 0 0 " << evtvtxz << " 0" <<
      endl;
    outTextFile << "0 0" << endl;
  }
}

cout << "total generated: "<< evtcount << endl;
cout << "in acceptance: " << inacccount << endl;

particle->Write();
outfile->Close();

  outTextFile.close();

}
float invMass(float MASS0,float P0[3],float MASS1,float P1[3]){
	float etot = E(MASS0,P0) + E(MASS1,P1);
	float invmass = sqrt(etot*etot
			-(P0[0]+P1[0])*(P0[0]+P1[0])
			-(P0[1]+P1[1])*(P0[1]+P1[1])
			-(P0[2]+P1[2])*(P0[2]+P1[2])
			);	
	return invmass;
}
float E(float MASS, float P[3]){
	float energy = sqrt(MASS*MASS+P[0]*P[0]+P[1]*P[1]+P[2]*P[2]);
	return energy;
}
float pT(float P[3]){
	float pt = sqrt(P[0]*P[0]+P[1]*P[1]);
	return pt;
}
bool in_phi_acceptance(float phi){

	if (phi>2.0 && phi<4.0) return true;
	return false;
}
bool in_same_arm(float phi1, float phi2){
	float diffphi= phi2-phi1;
	if (diffphi>M_PI) diffphi -= 2*M_PI;
	else if (diffphi<-M_PI) diffphi += 2*M_PI;

	//cout<<"phi1 "<<phi1<<" phi2 "<<phi2<< " diff "<<diffphi<<endl;
	return  (fabs(diffphi)< M_PI/2.);
}

