//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include "TMath.h"

#include "mxCoalition.h"
#include "mxParty.h"

//========
mxCoalition::mxCoalition():
  fNParties(0),
  fSgn(0),
  fSgnE(1),
  fSphi(0),
  fSeta(0),
  fSphiphi(0),
  fSetaeta(0),
  fSphieta(0) {
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
  fSgn = src.fSgn;
  fSgnE = src.fSgnE;
  fSphi = src.fSphi;
  fSeta = src.fSeta;
  fSphiphi = src.fSphiphi;
  fSetaeta = src.fSetaeta;
  fSphieta = src.fSphieta;
  fPEnergy = src.fPEnergy;
  fPEta = src.fPEta;
  fPPhi = src.fPPhi;
  for(int i=0; i!=9; ++i)
    fParties[i] = src.fParties[i];
}
//========
mxCoalition& mxCoalition::operator=(const mxCoalition &src) {
  // asgmnt operator
  if(&src!=this) {
    fNParties = src.fNParties;
    fSgn = src.fSgn;
    fSgnE = src.fSgnE;
    fSphi = src.fSphi;
    fSeta = src.fSeta;
    fSphiphi = src.fSphiphi;
    fSetaeta = src.fSetaeta;
    fSphieta = src.fSphieta;
    fPEnergy = src.fPEnergy;
    fPEta = src.fPEta;
    fPPhi = src.fPPhi;
    for(int i=0; i!=9; ++i)
      fParties[i] = src.fParties[i];
  }
  return *this;
}
//========
float mxCoalition::GetPhi() {
  // <phi>
  if( fNParties<1 ) return 0;
  if( fSgn<1e-6 ) return 0;
  return fSphi/fSgn;
}
//========
float mxCoalition::GetEta() {
  // <eta>
  if( fNParties<1 ) return 0;
  if( fSgn<1e-6 ) return 0;
  return fSeta/fSgn;
}
//========
void mxCoalition::Fill(int lyr,  mxParty *pty, float phi, float eta, float parten, float partet, float partphi) {
  // filler
  if(fParties[lyr]) return;
  fParties[ lyr ] = pty;
  pty->SetAssigned( true );
  float sgn = pty->Signal();
  fSphi += phi*sgn;
  fSeta += eta*sgn;
  fSphiphi += phi*phi*sgn;
  fSetaeta += eta*eta*sgn;
  fSphieta += phi*eta*sgn;
  ++fNParties;
  fSgn += sgn;
  fPEnergy = parten;
  fPEta = partet;
  fPPhi = partphi;
}
//========
float mxCoalition::Test(float phi, float eta, float ephi, float eeta) {
  // tester
  if(fNParties<1) return -1;
  float cx = GetPhi();
  float cy = GetEta();
  float varX = ephi*ephi;
  float varY = eeta*eeta;
  float dx2 = (phi-cx)/varX*(phi-cx);
  float dy2 = (eta-cy)/varY*(eta-cy);
  return TMath::Sqrt( dx2 + dy2 );
}
//========
void mxCoalition::Reset() {
  fSgn=0;
  fSeta=0;
  fSphi=0;
  fSetaeta=0;
  fSphiphi=0;
  fSphieta=0;
  fNParties=0;
  for(int i=0; i!=9; ++i) fParties[i] = NULL;
}
//========
float mxCoalition::GetCov(int dim) {
  // covariance matrix
  if( TMath::AreEqualAbs(fSgn,0,1e-3) ) return -1;
  float var;
  if(dim==0) var = fSphiphi/fSgn-fSphi/fSgn*fSphi/fSgn;
  else if(dim==1) var = fSetaeta/fSgn-fSeta/fSgn*fSeta/fSgn;
  else if(dim==2) var = fSphieta/fSgn-fSphi/fSgn*fSeta/fSgn;
  else var = -1;
  return TMath::Sqrt( var );
}
