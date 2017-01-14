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
  fStheta(0),
  fSphiphi(0),
  fSthetatheta(0),
  fSphitheta(0),
  fPhi0(0),
  fTheta0(0),
  fPSChi2Prob(-1) {
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
  fStheta = src.fStheta;
  fSphiphi = src.fSphiphi;
  fSthetatheta = src.fSthetatheta;
  fSphitheta = src.fSphitheta;
  fPhi0 = src.fPhi0;
  fTheta0 = src.fTheta0;
  fPSChi2Prob = src.fPSChi2Prob;
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
    fStheta = src.fStheta;
    fSphiphi = src.fSphiphi;
    fSthetatheta = src.fSthetatheta;
    fSphitheta = src.fSphitheta;
    fPhi0 = src.fPhi0;
    fTheta0 = src.fTheta0;
    fPSChi2Prob = src.fPSChi2Prob;
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
  return fSphi/fSgn+fPhi0;
}
//========
float mxCoalition::GetTheta() {
  // <theta>
  if( fNParties<1 ) return 0;
  if( fSgn<1e-6 ) return 0;
  return fStheta/fSgn+fTheta0;
}
//========
float mxCoalition::GetEta() {
  // <eta>
  float theta = GetTheta();
  float eta = -TMath::Log( TMath::Tan(theta/2.0) );
  return eta;
}
//========
float mxCoalition::SignalPreShower() {
  float sgn = 0;
  mxParty *pty;
  for(int l=0; l!=8; ++l) {
    pty = GetParty(l);
    if(pty) sgn += pty->Signal();
  }
  return sgn;
}
//========
int mxCoalition::NPreShower() {
  int hts = 0;
  for(int l=0; l!=8; ++l) {
    if(IsHitLayer(l)) hts++;
  }
  return hts;
}
//========
void mxCoalition::Fill(int lyr,  mxParty *pty, float phi, float theta) {
  // filler
  if(fParties[lyr]) return;
  fParties[ lyr ] = pty;
  pty->SetAssigned( true );
  float sgn = pty->Signal();
  fSgn += sgn;
  if(fNParties==0) {
    //seeding
    fPhi0 = phi;
    fTheta0 = theta;
    fSphi = 0;
    fStheta = 0;
    fSphiphi = 0;
    fSthetatheta = 0;
    fSphitheta = 0;
  } else {
    //adding
    float dphi = phi - fPhi0; // avoids catastrophic cancellation
    float dtheta = theta - fTheta0; // avoids catastrophic cancellation
    fSphi += dphi*sgn;
    fStheta += dtheta*sgn;
    fSphiphi += dphi*dphi*sgn;
    fSthetatheta += dtheta*dtheta*sgn;
    fSphitheta += dphi*dtheta*sgn;
  }
  ++fNParties;
}
//========
float mxCoalition::Test(float phi, float theta, float ephi, float etheta) {
  // tester
  if(fNParties<1) return -1;
  float cx = GetPhi();
  float cy = GetTheta();
  float varX = ephi*ephi;
  float varY = etheta*etheta;
  float dx2 = (phi-cx)/varX*(phi-cx);
  float dy2 = (theta-cy)/varY*(theta-cy);
  return TMath::Sqrt( dx2 + dy2 );
}
//========
void mxCoalition::Reset() {
  fSgn=0;
  fStheta=0;
  fSphi=0;
  fSthetatheta=0;
  fSphiphi=0;
  fSphitheta=0;
  fPhi0=0;
  fTheta0=0;
  fNParties=0;
  fPSChi2Prob = -1;
  for(int i=0; i!=9; ++i) fParties[i] = NULL;
}
//========
float mxCoalition::GetCov(int dim) {
  // covariance matrix
  if( TMath::AreEqualAbs(fSgn,0,1e-6) ) return -1;
  float var;
  if(dim==0) var = fSphiphi/fSgn-fSphi/fSgn*fSphi/fSgn;
  else if(dim==1) var = fSthetatheta/fSgn-fStheta/fSgn*fStheta/fSgn;
  else if(dim==2) var = fSphitheta/fSgn-fSphi/fSgn*fStheta/fSgn;
  else var = -1;
  if( TMath::AreEqualAbs(var,0,1e-6) ) var = 0;
  return var;
}
//========
float mxCoalition::GetEtaVar() {
  float sint2 = TMath::Sin(GetTheta())*TMath::Sin(GetTheta());
  if( TMath::AreEqualAbs(sint2,0,1e-6) ) return -1;
  return GetCov(1)/sint2;
}

