//=========================
// written by Carlos Perez
// 2015-2016
//=========================
#ifndef __mxCandidateCuts_HH__
#define __mxCandidateCuts_HH__

#include "TString.h"

class mxCandidate;
class TH1F;
class TH2F;
class TProfile;
class TList;

class mxCandidateCuts {
 public:
  mxCandidateCuts(TString);
  mxCandidateCuts(const mxCandidateCuts&);
  mxCandidateCuts& operator=(const mxCandidateCuts&);
  mxCandidateCuts* Clone(TString);
  virtual ~mxCandidateCuts();
  bool PassesCuts( mxCandidate* );
  void Set_PS_minNHits(int v) {fPS_minNHits=v;}
  void Set_PS_minSignal(float v) {fPS_minSignal=v;}
  void Set_HitLayer(int v) {fHitLayer[v]=true;}
  void Set_PS_minChi2Prob(float v) {fPS_minChi2Prob=v;}
  void InitQA();
  void FillQA( mxCandidate* );
  TList* GetList() {return fList;}
  void SetQA() {InitQA();}
  void SetEneMax(float v) {fEneMax=v;}
  void SetPSEneMax(float v) {fPSEneMax=v;}

 protected:
  TString fName;
  int fPS_minNHits;
  float fPS_minSignal;
  float fPS_minChi2Prob;
  bool fHitLayer[9];

  float fEneMax;
  float fPSEneMax;

  TList *fList;
  TH1F *fHEne;
  TH1F *fHPSSgn;
  TH1F *fHN;
  TProfile *fHPHits;
  TProfile *fHPEne;
  TH2F *fHPSChi2Prob;
  TH2F *fHEnePSEne;
  TProfile *fHX;
};
#endif /* __mxCandidateCuts_H__ */
