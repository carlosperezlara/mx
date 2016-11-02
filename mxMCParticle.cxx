//=========================
// written by Stacy Karthas
// 2016
//=========================
#include "mxMCParticle.h"


//=======
mxMCParticle::mxMCParticle():
  fPEnergy(0),
  fPEta(0),
  fPPhi(0),
  fPPdg(0){
  //ctor
}
//=======
mxMCParticle::~mxMCParticle() {
  // dtor
}
//=======
mxMCParticle::mxMCParticle(const mxMCParticle &src) {
  // copy ctor
  fPEnergy = src.fPEnergy;
  fPEta = src.fPEta;
  fPPhi = src.fPPhi;
  fPPdg = src.fPPdg;
}
//=======
mxMCParticle& mxMCParticle::operator=(const mxMCParticle &src) {
  // asgmnt operator
  if(&src!=this){
    fPEnergy = src.fPEnergy;
    fPEta= src.fPEta;
    fPPhi= src.fPPhi;
    fPPdg= src.fPPdg;
  }
  return *this;
}
//======
void mxMCParticle::Reset() {
  // clean variables
  fPEnergy = 0;
  fPEta = 0;
  fPPhi = 0;
  fPPdg = 0;
}
//=======
void mxMCParticle::Fill(float energy, float eta, float phi, int pdg) {
  fPEnergy = energy;
  fPEta = eta;
  fPPhi = phi;
  fPPdg = pdg;
}
