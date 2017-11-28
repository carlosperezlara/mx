#include "mxCalibBaseSiW.h"
#include "mxCalibBaseSiW.cxx"
#include "mxCalibStageSiW.h"
#include "mxCalibStageSiW.cxx"
#include "mxCalibBasePbWO4.h"
#include "mxCalibBasePbWO4.cxx"
#include "mxCalibMaster.h"
#include "mxCalibMaster.cxx"
#include "mxGeometry.h"
#include "mxGeometry.cxx"
#include "mxDB.cc"

int peddisplay(int pkt=0) {
  mxGeometry *geo = mxGeometry::Instance();
  int kini = pkt*3072;
  float a[4][768];
  float x[4][768];
  float tmp;
  
  for(int i=0; i!=3072; ++i) {
    a[i%4][i/4] = 0;
  }
  int i=0;
  for(int sen=0; sen!=24; ++sen) {
    TString dir = Form("out/SEN%03d/fit",pkt*24+sen);
    for(int j=0; j!=128; ++j) {
      int key = kini + sen*128 + j;
      x[i%4][i/4] = key;
      ifstream fin( Form("%s/P%05d.dat",dir.Data(),key) );
      fin >> tmp;
      if(fin.good()) {
	a[i%4][i/4] = tmp;
      }
      ++i;
    }
  }
  cout << i << endl;
  TGraph *grA[4];
  int color[4] = { kRed-3, kBlue-3, kGreen-3, kOrange-3 };
  for(int k=0; k!=4; ++k) {
    grA[k] = new TGraphErrors(768,x[k],a[k]);
    grA[k]->SetMarkerColor( color[k] );
    grA[k]->SetLineColor( color[k] );
  }
  
  bool bad[128];
  mxCalibMaster *cal = new mxCalibMaster();
  mxDB::read(454934,cal);
  for(int i=0; i!=3072; ++i) {
    bad[i] = false;
    if( cal->IsBadKey(kini+i) ) bad[i] = true;
  }
  for(int i=3071; i!=-1; --i) {
    if(!bad[i]) continue;
    grA[i%4]->RemovePoint( i/4 );
  }
  
  TCanvas *main = new TCanvas( Form("PKT%02d",pkt),
			       Form("PKT%02d",pkt),1600,900);
  grA[0]->Draw("A*L");
  grA[0]->SetTitle("Ped");
  grA[0]->GetYaxis()->SetRangeUser(-10,+10);
  for(int k=1; k!=4; ++k) grA[k]->Draw("*LSAME");
  return 0;
}
