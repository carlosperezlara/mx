//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
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
  mxParty* GetParty(int lyr, int idx) {return fPty[lyr].at(idx);}
  mxCoalition* GetCoalition(int arm, int idx) {return fCoa[arm].at(idx);}
  mxUnion* GetUnion(int arm, int idx) {return fUni[arm].at(idx);}
  void DumpMPCLayer(int);
  void DumpStats();
  void DumpHits();
  void DumpParties();
  void DumpCoalitions(int lvl=1);
  void DumpPreEventCoalitions(int lvl=1);
  void DumpUnions();
  void FillPP( float energy, float eta, float phi, int pdg);
  float GetMultiplicity(int lyr);

  void Parties();
  void Coalitions();
  void Unions();

  void FillPreEvent();
  void MixUnions();

  float GetVertexX() {return fV[0];}
  float GetVertexY() {return fV[1];}
  float GetVertexZ() {return fV[2];}
  int GetNHits(int lyr) {return fNHit[lyr];}
  int GetNParties(int lyr) {return fNPty[lyr];}
  int GetNCoalitions(int arm) {return fNCoa[arm];}
  int GetNUnions(int arm) {return fNUni[arm];}

  void SetDebug(int v) {fDebug=v;}
  void SetIdentificationAlgorithm(int v);

  void SetPtyAlgPadRow_Threshold(float v) {fPtyAlgPadRow_thr=v;}
  void SetCoaAlgSeed6_NCrystals(int v) {fCoaAlgSeed6_nc=v;}
  void SetPtyAlgMPCBreaker_NCrystals(int v) {fPtyAlgMPCBreaker_NCrystals = v;}
  void SetPtyAlgMPCBreaker_Threshold(float v) {fPtyAlgMPCBreaker_thr = v;}

 private:
  void SetPartyAlgorithm(int v) {fPtyAlg=v;}
  void SetCoalitionAlgorithm(int v) {fCoaAlg=v;}

  void Parties_ALGFocal(int);
  void Parties_ALGPadRow(int);
  void Parties_ALGLayer(int);
  void Parties_ALGMPCBreaker(int);

  void Coalitions_ALGSeedMPC();
  void Coalitions_ALGSeed6();
  mxParty* SeekHitInEM(float, float, int, int);
  float ComputePSChi2Prob(int, mxCoalition*);

  std::vector<mxHit*> fHit[18];
  std::vector<mxParty*> fPty[18];
  std::vector<mxCoalition*> fCoa[2];
  std::vector<mxUnion*> fUni[2];
  std::vector<mxCoalition*> fCoaPreEvent[2];
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
