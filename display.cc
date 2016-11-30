#include <cstdlib>
#include <iostream>
#include <fstream>

#include "TCanvas.h"
#include "TMath.h"
#include "TEllipse.h"
#include "TH2F.h"
#include "TError.h"
#include "TString.h"
#include "TBox.h"
#include "TStyle.h"
#include "TLatex.h"

#include "mxGeometry.h"
#include "phMath.h"

mxGeometry *geo = new mxGeometry();

int nhit;
int hitlayer[18];
float hit_kk[52000];
float hit_ee[52000];

int npty;
int ptylayer[18];
int pty_ll[52000];
float pty_xx[52000];
float pty_yy[52000];
float pty_sgnP[52000];
float pty_sx[52000];
float pty_sy[52000];
float pty_sxy[52000];
float pty_spx[52000];
float pty_spy[52000];

float z[18];
float ez[18];

TH2F *axis = new TH2F("axis",";X;Y",10,-22,+22,10,-22,+22);
TH2F *phietaS = new TH2F("phieta0","S;ETA;PHI",100,-4,-3,100,0,TMath::TwoPi());
TH2F *phietaN = new TH2F("phieta1","N;ETA;PHI",100,+3,+4,100,0,TMath::TwoPi());
TCanvas *cmain = new TCanvas("main","main",600,600);
TEllipse *lips = new TEllipse;
TLatex *tex = new TLatex();
TBox *box = new TBox();
int col[9] = {kRed-3, kOrange-3, kYellow-3, kGreen-3, kCyan-3, kBlue-3, kMagenta-3, kAzure-3, kBlack};
int heat[4] = { kBlue-10, kBlue-6, kBlue-2, kBlue+3 };
//int heat[4] = { kOrange-9, kOrange, kOrange-3, kOrange+7 };

void drawMiniPad(int key, float height=-1, float kol=kGray) {
  int tcol = height>0.75 ? 3: height>0.5 ? 2 : height>0.25 ? 1 : 0;
  if(height<0||height>1) box->SetFillColor( kol );
  else box->SetFillColor( heat[tcol] );
  float dx, dy;
  if((geo->LyrIdx(key)%9)%2==0) {
    dx = geo->Si_a0()/2;
    dy = geo->Si_a1()/2;
  } else {
    dx = geo->Si_a1()/2;
    dy = geo->Si_a0()/2;
  }
  float x0 = geo->X(key) - dx;
  float y0 = geo->Y(key) - dy;
  float x1 = geo->X(key) + dx;
  float y1 = geo->Y(key) + dy;
  box->DrawBox( x0,y0, x1,y1 );
}

void drawCrystal(int key, float height=-1, float kol=kGray) {
  if( TMath::Abs(geo->Z(49152+key)) < 10 ) return;
  int tcol = height>0.75 ? 3: height>0.5 ? 2 : height>0.25 ? 1 : 0;
  if(height<0||height>1) box->SetFillColor( kGray );
  else box->SetFillColor( heat[tcol] );
  float x0 = geo->X(49152+key) - geo->PWO4_a0()/2;
  float y0 = geo->Y(49152+key) - geo->PWO4_a1()/2;
  float x1 = geo->X(49152+key) + geo->PWO4_a0()/2;
  float y1 = geo->Y(49152+key) + geo->PWO4_a1()/2;
  box->DrawBox( x0,y0, x1,y1 );
}

void drawHit(int key, float height) {
  if(key<49152) drawMiniPad(key,height);
  else drawCrystal(key-49152,height);
}

void drawEX(int lyr) {
  int a;
  a = geo->RefKey(lyr,0);
  for(int i=a; i!=a+12*128; ++i) drawMiniPad(i,-1,kYellow-10);
  a = geo->RefKey(lyr,1);
  for(int i=a; i!=a+12*128; ++i) drawMiniPad(i,-1,kYellow-9);
}

void drawMPC(int sn) {
  box->SetFillColor( kGray );
  int a = 0;
  int b = 288;
  if(sn>0) {
    a = 288;
    b = 576;
  }
  for(int i=a; i!=b; ++i) drawCrystal(i);
}

void drawHitLayer(int lyridx) {
  float max = 0;
  for(int i=0; i!=nhit; ++i) {
    int key = hit_kk[i];
    if(geo->LyrIdx(key)!=lyridx) continue;
    float ene = hit_ee[i];
    if(ene>max) max=ene;
  }
  if(max<1e-6) return;
  for(int i=0; i!=nhit; ++i) {
    int key = hit_kk[i];
    if(geo->LyrIdx(key)!=lyridx) continue;
    float ene = hit_ee[i] / max;
    drawHit(key,ene);
  }
}

void drawPtyLayer(int lyridx) {
  for(int i=0; i!=npty; ++i) {
    int ll = pty_ll[i];
    if(ll!=lyridx) continue;
    lips->SetLineColor( kRed-3 );
    lips->SetFillColorAlpha(kRed-3,0.1);
    lips->DrawEllipse(pty_xx[i], pty_yy[i], pty_spx[i], pty_spy[i], 0, 360, 0);
  }
}

