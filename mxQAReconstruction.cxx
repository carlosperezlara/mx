//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include <iostream>
#include <vector>
#include <algorithm>

#include "TMath.h"
#include "TList.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TString.h"

#include "mxReconstruction.h"
#include "mxQAReconstruction.h"
#include "mxUnion.h"
#include "mxCoalition.h"
#include "mxParty.h"
#include "mxHit.h"
#include "mxGeometry.h"

//========
mxQAReconstruction::mxQAReconstruction(float maxe) {
  // ctor
  std::cout << "mxQAReconstruction:: ctor" << std::endl;
  fList = new TList();
  //fList->SetOwner();

  fHvx = new TH1F("mxReco_vx","mxReco_vx",100,-2,+2);
  fList->Add( fHvx );
  fHvy = new TH1F("mxReco_vy","mxReco_vy",100,-2,+2);
  fList->Add( fHvy );
  fHvz = new TH1F("mxReco_vz","mxReco_vz",100,-10,+10);
  fList->Add( fHvz );

  for(int arm=0; arm!=2; ++arm) {
    double sn = 0.0;
    if (arm == 0) sn = 1.0;
    for(int lyr=0; lyr!=9; ++lyr) {
      float emax = 1.8e-3; // 1.8 MeV
      float smax= 1;
      if(lyr==8||lyr==17) {
      	emax = maxe;
      	smax = 10;
      }
      //HIT
      fHhitN[arm][lyr] = new TH1F( Form("mxReco_%s%d_hitN",arm==0?"S":"N",lyr), Form("mxReco_%s%d_hitN;NO of HITS",arm==0?"S":"N",lyr), 800,-0.5,799.5);
      fList->Add( fHhitN[arm][lyr] );
      fHhitE[arm][lyr] = new TH1F( Form("mxReco_%s%d_hitE",arm==0?"S":"N",lyr), Form("mxReco_%s%d_hitE;ENERGY  GEV",arm==0?"S":"N",lyr), 500,0,emax);
      fList->Add( fHhitE[arm][lyr] );

      //PARTY
      fHptyN[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptyN",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptyN;NO of PARTIES",arm==0?"S":"N",lyr), 100,-0.5,99.5);
      fList->Add( fHptyN[arm][lyr] );
      fHptyE[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptyE",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptyE;ENERGY  GEV",arm==0?"S":"N",lyr), 100,0,emax);
      fList->Add( fHptyE[arm][lyr] );
      fHptyX[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptyX",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptyX;X  CM",arm==0?"S":"N",lyr), 100,-22,+22);
      fList->Add( fHptyX[arm][lyr] );
      fHptyY[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptyY",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptyY;Y  CM",arm==0?"S":"N",lyr), 100,-22,+22);
      fList->Add( fHptyY[arm][lyr] );
      fHptySX[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptySX",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptySX;SPREAD X  CM",arm==0?"S":"N",lyr), 100,0,smax);
      fList->Add( fHptySX[arm][lyr] );
      fHptySY[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptySY",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptySY;SPREAD Y  CM",arm==0?"S":"N",lyr), 100,0,smax);
      fList->Add( fHptySY[arm][lyr] );
      //COALITION-PERLAYER
      fHptyEN[arm][lyr] = new TH2F( Form("mxReco_%s%d_coaptyEN",arm==0?"S":"N",lyr), Form("mxReco_%s%d_coaptyEN;COA-ENERGY  GEV;NO of HITS",arm==0?"S":"N",lyr), 100,0,maxe, 60,-0.5,59.5);
      fList->Add( fHptyEN[arm][lyr] );
      fHptyESX[arm][lyr] = new TH2F( Form("mxReco_%s%d_coaptyESX",arm==0?"S":"N",lyr), Form("mxReco_%s%d_coaptyESX;COA-ENERGY  GEV;SPREAD X  CM",arm==0?"S":"N",lyr), 100,0,maxe, 100,0,smax);
      fList->Add( fHptyESX[arm][lyr] );
      fHptyESY[arm][lyr] = new TH2F( Form("mxReco_%s%d_coaptyESY",arm==0?"S":"N",lyr), Form("mxReco_%s%d_coaptyESY;COA-ENERGY  GEV;SPREAD Y  CM",arm==0?"S":"N",lyr), 100,0,maxe, 100,0,smax);
      fList->Add( fHptyESY[arm][lyr] );
    }
    //COALITION
    fHHits[arm] = new TProfile( Form("mxReco_%s_HitsPerLayer",arm==0?"S":"N"), Form("mxReco_%s_HitPerLayer;LAYER;AvgHits",arm==0?"S":"N"), 9,-0.5,8.5);
    fList->Add( fHHits[arm] );
    fHEnergy[arm] = new TProfile( Form("mxReco_%s_EnergyHitsPerLayer",arm==0?"S":"N"), Form("mxReco_%s_EnergyHitPerLayer;LAYER;AvgEnergy",arm==0?"S":"N"), 9,-0.5,8.5);
    fList->Add( fHEnergy[arm] );
    fHcoaN[arm] = new TH1F( Form("mxReco_%s_coaN",arm==0?"S":"N"), Form("mxReco_%s_coaN;NO of COALITIONS",arm==0?"S":"N"), 100,-0.5,99.5);
    fList->Add( fHcoaN[arm] );
    fHcoaEn[arm] = new TH1F( Form("mxReco_%s_coaEn",arm==0?"S":"N"), Form("mxReco_%s_coaEn;ENERGY  GEV",arm==0?"S":"N"), 100,0,maxe);
    fList->Add( fHcoaEn[arm] );
    fHcoaPS[arm] = new TH2F( Form("mxReco_%s_coaPS",arm==0?"S":"N"), Form("mxReco_%s_coaPS;Signal in Preshower  GEV;No Parties in PreShower",arm==0?"S":"N"), 100,0,0.5, 9,-0.5,8.5 );
    fList->Add( fHcoaPS[arm] );
    fHcoaET[arm] = new TH1F( Form("mxReco_%s_coaET",arm==0?"S":"N"), Form("mxReco_%s_coaET;ENERGY_{T}  GEV",arm==0?"S":"N"), 100,0,maxe/10);
    fList->Add( fHcoaET[arm] );
    fHcoaEL[arm] = new TH1F( Form("mxReco_%s_coaEL",arm==0?"S":"N"), Form("mxReco_%s_coaEL;ENERGY_{L}  GEV",arm==0?"S":"N"), 100,0,maxe);
    fList->Add( fHcoaEL[arm] );
    fHcoaPh[arm] = new TH1F( Form("mxReco_%s_coaPh",arm==0?"S":"N"), Form("mxReco_%s_coaPh;PHI  RAD",arm==0?"S":"N"), 100,0,TMath::TwoPi());
    fList->Add( fHcoaPh[arm] );
    fHcoaX[arm] = new TH1F( Form("mxReco_%s_coaX",arm==0?"S":"N"), Form("mxReco_%s_coaX;X",arm==0?"S":"N"), 500,-25,25);
    fList->Add( fHcoaX[arm] );
    fHcoaEt[arm] = new TH1F( Form("mxReco_%s_coaEt",arm==0?"S":"N"), Form("mxReco_%s_coaEt;ETA",arm==0?"S":"N"), 100,-4.2,+4.2);
    fList->Add( fHcoaEt[arm] );
    fHcoaY[arm] = new TH1F( Form("mxReco_%s_coaY",arm==0?"S":"N"), Form("mxReco_%s_coaY;Y",arm==0?"S":"N"), 500,-25,25);
    fList->Add( fHcoaY[arm] );
    fHcoaSPh[arm] = new TH1F( Form("mxReco_%s_coaSPh",arm==0?"S":"N"), Form("mxReco_%s_coaSPh;SPREAD PHI  RAD",arm==0?"S":"N"), 100,0,0.006);
    fList->Add( fHcoaSPh[arm] );
    fHcoaSEt[arm] = new TH1F( Form("mxReco_%s_coaSEt",arm==0?"S":"N"), Form("mxReco_%s_coaSEt;SPREAD ETA",arm==0?"S":"N"), 100,0,0.02);
    fList->Add( fHcoaSEt[arm] );
    fHcoaPty[arm] = new TH1F( Form("mxReco_%s_coaPty",arm==0?"S":"N"), Form("mxReco_%s_coaPty;NO of PARTIES in COALITION", arm==0?"S":"N"), 9, -0.5, 9.5);
    fList->Add( fHcoaPty[arm] );
    fHcoaEff[arm] = new TH1F( Form("mxReco_%s_coaEff",arm==0?"S":"N"), Form("mxReco_%s_coaEff;LAYER;",arm==0?"S":"N"), 9,-0.5,8.5);
    fList->Add( fHcoaEff[arm] );
    fHcoaSla[arm] = new TH1F( Form("mxReco_%s_coaSla",arm==0?"S":"N"), Form("mxReco_%s_coaSla;Starting LAYER;",arm==0?"S":"N"), 9,-0.5,8.5);
    fList->Add( fHcoaSla[arm] );
    fHcoaSta[arm] = new TH2F( Form("mxReco_%s_coaSta",arm==0?"S":"N"), Form("mxReco_%s_coaSta;STARTING  LAYER;ENERGY",arm==0?"S":"N"), 9,-0.5,8.5, 100,0,maxe);
    fList->Add( fHcoaSta[arm] );
    fHcoaHit[arm] = new TProfile( Form("mxReco_%s_coaHit",arm==0?"S":"N"), Form("mxReco_%s_coaHit;LAYER;AvgHits",arm==0?"S":"N"), 9,-0.5,8.5);
    fList->Add( fHcoaHit[arm] );
    fHcoaHitx[arm] = new TH1F( Form("mxReco_%s_coaHitx",arm==0?"S":"N"), Form("mxReco_%s_coaHitx;x;Hits",arm==0?"S":"N"), 100,-6,6);
    fList->Add( fHcoaHitx[arm] );
    fHcoaHitxTH2F[arm] = new TH2F( Form("mxReco_%s_coaHitxTH2F",arm==0?"S":"N"), Form("mxReco_%s_coaHitxTH2F;x;Hits",arm==0?"S":"N"),9,-0.5,8.5,300,-8,8);
    fList->Add( fHcoaHitxTH2F[arm] );
    fHcoaHitxProf[arm] = new TProfile( Form("mxReco_%s_coaHitxProf",arm==0?"S":"N"), Form("mxReco_%s_coaHitxProf;LAYER;Avgx",arm==0?"S":"N"), 9,-0.5,8.5);
    fList->Add( fHcoaHitxProf[arm] );
    fHcoaEnergy[arm] = new TProfile( Form("mxReco_%s_coaEnergy",arm==0?"S":"N"), Form("mxReco_%s_coaEnergy;LAYER;AvgEnergy",arm==0?"S":"N"), 8,-0.5,7.5);
    fList->Add( fHcoaEnergy[arm] );
    fHcoaEnergyx[arm] = new TH1F( Form("mxReco_%s_coaEnergyx",arm==0?"S":"N"), Form("mxReco_%s_coaEnergyx;x;Energy",arm==0?"S":"N"), 100,-6,6);
    fList->Add( fHcoaEnergyx[arm] );
    fHcoaEnergyxTH2F[arm] = new TH2F( Form("mxReco_%s_coaEnergyxTH2F",arm==0?"S":"N"), Form("mxReco_%s_coaEnergyxTH2F;Layer;x",arm==0?"S":"N"), 9,-0.5,8.5,300,-8,8);
    fList->Add( fHcoaEnergyxTH2F[arm] );
    fHcoaEnergyxProf[arm] = new TProfile( Form("mxReco_%s_coaEnergyxProf",arm==0?"S":"N"), Form("mxReco_%s_coaEnergyxProf;LAYER;Avgx",arm==0?"S":"N"), 8,-0.5,7.5);
    fList->Add( fHcoaEnergyxProf[arm] );
    fHcoaEtEn[arm] = new TH2F( Form("mxReco_%s_coaEtEn",arm==0?"S":"N"), Form("mxReco_%s_coaEtEn;Eta;Energy(GeV)",arm==0?"S":"N"),40,3.0-sn*6.8,3.8-sn*6.8,40,0,maxe);
    fList->Add( fHcoaEtEn[arm] );
    fHcoaEtEnProf[arm] = new TProfile( Form("mxReco_%s_coaEtEnProf",arm==0?"S":"N"), Form("mxReco_%s_coaEtEnProf;Eta;Energy(GeV)",arm==0?"S":"N"),15,-3.9,-3.0);
    fList->Add( fHcoaEtEnProf[arm] );
    fHcoaS[arm] = new TH1F( Form("mxReco_%s_coaS",arm==0?"S":"N"), Form("mxReco_%s_coaS;Layer",arm==0?"S":"N"),9,-0.5,8.5);
    fList->Add( fHcoaS[arm] );
    fHcoaExMpc[arm] = new TH2F( Form("mxReco_%s_coaExMpc",arm==0?"S":"N"), Form("mxReco_%s_coaExMpc;MPCEX Energy; MPC Energy",arm==0?"S":"N"),50,0,maxe/200.0,100,0,maxe);
    fList->Add( fHcoaExMpc[arm] );
    fHcoaTruthE[arm] = new TH2F( Form("mxReco_%s_coaTruthE",arm==0?"S":"N"), Form("mxReco_%s_coaTruthE;Coa Energy; Truth Energy",arm==0?"S":"N"),50,0,maxe,50,0,maxe-4);
    fList->Add( fHcoaTruthE[arm] );
    fHcoaTruthEL[arm] = new TH2F( Form("mxReco_%s_coaTruthEL",arm==0?"S":"N"), Form("mxRecoa_%s_coaTruthEL;Coa Energy Long; Truth Energy Long",arm==0?"S":"N"),50,0,maxe,50,0,maxe-4);
    fList->Add( fHcoaTruthEL[arm] );
    fHcoaTruthET[arm] = new TH2F( Form("mxReco_%s_coaTruthET",arm==0?"S":"N"), Form("mxRecoa_%s_coaTruthET;Coa Energy Transvers; Truth Energy Transverse",arm==0?"S":"N"),50,0,maxe/10.0,50,0,maxe/10.0);
    fList->Add( fHcoaTruthET[arm] );
    fHcoaTruthdET[arm] =new TH1F( Form("mxReco_%s_coaTruthdET",arm==0?"S":"N"), Form("mxRecoa_%s_coaTruthdET;Truth Energy Transverse - Coa Energy Transverse",arm==0?"S":"N"),50,-3,3);
    fList->Add( fHcoaTruthdET[arm] );
    fHcoaTruthdE[arm] = new TH1F( Form("mxReco_%s_coaTruthdE",arm==0?"S":"N"), Form("mxReco_%s_coaTruthdE;Truth Energy - Coa Energy",arm==0?"S":"N"),300,0,maxe);
    fList->Add( fHcoaTruthdE[arm] );
    fHcoaTruthEloc[arm] = new TH2F( Form("mxReco_%s_coaTruthEloc",arm==0?"S":"N"), Form("mxReco_%s_coaTruthEloc;Truth Phi; Truth Eta",arm==0?"S":"N"),100,0,TMath::TwoPi(),100,-4.2,-3.0);
    fList->Add( fHcoaTruthEloc[arm] );
    fHcoaTruthElocth[arm] = new TH2F( Form("mxReco_%s_coaTruthElocth",arm==0?"S":"N"), Form("mxReco_%s_coaTruthElocth;Truth Phi; Truth Theta",arm==0?"S":"N"),100,0,TMath::TwoPi(),200,3.0,3.2);
    fList->Add( fHcoaTruthElocth[arm] );
    fHcoaTruthPhi[arm] = new TH2F( Form("mxReco_%s_coaTruthPhi",arm==0?"S":"N"), Form("mxReco_%s_coaTruthPhi;Coa Phi; Truth Phi",arm==0?"S":"N"),300,0,TMath::TwoPi(),300,0,TMath::TwoPi());
    fList->Add( fHcoaTruthPhi[arm] );
    fHcoaTruthdPhi[arm] = new TH1F( Form("mxReco_%s_coaTruthdPhi",arm==0?"S":"N"), Form("mxReco_%s_coaTruthdPhi;Truth Phi - Coa Phi",arm==0?"S":"N"),300,-(1./10.)*TMath::TwoPi(),(1./10.)*TMath::TwoPi());
    fList->Add( fHcoaTruthdPhi[arm] );
    fHcoaTruthEta[arm] = new TH2F( Form("mxReco_%s_coaTruthEta",arm==0?"S":"N"), Form("mxReco_%s_coaTruthEta;Coa Eta; Truth Eta",arm==0?"S":"N"),1000,-4.2,+4.2,1000,-4.2,+4.2);
    fList->Add( fHcoaTruthEta[arm] );
    fHcoaTruthdEta[arm] = new TH1F( Form("mxReco_%s_coaTruthdEta",arm==0?"S":"N"), Form("mxReco_%s_coaTruthdEta;Truth Eta - Coa Eta",arm==0?"S":"N"),100,-4.2*(1./10.),+4.2*(1./10.));
    fList->Add( fHcoaTruthdEta[arm] );
    fHcoaTruthEtaRat[arm] = new TH1F( Form("mxReco_%s_coaTruthEtaRat",arm==0?"S":"N"), Form("mxReco_%s_coaTruthEtaRat;Truth Eta/Coa Eta",arm==0?"S":"N"),100,0.9,1.15);    
    fList->Add( fHcoaTruthEtaRat[arm] );
    //UNION
    fHuniN[arm] = new TH1F( Form("mxReco_%s_uniN",arm==0?"S":"N"), Form("mxReco_%s_uniN;NO of UNIONS",arm==0?"S":"N"), 100,-0.5,99.5);
    fList->Add( fHuniN[arm] );
    fHuniEn[arm] = new TH1F( Form("mxReco_%s_uniEn",arm==0?"S":"N"), Form("mxReco_%s_uniEn;ENERGY  GEV",arm==0?"S":"N"), 100,0,maxe);
    fList->Add( fHuniEn[arm] );
    fHuniET[arm] = new TH1F( Form("mxReco_%s_uniET",arm==0?"S":"N"), Form("mxReco_%s_uniET;ENERGY_{T}  GEV",arm==0?"S":"N"), 100,0,1);
    fList->Add( fHuniET[arm] );
    fHuniEL[arm] = new TH1F( Form("mxReco_%s_uniEL",arm==0?"S":"N"), Form("mxReco_%s_uniEL;ENERGY_{L}  GEV",arm==0?"S":"N"), 100,0,maxe);
    fList->Add( fHuniEL[arm] );
    fHuniDP[arm] = new TH1F( Form("mxReco_%s_uniDP",arm==0?"S":"N"), Form("mxReco_%s_uniDP;THETA_{O}  RAD",arm==0?"S":"N"), 100,0,TMath::TwoPi());
    fList->Add( fHuniDP[arm] );
    fHuniMa[arm] = new TH1F( Form("mxReco_%s_uniMa",arm==0?"S":"N"), Form("mxReco_%s_uniMa;MASS  GEV C^{-2}",arm==0?"S":"N"), 100,0,0.5);
    fList->Add( fHuniMa[arm] );
    fHuniPh[arm] = new TH1F( Form("mxReco_%s_uniPh",arm==0?"S":"N"), Form("mxReco_%s_uniPh;PHI  RAD",arm==0?"S":"N"), 100,0,TMath::TwoPi());
    fList->Add( fHuniPh[arm] );
    fHuniEt[arm] = new TH1F( Form("mxReco_%s_uniEta",arm==0?"S":"N"), Form("mxReco_%s_uniEt;ETA",arm==0?"S":"N"), 100,-5,+5);
    fList->Add( fHuniEt[arm] );
  }
}
//========
mxQAReconstruction::~mxQAReconstruction() {
  // dtor
  delete fList;
}
//========
void mxQAReconstruction::Make(mxReconstruction *r) {
  // filler
  mxGeometry *geo = new mxGeometry();
  
  fHvx->Fill( r->GetVertexX() );
  fHvy->Fill( r->GetVertexY() );
  fHvz->Fill( r->GetVertexZ() );
  
  for(int arm=0; arm!=2; ++arm) {
    //==>UNI
    //fHcoaEff[arm]->SetMinimum(0);
    //fHcoaS[arm]->SetMinimum(0);
    //fHHits[arm]->SetMaximum(.1);
    fHuniN[arm]->Fill( r->GetNUnions(arm) );
    std::vector<mxUnion*> uni = r->GetUnions(arm);
    for(int k=0; k!=r->GetNUnions(arm); ++k) {
      fHuniEn[arm]->Fill( uni[k]->GetEnergy() );
      fHuniET[arm]->Fill( uni[k]->GetEnergyT() );
      fHuniEL[arm]->Fill( uni[k]->GetEnergyL() );
      fHuniDP[arm]->Fill( uni[k]->GetDPhi() );
      fHuniMa[arm]->Fill( uni[k]->GetMass() );
      fHuniPh[arm]->Fill( uni[k]->GetPhi() );
      fHuniEt[arm]->Fill( uni[k]->GetEta() );
    }
    //==>COA
    fHcoaN[arm]->Fill( r->GetNCoalitions(arm) );
    std::vector<mxCoalition*> coa = r->GetCoalitions(arm);
    for(int k=0; k!=r->GetNCoalitions(arm); ++k) {
      fHcoaEn[arm]->Fill( coa[k]->GetEnergy() );
      fHcoaPS[arm]->Fill( coa[k]->SignalPreShower(), coa[k]->NPreShower() );
      fHcoaET[arm]->Fill( coa[k]->GetEnergyT() );
      fHcoaEL[arm]->Fill( coa[k]->GetEnergyL() );
      fHcoaPh[arm]->Fill( coa[k]->GetPhi() );
      fHcoaEt[arm]->Fill( coa[k]->GetEta() );
      fHcoaSPh[arm]->Fill( coa[k]->GetCov(0) );
      fHcoaSEt[arm]->Fill( coa[k]->GetCov(1) );
      int layhit = 0;
      mxParty *mypty;
      int keeplay = 0;
      int minlyr = -1;
      float mpcexenergy = 0.0;
      float mpcenergy = 0.0;
      for(int lyr=0;lyr!=9; ++lyr) {
      	if(coa[k]->IsHitLayer(lyr)) {
      	  fHcoaEff[arm]->Fill(lyr);
      	  layhit++;
      	  mypty = coa[k]->GetParty(lyr);
      	  fHptyEN[arm][lyr]->Fill( coa[k]->GetEnergy(), mypty->N() );
      	  fHptyESX[arm][lyr]->Fill( coa[k]->GetEnergy(), mypty->GetSpreadX() );
      	  fHptyESY[arm][lyr]->Fill( coa[k]->GetEnergy(), mypty->GetSpreadY() );
      	  fHcoaHit[arm]->Fill( lyr, (double)mypty->N()/1000.0 );
      	  if(lyr!=8) {
      	    for(int n=0; n!=mypty->N(); ++n) {
      	      mxHit* hit = mypty->GetHit(n);
      	      double hitx = 0.0;
      	      if(lyr%2 == 0) {
      		      hitx = mypty->GetX() - geo->X(hit->Idx()); 
              }
      	      if(lyr%2==1) {
            		hitx = mypty->GetY() - geo->Y(hit->Idx());       
      	      }
      	      fHcoaX[arm]->Fill(geo->X(hit->Idx()),hit->Signal());
      	      fHcoaY[arm]->Fill(geo->Y(hit->Idx()),hit->Signal());
      	      fHcoaHitx[arm]->Fill(hitx);
      	      fHcoaHitxTH2F[arm]->Fill(lyr,hitx);
      	      fHcoaHitxProf[arm]->Fill(lyr,hitx);
      	      fHcoaEnergyx[arm]->Fill(hitx,hit->Signal());
      	      fHcoaEnergyxTH2F[arm]->Fill(lyr, hitx, hit->Signal()/mypty->Signal());
      	      fHcoaEnergyxProf[arm]->Fill(lyr, hitx, hit->Signal()/mypty->Signal());
      	      //	      std::cout << hitx << std::endl;
      	    }
      	  }
      	  fHcoaEnergy[arm]->Fill( lyr, mypty->Signal() );
      	  if(minlyr<0) minlyr = lyr;
      	  if(lyr<8) mpcexenergy+=mypty->Signal();
      	  if(lyr == 8) mpcenergy=mypty->Signal();
      	}
      }
      fHcoaPty[arm]->Fill(layhit);
      fHcoaSta[arm]->Fill(minlyr,coa[k]->GetEnergy());
      fHcoaSla[arm]->Fill(minlyr);
      fHcoaEtEn[arm]->Fill(coa[k]->GetEta(),coa[k]->GetEnergy());
      fHcoaEtEnProf[arm]->Fill(coa[k]->GetEta(),coa[k]->GetEnergy());
      //      std::cout << mpcexenergy << " " << mpcenergy <<std::endl;
      fHcoaExMpc[arm]->Fill(mpcexenergy,mpcenergy);
      keeplay+=minlyr;
      for (int ll = minlyr; ll!=9; ++ll){
      	fHcoaS[arm]->Fill(ll);
      }
    }

    for(int lyr=0; lyr!=9; ++lyr) {
      int glyr = arm*9 + lyr;
      double enplay = 0.0;
      //==>PTY
      fHptyN[arm][lyr]->Fill( r->GetNParties(glyr) );
      std::vector<mxParty*> pty = r->GetParties(glyr);
      for(int k=0; k!=r->GetNParties(glyr); ++k) {
      	fHptyE[arm][lyr]->Fill( pty[k]->Signal() );
      	fHptyX[arm][lyr]->Fill( pty[k]->GetX() );
      	fHptyY[arm][lyr]->Fill( pty[k]->GetY() );
      	fHptySX[arm][lyr]->Fill( pty[k]->GetCov(0) );
      	fHptySY[arm][lyr]->Fill( pty[k]->GetCov(1) );
      	enplay+=pty[k]->Signal();
      }
      //==>HIT
      fHhitN[arm][lyr]->Fill( r->GetNHits(glyr) );
      fHHits[arm]->Fill(lyr,double(r->GetNHits(glyr)));
      fHEnergy[arm]->Fill(lyr,enplay);
      
      std::vector<mxHit*> hit = r->GetHits(glyr);
      for(int k=0; k!=r->GetNHits(glyr); ++k) {
      	fHhitE[arm][lyr]->Fill( hit[k]->Signal() );
      }
    }
  }
  delete geo;
}
