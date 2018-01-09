#include "qcCalibBase.h"
#include "qcCalibMaster.h"

qcCalibMaster::qcCalibMaster() {
  for(int i=0; i!=4; ++i)
    for(int j=0; j!=2; ++j) {
      fSecond[i][j] = new qcCalibBase();
    }
}

qcCalibMaster::qcCalibMaster(const qcCalibMaster &cpy) {
  for(int i=0; i!=4; ++i)
    for(int j=0; j!=2; ++j) {
      fSecond[i][j] = cpy.fSecond[i][j];
    }
}

qcCalibBase* qcCalibMaster::GetEXSecond(int det, int cir) {
  if(det<0||det>3) return 0;
  if(cir<0||cir>1) return 0;
  return fSecond[det][cir];
}
