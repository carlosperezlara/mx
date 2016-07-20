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
  fPhi = 0.5*(un->GetPhi() + tu->GetPhi());
  fEta = 0.5*(un->GetEta() + tu->GetEta());
  fEnergy = un->GetEnergy() + tu->GetEnergy();
  fEnergyT = fEnergy/TMath::CosH(fEta);
  fEnergyL = fEnergy*TMath::Abs(TMath::TanH(fEta));
  float cosdphi = TMath::Cos(un->GetPhi() - tu->GetPhi());
  //float cosheta = TMath::CosH(un->GetEta())*TMath::CosH(tu->GetEta());
  //float etet = un->GetEnergy()*tu->GetEnergy()/cosheta;
  //float ee = un->GetEnergy()*tu->GetEnergy();
  //fDPhi = TMath::ACos( etet/ee*(cosdphi+cosheta) );
  //std::cout << fDPhi << " ";
  //
  float t1 = 2*TMath::ATan(TMath::Exp(-un->GetEta()));
  float t2 = 2*TMath::ATan(TMath::Exp(-tu->GetEta()));
  float sinsin = TMath::Sin(t1)*TMath::Sin(t2);
  float coscos = TMath::Cos(t1)*TMath::Cos(t2);
  fDPhi = TMath::ACos( coscos+cosdphi*sinsin );
  //std::cout << fDPhi << " ";
  //
  //float sinhsinh = TMath::SinH(un->GetEta())*TMath::SinH(tu->GetEta());
  //fDPhi = TMath::ACos( cosdphi + sinhsinh );
  //std::cout << fDPhi << std::endl;

  //std::cout << fDPhi << std::endl;
  fMass = un->GetEnergy()*tu->GetEnergy()*(1-TMath::Cos(fDPhi));
}
