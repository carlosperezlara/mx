//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include <iostream>
#include "mxCoalitionCuts.h"
#include "mxCoalition.h"
#include "mxParty.h"
#include "mxHit.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TList.h"
#include "TString.h"
//========
mxCoalitionCuts::mxCoalitionCuts(TString name):
  fName(name),
  fPS_minNHits(0),
  fPS_minSignal(0),
  fPS_minChi2Prob(0),
  fQA(false),
  fList(NULL),
  fHEne(NULL),
  fHPSSgn(NULL),
  fHN(NULL),
  fHPHits(NULL),
  fHPEne(NULL),
  fHPSChi2Prob(NULL),
  fHX(NULL) {
  // ctor
  for(int i=0; i!=9; ++i) fHitLayer[i] = false;
}
//========
mxCoalitionCuts::~mxCoalitionCuts() {
  // dtor
  if(fList) delete fList;
}
//========
void mxCoalitionCuts::InitQA() {
  std::cout << "mxCoalitionCuts::QA for " << fName.Data() << std::endl;
  fList = new TList();
  fList->SetOwner();
  fHEne = new TH1F(Form("%s_Ene",fName.Data()), Form("%s_Ene",fName.Data()), 100,0,100);
  fHPSSgn = new TH1F(Form("%s_PS_Sgn",fName.Data()), Form("%s_PS_Sgn",fName.Data()), 100,0,0.7);
  fHN = new TH1F(Form("%s_N",fName.Data()), Form("%s_N",fName.Data()), 10,-0.5,9.5);
  fHPHits = new TProfile(Form("%s_Hits",fName.Data()), Form("%s_Hits",fName.Data()), 9,-0.5,8.5);
  fHPEne = new TProfile(Form("%s_PEne",fName.Data()), Form("%s_PEne",fName.Data()), 9,-0.5,8.5);
  fHPSChi2Prob = new TH2F(Form("%s_PS_Chi2Pro",fName.Data()), Form("%s_PS_Chi2Pro",fName.Data()), 1001,0,7, 100,0,1);
  fHX = new TH2F(Form("%s_X",fName.Data()), Form("%s_X",fName.Data()), 9,-0.5,8.5, 101,-5,+5);
  fList->Add( fHEne );
  fList->Add( fHPSSgn );
  fList->Add( fHN );
  fList->Add( fHPHits );
  fList->Add( fHPEne );
  fList->Add( fHPSChi2Prob );
  fList->Add( fHX );
}
//========
void mxCoalitionCuts::FillQA(mxCoalition *coalition) {
  if(!fList) InitQA();
  fHEne->Fill( coalition->GetEnergy() );
  fHPSSgn->Fill( coalition->SignalPreShower() );
  fHPSChi2Prob->Fill( coalition->SignalPreShower(), coalition->GetPSChi2Prob() );
  fHN->Fill( coalition->N() );
  for(int j=0; j!=9; ++j) {
    mxParty *pty = coalition->GetParty(j);
    if(!pty) continue;
    fHPHits->Fill(j,pty->N());
    fHPEne->Fill(j,pty->Signal());
    if(j==8) continue;
    for(int k=0; k!=pty->N(); ++k) {
      mxHit *hit = pty->GetHit(k);
      //if(j%2==0) fHX->Fill(j, geo->X(hit->Idx()) - pty->GetX() );
      //if(j%2==1) fHX->Fill(j, geo->Y(hit->Idx()) - pty->GetY() );
    }
  }
}
//========
mxCoalitionCuts::mxCoalitionCuts(const mxCoalitionCuts &src) {
  // copy ctor
  fName = src.fName;
  fPS_minNHits = src.fPS_minNHits;
  fPS_minSignal = src.fPS_minSignal;
  fPS_minChi2Prob = src.fPS_minChi2Prob;
  for(int i=0; i!=9; ++i) fHitLayer[i] = src.fHitLayer[i];
  fQA = src.fQA;
  fList = NULL;
  fHEne = NULL;
  fHPSSgn = NULL;
  fHN = NULL;
  fHPHits = NULL;
  fHPEne = NULL;
  fHPSChi2Prob = NULL;
  fHX = NULL;
}
//========
mxCoalitionCuts& mxCoalitionCuts::operator=(const mxCoalitionCuts &src) {
  // asgmnt operator
  if(&src!=this) {
    fName = src.fName;
    fPS_minNHits = src.fPS_minNHits;
    fPS_minSignal = src.fPS_minSignal;
    fPS_minChi2Prob = src.fPS_minChi2Prob;
    for(int i=0; i!=9; ++i) fHitLayer[i] = src.fHitLayer[i];
    fQA = src.fQA;
    fList = NULL;
    fHEne = NULL;
    fHPSSgn = NULL;
    fHN = NULL;
    fHPHits = NULL;
    fHPEne = NULL;
    fHPSChi2Prob = NULL;
    fHX = NULL;
  }
  return *this;
}
//========
mxCoalitionCuts* mxCoalitionCuts::Clone(TString name) {
  mxCoalitionCuts *cut = new mxCoalitionCuts(name);
  cut->fPS_minNHits = fPS_minNHits;
  cut->fPS_minSignal = fPS_minSignal;
  cut->fPS_minChi2Prob = fPS_minChi2Prob;
  for(int i=0; i!=9; ++i) cut->fHitLayer[i] = fHitLayer[i];
  cut->fQA = fQA;
  return cut;
}
//========
bool mxCoalitionCuts::PassesCuts(mxCoalition *coa) {
  if(fPS_minNHits > coa->NPreShower() ) return false;
  if(fPS_minSignal > coa->SignalPreShower() ) return false;
  if(fPS_minChi2Prob > coa->GetPSChi2Prob() ) return false;
  for(int i=0; i!=9; ++i)
    if(fHitLayer[i])
      if(!coa->IsHitLayer(i)) return false;
  if(fQA) FillQA(coa);
  return true;
}
