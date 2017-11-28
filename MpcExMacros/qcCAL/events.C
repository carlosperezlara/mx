int events(int run=0) {
  gStyle->SetOptStat(0);
  TFile *file = new TFile( Form("se-%d.root",run) );
  TH1F *ev = (TH1F*) file->Get("Test_Events");
  cout << run << " " << ev->GetBinContent(4) << endl;
  ofstream ofile( Form("evs.dat"), ios::out | ios::app );
  ofile << run << " " << ev->GetBinContent(4) << endl;
  ofile.close();

  return 0;
}
