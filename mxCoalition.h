//=========================
// written by Carlos Perez
// 2015-2016
//=========================
#ifndef __mxCoalition_HH__
#define __mxCoalition_HH__

#include "TMath.h"
class mxParty;

class mxCoalition {
 public:
  mxCoalition();
  mxCoalition(const mxCoalition&);
  mxCoalition& operator=(const mxCoalition&);
  virtual ~mxCoalition();
  void Fill(int,mxParty*,float,float);
  float Test(float,float,float,float);
  int N() {return fNParties;}
  mxParty* GetParty(int val) {return IsHitLayer(val)?fParties[val]:NULL;}
  float GetEta();
  float GetEtaVar();
  float GetTheta();
  float GetThetaVar() {return GetCov(1);}
  float GetPhi();
  float GetPhiVar() {return GetCov(0);}
  float GetCov(int);
  float Signal() {return fSgn;}
  float SignalPreShower();
  float GetPSChi2Prob() {return fPSChi2Prob;}
  void SetPSChi2Prob(float v) {fPSChi2Prob = v;}
  int NPreShower();
  float GetEnergy() {return fSgn*fSgnE;}
  float GetEnergyT() {return GetEnergy()/TMath::CosH(GetEta());}
  float GetEnergyL() {return GetEnergy()*TMath::Abs(TMath::TanH(GetEta()));}
  void Reset();
  bool IsHitLayer(int val) {return (fParties[val]!=NULL);}

 protected:
  mxParty* fParties[9];
  int fNParties;
  float fSgn;
  float fSgnE;
  float fSphi;
  float fStheta;
  float fSphiphi;
  float fSthetatheta;
  float fSphitheta;
  float fPhi0;
  float fTheta0;
  float fPSChi2Prob;
};
#endif /* __mxCoalition_H__ */
