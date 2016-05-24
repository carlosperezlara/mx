//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include "phMath.h"
#include "mxCoalition.h"
#include "mxParty.h"

//========
mxCoalition::mxCoalition(int arm):
  fNParties(0),
  fEne(0),
  fSP(0),
  fSE(0),
  fS2P(0),
  fS2E(0),
  fSPE(0),
  fArm(arm) {
  // ctor
  for(int i=0; i!=9; ++i) fParties[i]=NULL;
}
//========
mxCoalition::~mxCoalition() {
  // dtor
}
//========
mxCoalition::mxCoalition(const mxCoalition &src) {
  // copy ctor
  fNParties = src.fNParties;
  fEne = src.fEne;
  fSP = src.fSP;
  fSE = src.fSE;
  fS2P = src.fS2P;
  fS2E = src.fS2E;
  fSPE = src.fSPE;
  fArm = src.fArm;
  for(int i=0; i!=9; ++i)
    fParties[i] = src.fParties[i];
}
//========
mxCoalition& mxCoalition::operator=(const mxCoalition &src) {
  // asgmnt operator
  if(&src!=this) {
    fNParties = src.fNParties;
    fEne = src.fEne;
    fSP = src.fSP;
    fSE = src.fSE;
    fS2P = src.fS2P;
    fS2E = src.fS2E;
    fSPE = src.fSPE;
    fArm = src.fArm;
    for(int i=0; i!=9; ++i)
      fParties[i] = src.fParties[i];
  }
  return *this;
}
//========
void mxCoalition::Fill(int lyr, float z, mxParty *pty, float vx, float vy, float vz) {
  // filler
  if(fNParties>9) return;
  if(fParties[lyr]) return; // ignoring
  fParties[ lyr ] = pty;
  float ene = pty->Signal();
  float phi = _phi( pty->X()-vx, pty->Y()-vy  );
  float eta = _eta( pty->X()-vx, pty->Y()-vy, z-vz  );
  fSP = fSP + phi*ene;
  fSE = fSE + eta*ene;
  fS2P = fS2P + phi*phi*ene;
  fS2E = fS2E + eta*eta*ene;
  fSPE = fSPE + phi*eta*ene;
  ++fNParties;
  fEne += ene;
}
//========
float mxCoalition::Test(float z, mxParty *pty, float vx, float vy, float vz) {
  // tester
  if( TMath::AreEqualAbs(fEne,0,1e-3) ) return -1;
  float phi = _phi( pty->X()-vx, pty->Y()-vy  );
  float eta = _eta( pty->X()-vx, pty->Y()-vy, z-vz  );
  float cx = fSP/fEne;
  float cy = fSE/fEne;
  float varX = fS2P/fEne-fSP/fEne*fSP/fEne;
  float varY = fS2E/fEne-fSE/fEne*fSE/fEne;
  if(fNParties<3) {
    varX = 0.05;
    varY = 0.05;
  }
  float dx2 = (phi-cx)/varX*(phi-cx);
  float dy2 = (eta-cy)/varY*(eta-cy);
  return TMath::Sqrt( dx2 + dy2 );
}
//========
void mxCoalition::Reset() {
  fEne=0;
  fSE=0;
  fSP=0;
  fS2E=0;
  fS2P=0;
  fSPE=0;
  fNParties=0;
  for(int i=0; i!=0; ++i) fParties[i] = NULL;
}
//========
float mxCoalition::Cov(int dim) {
  // covariance matrix
  if( TMath::AreEqualAbs(fEne,0,1e-3) ) return -1;
  float var;
  if(dim==0) var = fS2P/fEne-fSP/fEne*fSP/fEne;
  else if(dim==1) var = fS2E/fEne-fSE/fEne*fSE/fEne;
  else if(dim==2) var = fSPE/fEne-fSP/fEne*fSE/fEne;
  else var = -1;
  return TMath::Sqrt( var );
}
