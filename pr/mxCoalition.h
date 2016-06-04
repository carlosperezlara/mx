//=========================
// written by Carlos Perez
// 2015-2016
//=========================
#ifndef __mxCoalition_HH__
#define __mxCoalition_HH__

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
  float Eta();
  float Phi();
  float Cov(int);
  void Reset();

 protected:
  mxParty* fParties[9];
  int fNParties;
  float fSgn;
  float fSphi;
  float fSeta;
  float fSphiphi;
  float fSetaeta;
  float fSphieta;
};
#endif /* __mxCoalition_H__ */
