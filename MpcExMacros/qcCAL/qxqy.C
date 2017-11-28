int qxqy(float central=3,int ord=2) {
  gStyle->SetOptStat(0);
  TFile *file = new TFile("se-455450.root");
  //file->ls();
  //return 1;
  TH2F *qx[4][2];
  TH2F *qy[4][2];

  qx[0][0] = (TH2F*) file->Get(Form("Qx%dex_ARM0_DET0I",ord));
  qy[0][0] = (TH2F*) file->Get(Form("Qy%dex_ARM0_DET0I",ord));
  qx[0][1] = (TH2F*) file->Get(Form("Qx%dex_ARM0_DET0O",ord));
  qy[0][1] = (TH2F*) file->Get(Form("Qy%dex_ARM0_DET0O",ord));

  qx[1][0] = (TH2F*) file->Get(Form("Qx%dex_ARM0_DET1I",ord));
  qy[1][0] = (TH2F*) file->Get(Form("Qy%dex_ARM0_DET1I",ord));
  qx[1][1] = (TH2F*) file->Get(Form("Qx%dex_ARM0_DET1O",ord));
  qy[1][1] = (TH2F*) file->Get(Form("Qy%dex_ARM0_DET1O",ord));

  qx[2][0] = (TH2F*) file->Get(Form("Qx%dex_ARM0_DET2I",ord));
  qy[2][0] = (TH2F*) file->Get(Form("Qy%dex_ARM0_DET2I",ord));
  qx[2][1] = (TH2F*) file->Get(Form("Qx%dex_ARM0_DET2O",ord));
  qy[2][1] = (TH2F*) file->Get(Form("Qy%dex_ARM0_DET2O",ord));

  qx[3][0] = (TH2F*) file->Get(Form("Qx%dex_ARM0_DET3I",ord));
  qy[3][0] = (TH2F*) file->Get(Form("Qy%dex_ARM0_DET3I",ord));
  qx[3][1] = (TH2F*) file->Get(Form("Qx%dex_ARM0_DET3O",ord));
  qy[3][1] = (TH2F*) file->Get(Form("Qy%dex_ARM0_DET3O",ord));

  TF1 *fqx[4][2], *fqy[4][2];
  TH1D *hqx[4][2], *hqy[4][2];
  int col[4] = {kBlue-3,kRed-3,kCyan-3,kOrange-3};
  int binpro = qx[0][0]->GetXaxis()->FindBin(central);
  float xmin = qx[0][0]->GetXaxis()->GetBinLowEdge(binpro);
  float xmax = xmin + qx[0][0]->GetXaxis()->GetBinWidth(binpro);
  TCanvas *main = new TCanvas( Form("C_%f_%f",xmin,xmax), Form("c_%f_%f",xmin,xmax) );
  main->Divide(2,2);
  main->cd(1);
  for(int i=0; i!=8; ++i) {
    hqx[i/2][i%2] = qx[i/2][i%2]->ProjectionY( Form("hqx_%d",i),binpro,binpro);
    hqy[i/2][i%2] = qy[i/2][i%2]->ProjectionY( Form("hqy_%d",i),binpro,binpro);
    hqx[i/2][i%2]->SetLineColor( col[i/2] );
    hqy[i/2][i%2]->SetLineColor( col[i/2] );
    fqx[i/2][i%2] = new TF1( Form("fqx_%d",i), "gaus" );
    fqy[i/2][i%2] = new TF1( Form("fqy_%d",i), "gaus" );
    fqx[i/2][i%2]->SetLineColor( col[i/2] );
    fqy[i/2][i%2]->SetLineColor( col[i/2] );
    hqx[i/2][i%2]->Fit(Form("fqx_%d",i));
    hqy[i/2][i%2]->Fit(Form("fqy_%d",i));
  }

  TLatex *tex = new TLatex();

  main->cd(1);
  float max = 0;
  for(int i=0; i!=4; ++i) {
    float mm = hqx[i][0]->GetMaximum();
    if( mm>max ) max = mm;
    mm = hqy[i][0]->GetMaximum();
    if( mm>max ) max = mm;
  }

  hqx[0][0]->Draw();
  hqx[0][0]->GetYaxis()->SetRangeUser(0,max);
  hqx[0][0]->SetTitle("Qx inner detector");
  for(int i=0; i!=4; ++i) {
    hqx[i][0]->Draw("same");
    tex->SetTextColor( col[i] );
    tex->DrawLatex(-0.95,(0.5+0.1*i)*max,Form("Det%d: %.1f M", i,
					      hqx[i][0]->GetEntries()*1e-6) );
    tex->DrawLatex(+0.25,(0.5+0.1*i)*max,Form("#mu = %.3f | #sigma = %.2f",
					      fqx[i][0]->GetParameter(1),
					      fqx[i][0]->GetParameter(2) ));
  }

  main->cd(2);
  hqy[0][0]->Draw();
  hqy[0][0]->GetYaxis()->SetRangeUser(0,max);
  hqy[0][0]->SetTitle("Qy inner detector");
  for(int i=0; i!=4; ++i) {
    hqy[i][0]->Draw("same");
    tex->SetTextColor( col[i] );
    tex->DrawLatex(-0.95,(0.5+0.1*i)*max,Form("Det%d: %.1f M", i,
					      hqy[i][0]->GetEntries()*1e-6) );
    tex->DrawLatex(+0.25,(0.5+0.1*i)*max,Form("#mu = %.3f | #sigma = %.2f",
					      fqy[i][0]->GetParameter(1),
					      fqy[i][0]->GetParameter(2) ));
  }

  max = 0;
  for(int i=0; i!=4; ++i) {
    float mm = hqx[i][1]->GetMaximum();
    if( mm>max ) max = mm;
    mm = hqy[i][1]->GetMaximum();
    if( mm>max ) max = mm;
  }

  main->cd(3);
  hqx[0][1]->Draw();
  hqx[0][1]->GetYaxis()->SetRangeUser(0,max);
  hqx[0][1]->SetTitle("Qx outer detector");
  for(int i=0; i!=4; ++i) {
    hqx[i][1]->Draw("same");
    tex->SetTextColor( col[i] );
    tex->DrawLatex(-0.95,(0.5+0.1*i)*max,Form("Det%d: %.1f M", i,
					      hqx[i][1]->GetEntries()*1e-6) );
    tex->DrawLatex(+0.25,(0.5+0.1*i)*max,Form("#mu = %.3f | #sigma = %.2f",
					      fqx[i][1]->GetParameter(1),
					      fqx[i][1]->GetParameter(2) ));
  }

  main->cd(4);
  hqy[0][1]->Draw();
  hqy[0][1]->GetYaxis()->SetRangeUser(0,max);
  hqy[0][1]->SetTitle("Qy outer detector");
  for(int i=0; i!=4; ++i) {
    hqy[i][1]->Draw("same");
    tex->SetTextColor( col[i] );
    tex->DrawLatex(-0.95,(0.5+0.1*i)*max,Form("Det%d: %.1f M", i,
					      hqy[i][1]->GetEntries()*1e-6) );
    tex->DrawLatex(+0.25,(0.5+0.1*i)*max,Form("#mu = %.3f | #sigma = %.2f",
					      fqy[i][1]->GetParameter(1),
					      fqy[i][1]->GetParameter(2) ));
  }

}
