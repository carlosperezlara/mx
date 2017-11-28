int ped() {
  TString aa = "tar -cf high.tar ";
  for(int sen=0; sen!=192; ++sen) {
    aa+=Form("out/SEN%03d/fit ",sen);
    aa+=Form("out/SEN%03d/fitped ",sen);
  }
  cout << aa.Data() << endl;
  gSystem->Exec( aa.Data() );
  return 0;
}
