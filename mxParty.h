//=========================
// written by Carlos Perez
// 2015-2016
//=========================
#ifndef __mxParty_HH__
#define __mxParty_HH__
#include "mxHit.h"

class mxParty {
 public:
  mxParty();
  mxParty(const mxParty&);
  mxParty& operator=(const mxParty&);
  virtual ~mxParty();
  void Fill(mxHit *hit,float,float);
  mxHit* GetHit(int val) {return val<N()?fHits[val]:NULL;}
  float Test(float,float);
  int N() {return fN+fNx+fNy;}
  float GetX();
  float GetY();
  float GetCov(int);
  float GetSpreadX() {return (fN+fNx)*fDx*0.29;}
  float GetSpreadY() {return (fN+fNy)*fDy*0.29;}
  float GetDx() {return fDx;}
  float GetDy() {return fDy;}
  float Signal() {return fSgn;}
  void Reset();
  bool IsAssigned() {return fAssigned;}
  void SetAssigned(bool val=true) {fAssigned=val;}
  void SetDxDy(float dx, float dy) {fDx=dx; fDy=dy;}

 protected:
  mxHit *fHits[128];
  bool fAssigned;
  float fSgn;
  float fSx;
  float fSy;
  float fSxx;
  float fSyy;
  float fSxy;
  int fN;
  int fNx;
  int fNy;
  float fDx;
  float fDy;
};
#endif /* __mxParty_H__ */
