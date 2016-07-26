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
  float Test(float,float);
  int N() {return fNParties;}
  mxParty* GetParty(int val) {return val<fNParties?fParties[val]:NULL;}
  float GetEta();
  float GetPhi();
  float GetCov(int);
  float Signal() {return fSgn;}
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
  float fSeta;
  float fSphiphi;
  float fSetaeta;
  float fSphieta;
};
#endif /* __mxCoalition_H__ */