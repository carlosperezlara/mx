//=========================
// written by Carlos Perez
// 2015-2016
//=========================
#ifndef __mxDecayCandidate_HH__
#define __mxDecayCandidate_HH__
#include "TMath.h"

class mxCandidate;

class mxDecayCandidate {
 public:
  mxDecayCandidate();
  mxDecayCandidate(const mxDecayCandidate&);
  mxDecayCandidate& operator=(const mxDecayCandidate&);
  virtual ~mxDecayCandidate();
  void Reset();
  void Make(mxCandidate*,mxCandidate*);
  mxCandidate* GetCandidate(int val) {return val<2?fCandidates[val]:NULL;}
  float GetEta() {return fEta;}
  float GetPhi() {return fPhi;}
  float GetEnergyAsymmetry() {return fEnergyAsymmetry;}
  float GetEnergy() {return fEnergy;}
  float GetEnergyT() {return fEnergy/TMath::CosH(fEta);}
  float GetEnergyL() {return fEnergy*TMath::SinH(fEta);}
  float GetEx() {return GetEnergyT()*TMath::Cos(fPhi);}
  float GetEy() {return GetEnergyT()*TMath::Sin(fPhi);}
  float GetEz() {return GetEnergyL();}
  float GetD8() {return fD8;}
  float GetMass() {return fMass;}

 protected:
  mxCandidate* fCandidates[2];
  float fPhi;
  float fEta;
  float fD8;
  float fEnergyAsymmetry;
  float fEnergy;
  float fMass;
};
#endif /* __mxDecayCandidate_H__ */
