int nruns = 0;
TH1D *pro[150];

float mpv = 15;
Double_t landau(Double_t *x, Double_t *p) {
  if(x[0]<3) return 0;
  return (TMath::Exp(-0.4*x[0]) + TMath::Landau(x[0],mpv,3));
}

void loadfake(int key) {
  //TF1 *fun = new TF1("LAN","TMath::Landau(x,25,3)");
  TF1 *fun = new TF1("LAN",landau);
  nruns = 100;
  for(int i=0; i!=nruns; ++i) {
    pro[i] = new TH1D(Form("FAKE_ADC_%d_%d",key,i), Form("FAKE_ADC_%d_%d",key,i),
		      100, -10.5, 89.5);
    mpv = 15 - i*1.0/100.0;
    pro[i]->FillRandom("LAN",500000);
    pro[i]->Sumw2();
    pro[i]->Scale(1.0/pro[i]->Integral());
  }
}

void loaddata(int key) {
  TFile *file;
  ifstream fin("runs.dat.final");
  int run;
  nruns = 0;
  for(;;) {
    fin >> run;
    if(!fin.good()) break;
    file = TFile::Open( Form("se-%d.root",run) );
    TH2F *adc = (TH2F*) file->Get("mxDet_Hadc0");
    TH1F *vtxz = (TH1F*) file->Get("mxReco_vz");
    pro[nruns] = adc->ProjectionY(Form("ADC_%d_%d",key,run),key+1,key+1);
    pro[nruns]->Sumw2();
    pro[nruns]->Scale(1.0/pro[nruns]->Integral());
    cout << run << endl;
    ++nruns;
    //if(nruns>2) break;
  }
}

int adcinvestigate(int key=954) {
  if(0) {
    loaddata(key);
  } else {
    loadfake(key);
  }
  cout << nruns << endl;
  if(nruns<1) return 1;

  //================================
  int nbins = pro[0]->GetXaxis()->GetNbins();
  TH2F *stats = new TH2F("stats","stats",
			 nbins,
			 pro[0]->GetXaxis()->GetBinLowEdge(1),
			 pro[0]->GetXaxis()->GetBinLowEdge(nbins+1),
			 100,-0.05,+0.05);
  /*
  TProfile *stats = new TProfile("stats","stats",
				 nbins,
				 pro[0]->GetXaxis()->GetBinLowEdge(1),
				 pro[0]->GetXaxis()->GetBinLowEdge(nbins+1), "s" );
  */
  TH1F *tmp = (TH1F*) pro[0]->Clone("tmphist");
  for(int n=1; n!=nruns; ++n) {
    tmp->Reset();
    //tmp->Divide( pro[n], pro[n-1] );
    tmp->Add( pro[n], pro[0], -1 );
    for(int i=0; i!=nbins; ++i) {
      float obs = tmp->GetBinContent(i+1);
      float eobs = tmp->GetBinError(i+1);
      if( eobs==0 ) continue;
      float x = obs;
      //float x = obs;
      float xbin = tmp->GetBinCenter( i+1 );
      stats->Fill(xbin,x);
    }
  }
  stats->Draw("colz");
  /*
  new TBrowser();
  pro[0]->Draw("H");
  pro[1]->Draw("HSAME");
  tmp->Draw("HSAME");
  */
  return 0;
}
