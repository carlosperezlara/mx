//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include "TMath.h"

#include "mxHit.h"
#include "mxParty.h"

//========
mxParty::mxParty():
  fAssigned(false),
  fSgn(0),
  fSx(0),
  fSy(0),
  fSxx(0),
  fSyy(0),
  fSxy(0),
  fN(0),
  fNx(0),
  fNy(0),
  fDx(0),
  fDy(0) {
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
  fAssigned = src.fAssigned;
  fSgn = src.fSgn;
  fSx = src.fSx;
  fSy = src.fSy;
  fSxx = src.fSxx;
  fSyy = src.fSyy;
  fSxy = src.fSxy;
  fN = src.fN;
  fNx = src.fNx;
  fNy = src.fNy;
  fDx = src.fDx;
  fDy = src.fDy;
  for(int i=0; i!=N(); ++i)
    fHits[i] = src.fHits[i];
}
//========
mxParty& mxParty::operator=(const mxParty &src) {
  // asgmnt operator
  if(&src!=this) {
    fAssigned = src.fAssigned;
    fSgn = src.fSgn;
    fSx = src.fSx;
    fSy = src.fSy;
    fSxx = src.fSxx;
    fSyy = src.fSyy;
    fSxy = src.fSxy;
    fN = src.fN;
    fNx = src.fNx;
    fNy = src.fNy;
    fDx = src.fDx;
    fDy = src.fDy;
    for(int i=0; i!=N(); ++i)
      fHits[i] = src.fHits[i];
  }
  return *this;
}
//========
void mxParty::Fill(mxHit *hit, float x, float y) {
  // filler
  int n = N();
  if(n>127) return;
  fHits[n] = hit;
  if(n<1) {
    fN = 1;
  } else {
    if(fDx>fDy) {
      if( TMath::Abs(GetX()-x)>0.9*GetSpreadX() ) {
	fNx++;
      } else {
	fNy++;
      }
    } else {
      if( TMath::Abs(GetY()-y)>0.9*GetSpreadY() ) {
	fNy++;
      } else {
	fNx++;
      }
    }
  }
  hit->SetAssigned(true);
  float sgn = hit->Signal();
  fSx += x*sgn;
  fSy += y*sgn;
  fSxx += x*x*sgn;
  fSyy += y*y*sgn;
  fSxy += x*y*sgn;
  fSgn += sgn;
}
//========
float mxParty::GetX() {
  // <x>
  if( N()<1 ) return 0;
  if( fSgn<1e-6 ) return 0;
  return fSx/fSgn;
}
//========
float mxParty::GetY() {
  // <y>
  if( N()<1 ) return 0;
  if( fSgn<1e-6 ) return 0;
  return fSy/fSgn;
}
//========
float mxParty::Test(float xx, float yy) {
  // tester
  if( N()<1 ) return -1;
  float cx = GetX();
  float cy = GetY();
  float varX = GetSpreadX()*GetSpreadX();
  float varY = GetSpreadY()*GetSpreadY();
  float dx2 = (xx-cx)/varX*(xx-cx);
  float dy2 = (yy-cy)/varY*(yy-cy);
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
  fN=0;
  fNx=0;
  fNy=0;
}
//========
float mxParty::GetCov(int dim) {
  // covariance matrix
  if( N()<1 ) return -1;
  if( fSgn<1e-6 ) return -1;
  float var;
  if     (dim==0) var = fSxx/fSgn-fSx/fSgn*fSx/fSgn;
  else if(dim==1) var = fSyy/fSgn-fSy/fSgn*fSy/fSgn;
  else if(dim==2) var = fSxy/fSgn-fSy/fSgn*fSy/fSgn;
  else var = -1;
  return var;
}
