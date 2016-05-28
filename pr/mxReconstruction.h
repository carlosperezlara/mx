#ifndef __mxReconstruction_HH__
#define __mxReconstruction_HH__

#include <vector>

#include "mxHit.h"
#include "mxParty.h"
#include "mxCoalition.h"

class mxGeometry;

class mxReconstruction {
 public:
  mxReconstruction();
  ~mxReconstruction();

  void Fill( int idx, float ene);
  void Reset();
  void Make();
  std::vector<mxCoalition*> GetCoalitions(int arm) {return fCoa[arm];}
  std::vector<mxParty*> GetParties(int lyr) {return fPty[lyr];}
  void SetVertex(float x, float y, float z) {fV[0]=x; fV[1]=y; fV[2]=z;}
  void DumpStats();
  void DumpParties();

 private:
  void Parties();
  void Coalitions();

  std::vector<mxHit*> fHit[18];
  std::vector<mxParty*> fPty[18];
  std::vector<mxCoalition*> fCoa[2];
  mxGeometry *fGeo;
  int fNHit[18];
  int fNPty[18];
  int fNCoa[2];
  float fV[3];
};

#endif /* __mxReconstruction_H__ */ 
