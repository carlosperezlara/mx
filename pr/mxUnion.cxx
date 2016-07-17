//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include "TMath.h"

#include "mxUnion.h"
#include "mxParty.h"
#include "mxCoalition.h"

//========
mxUnion::mxUnion():
  fPhi(0),
  fEta(0),
  fEnergy(0),
  fEnergyT(0),
  fEnergyL(0),
  fMass(0) {
  // ctor
}
//========
mxUnion::~mxUnion() {
  // dtor
}
//========
mxUnion::mxUnion(const mxUnion &src) {
  // copy ctor
  fPhi = src.fPhi;
  fEta = src.fEta;
  fEnergy = src.fEnergy;
  fEnergyT = src.fEnergyT;
  fEnergyL = src.fEnergyL;
  fMass = src.fMass;
}
//========
mxUnion& mxUnion::operator=(const mxUnion &src) {
  // asgmnt operator
  if(&src!=this) {
    fPhi = src.fPhi;
    fEta = src.fEta;
    fEnergy = src.fEnergy;
    fEnergyT = src.fEnergyT;
    fEnergyL = src.fEnergyL;
    fMass = src.fMass;
  }
  return *this;
}
//========
void mxUnion::Make(mxCoalition *un, mxCoalition *tu) {
  fCoalitions[0] = un;
  fCoalitions[1] = tu;
  fPhi = 0.5*(un->Phi() + tu->Phi());
  fEta = 0.5*(un->Eta() + tu->Eta());
  float cosdphi = TMath::Cos(un->Phi() - tu->Phi());
  float cosheta = TMath::CosH(un->Eta())*TMath::CosH(tu->Eta());
  float etet = un->Energy()*tu->Energy()/cosheta;
  float ee = un->Energy()*tu->Energy();
  fDPhi = etet/ee*(cosdphi+cosheta);
  fMass = un->Energy()*tu->Energy()*(1-TMath::Cos(fDPhi));
  fEnergyT = fEnergy/TMath::CosH(fEta);
  fEnergyL = fEnergy*TMath::TanH(fEta);
}
