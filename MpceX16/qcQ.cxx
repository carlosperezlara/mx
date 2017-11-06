//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include "TMath.h"
#include "qcQ.h"

//========
qcQ::qcQ(int n) :
  fN(n),
  fNP(0),
  fM(0.),
  fX(0.),
  fY(0.) {
  // ctor
}
//========
qcQ::~qcQ() {
  // dtor
}
//========
void qcQ::Reset() {
  fX = 0.0;
  fY = 0.0;
  fM = 0.0;
  fNP = 0;
}
//========
float qcQ::Psi() {
  if(fN==0) return -99999;
  return (TMath::Pi() + TMath::ATan2(-fY,-fX) )/fN;
}
//========
void qcQ::Fill(float phi, float w) {
  // filler
  fX += w*TMath::Cos(fN*phi);
  fY += w*TMath::Sin(fN*phi);
  fM += w;
  fNP++;
}
