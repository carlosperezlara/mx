#include "mxCalibBase.h"

mxCalibBase::mxCalibBase() {
  for(int k=0; k!=49152; ++k)
    fData[k] = -999999;
}

mxCalibBase::mxCalibBase(const mxCalibBase &cpy) {
  for(int k=0; k!=49152; ++k)
    fData[k] = cpy.fData[k];
}

float mxCalibBase::Get(int key) {
  if(key<0||key>49151) return -999999;
  return fData[key];
}
void mxCalibBase::FillWith(const float val[49152]) {
  for(int k=0; k!=49152; ++k)
    fData[k] = val[k];
}
void mxCalibBase::Set(int key, float val) {
  if(key<0||key>49151) return;
  fData[key] = val;;
}
