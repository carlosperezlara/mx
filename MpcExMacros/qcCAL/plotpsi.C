float a[4][2][8];
float b[4][2][8];
int color[4] = {kRed-3,kBlue-3,kGreen-3,kOrange-3};

void loadfile(int run=0, int ord=2, int cmin=0, int cmax=5) {
  ifstream ifile( Form("fit/PSI_%d_CC%02d-%02d_ORD%d.dat",run,cmin,cmax,ord) );
  for(int i=0; i!=8; ++i) {
    int det = i/2;
    int ioc = i%2;
    float tmp;
    ifile >> tmp;
    if(!ifile.good()) break;
    a[0][0][0] = tmp;
    for(int k=1; k!=16; ++k) {
      if(k%2==0) {
	ifile >> a[det][ioc][k/2];
      } else {
	ifile >> b[det][ioc][k/2];
      }
    }
  }
  ifile.close();
}

int plotpsi(int run=0, int ord=2, int cmin=0, int cmax=5) {
  gStyle->SetOptStat(0);
  loadfile(run,ord,cmin,cmax);
  float x[8] = {1,2,3,4,5,6,7,8};
  TGraph *ga[4][2];
  TGraph *gb[4][2];
  for(int i=0; i!=8; ++i) {
    int det = i/2;
    int ioc = i%2;
    ga[det][ioc] = new TGraph(8,x,a[det][ioc]);
    gb[det][ioc] = new TGraph(8,x,b[det][ioc]);
    ga[det][ioc]->SetMarkerColor( color[det] );
    ga[det][ioc]->SetLineColor( color[det] );
    gb[det][ioc]->SetLineColor( color[det] );
    gb[det][ioc]->SetMarkerColor( color[det] );
  }
  TCanvas *main = new TCanvas( Form("RUN%d",run), Form("RUN%d",run) );
  main->Divide(2,2);
  TH2F *axis = new TH2F("axis","",100,0.5,8.5,100,-0.5,+0.5);
  main->cd(1); axis->SetTitle( "A inner" ); axis->DrawCopy();
  main->cd(2); axis->SetTitle( "B inner" ); axis->DrawCopy();
  main->cd(3); axis->SetTitle( "A outer" ); axis->DrawCopy();
  main->cd(4); axis->SetTitle( "B outer" ); axis->DrawCopy();
  for(int i=0; i!=8; ++i) {
    int det = i/2;
    int ioc = i%2;
    main->cd(1+ioc*2);
    ga[det][ioc]->Draw("*lsame");
    main->cd(2+ioc*2);
    gb[det][ioc]->Draw("*lsame");
  }

  return 0;
}

int plotpsiallcc(int run=0, int ord=2) {
  float x[10];
  float pa[4][2][8][10];
  float pb[4][2][8][10];
  for(int cc=0; cc!=10; ++cc) {
    x[cc] = 5*cc + 2.5;
    loadfile(run,ord,x[cc]-2.5,x[cc]+2.5);
    for(int i=0; i!=8; ++i) {
      int det = i/2;
      int ioc = i%2;
      for(int k=0; k!=8; ++k) {
	pa[det][ioc][k][cc] = a[det][ioc][k];
	pb[det][ioc][k][cc] = b[det][ioc][k];
      }
    }
  }

  TGraph *ga[4][2][8];
  TGraph *gb[4][2][8];
  for(int i=0; i!=8; ++i) {
    for(int k=0; k!=8; ++k) {
      int det = i/2;
      int ioc = i%2;
      ga[det][ioc][k] = new TGraph( 10, x, pa[det][ioc][k] );
      gb[det][ioc][k] = new TGraph( 10, x, pb[det][ioc][k] );
    }
  }

  TCanvas *main = new TCanvas();
  TH2F *axis = new TH2F("axis",";centrality",100,0,70,100,-0.5,+0.5);
  main->Divide(4,4);
  for(int k=0; k!=8; ++k) {
    main->cd(1+k)->SetGridy(1);
    axis->SetTitle( Form("A_%d",k+1) );
    axis->DrawCopy();
    for(int i=0; i!=8; ++i)
      ga[i/2][i%2][k]->Draw("*LSAME");
    main->cd(9+k)->SetGridy(1);
    axis->SetTitle( Form("B_%d",k+1) );
    axis->DrawCopy();
    for(int i=0; i!=8; ++i)
      gb[i/2][i%2][k]->Draw("*LSAME");
  }


  return 0;
}
