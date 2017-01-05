//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
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
  TProfile *fHEnergy[2];
  //  TH1F *fHHitsx[2];

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
  TH1F *fHcoaX[2];
  TH1F *fHcoaY[2];
  TH1F *fHcoaEt[2];
  TH1F *fHcoaSPh[2];
  TH1F *fHcoaSEt[2];
  TH1F *fHcoaPty[2];
  TH1F *fHcoaEff[2];
  TH1F *fHcoaSla[2];
  TH2F *fHcoaPS[2];
  TH2F *fHcoaSta[2];
  TProfile *fHcoaHit[2];
  TH1F *fHcoaHitx[2];
  TH2F *fHcoaHitxTH2F[2];
  TProfile *fHcoaHitxProf[2];
  TProfile *fHcoaEnergy[2];
  TH1F *fHcoaEnergyx[2];
  TH2F *fHcoaEnergyxTH2F[2];
  TProfile *fHcoaEnergyxProf[2];
  TH2F *fHcoaEtEn[2];
  TProfile *fHcoaEtEnProf[2];
  TH1F *fHcoaS[2];
  TH2F *fHcoaExMpc[2];
  TH2F *fHcoaTruthE[2];
  TH2F *fHcoaTruthEL[2];
  TH2F *fHcoaTruthET[2];
  TH1F *fHcoaTruthdET[2];
  TH2F *fHcoaTruthEloc[2];
  TH2F *fHcoaTruthElocth[2];
  TH1F *fHcoaTruthdE[2];
  TH2F *fHcoaTruthPhi[2];
  TH1F *fHcoaTruthdPhi[2];
  TH2F *fHcoaTruthEta[2];
  TH1F *fHcoaTruthdEta[2];
  TH1F *fHcoaTruthEtaRat[2];
  
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
