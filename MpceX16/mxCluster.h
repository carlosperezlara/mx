//=========================
// written by Carlos Perez
// 2015-2016
//=========================
#ifndef __mxCluster_HH__
#define __mxCluster_HH__
#include "mxHit.h"

class mxCluster {
 public:
  mxCluster();
  mxCluster(const mxCluster&);
  mxCluster& operator=(const mxCluster&);
  void CopyFrom(mxCluster *src);
  virtual ~mxCluster();
  void Fill(mxHit *hit,float,float);
  mxHit* GetHit(int val) {return val<N()?fHits[val]:NULL;}
  float Test(float,float);
  int N() {return fN+fNx+fNy;}
  float GetX();
  float GetXVar() {return GetCov(0);}
  float GetY();
  float GetYVar() {return GetCov(1);}
  float GetCov(int);
  float GetSpreadX() {return (fN+fNx)*fDx*0.29;} //sqrt12
  float GetSpreadY() {return (fN+fNy)*fDy*0.29;} //sqrt12
  float GetDx() {return fDx;}
  float GetDy() {return fDy;}
  float Signal() {return fSgn;}
  void Reset();
  bool IsAssigned() {return fAssigned;}
  void SetAssigned(bool val=true) {fAssigned=val;}
  void SetDxDy(float dx, float dy) {fDx=dx; fDy=dy;}
  int GetSizeUmbral(float);

 protected:
  mxHit *fHits[128];
  bool fAssigned;
  float fSgn;
  float fSx;
  float fSy;
  float fSxx;
  float fSyy;
  float fSxy;
  float fX0;
  float fY0;
  int fN;
  int fNx;
  int fNy;
  float fDx;
  float fDy;
};
#endif /* __mxCluster_H__ */
