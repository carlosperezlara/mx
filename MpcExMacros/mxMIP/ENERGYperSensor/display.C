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

float ncs;
double pedval=0.0;

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
  ret->SetLineColor(kRed-3);

  float tmp;
  ifstream infit;
  infit.open( fout.Data() );
  cout << "OPENING  " << fout.Data() << endl;
  bool found=false;
  for(int n=0; n!=ret->GetNpar(); ++n) {
    infit >> tmp;
    if(!infit.good()) break;
    ret->SetParameter(n,tmp);
    infit >> tmp;
    found = true;
  }
  infit >> ncs;
  infit.close();
  return ret;
}

TF1* GetMIP(TF1 *fit, int n, int color=kBlue-3) {
  double a = fit->GetParameter(0);
  double b = fit->GetParameter(1);
  double c = fit->GetParameter(2);
  switch(n) {
  case(1):
    a *= (1-fit->GetParameter(3)-fit->GetParameter(4)-fit->GetParameter(5));
    break;
  case(2):
    a *= fit->GetParameter(3);
    b *= 2.14;
    c *= 2;
    break;
  case(3):
    a *= fit->GetParameter(4);
    b *= 3.33;
    c *= 3;
    break;
  case(4):
    a *= fit->GetParameter(5);
    b *= 4.55;
    c *= 4;
    break;
  }
  TF1 *ret = new TF1(Form("MIP%d",n), Form("%f*TMath::Landau(x-%f,%f,%f,1)",a,pedval,b,c),0,150);
  ret->SetLineColor(color);
  return ret;
}

TF1* GetBGR(TF1 *fit, double xinit, int color=kGray) {
  double a = fit->GetParameter(6);
  double b = fit->GetParameter(7);
  TF1 *ret = new TF1("BGR", Form("%f*TMath::Exp(%f*(x-%f))",a,b,xinit),0,150);
  ret->SetLineColor(color);
  return ret;
}

int display(int sen=0, int run=0) {
  TString dir = Form("out/SEN%03d/",sen);
  TString in = Form("%s/H%05d.root",dir.Data(),run);
  TString ou = Form("%s/fit/H%05d.dat",dir.Data(),run);

  TFile *file = new TFile( in.Data() );
  TH1F *hist = (TH1F*) file->Get( Form("SEN_%d",sen) );
  hist->SetMarkerStyle(20);
  hist->GetXaxis()->SetRangeUser(0,75);

  /*
  mxCalibMaster *cal = new mxCalibMaster();
  mxDB::read(454934,cal);
  if(cal->IsBadKey(key)) {
    return 1;
  }
  */
  float pxmin=0.1;
  float pxmax=1.0;
  float tmp;

  ifstream finit( Form("%s/fit/I%03d.dat",dir.Data(),sen) );
  finit >> tmp;
  if(finit.good()) {
    cout << "INIT FILE"<< endl;
    pxmin = tmp;
  }
  finit >> tmp;
  if(finit.good()) {
    pxmax = tmp;
  }

  cout << "NEW XMIN: " << pxmin << endl;
  cout << "NEW XMAX: " << pxmax << endl;

  int bmin = hist->GetXaxis()->FindBin(pxmin);
  int bmax = hist->GetXaxis()->FindBin(pxmax);
  double xfit_min=hist->GetBinLowEdge( bmin );
  double xfit_max=hist->GetBinLowEdge( bmax+1 );
  double entries = hist->Integral(bmin,bmax);

  TF1 *fitH = GetFit( ou.Data(), xfit_min );
  TF1 *MIPH1 = GetMIP(fitH,1,kCyan-3);
  TF1 *MIPH2 = GetMIP(fitH,2,kGreen-3);
  TF1 *MIPH3 = GetMIP(fitH,3,kOrange-3);
  TF1 *MIPH4 = GetMIP(fitH,4,kMagenta-3);
  TF1 *BGR = GetBGR(fitH,xfit_min);
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
  //double ncs = fitH->GetChisquare()/fitH->GetNDF();
  double ba = fitH->GetParameter(6);
  double eba= fitH->GetParError(6);
  double bsl = fitH->GetParameter(7);
  double ebsl= fitH->GetParError(7);

  // draw
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);

  TCanvas *main = new TCanvas("main","main");
  hist->Draw("HE");
  double ymax = hist->GetBinContent( hist->FindBin(pxmin) ) * 1.5;
  hist->GetYaxis()->SetRangeUser(0.5,ymax);
  hist->GetXaxis()->SetRangeUser(0,pxmax);
  hist->SetLineColor(kBlack);
  hist->SetMarkerStyle(20);
  hist->SetTitle("");
  hist->GetXaxis()->SetTitle("ADC-PED (a.u.)");
  BGR->Draw("SAME");
  MIPH1->Draw("SAME");
  MIPH2->Draw("SAME");
  MIPH3->Draw("SAME");
  MIPH4->Draw("SAME");
  TArrow *arrow = new TArrow;
  arrow->DrawArrow(pedval,0,pedval,1000);
  fitH->SetRange(xfit_min,xfit_max);
  fitH->Draw("SAME");
  TLatex *text = new TLatex();
  text->DrawLatex(0, (1.03*(ymax)), in.Data() );
  text->DrawLatex(0.250, (0.83*(ymax)), Form("Entries  %.0f",entries) );
  //text->DrawLatex(30, (0.73*(ymax)), Form("State  %d",state) );
  text->DrawLatex(0.250, (0.53*(ymax)), Form("#lambda  %.3f #pm %.1f",lda,elda) );
  text->DrawLatex(0.250, (0.43*(ymax)), Form("#sigma  %.3f #pm %.1f",sg1,esg1) );
  text->SetTextColor(kRed-3);
  text->DrawLatex(0.250, (0.63*(ymax)), Form("#Chi^{2} / NDF  %.2f",ncs) );
  text->SetTextColor(kBlue-3);
  text->DrawLatex(0.650, (0.73*(ymax)), Form("#Alpha  %.1f M #pm %.0f",fitH->GetParameter(0)*1e-6,fitH->GetParError(0)) );
  text->SetTextColor(kCyan-3);
  text->DrawLatex(0.650, (0.63*(ymax)), Form("f_{1}  %.2f",fr1) );
  text->SetTextColor(kGreen-3);
  text->DrawLatex(0.650, (0.53*(ymax)), Form("f_{2}  %.2f #pm %.2f",fr2,efr2) );
  text->SetTextColor(kOrange-3);
  text->DrawLatex(0.650, (0.43*(ymax)), Form("f_{3}  %.2f #pm %.2f",fr3,efr3) );
  text->SetTextColor(kMagenta-3);
  text->DrawLatex(0.650, (0.33*(ymax)), Form("f_{4}  %.2f #pm %.2f",fr4,efr4) );
  text->SetTextColor(kGray);
  text->DrawLatex(0.650, (0.83*(ymax)), Form("b  %.2f",bsl) );
  text->SetTextColor(kBlack);
  text->SetTextSize(0.035);
  text->SetTextColor( kRed-3 );
  text->DrawLatex(0.250, (0.93*(ymax)), "e^{bx} + #Alpha ( f_{1} L_{1}(x) + f_{2} L_{2}(x) + f_{3} L_{3}(x) + f_{4} L_{4}(x))");
  //main->SaveAs( Form("%s.eps",fout.Data()), "eps" );
  return 0;
}
