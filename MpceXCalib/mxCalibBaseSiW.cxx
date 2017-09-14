#include "mxCalibBaseSiW.h"

mxCalibBaseSiW::mxCalibBaseSiW() {
  for(int k=0; k!=49152; ++k)
    fData[k] = -999999;
}

mxCalibBaseSiW::mxCalibBaseSiW(const mxCalibBaseSiW &cpy) {
  for(int k=0; k!=49152; ++k)
    fData[k] = cpy.fData[k];
}

float mxCalibBaseSiW::Get(int key) {
  if(key<0||key>49151) return -999999;
  return fData[key];
}
void mxCalibBaseSiW::FillWith(const float val[49152]) {
  for(int k=0; k!=49152; ++k)
    fData[k] = val[k];
}
void mxCalibBaseSiW::Set(int key, float val) {
  if(key<0||key>49151) return;
  fData[key] = val;;
}
