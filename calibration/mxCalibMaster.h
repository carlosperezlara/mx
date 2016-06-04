#ifndef __mxCalibMaster_HH__
#define __mxCalibMaster_HH__

#include "mxCalibBase.h"
#include "mxChipStatus.h"

class mxCalibMaster {
 public:
  mxCalibMaster();
  mxCalibMaster(const mxCalibMaster &cpy);
  virtual ~mxCalibMaster();
  mxChipStatus* GetChpS() {return fChpS;} // Chip Status
  mxCalibBase* GetPHMu() {return fPHMu;}  // HiGain Pedestal Mean
  mxCalibBase* GetPHSg() {return fPHSg;}  // HiGain Pedestal Sigma
  mxCalibBase* GetPLMu() {return fPLMu;}  // LoGain Pedestal Mean
  mxCalibBase* GetPLSg() {return fPLSg;}  // LoGain Pedestal Sigma
  mxCalibBase* GetLMPV() {return fLMPV;}  // Landau MPV
  mxCalibBase* GetLSgm() {return fLSgm;}  // Landau Sigma
  mxCalibBase* GetLHft() {return fLHft;}  // Hi2Lo Factor

 protected:
  mxChipStatus *fChpS;
  mxCalibBase *fPHMu;
  mxCalibBase *fPHSg;
  mxCalibBase *fPLMu;
  mxCalibBase *fPLSg;
  mxCalibBase *fLMPV;
  mxCalibBase *fLSgm;
  mxCalibBase *fLHft;
};
#endif /* __mxCalibMaster_H__ */
