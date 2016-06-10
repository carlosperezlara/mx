#include "../loadbkeys.C"

void makebadsignal(int run=454798) {
  loadbkeys();
  TH1F *S = new TH1F( "S", "S", 100,0,10);

  ofstream ofile( "../badkeys.lowsignal.dat" );
  ifstream ifile( Form("%d.dat",run) );
  for(int k=0; k!=49152; ++k) {
    float int0, int1, int2, int3;
    ifile >> k >> int0 >> int1 >> int2 >> int3;
    if(fBad[k]<0) continue;
    S->Fill( int0 );
    if(int0<0.1) ofile << k << endl;
  }
  ifile.close();
  ofile.close();

  TCanvas *main = new TCanvas();
  S->Draw("col");

  return;
}
