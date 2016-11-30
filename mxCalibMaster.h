#ifndef __mxCalibMaster_HH__
#define __mxCalibMaster_HH__

#include "mxCalibBaseSiW.h"

class mxCalibMaster {
 public:
  mxCalibMaster();
  mxCalibMaster(const mxCalibMaster &cpy);
  virtual ~mxCalibMaster();
  mxCalibBaseSiW* GetPHMu() {return fPHMu;}  // HiGain Pedestal Mean
  mxCalibBaseSiW* GetPHSg() {return fPHSg;}  // HiGain Pedestal Sigma
  mxCalibBaseSiW* GetPLMu() {return fPLMu;}  // LoGain Pedestal Mean
  mxCalibBaseSiW* GetPLSg() {return fPLSg;}  // LoGain Pedestal Sigma
  mxCalibBaseSiW* GetLMPV() {return fLMPV;}  // Landau MPV
  mxCalibBaseSiW* GetLSgm() {return fLSgm;}  // Landau Sigma
  mxCalibBaseSiW* GetLHft() {return fLHft;}  // Hi2Lo Factor
  bool IsBadKey(int);

 protected:
  mxCalibBaseSiW *fPHMu;
  mxCalibBaseSiW *fPHSg;
  mxCalibBaseSiW *fPLMu;
  mxCalibBaseSiW *fPLSg;
  mxCalibBaseSiW *fLMPV;
  mxCalibBaseSiW *fLSgm;
  mxCalibBaseSiW *fLHft;
};
#endif /* __mxCalibMaster_H__ */
