//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#ifndef __mxReconstruction_HH__
#define __mxReconstruction_HH__

#include <vector>

#include "mxHit.h"
#include "mxCluster.h"
#include "mxCandidate.h"
#include "mxDecayCandidate.h"

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
  std::vector<mxCluster*> GetParties(int lyr) {return fPty[lyr];}
  std::vector<mxCandidate*> GetCandidates(int arm) {return fCoa[arm];}
  std::vector<mxDecayCandidate*> GetUnions(int arm) {return fUni[arm];}
  mxCluster* GetCluster(int lyr, int idx) {return fPty[lyr].at(idx);}
  mxCandidate* GetCandidate(int arm, int idx) {return fCoa[arm].at(idx);}
  mxDecayCandidate* GetUnion(int arm, int idx) {return fUni[arm].at(idx);}
  void DumpMPCLayer(int);
  void DumpStats();
  void DumpHits();
  void DumpParties();
  void DumpCandidates(int lvl=1);
  void DumpPreEventCandidates(int lvl=1);
  void DumpUnions();
  void FillPP( float energy, float eta, float phi, int pdg);
  float GetMultiplicity(int lyr);

  void Parties();
  void Candidates();
  void Unions();

  void FillPreEvent();
  void MixUnions();

  float GetVertexX() {return fV[0];}
  float GetVertexY() {return fV[1];}
  float GetVertexZ() {return fV[2];}
  int GetNHits(int lyr) {return fNHit[lyr];}
  int GetNParties(int lyr) {return fNPty[lyr];}
  int GetNCandidates(int arm) {return fNCoa[arm];}
  int GetNUnions(int arm) {return fNUni[arm];}

  void SetDebug(int v) {fDebug=v;}
  void SetIdentificationAlgorithm(int v);

  void SetPtyAlgPadRow_Threshold(float v) {fPtyAlgPadRow_thr=v;}
  void SetCoaAlgSeed6_NCrystals(int v) {fCoaAlgSeed6_nc=v;}
  void SetPtyAlgMPCBreaker_NCrystals(int v) {fPtyAlgMPCBreaker_NCrystals = v;}
  void SetPtyAlgMPCBreaker_Threshold(float v) {fPtyAlgMPCBreaker_thr = v;}

 private:
  void SetClusterAlgorithm(int v) {fPtyAlg=v;}
  void SetCandidateAlgorithm(int v) {fCoaAlg=v;}

  void Parties_ALGFocal(int);
  void Parties_ALGPadRow(int);
  void Parties_ALGLayer(int);
  void Parties_ALGMPCBreaker(int);

  void Candidates_ALGSeedMPC();
  void Candidates_ALGSeed6();
  mxCluster* SeekHitInEM(float, float, int, int);
  float ComputePSChi2Prob(int, mxCandidate*);

  std::vector<mxHit*> fHit[18];
  std::vector<mxCluster*> fPty[18];
  std::vector<mxCandidate*> fCoa[2];
  std::vector<mxDecayCandidate*> fUni[2];
  std::vector<mxCandidate*> fCoaPreEvent[2];
  mxGeometry *fGeo;

  int fDebug;
  int fPtyAlg;
  int fCoaAlg;
  float fPtyAlgPadRow_thr;
  float fPtyAlgMPCBreaker_thr;
  int fPtyAlgMPCBreaker_NCrystals;
  float fCoaAlgSeed6_nc;
  int fNHit[18];
  int fNPty[18];
  int fNCoa[2];
  int fNCoaPreEvent[2];
  int fNUni[2];
  float fV[3];
};

#endif /* __mxReconstruction_H__ */ 
