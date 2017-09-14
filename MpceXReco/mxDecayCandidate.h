//=========================
// written by Carlos Perez
// 2015-2016
//=========================
#ifndef __mxDecayCandidate_HH__
#define __mxDecayCandidate_HH__
#include "TMath.h"

class mxCoalition;

class mxDecayCandidate {
 public:
  mxDecayCandidate();
  mxDecayCandidate(const mxDecayCandidate&);
  mxDecayCandidate& operator=(const mxDecayCandidate&);
  virtual ~mxDecayCandidate();
  void Make(mxCoalition*,mxCoalition*);
  mxCoalition* GetCoalition(int val) {return val<2?fCoalitions[val]:NULL;}
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
  mxCoalition* fCoalitions[2];
  float fPhi;
  float fEta;
  float fD8;
  float fEnergyAsymmetry;
  float fEnergy;
  float fMass;
};
#endif /* __mxDecayCandidate_H__ */
