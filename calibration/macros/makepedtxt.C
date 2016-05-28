#include "mxChipStatus.h"
#include "mxChipStatus.cxx"
#include "mxCalibBase.h"
#include "mxCalibBase.cxx"
#include "mxCalibMaster.h"
#include "mxCalibMaster.cxx"
#include "mxCalibDAu16.h"
#include "mxCalibDAu16.cxx"

void makepedtxt() {
  mxCalibDAu16 *cal = new mxCalibDAu16();
  ofstream ofile("dau16.txt");
  for(int k=0; k!=49152; ++k) {
    ofile << Form("%05d  %.1f  %.1f  %.1f  %.1f\n",
		  k,
		  cal->GetPHMu()->Get(k),
		  cal->GetPHSg()->Get(k),
		  cal->GetPLMu()->Get(k),
		  cal->GetPLSg()->Get(k)
		  );
  }
  ofile.close();
}
