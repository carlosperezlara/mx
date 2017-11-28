#include "mxCalibBaseSiW.h"
#include "mxCalibBaseSiW.cxx"
#include "mxCalibStageSiW.h"
#include "mxCalibStageSiW.cxx"
#include "mxCalibBasePbWO4.h"
#include "mxCalibBasePbWO4.cxx"
#include "mxCalibMaster.h"
#include "mxCalibMaster.cxx"
#include "mxDB.cc"
#include "mxGeometry.h"
#include "mxGeometry.cxx"
#include <algorithm>

mxGeometry *fGeo = mxGeometry::Instance();

int showLayer(int lyr=0) {
  gStyle->SetOptStat(0);
  bool bad[49152];
  mxCalibMaster *cal = new mxCalibMaster();
  mxDB::read(454934,cal);
  for(int i=0; i!=49152; ++i) {
    bad[i] = false;
    if( cal->IsBadKey(i) ) bad[i] = true;
  }

  TH2Poly *lyr_amp = new TH2Poly(Form("AMP_LYR%0d",lyr),"INTEGRATED SIGNAL (x1E8)",-22,+22,-22,+22);
  TH2Poly *lyr_mpv = new TH2Poly(Form("MPV_LYR%0d",lyr),"LANDAU MPV",-22,+22,-22,+22);
  TH2Poly *lyr_sgm = new TH2Poly(Form("SGM_LYR%0d",lyr),"LANDAU SIGMA",-22,+22,-22,+22);
  TH2Poly *lyr_ped = new TH2Poly(Form("PED_LYR%0d",lyr),"PEDESTAL RESIDUAL",-22,+22,-22,+22);
  Double_t x1[4], y1[4];
  for(int k=0; k!=49152; ++k) {
    if( fGeo->PS_Idx2Lyr(k) != lyr ) continue;
    int sl = fGeo->PS_Idx2Sen(k);
    float a0 = fGeo->Si_a0();
    float a1 = fGeo->Si_a1();
    float x = fGeo->X( k );
    float y = fGeo->Y( k );
    if(lyr%2==0) {
      x1[0] = x - a0/2;
      x1[1] = x + a0/2;
      x1[2] = x + a0/2;
      x1[3] = x - a0/2;
      y1[0] = y - a1/2;
      y1[1] = y - a1/2;
      y1[2] = y + a1/2;
      y1[3] = y + a1/2;
    } else {
      x1[0] = x - a1/2;
      x1[1] = x + a1/2;
      x1[2] = x + a1/2;
      x1[3] = x - a1/2;
      y1[0] = y - a0/2;
      y1[1] = y - a0/2;
      y1[2] = y + a0/2;
      y1[3] = y + a0/2;
    }
    lyr_amp->AddBin(4,x1,y1);
    lyr_mpv->AddBin(4,x1,y1);
    lyr_sgm->AddBin(4,x1,y1);
    lyr_ped->AddBin(4,x1,y1);
  }
  for(int k=0; k!=49152; ++k) {
    if( fGeo->PS_Idx2Lyr(k) != lyr ) continue;
    float x = fGeo->X(k);
    float y = fGeo->Y(k);
    if( bad[k] ) {
      lyr_ped->Fill(x,y,-20);
      continue;
    }
    int sen = k/128;
    float amp = 0;
    float mpv = 0;
    float sgm = 0;
    float ped = -30;
    float tmp;
    TString dir = Form("out/SEN%03d/fit",sen);
    ifstream fin;
    //=====
    fin.open( Form("%s/P%05d.dat",dir.Data(),k) );
    fin >> tmp;
    if(fin.good()) {
      ped = tmp;
    }
    fin.close();
    //=====
    fin.open( Form("%s/H%05d.dat",dir.Data(),k) );
    fin >> tmp;
    if(fin.good()) {
      amp = tmp;
      fin >> tmp >> mpv >> tmp >> sgm;
    }
    fin.close();
    //=====
    if(amp<1e-8) {
      ped = -30;
    }
    //=====
    lyr_amp->Fill( fGeo->X(k), fGeo->Y(k), amp*1E-8 );
    lyr_mpv->Fill( fGeo->X(k), fGeo->Y(k), mpv );
    lyr_sgm->Fill( fGeo->X(k), fGeo->Y(k), sgm );
    lyr_ped->Fill( fGeo->X(k), fGeo->Y(k), ped );
  }
  float max = lyr_amp->GetMaximum();
  lyr_amp->GetZaxis()->SetRangeUser(0,max);
  if(lyr<2) {
    lyr_mpv->GetZaxis()->SetRangeUser(15,28);
  } else {
    lyr_mpv->GetZaxis()->SetRangeUser(10,18);
  }
  lyr_sgm->GetZaxis()->SetRangeUser(2,5);
  lyr_ped->GetZaxis()->SetRangeUser(-10,+10);

  TCanvas *main = new TCanvas( Form("LYR%0d",lyr), Form("LYR%0d",lyr), 1600, 1600 );
  main->Divide(2,2);
  main->cd(1);
  lyr_amp->Draw("COLZ");
  main->cd(2);
  lyr_mpv->Draw("COLZ");
  main->cd(3);
  lyr_ped->Draw("COLZ");
  main->cd(4);
  lyr_sgm->Draw("COLZ");

  main->SaveAs( Form("LYR%02d.pdf",lyr), "PDF" );


  new TCanvas("AAA","AAA",800,800);
  lyr_amp->Draw("COL");
  return 0;
}
