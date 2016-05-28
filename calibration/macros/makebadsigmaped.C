#include "../mxChipStatus.h"
#include "../mxChipStatus.cxx"
#include "../mxCalibBase.h"
#include "../mxCalibBase.cxx"
#include "../mxCalibMaster.h"
#include "../mxCalibMaster.cxx"
#include "../mxCalibDAu16.h"
#include "../mxCalibDAu16.cxx"

void makebadsigmaped() {
  gStyle->SetOptStat(111111);

  mxCalibDAu16 *cal = new mxCalibDAu16();
  TH2F *cHi = new TH2F("cHi","PedestalHi;Mean;Sigma", 120,0,60, 20,0.0,10);
  TH2F *cLo = new TH2F("cLo","PedestalLo;Mean;Sigma", 120,0,60, 20,0.0,10);

  ofstream ofile("badSgmPed.txt");
  for(int k=0; k!=49152; ++k) {
    float muhi = cal->GetPHMu()->Get(k);
    float sghi = cal->GetPHSg()->Get(k);
    float mulo = cal->GetPLMu()->Get(k);
    float sglo = cal->GetPLSg()->Get(k);
    cHi->Fill( muhi, sghi );
    cLo->Fill( mulo, sglo );
    if(sghi<0.5||sghi>1.5||sglo<0.5||sglo>1.5) ofile << Form("%05d\n",k);
  }
  ofile.close();

  new TCanvas(); cHi->Draw("colz");
  new TCanvas(); cLo->Draw("colz");
}
