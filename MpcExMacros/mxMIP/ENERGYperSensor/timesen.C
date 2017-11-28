#include "mxGeometry.h"
#include "mxGeometry.cxx"
#include <algorithm>

mxGeometry *fGeo = mxGeometry::Instance();

const int nruns = 91;
int run[100];
float data_a[192][nruns]; // sen fill
float data_m[192][nruns]; // sen fill
const int fbins=18;
int rrange[fbins][3];
TGraph *gr_a[24][16];
TGraph *gr_m[24][16];

void loaddata() {
  ifstream frun("runs.dat.final");
  for(int ii=0;;++ii) {
    frun >> run[ii];
    if(!frun.good()) break;
  }

  ifstream fin;
  for(int sen=0; sen!=192; ++sen) {
    //=====
    float a, m, tmp;
    for(int ii=0; ii!=nruns; ++ii) {
      fin.open( Form("out/SEN%03d/fit/H%d.dat",sen,run[ii]) );
      fin >> tmp >> tmp >> tmp;
      if(fin.good()) {
	a = tmp;
	fin >> tmp >> m ;
      }
      fin.close();
      //cout << a << endl;
      //=====
      data_a[sen][ii] = a*1000;
      data_m[sen][ii] = m;
    }
  }
}

int showLayer(int lyr=0) {
  gStyle->SetOptStat(0);
  loaddata();
  TH2Poly *lyr_a[nruns];
  TH2Poly *lyr_m[nruns];
  for(int i=0; i!=nruns; ++i) {
    lyr_a[i] = new TH2Poly(Form("A_LYR%0d_FILL%d",lyr,i),"A",-22,+22,-22,+22);
    lyr_m[i] = new TH2Poly(Form("M_LYR%0d_FILL%d",lyr,i),"M",-22,+22,-22,+22);
    Double_t x1[4], y1[4];
    for(int k=0; k!=24; ++k) {
      float a0 = fGeo->W_a0();
      float a1 = fGeo->W_a1();
      float x = fGeo->PS_SenX(lyr/8, k );
      float y = fGeo->PS_SenY(lyr/8, k );
      x1[0] = x - a0/2;
      x1[1] = x + a0/2;
      x1[2] = x + a0/2;
      x1[3] = x - a0/2;
      y1[0] = y - a1/2;
      y1[1] = y - a1/2;
      y1[2] = y + a1/2;
      y1[3] = y + a1/2;
      lyr_a[i]->AddBin(4,x1,y1);
      lyr_m[i]->AddBin(4,x1,y1);
    }
  }
  for(int k=0; k!=24; ++k) {
    float x = fGeo->PS_SenX(lyr/8,k);
    float y = fGeo->PS_SenY(lyr/8,k);
    float a = 99999;
    float m = 99999;
    float tmp;
    ifstream fin;
    //=====
    int sen = 24*lyr + k;
    for(int ii=0; ii!=nruns; ++ii) {
      lyr_a[ii]->Fill( x, y, data_a[sen][ii] );
      lyr_m[ii]->Fill( x, y, data_m[sen][ii] );
    }
  }
  TCanvas *main = new TCanvas( Form("LYR%0d",lyr), Form("LYR%0d",lyr), 800, 300 );
  main->Divide(2,1);

  for(int ii=0; ii!=nruns; ++ii) {
    main->cd(1);
    lyr_a[ii]->Draw("COLZ");
    main->cd(2);
    lyr_m[ii]->Draw("COLZ");
    lyr_a[ii]->GetZaxis()->SetRangeUser(135,165);
    lyr_m[ii]->GetZaxis()->SetRangeUser(-4,0);
    main->SaveAs( Form("AM_LYR%d_FILL%d.png",lyr,ii), "PNG" );
  }

  return 0;
}

