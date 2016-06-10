void Integrals(int run=454798) {
  gStyle->SetOptStat(0);
  TFile *file = new TFile( Form("%d.root",run) );
  //file->ls();
  TH2F *hist1 = (TH2F*) file->Get("mxSubsysReco_adchi");
  TH2F *hist2 = (TH2F*) file->Get("mxSubsysReco_adclo");
  int b8 = hist1->GetYaxis()->FindBin(8.0);
  int b14 = hist1->GetYaxis()->FindBin(14.0);
  int b18 = hist1->GetYaxis()->FindBin(18.0);
  int b30 = hist1->GetYaxis()->FindBin(30.0);
  int b35 = hist1->GetYaxis()->FindBin(35.0);
  int b80 = hist1->GetYaxis()->FindBin(80.0);
  int c14 = hist2->GetYaxis()->FindBin(14.0);
  int c80 = hist2->GetYaxis()->FindBin(80.0);

  ofstream ofile( Form("%d.dat",run) );
  for(int k=0; k!=49152; ++k) {
    float int0 = hist1->Integral( k+1, k+1, b8, b14 )/(b14-b8);
    float int1 = hist1->Integral( k+1, k+1, b18, b30 )/(b30-b18);
    float int2 = hist1->Integral( k+1, k+1, b35, b80 )/(b80-b35);
    float int3 = hist2->Integral( k+1, k+1, c14, c80 )/(c80-c14);
    ofile << Form("%d %.3f %.3f %.3f %.3f",k,int0,int1,int2,int3) << endl;
    if(k%1000==0) cout << " processing " << k << endl;
  }
  ofile.close();
  cout << "[DONE]" << endl;
}
