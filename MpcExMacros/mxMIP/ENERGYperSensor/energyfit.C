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
#include "mxCalibStageSiW.h"
#include "mxCalibStageSiW.cxx"
#include "mxCalibBasePbWO4.h"
#include "mxCalibBasePbWO4.cxx"
#include "mxCalibMaster.h"
#include "mxCalibMaster.cxx"
#include "mxDB.cc"

double pedval=0;

TF1* GetFit(TString fout, double xinit) {
  const char *bgr = Form("[6]*TMath::Exp([7]*(x-%f))",xinit);
  const char *la1 = Form("(1-[3]-[4]-[5])*TMath::Landau(x-%f,[1],[2],1)",pedval);
  const char *la2 = Form("[3]*TMath::Landau(x-%f,2*[1]+1.4*[2],2.0*[2],1)",pedval);
  const char *la3 = Form("[4]*TMath::Landau(x-%f,3*[1]+3.3*[2],3.0*[2],1)",pedval);
  const char *la4 = Form("[5]*TMath::Landau(x-%f,4*[1]+5.6*[2],4.0*[2],1)",pedval);
  TF1 *ret = new TF1("fit_H1",
                     Form("[0]*(%s+%s+%s+%s)+%s",la1,la2,la3,la4,bgr) );
  ret->SetParName(0,"A");
  ret->SetParName(1,"lambda");
  ret->SetParName(2,"sigma");
  ret->SetParName(3,"f2");
  ret->SetParName(4,"f3");
  ret->SetParName(5,"f4");
  ret->SetParName(6,"B");
  ret->SetParName(7,"b");
  ret->SetParameter(0,5e7);   ret->SetParLimits(0,1e3,1e10);
  ret->SetParameter(1,0.147); ret->SetParLimits(1,0.130,0.170);
  ret->SetParameter(2,0.010); ret->SetParLimits(2,0.001,0.050);
  ret->SetParameter(3,0.22); ret->SetParLimits(3,1,-1);
  ret->SetParameter(4,0.07); ret->SetParLimits(4,1,-1);
  ret->SetParameter(5,0.04); ret->SetParLimits(5,1,-1);
  ret->SetParameter(6,1e8);  ret->SetParLimits(6,1e5,1e10);
  ret->SetParameter(7,-0.4); ret->SetParLimits(7,-80.0,-0.1);
  ret->SetLineColor(kRed-3);

  if(1) {
  double tmp;
  ifstream infit;
  infit.open( fout.Data() );
  bool found=false;
  for(int n=0; n!=ret->GetNpar(); ++n) {
    infit >> tmp;
    if(!infit.good()) break;
    ret->SetParameter(n,tmp);
    infit >> tmp;
    found = true;
  }
  infit.close();
  if(found) {
    ret->SetParameter(3,0.22); ret->SetParLimits(3,0,0.4);
    ret->SetParameter(4,0.07); ret->SetParLimits(4,0,0.4);
    ret->SetParameter(5,0.04); ret->SetParLimits(5,0,0.4);
  }
  }

  return ret;
}

void fitme(TH1F *out, TString fout, float xfit_min, float xfit_max) {
  // fit
  TF1 *fitH = GetFit( fout.Data(), xfit_min);
  out->Fit(fitH,"MELR","",xfit_min,xfit_max);

  double amp = fitH->GetParameter(0);
  double eamp= fitH->GetParError(0);
  double lda = fitH->GetParameter(1);
  double elda= fitH->GetParError(1);
  double sg1 = fitH->GetParameter(2);
  double esg1= fitH->GetParError(2);
  double fr2 = fitH->GetParameter(3);
  double efr2= fitH->GetParError(3);
  double fr3 = fitH->GetParameter(4);
  double efr3= fitH->GetParError(4);
  double fr4 = fitH->GetParameter(5);
  double efr4= fitH->GetParError(5);
  double fr1 = 1 - fr2 - fr3 - fr4;
  double ncs = fitH->GetChisquare()/fitH->GetNDF();
  double ba = fitH->GetParameter(6);
  double eba= fitH->GetParError(6);
  double bsl = fitH->GetParameter(7);
  double ebsl= fitH->GetParError(7);

  cout << "chi2 " << fitH->GetChisquare() << endl; 
  cout << "ndf " << fitH->GetNDF() << endl; 
  cout << "prob " << fitH->GetProb() << endl; 

  // saving fit
  ofstream outfit;
  outfit.open( fout.Data() );
  outfit << amp << " " << eamp << endl;
  outfit << lda << " " << elda << endl;
  outfit << sg1 << " " << esg1 << endl;
  outfit << fr2 << " " << efr2 << endl;
  outfit << fr3 << " " << efr3 << endl;
  outfit << fr4 << " " << efr4 << endl;
  outfit << ba  << " " << eba  << endl;
  outfit << bsl << " " << ebsl << endl;
  outfit << ncs << endl;
  outfit.close();
  cout << "Parameters saved to ";
  cout << fout.Data() << endl;
  return;
}

int energyfit(int sen, int run, float xminovw=-1) {
  TString dir = Form("out/SEN%03d/",sen);
  TString in = Form("%s/H%05d.root",dir.Data(),run);
  TString ou = Form("%s/fit/H%05d.dat",dir.Data(),run);
  gSystem->Exec( Form("mkdir -p %s/fit",dir.Data()) );
  float pxmin=0.1;
  float pxmax=1.0;
  if(xminovw>-0.5) {
    ofstream finitovw( Form("%s/fit/I%03d.dat",dir.Data(),sen) );
    finitovw << xminovw + pedval << endl;
    finitovw.close();
  }
  float tmp;
  ifstream finit( Form("%s/fit/I%03d.dat",dir.Data(),sen) );
  finit >> tmp;
  if(finit.good()) {
    cout << "INIT FILE" << endl;
    pxmin = tmp;
  }
  finit.close();
  cout << "NEW XMIN: " << pxmin << endl;
  cout << "NEW XMAX: " << pxmax << endl;

  TFile *file = new TFile( in.Data() );
  //file->ls();
  TH1F *hist = (TH1F*) file->Get( Form("SEN_%d",sen) );
  hist->SetMarkerStyle(20);

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

  fitme(hist,ou,xfit_min,xfit_max);
  hist->GetXaxis()->SetRangeUser(0,1.0);
  return 0;
}

int energyfitrun(int run=0,float xmin=0.065) {
  for(int sen=0; sen!=192; ++sen) {
    energyfit(sen,run,xmin);
  }
  return 0;
}

int energyfitsen(int sen=0,float xmin=0.065) {
  ifstream fin("runs.dat.final");
  int run;
  for(;;) {
    fin >> run;
    if(!fin.good()) break;
    energyfit(sen,run,xmin);
  }
  return 0;
}
