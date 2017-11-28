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

int GB_KEY;
int GB_FORCE;
float GB_MPV01;
float GB_MPV23;
float GB_SGM;
float GB_F2;
float GB_F3;
float GB_F4;
float GB_SLOPE;
double pedval=0;

TF1* GetFit(TString fout, TString fped, int hgain, double xinit) {
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
  ret->SetParameter(0,5e7); ret->SetParLimits(0,1e3,1e10);
  if(hgain>0) {
    ret->SetParameter(1,20);
  } else {
    ret->SetParameter(1,15);
  }
  ret->SetParLimits(1,9,40);
  ret->SetParameter(2,3.0); ret->SetParLimits(2,1.5,5.0);
  ret->SetParameter(3,0.2); ret->SetParLimits(3,0,0.40);
  ret->SetParameter(4,0.0);
  ret->SetParameter(5,0.0);
  ret->SetParameter(6,1e8); ret->SetParLimits(6,1e5,1e10);
  ret->SetParameter(7,-0.4); ret->SetParLimits(7,-2.0,-0.1);
  ret->SetLineColor(kRed-3);

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
    cout << " Previous fit results found" << endl;
    ret->SetParLimits(4,0,0.15);
    ret->SetParLimits(5,0,0.10);
  } else {
    cout << " Previous fit results NOT found" << endl;
    ret->SetParLimits(4,+1,-1);
    ret->SetParLimits(5,+1,-1);
  }
  if(GB_FORCE>0) {
    cout << " FORCING PARAMETERS lvl " << GB_FORCE << endl;
    ret->SetParameter(7,GB_SLOPE); ret->SetParLimits(7,+1,-1); // SLOPE
    if(GB_FORCE>1) {
      ret->SetParameter(3,GB_F2);    ret->SetParLimits(3,+1,-1); // F2
      ret->SetParameter(4,GB_F3);    ret->SetParLimits(4,+1,-1); // F3
      ret->SetParameter(5,GB_F4);    ret->SetParLimits(5,+1,-1); // F4
    }
    if(GB_FORCE>2) {
      ret->SetParameter(2,GB_SGM);   ret->SetParLimits(2,+1,-1); // SGM
      if((GB_KEY%4)<2) {
	ret->SetParameter(1,GB_MPV01);   ret->SetParLimits(1,+1,-1); // MPV01
      } else {
	ret->SetParameter(1,GB_MPV23);   ret->SetParLimits(1,+1,-1); // MPV23
      }
    }
  }
  ret->SetParameter(3,0.22); ret->SetParLimits(3,+1,-1);
  ret->SetParameter(4,0.07); ret->SetParLimits(4,+1,-1);
  ret->SetParameter(5,0.04); ret->SetParLimits(5,+1,-1);
  return ret;
}

void fitme(TH1F *out, TString fout, TString fped, float xfit_min, float xfit_max, int ppp) {
  // fit
  TF1 *fitH = GetFit( fout.Data(), fped.Data(), ppp, xfit_min);
  //out->Fit(fitH,"MELIR","",xfit_min,xfit_max);
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

int adcfit(int key, int force=0, float xminovw=-1) {
  GB_FORCE = force;
  GB_KEY = key;
  cout << "================= " << key << " =================" << endl;
  mxCalibMaster *cal = new mxCalibMaster();
  mxDB::read(454934,cal);
  if(cal->IsBadKey(key)) {
    return 1;
  }

  int sen = key/128;
  TString dir = Form("out/SEN%03d/",sen);
  TString in = Form("%s/H%05d.root",dir.Data(),key);
  TString ou = Form("%s/fit/H%05d.dat",dir.Data(),key);
  TString ouP = Form("%s/fit/P%05d.dat",dir.Data(),key);
  gSystem->Exec( Form("mkdir -p %s/fit",dir.Data()) );
  float pxmin=4.5;
  float pxmax=69.5;

  ifstream inped;
  inped.open( ouP.Data() );
  double tmp;
  inped >> tmp;
  if(inped.good()) {
    cout << "PEDESTAL FILE FOUND: " << ouP.Data() << endl;
    pedval = tmp;
  } else {
    pedval = 0; // important when running in groups!
  }
  cout << "Pedestal value set to " << pedval << endl;
  inped.close();

  float xinir[16][2];
  ifstream finits( "RANGE_PERPACKET.dat" );
  finits >> tmp;
  if(finits.good()) {
    xinir[0][0] = tmp;
    finits >> tmp;
    xinir[0][1] = tmp;
    for(int i=0; i!=15; ++i) {
      finits >> xinir[i+1][0] >> xinir[i+1][1];
    }
    pxmin = xinir[key/3072][0];
    pxmax = xinir[key/3072][1];
  }

  pxmin = 5*cal->GetPHSg()->Get(key) + 1.0;

  if(xminovw>-0.5) {
    ofstream finitovw( Form("%s/fit/I%05d.dat",dir.Data(),key) );
    finitovw << xminovw + pedval << endl;
    finitovw.close();
  }
  ifstream finit( Form("%s/fit/I%05d.dat",dir.Data(),key) );
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
  TH1F *hist = (TH1F*) file->Get( Form("ADC_%d",key) );
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

  fitme(hist,ou,ouP,xfit_min,xfit_max,0);
  hist->GetXaxis()->SetRangeUser(0,75);
  return 0;
}

int fit(int sen=0, int force=0, float sgm=3, float f2=0.2, float f3=0.4, float f4=0.2, float slope=-0.4, float mpv01=17, float mpv23=15) {
  GB_MPV01 = mpv01;
  GB_MPV23 = mpv23;
  GB_SGM = sgm;
  GB_F2 = f2;
  GB_F3 = f3;
  GB_F4 = f4;
  GB_SLOPE = slope;
  int kini = sen*128;
  int kfin = (sen+1)*128;
  for(int i=kini; i!=kfin; ++i) {
    adcfit(i,force);
  }
  return 0;
}

int adcfitsenrow(int sen=0, int row=0, float init=-1, int sec=3) {
  int kini = sen*128;
  int kfin = (sen+1)*128;
  for(int i=kini; i!=kfin; ++i) {
    if(i%4!=row) continue;
    if( (sec==1) && (i>=kini+64) ) continue;
    if( (sec==2) && (i<kini+64) ) continue;
    adcfit(i,0,init);
  }
  return 0;
}

int adcfitsen(int sen=0, float init=-1) {
  int kini = sen*128;
  int kfin = (sen+1)*128;
  for(int i=kini; i!=kfin; ++i) {
    adcfit(i,0,init);
  }
  return 0;
}

