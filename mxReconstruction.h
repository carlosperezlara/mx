#ifndef __mxReconstruction_HH__
#define __mxReconstruction_HH__

#include <vector>

#include "mxHit.h"
#include "mxParty.h"
#include "mxCoalition.h"
#include "mxUnion.h"

class mxGeometry;

class mxReconstruction {
 public:
  mxReconstruction();
  ~mxReconstruction();

  void Fill( int idx, float ene);
  void Reset();
  void Make();
  void SetVertex(float x, float y, float z) {fV[0]=x; fV[1]=y; fV[2]=z;}
  std::vector<mxHit*> GetHits(int lyr) {return fHit[lyr];}
  std::vector<mxParty*> GetParties(int lyr) {return fPty[lyr];}
  std::vector<mxCoalition*> GetCoalitions(int arm) {return fCoa[arm];}
  std::vector<mxUnion*> GetUnions(int arm) {return fUni[arm];}
  void DumpStats();
  void DumpParties();

  float GetVertexX() {return fV[0];}
  float GetVertexY() {return fV[1];}
  float GetVertexZ() {return fV[2];}
  int GetNHits(int lyr) {return fNHit[lyr];}
  int GetNParties(int lyr) {return fNPty[lyr];}
  int GetNCoalitions(int arm) {return fNCoa[arm];}
  int GetNUnions(int arm) {return fNUni[arm];}

 private:
  void Parties();
  void Coalitions();
  void Unions();

  std::vector<mxHit*> fHit[18];
  std::vector<mxParty*> fPty[18];
  std::vector<mxCoalition*> fCoa[2];
  std::vector<mxUnion*> fUni[2];
  mxGeometry *fGeo;
  int fNHit[18];
  int fNPty[18];
  int fNCoa[2];
  int fNUni[2];
  float fV[3];
};

#endif /* __mxReconstruction_H__ */ 
