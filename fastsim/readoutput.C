{
  const int nevents=100;
  TFile *file = new TFile("G4beamline.root");
  TNtuple *det;
  vector<float> key[nevents];
  vector<float> sgn[nevents];
  vector<float> x[nevents];
  vector<float> y[nevents];

  float evnum, evsgn, evx, evy;
  for(int i=24576; i!=49152; ++i) {
    det = (TNtuple*) file->Get( Form("Detector/Si%05d",i) );
    if(!det) continue;
    if(det->GetEntries()<1) continue;
    det->SetBranchAddress("EventID",&evnum);
    det->SetBranchAddress("Edep",&evsgn);
    det->SetBranchAddress("x",&evx);
    det->SetBranchAddress("y",&evy);
    for(int n=0; n!=det->GetEntries(); ++n) {
      det->GetEntry(n);
      if(evnum>nevents-1) continue;
      if(evsgn<0.01) continue;
      int ev = evnum;
      key[ev].push_back(i);
      sgn[ev].push_back(evsgn);
      x[ev].push_back(evx);
      y[ev].push_back(evy);
    }
  }
  ofstream ofile("input.dat");
  for(int i=0; i!=nevents; ++i) {
    int n = key[i].size();
    if(n<1) continue;
    ofile << n << endl;
    for(int j=0; j!=n; ++j) {
      ofile << key[i].at(j) << " " << sgn[i].at(j);
      //ofile << " " << x[i].at(j) << " " << y[i].at(j);
      ofile << endl;
    }
  }
  ofile.close();
}
