//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include <iostream>

#include "TMath.h"

#include "mxDecayCandidate.h"
#include "mxCluster.h"
#include "mxCandidate.h"

//========
mxDecayCandidate::mxDecayCandidate():
  fPhi(0),
  fEta(0),
  fD8(-1),
  fEnergyAsymmetry(0),
  fEnergy(0),
  fMass(0) {
  // ctor
  fCandidates[0] = 0;
  fCandidates[1] = 0;
}
//========
mxDecayCandidate::~mxDecayCandidate() {
  // dtor
}
//========
mxDecayCandidate::mxDecayCandidate(const mxDecayCandidate &src) {
  // copy ctor
  fPhi = src.fPhi;
  fEta = src.fEta;
  fD8 = src.fD8;
  fEnergyAsymmetry = src.fEnergyAsymmetry;
  fEnergy = src.fEnergy;
  fMass = src.fMass;
  fCandidates[0] = src.fCandidates[0];
  fCandidates[1] = src.fCandidates[1];
}
//========
mxDecayCandidate& mxDecayCandidate::operator=(const mxDecayCandidate &src) {
  // asgmnt operator
  if(&src!=this) {
    fPhi = src.fPhi;
    fEta = src.fEta;
    fD8 = src.fD8;
    fEnergyAsymmetry = src.fEnergyAsymmetry;
    fEnergy = src.fEnergy;
    fMass = src.fMass;
    fCandidates[0] = src.fCandidates[0];
    fCandidates[1] = src.fCandidates[1];
  }
  return *this;
}
//========
void mxDecayCandidate::Make(mxCandidate *un, mxCandidate *tu) {
  fCandidates[0] = un;
  fCandidates[1] = tu;
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
  mxCluster *pty1 = un->GetCluster(8);
  mxCluster *pty2 = tu->GetCluster(8);
  if(pty1&&pty2) {
    float x1 = pty1->GetX();
    float y1 = pty1->GetY();
    float x2 = pty2->GetX();
    float y2 = pty2->GetY();
    fD8 = TMath::Sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
  }
  fPhi = TMath::ATan2(-ey,-ex)+TMath::Pi();
  fEta = 0.5*TMath::Log((fEnergy+ez)/(fEnergy-ez));
  //float et = TMath::Sqrt(ex*ex+ey*ey);
  float unxtu = ex1*ex2 + ey1*ey2 + ez1*ez2;
  float cosdphi = unxtu / (un->GetEnergy()*tu->GetEnergy());
  fMass = TMath::Sqrt( 2*un->GetEnergy()*tu->GetEnergy()*(1-cosdphi) );
}
