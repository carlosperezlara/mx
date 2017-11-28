#include "TFile.h"
#include "TH1D.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TString.h"
#include "TSystem.h"

#include <iostream>
#include <fstream>

#include "mxCalibBaseSiW.h"
#include "mxCalibBaseSiW.cxx"
#include "mxCalibBasePbWO4.h"
#include "mxCalibBasePbWO4.cxx"
#include "mxCalibMaster.h"
#include "mxCalibMaster.cxx"
#include "mxDB.cc"

TF1* GetFit(TString fout, int hgain, double xinit, double xfin) {
  TF1 *ret = new TF1("fit_H1", "gaus");
  ret->SetParName(0,"A");
  ret->SetParName(1,"MEAN");
  ret->SetParName(2,"SIGMA");
  //ret->SetParameter(0,5e7); ret->SetParLimits(0,1e3,1e10);
  ret->SetParameter(1,0);  ret->SetParLimits(1,xinit,xfin);
  ret->SetParameter(2,1.0); ret->SetParLimits(2,0.1,2.5);
  ret->SetLineColor(kRed-3);

  ifstream infit;
  infit.open( fout.Data() );
  double tmp;
  bool found=false;
  for(int n=0; n!=ret->GetNpar(); ++n) {
    infit >> tmp;
    if(!infit.good()) break;
    ret->SetParameter(n,tmp);
    infit >> tmp;
    found = true;
  }
  infit.close();
  return ret;
}

void fitme(TH1F *out, TString fout, float xfit_min, float xfit_max, int ppp) {
  // fit
  TF1 *fitH = GetFit( fout.Data(), ppp, xfit_min, xfit_max);
  out->Fit(fitH,"MELIR","",xfit_min,xfit_max);

  double amp = fitH->GetParameter(0);
  double eamp= fitH->GetParError(0);
  double mea = fitH->GetParameter(1);
  double emea= fitH->GetParError(1);
  double sgm = fitH->GetParameter(2);
  double esgm= fitH->GetParError(2);

  cout << "chi2 " << fitH->GetChisquare() << endl; 
  cout << "ndf " << fitH->GetNDF() << endl; 
  cout << "prob " << fitH->GetProb() << endl; 

  double hmean = out->GetMean();
  double hrms = out->GetRMS();

  // saving fit
  ofstream outfit;
  outfit.open( fout.Data() );
  outfit << mea << endl;
  outfit << sgm << endl;
  outfit << hmean << endl;
  outfit << hrms << endl;
  outfit.close();
  cout << "Parameters saved to ";
  cout << fout.Data() << endl;
  return;
}

int pedfit(int key, float xmin=-1000, float xmax=-1000) {
  cout << "================= " << key << " =================" << endl;
  mxCalibMaster *cal = new mxCalibMaster();
  mxDB::read(454934,cal);
  if(cal->IsBadKey(key)) {
    return 1;
  }

  int sen = key/128;
  TString dir = Form("out/SEN%03d/",sen);
  TString in = Form("%s/H%05d.root",dir.Data(),key);
  TString ou = Form("%s/fit/P%05d.dat",dir.Data(),key);
  gSystem->Exec( Form("mkdir -p %s/fit",dir.Data()) );
  gSystem->Exec( Form("mkdir -p %s/fitped",dir.Data()) );
  float pxmin=-10;
  float pxmax=+10;

  if(xmin>-100 && xmax>-100) {
    cout << "Overwriting pedestal range file with arguments provided" << endl;
    ofstream fovw( Form("%s/fitped/I%05d.dat",dir.Data(),key) );
    fovw << xmin << " " << xmax << endl;
    fovw.close();
  }

  float tmp;
  ifstream finit( Form("%s/fitped/I%05d.dat",dir.Data(),key) );
  finit >> tmp;
  if(finit.good()) {
    cout << "INIT FILE" << endl;
    pxmin = tmp;
  }
  finit >> tmp;
  if(finit.good()) {
    pxmax = tmp;
  }
  cout << "NEW XMIN: " << pxmin << endl;
  cout << "NEW XMAX: " << pxmax << endl;

  TFile *file = new TFile( in.Data() );
  //file->ls();
  TH1F *hist = (TH1F*) file->Get( Form("ADC_%d",key) );
  hist->SetMarkerStyle(20);
  //hist->Draw("hist");
  int bmin = hist->GetXaxis()->FindBin(pxmin);
  int bmax = hist->GetXaxis()->FindBin(pxmax);
  double xfit_min=hist->GetBinLowEdge( bmin );
  double xfit_max=hist->GetBinLowEdge( bmax+1 );
  double entries = hist->Integral(bmin,bmax);

  cout << bmin << " " << bmax << endl;
  if(entries<10000) {
    cout << "not enough entries: ";
    cout << entries << endl;
    return 0;
  }

  hist->GetXaxis()->SetRange(bmin,bmax);
  fitme(hist,ou,xfit_min,xfit_max,0);
  float frange = xfit_max - xfit_min;
  hist->GetXaxis()->SetRangeUser(xfit_min-0.5*frange,xfit_max+2*frange);
  return 0;
}

int pedfitsen(int sen=0, float xmin=-1000, float xmax=-1000) {
  int kini = sen*128;
  int kfin = (sen+1)*128;
  for(int i=kini; i!=kfin; ++i) {
    pedfit(i,xmin,xmax);
  }
  return 0;
}

int pedfitsenrow(int sen=0, int row=0, float xmin=-1000, float xmax=-1000, int sec=3) {
  int kini = sen*128;
  int kfin = (sen+1)*128;
  for(int i=kini; i!=kfin; ++i) {
    if(i%4!=row) continue;
    if( (sec==1) && (i>=kini+64) ) continue;
    if( (sec==2) && (i<kini+64) ) continue;
    pedfit(i,xmin,xmax);
  }
  return 0;
}
