int psi(int run=0, float central=3,int ord=2) {
  gStyle->SetOptStat(0);
  TFile *file = new TFile( Form("se-%d.root",run) );
  //file->ls();
  //return 1;
  TH2F *qx[4][2];

  qx[0][0] = (TH2F*) file->Get(Form("Psi%dex_ARM0_DET0I",ord));
  qx[0][1] = (TH2F*) file->Get(Form("Psi%dex_ARM0_DET0O",ord));
  qx[1][0] = (TH2F*) file->Get(Form("Psi%dex_ARM0_DET1I",ord));
  qx[1][1] = (TH2F*) file->Get(Form("Psi%dex_ARM0_DET1O",ord));
  qx[2][0] = (TH2F*) file->Get(Form("Psi%dex_ARM0_DET2I",ord));
  qx[2][1] = (TH2F*) file->Get(Form("Psi%dex_ARM0_DET2O",ord));
  qx[3][0] = (TH2F*) file->Get(Form("Psi%dex_ARM0_DET3I",ord));
  qx[3][1] = (TH2F*) file->Get(Form("Psi%dex_ARM0_DET3O",ord));

  TF1 *fqx[4][2];
  TH1D *hqx[4][2];
  int col[4] = {kBlue-3,kRed-3,kCyan-3,kOrange-3};
  int binpro = qx[0][0]->GetXaxis()->FindBin(central);
  float xmin = qx[0][0]->GetXaxis()->GetBinLowEdge(binpro);
  float xmax = xmin + qx[0][0]->GetXaxis()->GetBinWidth(binpro);
  TCanvas *main = new TCanvas( Form("C_%f_%f",xmin,xmax), Form("c_%f_%f",xmin,xmax) );
  main->Divide(2,1);
  main->cd(1);
  TString fitstr = "[0]*(1";
  for(int n=0; n!=8; ++n) {
    fitstr += Form("+[%d]*TMath::Cos(%d*x)+[%d]*TMath::Sin(%d*x)",n*2+1,n+1,n*2+2,n+1);
  }
  fitstr += ")";
  cout << fitstr.Data() << endl;
  //return 1;
  for(int i=0; i!=8; ++i) {
    hqx[i/2][i%2] = qx[i/2][i%2]->ProjectionY( Form("hqx_%d",i),binpro,binpro);
    hqx[i/2][i%2]->SetLineColor( col[i/2] );
    hqx[i/2][i%2]->SetLineWidth( 3 );
    fqx[i/2][i%2] = new TF1( Form("fqx_%d",i), fitstr.Data() );
    for(int k=1; k!=17; ++k) {
      fqx[i/2][i%2]->SetParameter(k+1,0);
      fqx[i/2][i%2]->SetParLimits(k+1,-0.5,+0.5);
    }
    fqx[i/2][i%2]->SetLineColor( col[i/2] );
    fqx[i/2][i%2]->SetParLimits( 3, -0.5,0.0 );
    fqx[i/2][i%2]->SetParLimits( 7, 0.0,0.5 );
    hqx[i/2][i%2]->Fit(Form("fqx_%d",i), "");
  }

  TLatex *tex = new TLatex();

  main->cd(1);
  float max = 0;
  for(int i=0; i!=4; ++i) {
    float mm = hqx[i][0]->GetMaximum();
    if( mm>max ) max = mm;
  }

  hqx[0][0]->Draw();
  hqx[0][0]->GetYaxis()->SetRangeUser(0,max);
  hqx[0][0]->SetTitle( Form("Psi%d inner detector",ord) );
  for(int i=0; i!=4; ++i) {
    hqx[i][0]->Draw("same");
    tex->SetTextColor( col[i] );
    tex->DrawLatex(0.01,(0.1+0.06*i)*max,Form("D%d => 1: %.1f %.1f || 2: %.1f %.1f",i,
					      fqx[i][0]->GetParameter(1), fqx[i][0]->GetParameter(1),
					      fqx[i][0]->GetParameter(2), fqx[i][0]->GetParameter(2)
					      ));
  }
  max = 0;
  for(int i=0; i!=4; ++i) {
    float mm = hqx[i][1]->GetMaximum();
    if( mm>max ) max = mm;
  }

  main->cd(2);
  hqx[0][1]->Draw();
  hqx[0][1]->GetYaxis()->SetRangeUser(0,max);
  hqx[0][1]->SetTitle( Form("Psi%d outer detector",ord) );
  for(int i=0; i!=4; ++i) {
    hqx[i][1]->Draw("same");
    tex->SetTextColor( col[i] );
    tex->DrawLatex(0.01,(0.1+0.06*i)*max,Form("Det%d: %.1f K", i,
					      hqx[i][1]->GetEntries()*1e-3) );
  }

  //main->SaveAs( Form("PSI_%d_CC%03.0f-%03.0f_ORD%d.png",run,xmin*10,xmax*10,ord), "PNG" );

  ofstream ofile( Form("fit/PSI_%d_CC%02.0f-%02.0f_ORD%d.dat",run,xmin+0.5,xmax+0.5,ord) );
  for(int i=0; i!=8; ++i) {
    for(int k=1; k!=17; ++k)
      ofile << fqx[i/2][i%2]->GetParameter(k) << " ";
    ofile << endl;
  }
  ofile.close();

  return 0;
}

int psiallcc(int run=0, int ord=2) {
  gSystem->Exec("mkdir fit");
  for(float central=3.0; central<55.0; central += 5) {
    psi(run,central,ord);
  }
  return 0;
}
