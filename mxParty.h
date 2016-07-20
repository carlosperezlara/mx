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
  mxHit* GetHit(int val) {return val<fNHits?fHits[val]:NULL;}
  float Test(float,float);
  int N() {return fNHits;}
  float GetX();
  float GetY();
  float GetCov(int);
  float Signal() {return fSgn;}
  void Reset();
  bool IsAssigned() {return fAssigned;}
  void SetAssigned(bool val) {fAssigned=val;}

 protected:
  mxHit *fHits[128];
  int fNHits;
  bool fAssigned;
  float fSgn;
  float fSx;
  float fSy;
  float fSxx;
  float fSyy;
  float fSxy;
};
#endif /* __mxParty_H__ */
