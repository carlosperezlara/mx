#include "mxCalibBaseSiW.h"
#include "mxCalibBaseSiW.cxx"
#include "mxCalibBasePbWO4.h"
#include "mxCalibBasePbWO4.cxx"
#include "mxCalibMaster.h"
#include "mxCalibMaster.cxx"
#include "mxDB.cc"

int peddisplaysen(int sen=0) {
  int kini = sen*128;
  float a[4][32];
  float s[4][32];
  float ha[4][32];
  float hs[4][32];
  float x[4][32];
  float tmp;
  
  for(int i=0; i!=128; ++i) {
    a[i%4][i/4] = 0;
    s[i%4][i/4] = 0;
    ha[i%4][i/4] = 0;
    hs[i%4][i/4] = 0;
  }
  TString dir = Form("out/SEN%03d/fit",sen);
  for(int i=0; i!=128; ++i) {
    int key = kini + i;
    x[i%4][i/4] = key;
    ifstream fin( Form("%s/P%05d.dat",dir.Data(),key) );
    fin >> tmp;
    if(fin.good()) {
      a[i%4][i/4] = tmp;
      fin >> s[i%4][i/4];
      fin >> ha[i%4][i/4];
      fin >> hs[i%4][i/4];
      ha[i%4][i/4] -= a[i%4][i/4];
      hs[i%4][i/4] -= s[i%4][i/4];
    }
  }
  TGraph *grA[4];
  TGraph *grS[4];
  TGraph *grHA[4];
  TGraph *grHS[4];
  int color[4] = { kRed-3, kBlue-3, kGreen-3, kOrange-3 };
  for(int k=0; k!=4; ++k) {
    grA[k] = new TGraphErrors(32,x[k],a[k]);
    grA[k]->SetMarkerColor( color[k] );
    grA[k]->SetLineColor( color[k] );

    grS[k] = new TGraphErrors(32,x[k],s[k]);
    grS[k]->SetMarkerColor( color[k] );
    grS[k]->SetLineColor( color[k] );

    grHA[k] = new TGraphErrors(32,x[k],ha[k]);
    grHA[k]->SetMarkerColor( color[k] );
    grHA[k]->SetLineColor( color[k] );

    grHS[k] = new TGraphErrors(32,x[k],hs[k]);
    grHS[k]->SetMarkerColor( color[k] );
    grHS[k]->SetLineColor( color[k] );
  }
  
  bool bad[128];
  mxCalibMaster *cal = new mxCalibMaster();
  mxDB::read(454934,cal);
  for(int i=0; i!=128; ++i) {
    bad[i] = false;
    if( cal->IsBadKey(kini+i) ) bad[i] = true;
  }
  for(int i=127; i!=-1; --i) {
    if(!bad[i]) continue;
    grA[i%4]->RemovePoint( i/4 );
    grS[i%4]->RemovePoint( i/4 );
    grHA[i%4]->RemovePoint( i/4 );
    grHS[i%4]->RemovePoint( i/4 );
  }
  
  TCanvas *main = new TCanvas( Form("SEN%02d",sen),
			       Form("SEN%02d",sen),800,600);
  main->Divide(2,2);

  main->cd(1)->SetGridx(1);
  grA[0]->Draw("A*L");
  grA[0]->SetTitle("Mean");
  grA[0]->GetYaxis()->SetRangeUser(-10,+10);
  for(int k=1; k!=4; ++k) grA[k]->Draw("*LSAME");

  main->cd(2)->SetGridx(1);
  grS[0]->Draw("A*L");
  grS[0]->SetTitle("Sigma");
  grS[0]->GetYaxis()->SetRangeUser(0.0,2.5);
  for(int k=1; k!=4; ++k) grS[k]->Draw("*LSAME");

  main->cd(3)->SetGridx(1);
  grHA[0]->Draw("A*L");
  grHA[0]->SetTitle("Mean-HMEAN");
  grHA[0]->GetYaxis()->SetRangeUser(-0.5,+0.5);
  for(int k=1; k!=4; ++k) grHA[k]->Draw("*LSAME");

  main->cd(4)->SetGridx(1);
  grHS[0]->Draw("A*L");
  grHS[0]->SetTitle("Sigma-HRMS");
  grHS[0]->GetYaxis()->SetRangeUser(-0.5,+0.5);
  for(int k=1; k!=4; ++k) grHS[k]->Draw("*LSAME");

  return 0;
}
