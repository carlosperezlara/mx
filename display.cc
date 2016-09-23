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

void drawEX() {
  TBox *box = new TBox();
  for(int i=0; i!=24; ++i) {
    float x0 = geo->W_X(i) - geo->W_a0()/2;
    float y0 = geo->W_Y(i) - geo->W_a1()/2;
    float x1 = geo->W_X(i) + geo->W_a0()/2;
    float y1 = geo->W_Y(i) + geo->W_a1()/2;
    box->DrawBox( x0,y0, x1,y1 );
  }
}

void drawMPC(int sn) {
  TBox *box = new TBox();
  int a = 0;
  int b = 288;
  if(sn>0) {
    a = 288;
    b = 576;
  }
  for(int i=a; i!=b; ++i) {
    if( TMath::Abs(geo->Z(49152+i)) < 10 ) continue;
    float x0 = geo->X(49152+i) - geo->PWO4_a0()/2;
    float y0 = geo->Y(49152+i) - geo->PWO4_a1()/2;
    float x1 = geo->X(49152+i) + geo->PWO4_a0()/2;
    float y1 = geo->Y(49152+i) + geo->PWO4_a1()/2;
    box->DrawBox( x0,y0, x1,y1 );
  }
}

int main(int argn, char **argc) {
  gStyle->SetOptStat(0);
  TString file = "input";
  if(argn>1) {
    file = argc[1];
  }
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
  TH2F *aero[18];
  for(int i=0; i!=8; ++i) {
    if(i%2==0) {
      aero[i]   = new TH2F( Form("S%d",i), Form("S%d",i), 420, -22, +22, 105, -22, +22 );
      aero[9+i] = new TH2F( Form("N%d",i), Form("N%d",i), 420, -22, +22, 105, -22, +22 );
    } else {
      aero[i]   = new TH2F( Form("S%d",i), Form("S%d",i), 105, -22, +22, 420, -22, +22 );
      aero[9+i] = new TH2F( Form("N%d",i), Form("N%d",i), 105, -22, +22, 420, -22, +22 );
    }
  }
  aero[8]  = new TH2F( "S8", "S8", 120, -22, +22, 120, -22, +22 );
  aero[17] = new TH2F( "N8", "N8", 120, -22, +22, 120, -22, +22 );
  TH2F *phieta[2];
  phieta[0] = new TH2F("phieta0","S;ETA;PHI",100,-4,-3,100,0,TMath::TwoPi());
  phieta[1] = new TH2F("phieta1","N;ETA;PHI",100,+3,+4,100,0,TMath::TwoPi());
  TCanvas *main = new TCanvas("main","main",600,600);
  main->Divide(2,5,0,0);
  int plotarm = 0;
  TEllipse *lips = new TEllipse;
  main->SaveAs( Form("%s_disp.pdf[",file.Data()),"pdf");
  int hits, idx;
  float sgn, xx, yy;
  TLatex *tex = new TLatex();
  int col[9] = {kRed-3, kOrange-3, kYellow-3, kGreen-3, kCyan-3, kBlue-3, kMagenta-3, kAzure-3, kBlack};
  float z[18];
  float ez[18];
  for(int i=0;i!=18;++i) ez[i] = geo->Si_a2();
  ez[8] = geo->PWO4_a2();
  ez[17] = geo->PWO4_a2();
  for(int i=0;i!=18;++i) z[i] = geo->RZ(i);// + 0.5*ez[i];
  for(int ev=0;; ++ev) {
    input >> hits;
    if(!input.good()) break;
    if(eventnumber>-1 && eventnumber!=ev) continue;
    for(int i=0; i!=9; ++i) {
      aero[i]->Reset();
      aero[9+i]->Reset();
    }
    for(int i=0; i!=hits; ++i) {
      input >> idx >> sgn;// >> xx >> yy;
      float x = geo->X(idx);
      float y = geo->Y(idx);
      //if(idx>49152 && sgn<1) continue;
      aero[geo->LyrIdx(idx)]->Fill(x,y,sgn);
    }
    int npt[18] = {0};
    switch(plotarm) {
    case(0):
      tex->SetTextColor(kBlue-3);
      main->cd(1);  aero[0]->Draw("colz"); drawEX();   aero[0]->Draw("colz same"); tex->DrawLatex(18,18,Form("%.0f",aero[0]->GetEntries()));
      main->cd(2);  aero[1]->Draw("colz"); drawEX();   aero[1]->Draw("colz same"); tex->DrawLatex(18,18,Form("%.0f",aero[1]->GetEntries()));
      main->cd(3);  aero[2]->Draw("colz"); drawEX();   aero[2]->Draw("colz same"); tex->DrawLatex(18,18,Form("%.0f",aero[2]->GetEntries()));
      main->cd(4);  aero[3]->Draw("colz"); drawEX();   aero[3]->Draw("colz same"); tex->DrawLatex(18,18,Form("%.0f",aero[3]->GetEntries()));
      main->cd(5);  aero[4]->Draw("colz"); drawEX();   aero[4]->Draw("colz same"); tex->DrawLatex(18,18,Form("%.0f",aero[4]->GetEntries()));
      main->cd(6);  aero[5]->Draw("colz"); drawEX();   aero[5]->Draw("colz same"); tex->DrawLatex(18,18,Form("%.0f",aero[5]->GetEntries()));
      main->cd(7);  aero[6]->Draw("colz"); drawEX();   aero[6]->Draw("colz same"); tex->DrawLatex(18,18,Form("%.0f",aero[6]->GetEntries()));
      main->cd(8);  aero[7]->Draw("colz"); drawEX();   aero[7]->Draw("colz same"); tex->DrawLatex(18,18,Form("%.0f",aero[7]->GetEntries()));
      main->cd(9);  aero[8]->Draw("colz"); drawMPC(0); aero[8]->Draw("colz same"); tex->DrawLatex(18,18,Form("%.0f",aero[8]->GetEntries()));
      main->cd(10); phieta[0]->Draw("colz");
      int pties;
      input2 >> pties;
      if(!input2.good()) pties = 0;
      for(int i=0; i!=pties; ++i) {
	int ll;
	float xx, yy, sgnP, sx, sy, sxy, spx, spy;
	input2 >> ll >> xx >> yy >> sgnP >> sx >> sy >> sxy >> spx >> spy;
	if(ll>8) continue;
	main->cd(1+ll);
	lips->SetLineColor( col[ll] );
	lips->SetFillColorAlpha(kBlue-3,0.1);
	lips->DrawEllipse(xx,yy,spx,spy,0,360,0);
	npt[ll]++;
	main->cd(10);
	lips->SetLineColor( col[ll] );
	float ephi, eeta;
        float phi = _phi( xx, yy, ephi, spx, spy );
        float eta = _eta( xx, yy, z[ll], eeta, spx, spy, ez[ll] );
        lips->DrawEllipse(eta,phi,eeta,ephi,0,360,0);
	//draw
      }
      tex->SetTextColor(kRed-3);
      main->cd(1);  tex->DrawLatex(18,16,Form("%d",npt[0]));
      main->cd(2);  tex->DrawLatex(18,16,Form("%d",npt[1]));
      main->cd(3);  tex->DrawLatex(18,16,Form("%d",npt[2]));
      main->cd(4);  tex->DrawLatex(18,16,Form("%d",npt[3]));
      main->cd(5);  tex->DrawLatex(18,16,Form("%d",npt[4]));
      main->cd(6);  tex->DrawLatex(18,16,Form("%d",npt[5]));
      main->cd(7);  tex->DrawLatex(18,16,Form("%d",npt[6]));
      main->cd(8);  tex->DrawLatex(18,16,Form("%d",npt[7]));
      main->cd(9);  tex->DrawLatex(18,16,Form("%d",npt[8]));
      main->cd(10);
      tex->SetTextColor(col[0]); tex->DrawLatex(-3.9,5.5,Form("%d",npt[0]));
      tex->SetTextColor(col[1]); tex->DrawLatex(-3.9,4.5,Form("%d",npt[1]));
      tex->SetTextColor(col[2]); tex->DrawLatex(-3.9,3.0,Form("%d",npt[2]));
      tex->SetTextColor(col[3]); tex->DrawLatex(-3.9,1.5,Form("%d",npt[3]));
      tex->SetTextColor(col[4]); tex->DrawLatex(-3.9,0.5,Form("%d",npt[4]));
      tex->SetTextColor(col[5]); tex->DrawLatex(-3.2,5.5,Form("%d",npt[5]));
      tex->SetTextColor(col[6]); tex->DrawLatex(-3.2,4.5,Form("%d",npt[6]));
      tex->SetTextColor(col[7]); tex->DrawLatex(-3.2,1.5,Form("%d",npt[7]));
      tex->SetTextColor(col[8]); tex->DrawLatex(-3.2,0.5,Form("%d",npt[8]));
      break;
    case(1):
      //main->Divide(3,3,0,0);
      main->cd(1);  aero[9]->Draw("colz");
      main->cd(2); aero[10]->Draw("colz");
      main->cd(3); aero[11]->Draw("colz");
      main->cd(4); aero[12]->Draw("colz");
      main->cd(5); aero[13]->Draw("colz");
      main->cd(6); aero[14]->Draw("colz");
      main->cd(7); aero[15]->Draw("colz");
      main->cd(8); aero[16]->Draw("colz");
      main->cd(9); aero[17]->Draw("colz");
      break;
    default:
      //main->Divide(4,4,0,0);
      main->cd(1);  aero[0]->Draw("colz");
      main->cd(2);  aero[1]->Draw("colz");
      main->cd(3);  aero[2]->Draw("colz");
      main->cd(4);  aero[3]->Draw("colz");
      main->cd(5);  aero[4]->Draw("colz");
      main->cd(6);  aero[5]->Draw("colz");
      main->cd(7);  aero[6]->Draw("colz");
      main->cd(8);  aero[7]->Draw("colz");
      main->cd(9);  aero[9]->Draw("colz");
      main->cd(10); aero[10]->Draw("colz");
      main->cd(11); aero[11]->Draw("colz");
      main->cd(12); aero[12]->Draw("colz");
      main->cd(13); aero[13]->Draw("colz");
      main->cd(14); aero[14]->Draw("colz");
      main->cd(15); aero[15]->Draw("colz");
      main->cd(16); aero[16]->Draw("colz");
    }
    main->SaveAs(Form("%s_disp.pdf",file.Data()),"pdf");
    if(eventnumber>-1 && eventnumber!=ev) break;
  }
  input.close();
  input2.close();
  main->SaveAs(Form("%s_disp.pdf]",file.Data()),"pdf");
  std::cout << "OUTPUT: " << Form("%s_disp.pdf",file.Data()) << std::endl;
  return 0;
}
