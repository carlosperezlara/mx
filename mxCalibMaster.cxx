#include "mxCalibMaster.h"
#include "mxCalibBaseSiW.h"

bool mxCalibMaster::IsBadKey(int key) {
  if(fLHft->Get(key)<0) return true;
  //if(fLMPV->Get(key)<0) return true;
  //if(fLSgm->Get(key)<0) return true;
  if(fPHSg->Get(key)>1.5) return true;
  if(fPHSg->Get(key)<0.5) return true;
  if(fPLSg->Get(key)>1.5) return true;
  if(fPLSg->Get(key)<0.5) return true;
  return false;
}

mxCalibMaster::mxCalibMaster() :
  fPHMu( new mxCalibBaseSiW() ),
  fPHSg( new mxCalibBaseSiW() ),
  fPLMu( new mxCalibBaseSiW() ),
  fPLSg( new mxCalibBaseSiW() ),
  fLMPV( new mxCalibBaseSiW() ),
  fLSgm( new mxCalibBaseSiW() ),
  fLHft( new mxCalibBaseSiW() )
{
}

mxCalibMaster::mxCalibMaster(const mxCalibMaster &cpy) :
  fPHMu( cpy.fPHMu ),
  fPHSg( cpy.fPHSg ),
  fPLMu( cpy.fPLMu ),
  fPLSg( cpy.fPLSg ),
  fLMPV( cpy.fLMPV ),
  fLSgm( cpy.fLSgm ),
  fLHft( cpy.fLHft )
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
}
