#include "qcCalibBase.h"

qcCalibBase::qcCalibBase() {
  for(int k=0; k!=16; ++k) {
    fRe[k] = 0;
    fIm[k] = 0;
  }
}

qcCalibBase::qcCalibBase(const qcCalibBase &cpy) {
  for(int k=0; k!=16; ++k) {
    fRe[k] = cpy.fRe[k];
    fIm[k] = cpy.fIm[k];
  }
}

float qcCalibBase::GetRe(int ord) {
  if(ord<0||ord>15) return -999999;
  return fRe[ord];
}

float qcCalibBase::GetIm(int ord) {
  if(ord<0||ord>15) return -999999;
  return fIm[ord];
}

void qcCalibBase::SetRe(int ord, float val) {
  if(ord<0||ord>15) return;
  fRe[ord] = val;
}

void qcCalibBase::SetIm(int ord, float val) {
  if(ord<0||ord>15) return;
  fIm[ord] = val;
}

void qcCalibBase::FillReImWith(float re[16], float im[16]) {
  for(int i=0; i!=16; ++i) {
    fRe[i] = re[i];
    fIm[i] = im[i];
  }
}
