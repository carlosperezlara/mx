//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include <iostream>

#include "TMath.h"

#include "mxUnion.h"
#include "mxParty.h"
#include "mxCoalition.h"

//========
mxUnion::mxUnion():
  fPhi(0),
  fEta(0),
  fEnergyAsymmetry(0),
  fEnergy(0),
  fMass(0) {
  // ctor
  fCoalitions[0] = 0;
  fCoalitions[1] = 0;
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
  fEnergyAsymmetry = src.fEnergyAsymmetry;
  fEnergy = src.fEnergy;
  fMass = src.fMass;
  fCoalitions[0] = src.fCoalitions[0];
  fCoalitions[1] = src.fCoalitions[1];
}
//========
mxUnion& mxUnion::operator=(const mxUnion &src) {
  // asgmnt operator
  if(&src!=this) {
    fPhi = src.fPhi;
    fEta = src.fEta;
    fEnergyAsymmetry = src.fEnergyAsymmetry;
    fEnergy = src.fEnergy;
    fMass = src.fMass;
    fCoalitions[0] = src.fCoalitions[0];
    fCoalitions[1] = src.fCoalitions[1];
  }
  return *this;
}
//========
void mxUnion::Make(mxCoalition *un, mxCoalition *tu) {
  fCoalitions[0] = un;
  fCoalitions[1] = tu;
  fEnergy = un->GetEnergy() + tu->GetEnergy();
  fEnergyAsymmetry = (un->GetEnergy() - tu->GetEnergy())/fEnergy;
  float ex1 = un->GetEx();
  float ey1 = un->GetEy();
  float ez1 = un->GetEz();
  float ex2 = tu->GetEx();
  float ey2 = tu->GetEy();
  float ez2 = tu->GetEz();
  float ex = ex1 + ex2;
  float ey = ey1 + ey2;
  float ez = ez1 + ez2;
  fPhi = TMath::ATan2(-ey,-ex)+TMath::Pi();
  fEta = 0.5*TMath::Log((fEnergy+ez)/(fEnergy-ez));
  //float et = TMath::Sqrt(ex*ex+ey*ey);
  float unxtu = ex1*ex2 + ey1*ey2 + ez1*ez2;
  float cosdphi = unxtu / (un->GetEnergy()*tu->GetEnergy());
  fMass = TMath::Sqrt( 2*un->GetEnergy()*tu->GetEnergy()*(1-cosdphi) );
}
