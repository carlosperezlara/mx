#ifndef __mxQAReconstruction_HH__
#define __mxQAReconstruction_HH__

#include <vector>

class TH1F;
class TH2F;
class TProfile;
class TList;
class mxReconstruction;

class mxQAReconstruction {
 public:
  mxQAReconstruction(float maxe=100.);
  ~mxQAReconstruction();

  void Make(mxReconstruction*);
  TList* GetList() {return fList;}

 private:
  TList *fList;

  TH1F *fHvx;
  TH1F *fHvy;
  TH1F *fHvz;

  TProfile *fHHits[2];
  TH1F *fHhitN[2][9];
  TH1F *fHhitE[2][9];
  TH1F *fHhitX[2][9];
  TH1F *fHhitY[2][9];

  TH1F *fHptyN[2][9];
  TH1F *fHptyE[2][9];
  TH2F *fHptyEN[2][9];
  TH1F *fHptyX[2][9];
  TH1F *fHptyY[2][9];
  TH1F *fHptySX[2][9];
  TH1F *fHptySY[2][9];
  TH2F *fHptyESX[2][9];
  TH2F *fHptyESY[2][9];

  TH1F *fHcoaN[2];
  TH1F *fHcoaEn[2];
  TH1F *fHcoaET[2];
  TH1F *fHcoaEL[2];
  TH1F *fHcoaPh[2];
  TH1F *fHcoaEt[2];
  TH1F *fHcoaSPh[2];
  TH1F *fHcoaSEt[2];
  TH1F *fHcoaPty[2];
  TH1F *fHcoaEff[2];
  TH1F *fHcoaSla[2];
  TH2F *fHcoaSta[2];
  TProfile *fHcoaHit[2];
  TH2F *fHcoaEtEn[2];
  TH1F *fHcoaS[2];
  TH2F *fHcoaExMpc[2];
  
  TH1F *fHuniN[2];
  TH1F *fHuniEn[2];
  TH1F *fHuniET[2];
  TH1F *fHuniEL[2];
  TH1F *fHuniDP[2];
  TH1F *fHuniMa[2];
  TH1F *fHuniPh[2];
  TH1F *fHuniEt[2];
};

#endif /* __mxQAReconstruction_H__ */ 
