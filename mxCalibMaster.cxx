#include "mxCalibMaster.h"
#include "mxCalibBaseSiW.h"
#include "mxCalibBasePbWO4.h"

bool mxCalibMaster::IsBadKey(int key) {
  if(key>-1&&key<49152) {
    if(fLHft->Get(key)<0) return true;
    //if(fLMPV->Get(key)<0) return true;
    //if(fLSgm->Get(key)<0) return true;
    if(fPHSg->Get(key)>1.5) return true;
    if(fPHSg->Get(key)<0.5) return true;
    if(fPLSg->Get(key)>1.5) return true;
    if(fPLSg->Get(key)<0.5) return true;
  }
  return false;
}

mxCalibMaster::mxCalibMaster() :
  fPHMu( new mxCalibBaseSiW() ),
  fPHSg( new mxCalibBaseSiW() ),
  fPLMu( new mxCalibBaseSiW() ),
  fPLSg( new mxCalibBaseSiW() ),
  fLMPV( new mxCalibBaseSiW() ),
  fLSgm( new mxCalibBaseSiW() ),
  fLHft( new mxCalibBaseSiW() ),
  fTau( new mxCalibBasePbWO4() ),
  fBet( new mxCalibBasePbWO4() ),
  fGam( new mxCalibBasePbWO4() )
{
}

mxCalibMaster::mxCalibMaster(const mxCalibMaster &cpy) :
  fPHMu( cpy.fPHMu ),
  fPHSg( cpy.fPHSg ),
  fPLMu( cpy.fPLMu ),
  fPLSg( cpy.fPLSg ),
  fLMPV( cpy.fLMPV ),
  fLSgm( cpy.fLSgm ),
  fLHft( cpy.fLHft ),
  fTau( cpy.fTau ),
  fBet( cpy.fBet ),
  fGam( cpy.fGam )
{
}

mxCalibMaster::~mxCalibMaster() {
  delete fPHMu;
  delete fPHSg;
  delete fPLMu;
  delete fPLSg;
  delete fLMPV;
  delete fLSgm;
  delete fLHft;
  delete fTau;
  delete fBet;
  delete fGam;
}
