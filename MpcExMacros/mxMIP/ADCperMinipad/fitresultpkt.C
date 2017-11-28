#include "mxCalibBaseSiW.h"
#include "mxCalibBaseSiW.cxx"
#include "mxCalibStageSiW.h"
#include "mxCalibStageSiW.cxx"
#include "mxCalibBasePbWO4.h"
#include "mxCalibBasePbWO4.cxx"
#include "mxCalibMaster.h"
#include "mxCalibMaster.cxx"
#include "mxGeometry.h"
#include "mxGeometry.cxx"
#include "mxDB.cc"

int fitresultpkt(int pkt=0) {
  mxGeometry *geo = mxGeometry::Instance();
  int kini = pkt*3072;
  float ped[4][768];
  float a[4][768];
  float ea[4][768];
  float b[4][768];
  float eb[4][768];
  float sl[4][768];
  float esl[4][768];
  float f2[4][768];
  float ef2[4][768];
  float f3[4][768];
  float ef3[4][768];
  float f4[4][768];
  float ef4[4][768];
  float mip[4][768];
  float emip[4][768];
  float sgm[4][768];
  float esgm[4][768];
  float x[4][768];
  float r[4][768];
  float ex[4][768];
  float tmp;

  for(int i=0; i!=3072; ++i) {
    a[i%4][i/4] = ea[i%4][i/4] = b[i%4][i/4] = eb[i%4][i/4] = sl[i%4][i/4] = esl[i%4][i/4] = 0;
    ped[i%4][i/4] = f2[i%4][i/4] = ef2[i%4][i/4] = f3[i%4][i/4] = ef3[i%4][i/4] = f4[i%4][i/4] = 0;
    ef4[i%4][i/4] = mip[i%4][i/4] = emip[i%4][i/4] = sgm[i%4][i/4] = esgm[i%4][i/4] = 0;
  }
  int i=0;
  for(int sen=0; sen!=24; ++sen) {
    TString dir = Form("out/SEN%03d/fit",pkt*24+sen);
    for(int j=0; j!=128; ++j) {
      int key = kini + sen*128 + j;
      x[i%4][i/4] = key;
      ex[i%4][i/4] = 0;
      ifstream fin;
      //=====
      fin.open( Form("%s/H%05d.dat",dir.Data(),key) );
      fin >> tmp;
      if(fin.good()) {
	a[i%4][i/4] = tmp;
	fin >> ea[i%4][i/4] >> mip[i%4][i/4] >> emip[i%4][i/4] >> sgm[i%4][i/4] >> esgm[i%4][i/4];
	fin >> f2[i%4][i/4] >> ef2[i%4][i/4] >> f3[i%4][i/4] >> ef3[i%4][i/4] >> f4[i%4][i/4] >> ef4[i%4][i/4];
	fin >> b[i%4][i/4] >> eb[i%4][i/4] >> sl[i%4][i/4] >> esl[i%4][i/4];
	r[i%4][i/4] = sqrt( geo->X(key)*geo->X(key) + geo->Y(key)*geo->Y(key) );
      }
      fin.close();
      //=====
      fin.open( Form("%s/P%05d.dat",dir.Data(),key) );
      fin >> tmp;
      if(fin.good()) {
	ped[i%4][i/4] = tmp;
      }
      fin.close();
      //=====
      ++i;
    }
  }
  cout << i << endl;
  TGraphErrors *grPED[4];
  TGraphErrors *grA[4];
  TGraphErrors *grB[4];
  TGraphErrors *grR[4];
  TGraphErrors *grF2[4];
  TGraphErrors *grF3[4];
  TGraphErrors *grF4[4];
  TGraphErrors *grSL[4];
  TGraphErrors *grMIP[4];
  TGraphErrors *grSGM[4];
  int color[4] = { kRed-3, kBlue-3, kGreen-3, kOrange-3 };
  for(int k=0; k!=4; ++k) {
    grPED[k] = new TGraphErrors(768,x[k],ped[k],ex[k],ex[k]);
    grA[k] = new TGraphErrors(768,x[k],a[k],ex[k],ea[k]);
    grB[k] = new TGraphErrors(768,x[k],b[k],ex[k],eb[k]);
    grR[k] = new TGraphErrors(768,r[k],a[k],ex[k],ea[k]);
    grF2[k] = new TGraphErrors(768,x[k],f2[k],ex[k],ef2[k]);
    grF3[k] = new TGraphErrors(768,x[k],f3[k],ex[k],ef3[k]);
    grF4[k] = new TGraphErrors(768,x[k],f4[k],ex[k],ef4[k]);
    grSL[k] = new TGraphErrors(768,x[k],sl[k],ex[k],esl[k]);
    grMIP[k] = new TGraphErrors(768,x[k],mip[k],ex[k],emip[k]);
    grSGM[k] = new TGraphErrors(768,x[k],sgm[k],ex[k],esgm[k]);

    grPED[k]->SetLineColor( color[k] );
    grPED[k]->SetMarkerColor( color[k] );
    grA[k]->SetLineColor( color[k] );
    grA[k]->SetMarkerColor( color[k] );
    grB[k]->SetLineColor( color[k] );
    grB[k]->SetMarkerColor( color[k] );
    grR[k]->SetLineColor( color[k] );
    grR[k]->SetMarkerColor( color[k] );
    grF2[k]->SetLineColor( color[k] );
    grF2[k]->SetMarkerColor( color[k] );
    grF3[k]->SetLineColor( color[k] );
    grF3[k]->SetMarkerColor( color[k] );
    grF4[k]->SetLineColor( color[k] );
    grF4[k]->SetMarkerColor( color[k] );
    grSL[k]->SetLineColor( color[k] );
    grSL[k]->SetMarkerColor( color[k] );
    grMIP[k]->SetLineColor( color[k] );
    grMIP[k]->SetMarkerColor( color[k] );
    grSGM[k]->SetLineColor( color[k] );
    grSGM[k]->SetMarkerColor( color[k] );
  }

  bool bad[128];
  mxCalibMaster *cal = new mxCalibMaster();
  mxDB::read(454934,cal);
  for(int i=0; i!=3072; ++i) {
    bad[i] = false;
    if( cal->IsBadKey(kini+i) ) bad[i] = true;
  }
  for(int i=3071; i!=-1; --i) {
    if(!bad[i]) continue;
    grPED[i%4]->RemovePoint( i/4 );
    grA[i%4]->RemovePoint( i/4 );
    grB[i%4]->RemovePoint( i/4 );
    grR[i%4]->RemovePoint( i/4 );
    grF2[i%4]->RemovePoint( i/4 );
    grF3[i%4]->RemovePoint( i/4 );
    grF4[i%4]->RemovePoint( i/4 );
    grSL[i%4]->RemovePoint( i/4 );
    grMIP[i%4]->RemovePoint( i/4 );
    grSGM[i%4]->RemovePoint( i/4 );
  }

  TCanvas *main = new TCanvas( Form("PKT%02d",pkt),
			       Form("PKT%02d",pkt),1600,900);
  main->Divide(3,3);

  TLine *line = new TLine;
  line->SetLineColor(kBlack);

  main->cd(1);
  grA[0]->Draw("A*L");
  grA[0]->SetTitle("Sgn");
  grA[0]->GetYaxis()->SetRangeUser(0,1e9);
  for(int k=1; k!=4; ++k) grA[k]->Draw("*LSAME");

  main->cd(4);
  grPED[0]->Draw("A*L");
  grPED[0]->SetTitle("Ped");
  grPED[0]->GetYaxis()->SetRangeUser(-10,+10);
  for(int k=1; k!=4; ++k) grPED[k]->Draw("*LSAME");

  main->cd(6);
  grF2[0]->Draw("A*L");
  grF2[0]->SetTitle("f2, f3 and f4");
  grF2[0]->GetYaxis()->SetRangeUser(0.00,0.25);
  for(int k=1; k!=4; ++k) grF2[k]->Draw("*LSAME");
  line->DrawLine(0,0.22,50000,0.22);
  //main->cd(5);
  //grF3[0]->Draw("A*L");
  grF3[0]->SetTitle("f3");
  grF3[0]->GetYaxis()->SetRangeUser(0.00,0.10);
  for(int k=0; k!=4; ++k) grF3[k]->Draw("*LSAME");
  line->DrawLine(0,0.07,50000,0.07);
  //main->cd(6);
  //grF4[0]->Draw("A*L");
  grF4[0]->SetTitle("f4");
  grF4[0]->GetYaxis()->SetRangeUser(0.00,0.05);
  for(int k=0; k!=4; ++k) grF4[k]->Draw("*LSAME");
  line->DrawLine(0,0.04,50000,0.04);

  main->cd(2);
  grB[0]->Draw("A*L");
  grB[0]->SetTitle("Bgr");
  grB[0]->GetYaxis()->SetRangeUser(0,1e8);
  for(int k=1; k!=4; ++k) grB[k]->Draw("*LSAME");

  main->cd(3);
  grSL[0]->Draw("A*L");
  grSL[0]->SetTitle("Slope");
  grSL[0]->GetYaxis()->SetRangeUser(-1,0);
  for(int k=1; k!=4; ++k) grSL[k]->Draw("*LSAME");
  line->DrawLine(0,-0.4,50000,-0.4);

  main->cd(7);
  main->cd(7);
  grR[0]->Draw("A*");
  grR[0]->SetTitle("A");
  grR[0]->GetYaxis()->SetRangeUser(10,1E9);
  for(int k=1; k!=4; ++k) grR[k]->Draw("*SAME");

  main->cd(8)->SetGridx(1);
  main->cd(8)->SetGridy(1);
  grMIP[0]->Draw("A*L");
  grMIP[0]->SetTitle("MPV");
  grMIP[0]->GetYaxis()->SetRangeUser(10,30);
  for(int k=1; k!=4; ++k) grMIP[k]->Draw("*LSAME");

  main->cd(9);
  grSGM[0]->Draw("A*L");
  grSGM[0]->SetTitle("SGM");
  grSGM[0]->GetYaxis()->SetRangeUser(2,4);
  for(int k=1; k!=4; ++k) grSGM[k]->Draw("*LSAME");
  line->DrawLine(0,3.0,50000,3.0);


  return 0;
}
