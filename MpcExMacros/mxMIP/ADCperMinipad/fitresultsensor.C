#include "mxCalibBaseSiW.h"
#include "mxCalibBaseSiW.cxx"
#include "mxCalibStageSiW.h"
#include "mxCalibStageSiW.cxx"
#include "mxCalibBasePbWO4.h"
#include "mxCalibBasePbWO4.cxx"
#include "mxCalibMaster.h"
#include "mxCalibMaster.cxx"
#include "mxDB.cc"

int fitresultsensor(int sen=0) {
  TString dir = Form("out/SEN%03d/fit",sen);
  int kini = sen*128;

  float p[4][32];
  float in[4][32];
  float a[4][32];
  float ea[4][32];
  float b[4][32];
  float eb[4][32];
  float sl[4][32];
  float esl[4][32];
  float f2[4][32];
  float ef2[4][32];
  float f3[4][32];
  float ef3[4][32];
  float f4[4][32];
  float ef4[4][32];
  float mip[4][32];
  float emip[4][32];
  float sgm[4][32];
  float esgm[4][32];
  float tmp;

  float x[4][32];
  float ex[4][32];
  for(int i=0; i!=128; ++i) {
    p[i%4][i/4] = in[i%4][i/4] = 0;
    a[i%4][i/4] = ea[i%4][i/4] = b[i%4][i/4] = eb[i%4][i/4] = sl[i%4][i/4] = esl[i%4][i/4] = 0;
    f2[i%4][i/4] = ef2[i%4][i/4] = f3[i%4][i/4] = ef3[i%4][i/4] = f4[i%4][i/4] = 0;
    ef4[i%4][i/4] = mip[i%4][i/4] = emip[i%4][i/4] = sgm[i%4][i/4] = esgm[i%4][i/4] = 0;
  }
  for(int i=0; i!=128; ++i) {
    int key = kini + i;
    x[i%4][i/4] = key;
    ex[i%4][i/4] = 0;
    ifstream fin;
    fin.open( Form("%s/H%05d.dat",dir.Data(),key) );
    fin >> tmp;
    if(fin.good()) {
      a[i%4][i/4] = tmp;
      fin >> ea[i%4][i/4] >> mip[i%4][i/4] >> emip[i%4][i/4] >> sgm[i%4][i/4] >> esgm[i%4][i/4];
      fin >> f2[i%4][i/4] >> ef2[i%4][i/4] >> f3[i%4][i/4] >> ef3[i%4][i/4] >> f4[i%4][i/4] >> ef4[i%4][i/4];
      fin >> b[i%4][i/4] >> eb[i%4][i/4] >> sl[i%4][i/4] >> esl[i%4][i/4];
    }
    fin.close();
    //
    fin.open( Form("%s/P%05d.dat",dir.Data(),key) );
    fin >> tmp;
    if(fin.good()) {
      p[i%4][i/4] = tmp;
    }
    fin.close();
    //
    fin.open( Form("%s/I%05d.dat",dir.Data(),key) );
    fin >> tmp;
    if(fin.good()) {
      in[i%4][i/4] = tmp;
    }
    fin.close();
  }
  TGraphErrors *grP[4];
  TGraphErrors *grIN[4];
  TGraphErrors *grA[4];
  TGraphErrors *grB[4];
  TGraphErrors *grF2[4];
  TGraphErrors *grF3[4];
  TGraphErrors *grF4[4];
  TGraphErrors *grSL[4];
  TGraphErrors *grMIP[4];
  TGraphErrors *grSGM[4];
  int color[4] = { kRed-3, kBlue-3, kGreen-3, kOrange-3 };
  for(int k=0; k!=4; ++k) {
    grP[k] = new TGraphErrors(32,x[k],p[k],ex[k],ex[k]);
    grIN[k] = new TGraphErrors(32,x[k],in[k],ex[k],ex[k]);
    grA[k] = new TGraphErrors(32,x[k],a[k],ex[k],ea[k]);
    grB[k] = new TGraphErrors(32,x[k],b[k],ex[k],eb[k]);
    grF2[k] = new TGraphErrors(32,x[k],f2[k],ex[k],ef2[k]);
    grF3[k] = new TGraphErrors(32,x[k],f3[k],ex[k],ef3[k]);
    grF4[k] = new TGraphErrors(32,x[k],f4[k],ex[k],ef4[k]);
    grSL[k] = new TGraphErrors(32,x[k],sl[k],ex[k],esl[k]);
    grMIP[k] = new TGraphErrors(32,x[k],mip[k],ex[k],emip[k]);
    grSGM[k] = new TGraphErrors(32,x[k],sgm[k],ex[k],esgm[k]);

    grP[k]->SetLineColor( color[k] );
    grP[k]->SetMarkerColor( color[k] );
    grIN[k]->SetLineColor( color[k] );
    grIN[k]->SetMarkerColor( color[k] );
    grA[k]->SetLineColor( color[k] );
    grA[k]->SetMarkerColor( color[k] );
    grB[k]->SetLineColor( color[k] );
    grB[k]->SetMarkerColor( color[k] );
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
  for(int i=0; i!=128; ++i) {
    bad[i] = false;
    if( cal->IsBadKey(kini+i) ) bad[i] = true;
  }
  for(int i=127; i!=-1; --i) {
    if(!bad[i]) continue;
    grP[i%4]->RemovePoint( i/4 );
    grIN[i%4]->RemovePoint( i/4 );
    grA[i%4]->RemovePoint( i/4 );
    grB[i%4]->RemovePoint( i/4 );
    grF2[i%4]->RemovePoint( i/4 );
    grF3[i%4]->RemovePoint( i/4 );
    grF4[i%4]->RemovePoint( i/4 );
    grSL[i%4]->RemovePoint( i/4 );
    grMIP[i%4]->RemovePoint( i/4 );
    grSGM[i%4]->RemovePoint( i/4 );
  }

  TCanvas *main = new TCanvas( Form("SEN%03d",sen),
			       Form("SEN%03d",sen),1600,900);
  main->Divide(3,3);

  TLine *line = new TLine;
  line->SetLineColor(kBlack);

  main->cd(5)->SetGridx(1);
  grA[0]->Draw("A*L");
  grA[0]->SetTitle("Sgn");
  grA[0]->GetYaxis()->SetRangeUser(0,1e9);
  grA[0]->GetXaxis()->SetNdivisions(515);
  //grA[0]->GetXaxis()->SetLabelSize(0.05);
  for(int k=1; k!=4; ++k) grA[k]->Draw("*LSAME");

  main->cd(1)->SetGridx(1);
  grIN[0]->Draw("A*L");
  grIN[0]->SetTitle("Init");
  grIN[0]->GetYaxis()->SetRangeUser(0,20);
  for(int k=1; k!=4; ++k) grIN[k]->Draw("*LSAME");

  main->cd(7)->SetGridx(1);
  grP[0]->Draw("A*L");
  grP[0]->SetTitle("Ped");
  grP[0]->GetYaxis()->SetRangeUser(-10,+10);
  //grA[0]->GetXaxis()->SetLabelSize(0.05);
  for(int k=1; k!=4; ++k) grP[k]->Draw("*LSAME");

  main->cd(6)->SetGridx(1);
  grF2[0]->Draw("A*L");
  grF2[0]->SetTitle("f2, f3, f4");
  grF2[0]->GetYaxis()->SetRangeUser(0.0,0.40);
  grF2[0]->GetXaxis()->SetNdivisions(515);
  //grF2[0]->GetXaxis()->SetLabelSize(0.05);
  for(int k=1; k!=4; ++k) grF2[k]->Draw("*LSAME");
  line->DrawLine(0,0.22,50000,0.22);

  main->cd(6)->SetGridx(1);
  grF3[0]->Draw("*LSAME");
  grF3[0]->SetTitle("f3");
  grF3[0]->GetYaxis()->SetRangeUser(0.0,0.15);
  grF3[0]->GetXaxis()->SetNdivisions(515);
  //grF3[0]->GetXaxis()->SetLabelSize(0.05);
  for(int k=1; k!=4; ++k) grF3[k]->Draw("*LSAME");
  line->DrawLine(0,0.07,50000,0.07);
  //  main->cd(6)->SetGridx(1);
  grF4[0]->Draw("*LSAME");
  //grF4[0]->SetTitle("f4");
  grF4[0]->GetYaxis()->SetRangeUser(0.0,0.10);
  grF4[0]->GetXaxis()->SetNdivisions(515);
  //grF4[0]->GetXaxis()->SetLabelSize(0.05);
  for(int k=1; k!=4; ++k) grF4[k]->Draw("*LSAME");
  line->DrawLine(0,0.04,50000,0.04);

  main->cd(2)->SetGridx(1);
  grB[0]->Draw("A*L");
  grB[0]->SetTitle("Bgr");
  grB[0]->GetYaxis()->SetRangeUser(0,5e7);
  grB[0]->GetXaxis()->SetNdivisions(515);
  //grB[0]->GetXaxis()->SetLabelSize(0.05);
  for(int k=1; k!=4; ++k) grB[k]->Draw("*LSAME");

  main->cd(3)->SetGridx(1);
  grSL[0]->Draw("A*L");
  grSL[0]->SetTitle("Slope");
  grSL[0]->GetYaxis()->SetRangeUser(-1,-0.2);
  grSL[0]->GetXaxis()->SetNdivisions(515);
  //grSL[0]->GetXaxis()->SetLabelSize(0.05);
  for(int k=1; k!=4; ++k) grSL[k]->Draw("*LSAME");
  line->DrawLine(0,-0.4,50000,-0.4);

  main->cd(8)->SetGridx(1);
  main->cd(8)->SetGridy(1);
  grMIP[0]->Draw("A*L");
  grMIP[0]->SetTitle("MPV");
  grMIP[0]->GetYaxis()->SetRangeUser(9,30);
  grMIP[0]->GetXaxis()->SetNdivisions(515);
  //grMIP[0]->GetXaxis()->SetLabelSize(0.05);
  for(int k=1; k!=4; ++k) grMIP[k]->Draw("*LSAME");

  main->cd(9)->SetGridx(1);
  grSGM[0]->Draw("A*L");
  grSGM[0]->SetTitle("SGM");
  grSGM[0]->GetYaxis()->SetRangeUser(1.5,5.0);
  grSGM[0]->GetXaxis()->SetNdivisions(515);
  //grSGM[0]->GetXaxis()->SetLabelSize(0.05);
  for(int k=1; k!=4; ++k) grSGM[k]->Draw("*LSAME");
  line->DrawLine(0,3.0,50000,3.0);


  //main->cd(2); grSL->Draw("A*"); grSL->SetTitle("Slope");
  //main->cd(3); grMIP->Draw("A*"); grMIP->SetTitle("MPV");
  //main->cd(4); grSGM->Draw("A*"); grSGM->SetTitle("SGM");
  return 0;
}
