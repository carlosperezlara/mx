#include "TH2D.h"
#include "TCanvas.h"
#include "TLatex.h"

#include <map>
#include <fstream>

#include "AeroView.h"

AeroView::AeroView() {
  const double rg = 20;
  const int nx = 250;
  const int ny = 26;
  for(int arm=0; arm!=2; ++arm)
    for(int lyr=0; lyr!=8; ++lyr)
      if(lyr%2) {
	fLayer[arm][lyr] = new TH2D( Form("DisplayARM%dLYR%d",arm,lyr), Form("ARM%d  LYR%d",arm,lyr), ny, -rg, +rg, nx, -rg, +rg );
      } else {
	fLayer[arm][lyr] = new TH2D( Form("DisplayARM%dLYR%d",arm,lyr), Form("ARM%d  LYR%d",arm,lyr), nx, -rg, +rg, ny, -rg, +rg );
      }
  InitGeo();
  return;
}
AeroView::~AeroView() {
  // dtor
}
void AeroView::InitGeo() {
  ifstream inf;
  inf.open("map.txt");
  //# KEY ARM PKT LYR CHN CHP LX LY X Y
  int key;
  double tmp;
  for(;;) {
    inf >> key >> fA[key] >> tmp >> fL[key] >> tmp >> tmp >> tmp >> tmp >> fX[key] >> fY[key];
    if(!inf.good()) break;
  }
  fLogz=false;
  fRangez=false;
  return;
}

void AeroView::Fill(const int key, const double val) {
  fLayer[ fA[key] ][ fL[key] ]->Fill( fX[key], fY[key], val );
  return;
}

TCanvas* AeroView::Draw() {
  TCanvas *ret = new TCanvas("AeroView","AeroView",1200,300);
  ret->Divide(8,2);
  for(int arm=0; arm!=2; ++arm)
    for(int lyr=0; lyr!=8; ++lyr) {
      ret->cd(arm*8+lyr+1)->SetLogz(fLogz);
      fLayer[arm][lyr]->Draw("colz");
      if(fRangez)
	fLayer[arm][lyr]->GetZaxis()->SetRangeUser(fMinz,fMaxz);
    }
  return ret;
}

void AeroView::PrintKeys(int arm, int lyr) {
  TLatex tex;
  int key = -999;
  tex.SetTextSize(0.5);

  for (int k = 0; k!= 49152; k++){
    if (fA[k] == arm && fL[k] == lyr) {
      key = k;
      tex.DrawLatex( fX[key], fY[key], Form("%d",key) );
    }
  }
}

float AeroView::GetX(int key) {
  return fX[key];
}

float AeroView::GetY(int key) {
  return fY[key];
}
