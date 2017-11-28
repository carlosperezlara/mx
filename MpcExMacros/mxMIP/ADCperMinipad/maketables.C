#include "mxCalibBaseSiW.h"
#include "mxCalibBaseSiW.cxx"
#include "mxCalibBasePbWO4.h"
#include "mxCalibBasePbWO4.cxx"
#include "mxCalibMaster.h"
#include "mxCalibMaster.cxx"
#include "mxDB.cc"
#include "mxGeometry.h"
#include "mxGeometry.cxx"
#include <algorithm>

mxGeometry *fGeo = mxGeometry::Instance();

int maketables() {
  bool bad[49152];
  mxCalibMaster *cal = new mxCalibMaster();
  mxDB::read(454934,cal);
  for(int i=0; i!=49152; ++i) {
    bad[i] = false;
    if( cal->IsBadKey(i) ) bad[i] = true;
  }
  float xped[49152];
  float xmpv[49152];
  float xsgm[49152];
  for(int k=0; k!=49152; ++k) {
    if( bad[k] ) {
      xped[k] = 0;
      xmpv[k] = -1;
      xsgm[k] = -1;
    } else {
      int sen = k/128;
      float amp = 0;
      float mpv = -1;
      float sgm = -1;
      float ped = 0;
      float tmp;
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
      fin.open( Form("%s/H%05d.dat",dir.Data(),k) );
      fin >> amp;
      if(fin.good()) {
	fin >> tmp >> mpv >> tmp >> sgm;
      }
      fin.close();
      //=====
      if(amp<1) {
	sgm = -1;
      }
      xped[k] = ped;
      xmpv[k] = mpv;
      xsgm[k] = sgm;
    }
  }

  for(int k=24576; k!=49152; ++k) {
    xped[k] = 0;
    xmpv[k] = 1;
    xsgm[k] = 1;
  }

  ofstream fout("aaa.xxx");
  fout << "phsh[49152] = { ";
  for(int k=0; k!=49152; ++k) {
    fout << Form("%.1f, ",xped[k]);
  }
  fout << "};" << endl;
  //=======
  fout << "mpv[49152] = { ";
  for(int k=0; k!=49152; ++k) {
    fout << Form("%.1f, ",xmpv[k]);
  }
  fout << "};" << endl;
  //=======
  fout << "sgm[49152] = { ";
  for(int k=0; k!=49152; ++k) {
    fout << Form("%.1f, ",xsgm[k]);
  }
  fout << "};" << endl;
  return 0;
}
