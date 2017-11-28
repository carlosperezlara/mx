#include "mxCalibBaseSiW.h"
#include "mxCalibBaseSiW.cxx"
#include "mxCalibStageSiW.h"
#include "mxCalibStageSiW.cxx"
#include "mxCalibBasePbWO4.h"
#include "mxCalibBasePbWO4.cxx"
#include "mxCalibMaster.h"
#include "mxCalibMaster.cxx"
#include "mxDB.cc"
#include "mxGeometry.h"
#include "mxGeometry.cxx"
#include <algorithm>

mxGeometry *fGeo = mxGeometry::Instance();

int makepedhightable() {
  bool bad[49152];
  mxCalibMaster *cal = new mxCalibMaster();
  mxDB::read(454934,cal);
  for(int i=0; i!=49152; ++i) {
    bad[i] = false;
    if( cal->IsBadKey(i) ) bad[i] = true;
  }
  float xped[49152];
  for(int k=0; k!=49152; ++k) {
    if( bad[k] ) {
      xped[k] = -1;
    } else {
      int sen = k/128;
      float amp = 0;
      float mpv = -1;
      float sgm = -1;
      float ped = 0;
      float tmp = -1;
      TString dir = Form("out/SEN%03d/fit",sen);
      ifstream fin;
      //=====
      fin.open( Form("%s/P%05d.dat",dir.Data(),k) );
      fin >> tmp;
      if(fin.good()) {
	ped = tmp;
      }
      fin.close();
      //=====
      xped[k] = ped;
    }
  }

  for(int k=24576; k!=49152; ++k) {
    xped[k] = 0;
  }

  ofstream fout("aaa.xxx");
  fout << "phsh[49152] = { ";
  for(int k=0; k!=49152; ++k) {
    fout << Form("%.1f, ",xped[k]);
  }
  fout << "};" << endl;
  return 0;
}
