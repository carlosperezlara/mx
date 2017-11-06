#include "mxCalibStageSiW.h"

mxCalibStageSiW::mxCalibStageSiW() {
  for(int k=0; k!=384; ++k)
    fData[k] = -999999;
}

mxCalibStageSiW::mxCalibStageSiW(const mxCalibStageSiW &cpy) {
  for(int k=0; k!=384; ++k)
    fData[k] = cpy.fData[k];
}

float mxCalibStageSiW::Get(int key) {
  if(key<0||key>383) return -999999;
  return fData[key];
}
void mxCalibStageSiW::FillWith(const float val[384]) {
  for(int k=0; k!=384; ++k)
    fData[k] = val[k];
}
void mxCalibStageSiW::FillWithConst(const float val) {
  for(int k=0; k!=384; ++k)
    fData[k] = val;
}
void mxCalibStageSiW::Set(int key, float val) {
  if(key<0||key>383) return;
  fData[key] = val;;
}
