#ifndef __mxCalibMaster_HH__
#define __mxCalibMaster_HH__

#include "mxCalibBaseSiW.h"
#include "mxCalibBasePbWO4.h"

class mxCalibMaster {
 public:
  mxCalibMaster();
  mxCalibMaster(const mxCalibMaster &cpy);
  virtual ~mxCalibMaster();
  mxCalibBaseSiW* GetPHMu() {return fPHMu;}  // HiGain Pedestal Mean
  mxCalibBaseSiW* GetPHSg() {return fPHSg;}  // HiGain Pedestal Sigma
  mxCalibBaseSiW* GetPHSh() {return fPHSh;}  // HiGain Pedestal Offline Recal
  mxCalibBaseSiW* GetPLMu() {return fPLMu;}  // LoGain Pedestal Mean
  mxCalibBaseSiW* GetPLSg() {return fPLSg;}  // LoGain Pedestal Sigma
  mxCalibBaseSiW* GetPLSh() {return fPLSh;}  // LoGain Pedestal Offline Recal
  mxCalibBaseSiW* GetLMPV() {return fLMPV;}  // Landau MPV
  mxCalibBaseSiW* GetLSgm() {return fLSgm;}  // Landau Sigma
  mxCalibBaseSiW* GetLHft() {return fLHft;}  // Hi2Lo Factor
  mxCalibBasePbWO4* GetTau() {return fTau;}  // Tau shift
  mxCalibBasePbWO4* GetBet() {return fBet;}  // Slewing Correction
  mxCalibBasePbWO4* GetGam() {return fGam;}  // Gain correction
  bool IsBadKey(int);

 protected:
  mxCalibBaseSiW *fPHMu;
  mxCalibBaseSiW *fPHSg;
  mxCalibBaseSiW *fPHSh;
  mxCalibBaseSiW *fPLMu;
  mxCalibBaseSiW *fPLSg;
  mxCalibBaseSiW *fPLSh;
  mxCalibBaseSiW *fLMPV;
  mxCalibBaseSiW *fLSgm;
  mxCalibBaseSiW *fLHft;
  mxCalibBasePbWO4 *fTau;
  mxCalibBasePbWO4 *fBet;
  mxCalibBasePbWO4 *fGam;
};
#endif /* __mxCalibMaster_H__ */
