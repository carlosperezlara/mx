void SetInitialParameters(TF1 *t) {
  t->SetParameter(0,12);  t->SetParLimits(0,9,30);
  t->SetParameter(1,2);   t->SetParLimits(1,1,8);
  t->SetParameter(2,0.05); t->SetParLimits(2,0.001,0.1);
  t->SetParameter(3,0.05); t->SetParLimits(3,0.00001,0.1);
  t->SetParameter(4,0.1); t->SetParLimits(4,0.0,3.0);

}

int adcextract() {
  TFile *file = new TFile("merged.root");
  TH2F *adc = (TH2F*) file->Get("AdcHigh");
  TH1D *pro;
  //  for(int key=0; key!=49152; ++key) {
  //for(int key=0; key!=128; ++key) {
  for(int key=1984; key!=1985; ++key) {
    int sen = key/128;
    gSystem->Exec( Form("mkdir -p out/SEN%03d/",sen) );
    pro = adc->ProjectionY(Form("ADC_%d",key),key+1,key+1);
    pro->Sumw2();
    pro->Draw("E");
    pro->SaveAs( Form("out/SEN%03d/H%05d.root",sen,key), "ROOT" );
  }
  return 0;
}
