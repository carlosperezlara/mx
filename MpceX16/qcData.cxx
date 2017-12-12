//=========================
// written by Carlos Perez 
//=========================
#include <iostream>
#include "qcData.h"

//========
qcData::qcData() {
  // ctor
  std::cout << "qcData:: ctor" << std::endl;
  for(int ord=0; ord!=10; ++ord) {
    for(int det=0; det!=8; ++det) {
      fQex[ord][det/4][det%4] = new qcQ(ord);
    }
  }
  fQbb = new qcQ(2);
  fQmp = new qcQ(2);
  fQfv = new qcQ(2);
}
//========
void qcData::Reset() {
  for(int ord=0; ord!=10; ++ord) {
    for(int det=0; det!=8; ++det) {
      fQex[ord][det/4][det%4]->Reset();
    }
  }
  fQbb->Reset();
  fQmp->Reset();
  fQfv->Reset();
}
//========
qcData::~qcData() {
  // dtor
  for(int ord=0; ord!=10; ++ord) {
    for(int det=0; det!=8; ++det) {
      delete fQex[ord][det/4][det%4];
    }
  }
  delete fQbb;
  delete fQmp;
  delete fQfv;
}
//========
qcQ* qcData::GetQex(int ord, int ioc, int det) {
  if(ord<0||ord>9) return NULL;
  if(ioc<0||ioc>1) return NULL;
  if(det<0||det>3) return NULL;
  return fQex[ord][ioc][det];
}
