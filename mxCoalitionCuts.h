//=========================
// written by Carlos Perez
// 2015-2016
//=========================
#ifndef __mxCoalitionCuts_HH__
#define __mxCoalitionCuts_HH__

#include "TString.h"

class mxCoalition;
class TH1F;
class TH2F;
class TProfile;
class TList;

class mxCoalitionCuts {
 public:
  mxCoalitionCuts(TString);
  mxCoalitionCuts(const mxCoalitionCuts&);
  mxCoalitionCuts& operator=(const mxCoalitionCuts&);
  mxCoalitionCuts* Clone(TString);
  virtual ~mxCoalitionCuts();
  bool PassesCuts( mxCoalition* );
  void Set_PS_minNHits(int v) {fPS_minNHits=v;}
  void Set_PS_minSignal(float v) {fPS_minSignal=v;}
  void Set_HitLayer(int v) {fHitLayer[v]=true;}
  void Set_PS_minChi2Prob(float v) {fPS_minChi2Prob=v;}
  void InitQA();
  void FillQA( mxCoalition* );
  TList* GetList() {return fList;}
  void SetQA() {InitQA();}

 protected:
 	TString fName;
  int fPS_minNHits;
  float fPS_minSignal;
  float fPS_minChi2Prob;
  bool fHitLayer[9];

 	TList *fList;
	TH1F *fHEne;
	TH1F *fHPSSgn;
	TH1F *fHN;
	TProfile *fHPHits;
	TProfile *fHPEne;
	TH2F *fHPSChi2Prob;
	TH2F *fHX;
};
#endif /* __mxCoalitionCuts_H__ */
