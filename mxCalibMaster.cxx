#include "mxCalibMaster.h"
#include "mxCalibBaseSiW.h"

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
