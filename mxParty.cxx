//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include "TMath.h"

#include "mxHit.h"
#include "mxParty.h"

//========
mxParty::mxParty():
  fNHits(0),
  fAssigned(false),
  fSgn(0),
  fSx(0),
  fSy(0),
  fSxx(0),
  fSyy(0),
  fSxy(0) {
  // ctor
  for(int i=0; i!=128; ++i) fHits[i]=NULL;
}
//========
mxParty::~mxParty() {
  // dtor
}
//========
mxParty::mxParty(const mxParty &src) {
  // copy ctor
  fNHits = src.fNHits;
  fAssigned = src.fAssigned;
  fSgn = src.fSgn;
  fSx = src.fSx;
  fSy = src.fSy;
  fSxx = src.fSxx;
  fSyy = src.fSyy;
  fSxy = src.fSxy;
  for(int i=0; i!=fNHits; ++i)
    fHits[i] = src.fHits[i];
}
//========
mxParty& mxParty::operator=(const mxParty &src) {
  // asgmnt operator
  if(&src!=this) {
    fNHits = src.fNHits;
    fAssigned = src.fAssigned;
    fSgn = src.fSgn;
    fSx = src.fSx;
    fSy = src.fSy;
    fSxx = src.fSxx;
    fSyy = src.fSyy;
    fSxy = src.fSxy;
    for(int i=0; i!=fNHits; ++i)
      fHits[i] = src.fHits[i];
  }
  return *this;
}
//========
void mxParty::Fill(mxHit *hit, float x, float y) {
  // filler
  if(fNHits>127) return;
  fHits[fNHits] = hit;
  hit->SetAssigned(true);
  float sgn = hit->Signal();
  fSx += x*sgn;
  fSy += y*sgn;
  fSxx += x*x*sgn;
  fSyy += y*y*sgn;
  fSxy += x*y*sgn;
  ++fNHits;
  fSgn += sgn;
}
//========
float mxParty::X() {
  // <x>
  if( fNHits<1 ) return 0;
  if( fSgn<1e-6 ) return 0;
  return fSx/fSgn;
}
//========
float mxParty::Y() {
  // <y>
  if( fNHits<1 ) return 0;
  if( fSgn<1e-6 ) return 0;
  return fSy/fSgn;
}
//========
float mxParty::Test(float nx, float ny) {
  // tester
  if( fNHits<1 ) return -1;
  float cx = X();
  float cy = Y();
  float varX = Cov(0);
  float varY = Cov(1);
  //if(varX<1e-6)
  varX = 0.2*0.2*fNHits*fNHits; // prior
  //if(varY<1e-6)
  varY = 0.2*0.2*fNHits*fNHits; // prior
  float dx2 = (nx-cx)/varX*(nx-cx);
  float dy2 = (ny-cy)/varY*(ny-cy);
  return TMath::Sqrt( dx2 + dy2 );
}
//========
void mxParty::Reset() {
  // clean variables
  fSgn=0;
  fSx=0;
  fSy=0;
  fSxx=0;
  fSyy=0;
  fSxy=0;
  fNHits=0;
}
//========
float mxParty::Cov(int dim) {
  // covariance matrix
  if( fNHits<1 ) return -1;
  if( fSgn<1e-6 ) return -1;
  float var;
  if     (dim==0) var = fSxx/fSgn-fSx/fSgn*fSx/fSgn;
  else if(dim==1) var = fSyy/fSgn-fSy/fSgn*fSy/fSgn;
  else if(dim==2) var = fSxy/fSgn-fSy/fSgn*fSy/fSgn;
  else var = -1;
  return var;
}
