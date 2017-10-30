#include "qcCalibBase.h"

qcCalibBase::qcCalibBase() {
  for(int k=0; k!=10; ++k) {
    fRe[k] = 0;
    fIm[k] = 0;
  }
}

qcCalibBase::qcCalibBase(const qcCalibBase &cpy) {
  for(int k=0; k!=10; ++k) {
    fRe[k] = cpy.fRe[k];
    fIm[k] = cpy.fIm[k];
  }
}

float qcCalibBase::GetRe(int ord) {
  if(ord<1||ord>10) return -999999;
  return fRe[ord-1];
}

float qcCalibBase::GetIm(int ord) {
  if(ord<1||ord>10) return -999999;
  return fIm[ord-1];
}

void qcCalibBase::SetRe(int ord, float val) {
  if(ord<1||ord>10) return;
  fRe[ord-1] = val;
}

void qcCalibBase::SetIm(int ord, float val) {
  if(ord<1||ord>10) return;
  fIm[ord-1] = val;
}

void qcCalibBase::FillReImWith(float re[10], float im[10]) {
  for(int i=0; i!=10; ++i) {
    fRe[i] = re[i];
    fIm[i] = im[i];
  }
}
