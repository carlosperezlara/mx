#include "mxCalibMaster.h"
#include "mxCalibBase.h"
#include "mxChipStatus.h"

mxCalibMaster::mxCalibMaster() :
  fPHMu( new mxCalibBase() ),
  fPHSg( new mxCalibBase() ),
  fPLMu( new mxCalibBase() ),
  fPLSg( new mxCalibBase() ),
  fLMPV( new mxCalibBase() ),
  fLSgm( new mxCalibBase() ),
  fLHft( new mxCalibBase() ),
  fChpS( new mxChipStatus() )
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
  fChpS( cpy.fChpS )
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
  delete fChpS;
}
