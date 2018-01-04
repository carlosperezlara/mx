#include "qcCalibMaster.h"

qcCalibMaster::qcCalibMaster() {
  fSecond = new qcCalibBase();
}

qcCalibMaster::qcCalibMaster(const qcCalibMaster &cpy) {
  fSecond = cpy.fSecond;
}

qcCalibBase* qcCalibMaster::GetEXSecond(int det, int cir) {
  if(det<0||det>3) return NULL;
  if(cir<0||cir>1) return NULL;
  return fSecond[det][cir];
}