/*
void drawSummary(int arm) {
  if(arm==0) phietaS->Draw();
  else phietaN->Draw();
  for(int i=0; i!=npty; ++i) {
    lips->SetLineColor( col[ll] );
    float ephi, eeta;
    float phi = _phi( xx, yy, ephi, spx, spy );
    float eta = _eta( xx, yy, z[ll], eeta, spx, spy, ez[ll] );
    lips->DrawEllipse(eta,phi,eeta,ephi,0,360,0);
  }
  tex->SetTextColor(col[0]); tex->DrawLatex(-3.9,5.5,Form("%d",npt[0]));
  tex->SetTextColor(col[1]); tex->DrawLatex(-3.9,4.5,Form("%d",npt[1]));
  tex->SetTextColor(col[2]); tex->DrawLatex(-3.9,3.0,Form("%d",npt[2]));
  tex->SetTextColor(col[3]); tex->DrawLatex(-3.9,1.5,Form("%d",npt[3]));
  tex->SetTextColor(col[4]); tex->DrawLatex(-3.9,0.5,Form("%d",npt[4]));
  tex->SetTextColor(col[5]); tex->DrawLatex(-3.2,5.5,Form("%d",npt[5]));
  tex->SetTextColor(col[6]); tex->DrawLatex(-3.2,4.5,Form("%d",npt[6]));
  tex->SetTextColor(col[7]); tex->DrawLatex(-3.2,1.5,Form("%d",npt[7]));
  tex->SetTextColor(col[8]); tex->DrawLatex(-3.2,0.5,Form("%d",npt[8]));
}
*/
void drawLayer(int lyridx) {
  axis->Draw();
  if(lyridx==8) {
    drawMPC(0);
  } else if (lyridx==17) {
    drawMPC(1);
  } else {
    drawEX(lyridx);
  }
  drawHitLayer(lyridx);
  drawPtyLayer(lyridx);
  tex->SetTextColor(kBlue-3);
  tex->DrawLatex( 12, 18.5, Form("Hts: %d",hitlayer[lyridx]) );
  tex->SetTextColor(kRed-3);
  tex->DrawLatex( 12, 16.5, Form("Pts: %d",ptylayer[lyridx]) );
}

int main(int argn, char **argc) {
  gStyle->SetOptStat(0);
  // input file
  TString file = "input";
  if(argn>1) file = argc[1];
  // event selection
  TString aa;
  int eventnumber = -1;
  if(argn>2) {
    aa = argc[2];
    eventnumber = aa.Atoi();
  }
  std::cout << "INPUT: " << Form("%s.hit",file.Data()) << std::endl;
  std::cout << "INPUT: " << Form("%s.pty",file.Data()) << std::endl;
  std::ifstream input( Form("%s.hit",file.Data()) );
  std::ifstream input2( Form("%s.pty",file.Data()) );
  gErrorIgnoreLevel = kWarning;

  cmain->SaveAs( Form("%s_disp.pdf[",file.Data()),"pdf");

  for(int i=0;i!=18;++i) ez[i] = geo->Si_a2();
  ez[8] = geo->PWO4_a2();
  ez[17] = geo->PWO4_a2();
  for(int i=0;i!=18;++i) z[i] = geo->RZ(i);// + 0.5*ez[i];

  for(int ev=0;; ++ev) {
    nhit=0;
    npty=0;
    //reading hits
    input >> nhit;
    if(!input.good()) break;
    if(eventnumber>-1 && eventnumber!=ev) continue;
    for(int i=0; i!=18; ++i) hitlayer[i] = 0;
    for(int i=0; i!=nhit; ++i) {
      input >> hit_kk[i] >> hit_ee[i];
      hitlayer[ geo->LyrIdx(hit_kk[i]) ]++;
    }
    //reading parties
    for(int i=0; i!=18; ++i) ptylayer[i] = 0;
    input2 >> npty;
    if(!input2.good()) npty = 0;
    for(int i=0; i!=npty; ++i) {
      input2 >> pty_ll[i] >> pty_xx[i] >> pty_yy[i] >> pty_sgnP[i];
      input2 >> pty_sx[i] >> pty_sy[i] >> pty_sxy[i] >> pty_spx[i] >> pty_spy[i];
      ptylayer[ pty_ll[i] ]++;
    }
    // draw south
    cmain->Divide(3,3,0,0);
    for(int i=0; i!=9; ++i) {
      cmain->cd(i+1);
      drawLayer(i);
    }
    cmain->SaveAs(Form("%s_disp.pdf",file.Data()),"pdf");
    for(int i=9; i!=18; ++i) {
      cmain->cd(i-8);
      drawLayer(i);
    }
    cmain->SaveAs(Form("%s_disp.pdf",file.Data()),"pdf");
    //cmain->cd(10);
    //drawSummary(0);
    if(eventnumber>-1 && eventnumber!=ev) break;
  }
  input.close();
  input2.close();
  cmain->SaveAs(Form("%s_disp.pdf]",file.Data()),"pdf");
  std::cout << "OUTPUT: " << Form("%s_disp.pdf",file.Data()) << std::endl;
  return 0;
}
