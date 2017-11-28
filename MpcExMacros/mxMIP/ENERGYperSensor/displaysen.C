#include "mxCalibBaseSiW.h"
#include "mxCalibBaseSiW.cxx"
#include "mxCalibStageSiW.h"
#include "mxCalibStageSiW.cxx"
#include "mxCalibBasePbWO4.h"
#include "mxCalibBasePbWO4.cxx"
#include "mxCalibMaster.h"
#include "mxCalibMaster.cxx"
#include "mxDB.cc"

int displaysen(int sen=0, int run=0) {
  gStyle->SetOptStat(0);

  if(0) {
  mxCalibMaster *cal = new mxCalibMaster();
  mxDB::read(454934,cal);
  for(int k=0; k!=49152; ++k) {
    if(k%128==14)  {
      cal->GetLMPV()->Set(k,-1);
      cal->GetLSgm()->Set(k,-1);
    }
    if(k%128==45) {
      cal->GetLMPV()->Set(k,-1);
      cal->GetLSgm()->Set(k,-1);
    }
    if(k%128==109) {
      cal->GetLMPV()->Set(k,-1);
      cal->GetLSgm()->Set(k,-1);
    }
  }
  int others[25] = {5160, 5164, 5168, 5176, 5180,
		  5120, 5124, 5132, 5144, 5152,
		  5122, 5162, 5166,
		  5145, 5161, 5173, 5177, 5181,
		  5139, 4143, 5147, 5151, 5159, 5163, 5183};
  for(int k=0; k!=25; ++k) {
    cal->GetLMPV()->Set(others[k],-1);
    cal->GetLSgm()->Set(others[k],-1);
  }

  TH1F *hbad = new TH1F("hbad","hbad",128,-0.5,127.5);
  TH1F *hbad2 = new TH1F("hbad2","hbad2",4,-0.5,3.5);
  for(int k=0; k!=49152/2; ++k) {
    if(cal->IsBadKey(k)) {
      hbad->Fill( k%128 );
      hbad2->Fill( k%4 );
    }
  }
  hbad->Draw();
  new TCanvas();
  hbad2->Draw();

  ofstream fout("correct.xxx");
  fout << "mpv[49152] = { ";
  for(int k=0; k!=49152; ++k) {
    fout << Form("%.1f, ",cal->GetLMPV()->Get(k));
  }
  fout << "};" << endl;
  fout << "sgm[49152] = { ";
  for(int k=0; k!=49152; ++k) {
    fout << Form("%.1f, ",cal->GetLSgm()->Get(k));
  }
  fout << "};" << endl;
  return 0;
  }

  int ini = sen*128;
  int fin = sen*128+128;
  TFile *file = new TFile( Form("se-%d.root",run) );
  TH2F *adc = (TH2F*) file->Get("EnergyHigh");
  TH1D *hsen[128];
  float max[4] = {0,0,0,0};
  float integra[128];
  for(int k=0; k!=128; ++k) {
    int key = sen*128 + k;
    hsen[k] = adc->ProjectionY(Form("H%d",key),key+1,key+1);
    if(hsen[k]->GetMaximum() > max[k%4]) max[k%4] = hsen[k]->GetMaximum();
    int bina = hsen[k]->GetXaxis()->FindBin(0.2);
    int binb = hsen[k]->GetXaxis()->FindBin(0.4);
    integra[k] = hsen[k]->Integral(bina,binb);
  }
  TH2F *adc2[4];
  int bina = hsen[0]->GetXaxis()->FindBin(0.1);
  int MAXBINS = 20;
  for(int i=0; i!=4; ++i) {
    adc2[i] = new TH2F(Form("SUM%d",i),Form("SUM%d",i),32,-0.5,31.5,MAXBINS,0,1);
    for(int b=0; b!=MAXBINS; ++b) {
      float bc = hsen[0]->GetXaxis()->GetBinCenter(bina+b);
      adc2[i]->GetYaxis()->SetBinLabel(b+1,Form("%f",bc));
    }
  }
  for(int k=0; k!=128; ++k) {
    hsen[k]->Scale(1/integra[k]);
    for(int b=0; b!=MAXBINS; ++b) {
      adc2[k%4]->SetBinContent( k/4+1, b+1, hsen[k]->GetBinContent(bina+b) );
    }
  }
  TCanvas *main = new TCanvas();
  main->Divide(2,2);
  main->cd(1); adc2[0]->Draw("COLZ");
  main->cd(2); adc2[1]->Draw("COLZ");
  main->cd(3); adc2[2]->Draw("COLZ");
  main->cd(4); adc2[3]->Draw("COLZ");

  return 0;
}
