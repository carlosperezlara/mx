void SetInitialParameters(TF1 *t) {
  t->SetParameter(0,12);  t->SetParLimits(0,9,30);
  t->SetParameter(1,2);   t->SetParLimits(1,1,8);
  t->SetParameter(2,0.05); t->SetParLimits(2,0.001,0.1);
  t->SetParameter(3,0.05); t->SetParLimits(3,0.00001,0.1);
  t->SetParameter(4,0.1); t->SetParLimits(4,0.0,3.0);

}

int adcextractsensor(int run) {
  TFile *file = new TFile( Form("se-%d.root",run) );
  TH2F *adc = (TH2F*) file->Get("EnergyHigh");
  TH1D *pro;
  for(int sen=0; sen!=192; ++sen) {
    int keyinit = sen*128;;
    int bkey = keyinit +1;
    gSystem->Exec( Form("mkdir -p out/SEN%03d/",sen) );
    pro = adc->ProjectionY(Form("SEN_%d",sen),bkey,bkey+127);
    pro->Sumw2();
    pro->Draw("E");
    pro->SaveAs( Form("out/SEN%03d/H%05d.root",sen,run), "ROOT" );
  }
  return 0;
}
