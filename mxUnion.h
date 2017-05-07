//=========================
// written by Carlos Perez
// 2015-2016
//=========================
#ifndef __mxUnion_HH__
#define __mxUnion_HH__
#include "TMath.h"

class mxCoalition;

class mxUnion {
 public:
  mxUnion();
  mxUnion(const mxUnion&);
  mxUnion& operator=(const mxUnion&);
  virtual ~mxUnion();
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
#endif /* __mxUnion_H__ */
