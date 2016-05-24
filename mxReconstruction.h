#ifndef __mxReconstruction_HH__
#define __mxReconstruction_HH__

#include "TMath.h"
class mxGeometry;

class mxReconstruction {
 public:
  mxReconstruction();
  ~mxReconstruction();

  void Fill( int idx, float ene);
  void Reset();
  void Make();
  TClonesArray* GetCoalitions(int arm) {return fCoa[arm];}
  void SetVertex(float x, float y, float z) {fV[0]=x; fV[1]=y; fV[2]=z;}

 private:
  void Parties();
  void Coalitions();

  TClonesArray *fHit[18];
  TClonesArray *fPty[18];
  TClonesArray *fCoa[2];
  mxGeometry *fGeo;
  int fNHit[18];
  int fNPty[18];
  int fNCoa[2];
  float fV[3];
};

#endif /* __mxReconstruction_H__ */ 
