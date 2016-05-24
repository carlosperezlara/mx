//=========================
// written by Carlos Perez
// 2015-2016
//=========================
#ifndef __mxCoalition_HH__
#define __mxCoalition_HH__

class mxParty;

class mxCoalition {
 public:
  mxCoalition(int);
  mxCoalition(const mxCoalition&);
  mxCoalition& operator=(const mxCoalition&);
  virtual ~mxCoalition();
  void Fill(int,float,mxParty*,float,float,float);
  float Test(float,mxParty*,float,float,float);
  int N() {return fNParties;}
  mxParty* GetParty(int val) {return val<fNParties?fParties[val]:NULL;}
  float Eta() {return fEne>0?fSE/fEne:0;}
  float Phi() {return fEne>0?fSP/fEne:0;}
  float Cov(int);
  int Arm() {return fArm;}
  void Reset();

 protected:
  mxParty* fParties[9];
  int fNParties;
  float fZ[9];
  float fEne;
  float fSP;
  float fSE;
  float fS2P;
  float fS2E;
  float fSPE;
  int fArm;
};
#endif /* __mxCoalition_H__ */
