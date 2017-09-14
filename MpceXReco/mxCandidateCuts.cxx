//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include <iostream>
#include "mxCandidateCuts.h"
#include "mxCandidate.h"
#include "mxCluster.h"
#include "mxHit.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TList.h"
#include "TString.h"
//========
mxCandidateCuts::mxCandidateCuts(TString name):
  fName(name),
  fPS_minNHits(0),
  fPS_minSignal(0),
  fPS_minChi2Prob(0),
  fEneMax(30),
  fPSEneMax(0.1),
  fList(NULL),
  fHEne(NULL),
  fHPSSgn(NULL),
  fHN(NULL),
  fHPHits(NULL),
  fHPEne(NULL),
  fHPSChi2Prob(NULL),
  fHEnePSEne(NULL),
  fHX(NULL) {
  // ctor
  for(int i=0; i!=9; ++i) fHitLayer[i] = false;
}
//========
mxCandidateCuts::~mxCandidateCuts() {
  // dtor
  if(fList) delete fList;
}
//========
void mxCandidateCuts::InitQA() {
  std::cout << "mxCandidateCuts::QA for " << fName.Data() << std::endl;
  fList = new TList();
  fList->SetOwner();
  fHEne = new TH1F(Form("%s_Ene",fName.Data()), Form("%s_Ene;Energy in GeV",fName.Data()), 1000,0,fEneMax);
  fHPSSgn = new TH1F(Form("%s_PS_Sgn",fName.Data()), Form("%s_PS_Sgn;Energy in GeV",fName.Data()), 1000,0,fPSEneMax);
  fHN = new TH1F(Form("%s_N",fName.Data()), Form("%s_N;Number of Layers Hit",fName.Data()), 10,-0.5,9.5);
  fHPHits = new TProfile(Form("%s_Hits",fName.Data()), Form("%s_Hits;Layer Idx",fName.Data()), 9,-0.5,8.5);
  fHPEne = new TProfile(Form("%s_PEne",fName.Data()), Form("%s_PEne;Layer Idx",fName.Data()), 9,-0.5,8.5);
  fHPSChi2Prob = new TH2F(Form("%s_PS_Chi2Pro",fName.Data()), Form("%s_PS_Chi2Pro;Energy in GeV;Prob",fName.Data()), 100,0,fPSEneMax, 100,0,1);
  fHEnePSEne = new TH2F(Form("%s_Ene_PSEne",fName.Data()), Form("%s_Ene_PSEne;Energy in GeV;Energy in GeV",fName.Data()), 100,0,fPSEneMax, 100,0,fEneMax);
  fHX = new TProfile(Form("%s_X",fName.Data()), Form("%s_X;Layer Idx;Cluster Width in mm",fName.Data()), 9,-0.5,8.5);
  fList->Add( fHEne );
  fList->Add( fHPSSgn );
  fList->Add( fHN );
  fList->Add( fHPHits );
  fList->Add( fHPEne );
  fList->Add( fHPSChi2Prob );
  fList->Add( fHEnePSEne );
  fList->Add( fHX );
}
//========
void mxCandidateCuts::FillQA(mxCandidate *coalition) {
  fHEne->Fill( coalition->GetEnergy() );
  fHPSSgn->Fill( coalition->SignalPreShower() );
  fHPSChi2Prob->Fill( coalition->SignalPreShower(), coalition->GetPSChi2Prob() );
  fHEnePSEne->Fill( coalition->SignalPreShower(), coalition->SignalPbWO4() );
  fHN->Fill( coalition->N() );
  for(int j=0; j!=9; ++j) {
    mxCluster *pty = coalition->GetCluster(j);
    if(!pty) continue;
    fHPHits->Fill(j,pty->N());
    fHPEne->Fill(j,pty->Signal());
    if(j==8) continue;
    int nh = pty->GetSizeUmbral(0.7);
    float dx=0;
    if(j%2==0) dx = pty->GetDx();
    if(j%2==1) dx = pty->GetDy();
    fHX->Fill(j, nh*dx );
  }
}
//========
mxCandidateCuts::mxCandidateCuts(const mxCandidateCuts &src) {
  // copy ctor
  fName = src.fName;
  fPS_minNHits = src.fPS_minNHits;
  fPS_minSignal = src.fPS_minSignal;
  fPS_minChi2Prob = src.fPS_minChi2Prob;
  fEneMax = src.fEneMax;
  fPSEneMax = src.fPSEneMax;
  for(int i=0; i!=9; ++i) fHitLayer[i] = src.fHitLayer[i];
  fList = NULL;
  fHEne = NULL;
  fHPSSgn = NULL;
  fHN = NULL;
  fHPHits = NULL;
  fHPEne = NULL;
  fHPSChi2Prob = NULL;
  fHEnePSEne = NULL;
  fHX = NULL;
}
//========
mxCandidateCuts& mxCandidateCuts::operator=(const mxCandidateCuts &src) {
  // asgmnt operator
  if(&src!=this) {
    fName = src.fName;
    fPS_minNHits = src.fPS_minNHits;
    fPS_minSignal = src.fPS_minSignal;
    fPS_minChi2Prob = src.fPS_minChi2Prob;
    fEneMax = src.fEneMax;
    fPSEneMax = src.fPSEneMax;
    for(int i=0; i!=9; ++i) fHitLayer[i] = src.fHitLayer[i];
    fList = NULL;
    fHEne = NULL;
    fHPSSgn = NULL;
    fHN = NULL;
    fHPHits = NULL;
    fHPEne = NULL;
    fHPSChi2Prob = NULL;
    fHEnePSEne = NULL;
    fHX = NULL;
  }
  return *this;
}
//========
mxCandidateCuts* mxCandidateCuts::Clone(TString name) {
  mxCandidateCuts *cut = new mxCandidateCuts(name);
  cut->fPS_minNHits = fPS_minNHits;
  cut->fPS_minSignal = fPS_minSignal;
  cut->fPS_minChi2Prob = fPS_minChi2Prob;
  for(int i=0; i!=9; ++i) cut->fHitLayer[i] = fHitLayer[i];
  return cut;
}
//========
bool mxCandidateCuts::PassesCuts(mxCandidate *coa) {
  //std::cout << "Passes INIT" << fName.Data() << std::endl;
  //std::cout << coa->NPreShower() << " ";
  //std::cout << coa->SignalPreShower() << " ";
  //std::cout << coa->GetPSChi2Prob() << " ";
  //std::cout << std::endl;
  if(fPS_minNHits > coa->NPreShower() ) return false;
  if(fPS_minSignal > coa->SignalPreShower() ) return false;
  if(fPS_minChi2Prob > coa->GetPSChi2Prob() ) return false;
  for(int i=0; i!=9; ++i)
    if(fHitLayer[i])
      if(!coa->IsHitLayer(i)) return false;
  //std::cout << "Passes END" << fName.Data() << std::endl;
  if(fList) FillQA(coa);
  return true;
}
