#include "mxCalibBasePbWO4.h"

mxCalibBasePbWO4::mxCalibBasePbWO4() {
  for(int k=0; k!=416; ++k)
    fData[k] = -999999;
}

mxCalibBasePbWO4::mxCalibBasePbWO4(const mxCalibBasePbWO4 &cpy) {
  for(int k=0; k!=416; ++k)
    fData[k] = cpy.fData[k];
}

float mxCalibBasePbWO4::Get(int key) {
  if(key<0||key>416) return -999999;
  return fData[key];
}
void mxCalibBasePbWO4::FillWith(const float val[416]) {
  for(int k=0; k!=416; ++k)
    fData[k] = val[k];
}
void mxCalibBasePbWO4::Set(int key, float val) {
  if(key<0||key>416) return;
  fData[key] = val;;
}
