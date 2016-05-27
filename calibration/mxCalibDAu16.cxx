#include "stdlib.h"
#include <iostream>
#include "mxCalibDAu16.h"
#include "mxCalibBase.h"
#include "mxChipStatus.h"

mxCalibDAu16::mxCalibDAu16(): mxCalibMaster() {
  std::cout << "mxCalibDAu16 was called" << std::endl;
  //======== PMH
  {
    float pedhigh[49152] = {0};
    fPHMu->FillWith(pedhigh);
  }
  //======== PML
  {
    float pedlow[49152] = {0};
    fPLMu->FillWith(pedlow);
  }
  //======== PSH
  {
    float sighi[49152] = {0};
    fPHSg->FillWith(sighi);
  }
  //======== PSL
  {
    float siglo[49152] = {0};
    fPLSg->FillWith(siglo);
  }
  //======== MPV
  {
    for(int k=0; k!=49152; ++k)
      fLMPV->Set(k,1); // needs to be determined
  }
  //======== SGM
  {
    for(int k=0; k!=49152; ++k)
      fLSgm->Set(k,1); // needs to be determined
  }
  //======== LHF
  {
    for(int k=0; k!=49152; ++k)
      fLHft->Set(k,1); // needs to be determined
  }
  //======== CHP
  {
  }
}

mxCalibDAu16::mxCalibDAu16(const mxCalibDAu16 &cpy):
  mxCalibMaster( cpy )
{
}

mxCalibDAu16::~mxCalibDAu16() {
}
