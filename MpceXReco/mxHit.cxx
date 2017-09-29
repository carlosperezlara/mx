//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include "mxHit.h"

//========
mxHit::mxHit() :
  fIdx(-1),
  fSignal(0),
  fAssigned(false) {
  // ctor
}
//========
mxHit::~mxHit() {
  // dtor
}
//========
void mxHit::Reset() {
  fIdx = -1;
  fSignal = 0;
  fAssigned = false;
}//========
void mxHit::Fill(int idx, float sgn) {
  // filler
  fIdx = idx;
  fSignal = sgn;
}
