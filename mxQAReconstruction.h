#ifndef __mxQAReconstruction_HH__
#define __mxQAReconstruction_HH__

#include <vector>

class TH1F;
class TH2F;
class TList;
class mxReconstruction;

class mxQAReconstruction {
 public:
  mxQAReconstruction();
  ~mxQAReconstruction();

  void Make(mxReconstruction*);
  TList* GetList() {return fList;}

 private:
  TList *fList;

  TH1F *fHvx;
  TH1F *fHvy;
  TH1F *fHvz;

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
  TH1D *fHcoaLpC[2];
  
  TH1F *fHuniN[2];
  TH1F *fHuniEn[2];
  TH1F *fHuniET[2];
  TH1F *fHuniEL[2];
  TH1F *fHuniDP[2];
  TH1F *fHuniMa[2];
  TH1F *fHuniPh[2];
  TH1F *fHuniEt[2];

  TH1F *fHpcEn[2];
  TH1D *fHpcLyr[2];
  TH1F *fHpcEffic[2];
};

#endif /* __mxQAReconstruction_H__ */ 