int showtime(int lyr) {
  gStyle->SetOptStat(0);
  rrange[0][0] = 454774; rrange[0][1] = 454789; rrange[0][2] = 19900;
  rrange[1][0] = 454794; rrange[1][1] = 454810; rrange[1][2] = 19901;
  rrange[2][0] = 454933; rrange[2][1] = 454937; rrange[2][2] = 19904;
  rrange[3][0] = 454943; rrange[3][1] = 454948; rrange[3][2] = 19907;
  rrange[4][0] = 455049; rrange[4][1] = 455053; rrange[4][2] = 19914;
  rrange[5][0] = 455056; rrange[5][1] = 455066; rrange[5][2] = 19915;
  rrange[6][0] = 455071; rrange[6][1] = 455083; rrange[6][2] = 19917;
  rrange[7][0] = 455200; rrange[7][1] = 455203; rrange[7][2] = 19920;
  rrange[8][0] = 455206; rrange[8][1] = 455213; rrange[8][2] = 19921;
  rrange[9][0] = 455218; rrange[9][1] = 455224; rrange[9][2] = 19924;
  rrange[10][0] = 455302; rrange[10][1] = 455306; rrange[10][2] = 19925;
  rrange[11][0] = 455342; rrange[11][1] = 455355; rrange[11][2] = 19928;
  rrange[12][0] = 455362; rrange[12][1] = 455367; rrange[12][2] = 19930;
  rrange[13][0] = 455446; rrange[13][1] = 455452; rrange[13][2] = 19932;
  rrange[14][0] = 455476; rrange[14][1] = 455478; rrange[14][2] = 19935;
  rrange[15][0] = 455545; rrange[15][1] = 455552; rrange[15][2] = 19937;
  rrange[16][0] = 455585; rrange[16][1] = 455605; rrange[16][2] = 19940;
  rrange[17][0] = 455637; rrange[17][1] = 455639; rrange[17][2] = 19941;
  loaddata();
  float x[nruns];
  int color[2] = {kBlue-3, kRed-3};
  for(int i=0; i!=nruns; ++i) x[i]=run[i];
  for(int isen=0; isen!=24; ++isen) {
    int sen = isen + 24*lyr;
    for(int fff=0; fff!=16; ++fff) {
      gr_a[isen][fff] = new TGraph(nruns,x,data_a[sen]);
      gr_m[isen][fff] = new TGraph(nruns,x,data_m[sen]);
      gr_a[isen][fff]->SetLineColor( color[fff%2] );
      gr_m[isen][fff]->SetLineColor( color[fff%2] );
      gr_a[isen][fff]->SetMarkerColor( color[fff%2] );
      gr_m[isen][fff]->SetMarkerColor( color[fff%2] );
      for(int irun=nruns-1; irun!=-1; --irun) {
	if(x[irun]>rrange[fff][1]) {
	  gr_a[isen][fff]->RemovePoint(irun);
	  gr_m[isen][fff]->RemovePoint(irun);
	}
      }
      for(int irun=0; irun!=nruns; ++irun) {
	if(x[irun]<rrange[fff][0]) {
	  gr_a[isen][fff]->RemovePoint(0);
	  gr_m[isen][fff]->RemovePoint(0);
	}
      }
    }
  }
  return 0;
}

int timelyr(int lyr=0) {
  showtime(lyr);
  TH2F *axisA = new TH2F("axisA",";;A",30,run[0],run[nruns-1],100,100,170);
  TCanvas *main;
  main = new TCanvas( Form("LYR%0d",lyr), Form("LYR%0d",lyr), 800, 600 );
  main->Divide(6,4,0,0);
  for(int ii=0; ii!=24; ++ii) {
    main->cd(1+ii);
    if(1) {
      axisA->SetTitle( Form("SEN %03d",24*lyr+ii) );
      axisA->DrawCopy();
      for(int fff=0; fff!=16; ++fff)
	gr_a[ii][fff]->Draw("*LSAME");
    } else {
      gr_m[ii][0]->Draw("A*L");
    }
  }

  return 0;
}

int timesen(int sen=0) {
  int lyr = sen/24;
  showtime(lyr);
  TH2F *axisA = new TH2F("axisA",";;A",300,run[0],run[nruns-1],100,100,170);
  TCanvas *main;
  main = new TCanvas( Form("LYR%0d",lyr), Form("LYR%0d",lyr), 800, 600 );
  main->SetGridx(1);
  main->SetGridy(1);
  if(1) {
    axisA->SetTitle( Form("SEN %03d", sen) );
    axisA->DrawCopy();
    for(int fff=0; fff!=16; ++fff)
      gr_a[sen%24][fff]->Draw("*LSAME");
  } else {
    gr_m[sen%24][0]->Draw("A*L");
  }

  return 0;
}
