//=========================
// written by Carlos Perez 
//=========================
#ifndef __mxData_HH__
#define __mxData_HH__

#include <vector>

#include "mxHit.h"
#include "mxCluster.h"
#include "mxCandidate.h"
#include "mxDecayCandidate.h"

class mxGeometry;

class mxData {
 public:
  mxData();
  ~mxData();
  void Fill( int idx, float ene);
  void Reset();
  inline void ClearHits() {for(int i=0; i!=18; ++i) fNHit[i]=0;}
  inline void ClearClusters() {for(int i=0; i!=18; ++i) fNPty[i]=0;}
  inline void ClearCandidates() {for(int i=0; i!=2; ++i) fNCoa[i]=0;}
  inline void ClearCandidatesPreEvent() {for(int i=0; i!=2; ++i) fNCoaPreEvent[i]=0;}
  inline void ClearDecayCandidates() {for(int i=0; i!=2; ++i) fNUni[i]=0;}
  void SetVertex(float x, float y, float z) {fV[0]=x; fV[1]=y; fV[2]=z;}

  float GetMultiplicity(int lyr);
  float GetVertexX() {return fV[0];}
  float GetVertexY() {return fV[1];}
  float GetVertexZ() {return fV[2];}
  int GetNHits(int lyr) {return fNHit[lyr];}
  int GetNClusters(int lyr) {return fNPty[lyr];}
  int GetNCandidates(int arm) {return fNCoa[arm];}
  int GetNCandidatesPreEvent(int arm) {return fNCoaPreEvent[arm];}
  int GetNDecayCandidates(int arm) {return fNUni[arm];}
  std::vector<mxHit*> GetHits(int lyr) {return fHit[lyr];}
  std::vector<mxCluster*> GetClusters(int lyr) {return fPty[lyr];}
  std::vector<mxCandidate*> GetCandidates(int arm) {return fCoa[arm];}
  std::vector<mxCandidate*> GetCandidatesPreEvent(int arm) {return fCoaPreEvent[arm];}
  std::vector<mxDecayCandidate*> GetDecayCandidates(int arm) {return fUni[arm];}
  mxHit* PushNewHit(int lyr);
  mxCluster* PushNewCluster(int lyr);
  mxCandidate* PushNewCandidate(int arm);
  mxCandidate* PushNewCandidatePreEvent(int arm);
  mxDecayCandidate* PushNewDecayCandidate(int arm);
  void SaveCandidatesToPreEvent();

 private:
  std::vector<mxHit*> fHit[18];
  std::vector<mxCluster*> fPty[18];
  std::vector<mxCandidate*> fCoa[2];
  std::vector<mxCandidate*> fCoaPreEvent[2];
  std::vector<mxDecayCandidate*> fUni[2];

  mxGeometry *fGeo;
  int fNHit[18];
  int fNPty[18];
  int fNCoa[2];
  int fNCoaPreEvent[2];
  int fNUni[2];
  float fV[3];
};

#endif /* __mxData_H__ */ 
