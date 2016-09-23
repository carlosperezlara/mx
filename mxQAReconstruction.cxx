//#include <stdlib>
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

//========
mxQAReconstruction::mxQAReconstruction(float maxe) {
  // ctor
  std::cout << "mxQAReconstruction:: ctor" << std::endl;
  fList = new TList();
  fList->SetOwner();

  fHvx = new TH1F("mxReco_vx","mxReco_vx",100,-2,+2);
  fHvy = new TH1F("mxReco_vy","mxReco_vy",100,-2,+2);
  fHvz = new TH1F("mxReco_vz","mxReco_vz",100,-10,+10);

  fList->Add( fHvx );
  fList->Add( fHvy );
  fList->Add( fHvz );

  for(int arm=0; arm!=2; ++arm) {
    double sn = 0.0;
    if (arm == 0) sn = 1.0;
    
    for(int lyr=0; lyr!=9; ++lyr) {
      float emax = maxe/100;
      float smax= 1;
      if(lyr==8||lyr==17) {
	emax = maxe;
	smax = 10;
      }

      fHhitN[arm][lyr] = new TH1F( Form("mxReco_%s%d_hitN",arm==0?"S":"N",lyr), Form("mxReco_%s%d_hitN;NO of HITS",arm==0?"S":"N",lyr), 800,-0.5,799.5);
      fHhitE[arm][lyr] = new TH1F( Form("mxReco_%s%d_hitE",arm==0?"S":"N",lyr), Form("mxReco_%s%d_hitE;ENERGY  GEV",arm==0?"S":"N",lyr), 100,0,emax);
      fList->Add( fHhitN[arm][lyr] );
      fList->Add( fHhitE[arm][lyr] );

      fHptyN[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptyN",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptyN;NO of PARTIES",arm==0?"S":"N",lyr), 100,-0.5,99.5);
      fHptyE[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptyE",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptyE;ENERGY  GEV",arm==0?"S":"N",lyr), 100,0,emax);
      fHptyX[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptyX",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptyX;X  CM",arm==0?"S":"N",lyr), 100,-22,+22);
      fHptyY[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptyY",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptyY;Y  CM",arm==0?"S":"N",lyr), 100,-22,+22);
      fHptySX[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptySX",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptySX;SPREAD X  CM",arm==0?"S":"N",lyr), 100,0,smax);
      fHptySY[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptySY",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptySY;SPREAD Y  CM",arm==0?"S":"N",lyr), 100,0,smax);
      fList->Add( fHptyN[arm][lyr] );
      fList->Add( fHptyE[arm][lyr] );
      fList->Add( fHptyX[arm][lyr] );
      fList->Add( fHptyY[arm][lyr] );
      fList->Add( fHptySX[arm][lyr] );
      fList->Add( fHptySY[arm][lyr] );
      fHptyEN[arm][lyr] = new TH2F( Form("mxReco_%s%d_coaptyEN",arm==0?"S":"N",lyr),
				    Form("mxReco_%s%d_coaptyEN;COA-ENERGY  GEV;NO of HITS",arm==0?"S":"N",lyr),
				    100,0,maxe, 60,-0.5,59.5);
      fHptyESX[arm][lyr] = new TH2F( Form("mxReco_%s%d_coaptyESX",arm==0?"S":"N",lyr),
				     Form("mxReco_%s%d_coaptyESX;COA-ENERGY  GEV;SPREAD X  CM",arm==0?"S":"N",lyr),
				     100,0,maxe, 100,0,smax);
      fHptyESY[arm][lyr] = new TH2F( Form("mxReco_%s%d_coaptyESY",arm==0?"S":"N",lyr),
				     Form("mxReco_%s%d_coaptyESY;COA-ENERGY  GEV;SPREAD Y  CM",arm==0?"S":"N",lyr),
				     100,0,maxe, 100,0,smax);
      fList->Add( fHptyEN[arm][lyr] );
      fList->Add( fHptyESX[arm][lyr] );
      fList->Add( fHptyESY[arm][lyr] );
    }
    fHHits[arm] = new TProfile( Form("mxReco_%s_HitsPerLayer",arm==0?"S":"N"),
				Form("mxReco_%s_HitPerLayer;LAYER;AvgHits",arm==0?"S":"N"),
				9,-0.5,8.5);
    fHcoaN[arm] = new TH1F( Form("mxReco_%s_coaN",arm==0?"S":"N"), Form("mxReco_%s_coaN;NO of COALITIONS",arm==0?"S":"N"), 100,-0.5,99.5);
    fHcoaEn[arm] = new TH1F( Form("mxReco_%s_coaEn",arm==0?"S":"N"), Form("mxReco_%s_coaEn;ENERGY  GEV",arm==0?"S":"N"), 100,0,maxe);
    fHcoaET[arm] = new TH1F( Form("mxReco_%s_coaET",arm==0?"S":"N"), Form("mxReco_%s_coaET;ENERGY_{T}  GEV",arm==0?"S":"N"), 100,0,maxe/10);
    fHcoaEL[arm] = new TH1F( Form("mxReco_%s_coaEL",arm==0?"S":"N"), Form("mxReco_%s_coaEL;ENERGY_{L}  GEV",arm==0?"S":"N"), 100,0,maxe);
    fHcoaPh[arm] = new TH1F( Form("mxReco_%s_coaPh",arm==0?"S":"N"), Form("mxReco_%s_coaPh;PHI  RAD",arm==0?"S":"N"), 100,0,TMath::TwoPi());
    fHcoaEt[arm] = new TH1F( Form("mxReco_%s_coaEt",arm==0?"S":"N"), Form("mxReco_%s_coaEt;ETA",arm==0?"S":"N"), 100,-4.2,+4.2);
    fHcoaSPh[arm] = new TH1F( Form("mxReco_%s_coaSPh",arm==0?"S":"N"), Form("mxReco_%s_coaSPh;SPREAD PHI  RAD",arm==0?"S":"N"), 100,0,0.006);
    fHcoaSEt[arm] = new TH1F( Form("mxReco_%s_coaSEt",arm==0?"S":"N"), Form("mxReco_%s_coaSEt;SPREAD ETA",arm==0?"S":"N"), 100,0,0.02);
    fHcoaPty[arm] = new TH1F( Form("mxReco_%s_coaPty",arm==0?"S":"N"), Form("mxReco_%s_coaPty;NO of PARTIES in COALITION", arm==0?"S":"N"), 9, -0.5, 9.5);
    fHcoaEff[arm] = new TH1F( Form("mxReco_%s_coaEff",arm==0?"S":"N"), Form("mxReco_%s_coaEff;LAYER;",arm==0?"S":"N"), 9,-0.5,8.5);
    fHcoaSla[arm] = new TH1F( Form("mxReco_%s_coaSla",arm==0?"S":"N"), Form("mxReco_%s_coaSla;Starting LAYER;",arm==0?"S":"N"), 9,-0.5,8.5);
    fHcoaSta[arm] = new TH2F( Form("mxReco_%s_coaSta",arm==0?"S":"N"), Form("mxReco_%s_coaSta;STARTING  LAYER;ENERGY",arm==0?"S":"N"), 9,-0.5,8.5, 100,0,maxe);
    fHcoaHit[arm] = new TProfile( Form("mxReco_%s_coaHit",arm==0?"S":"N"), Form("mxReco_%s_coaHit;LAYER;AvgHits",arm==0?"S":"N"), 9,-0.5,8.5);
    fHcoaEtEn[arm] = new TH2F( Form("mxReco_%s_coaEtEn",arm==0?"S":"N"), Form("mxReco_%s_coaEtEn;Eta;Energy(GeV)",arm==0?"S":"N"),40,3.0-sn*6.8,3.8-sn*6.8,40,0,maxe);
    fHcoaS[arm] = new TH1F( Form("mxReco_%s_coaS",arm==0?"S":"N"), Form("mxReco_%s_coaS;Layer",arm==0?"S":"N"),9,-0.5,8.5);
    fHcoaExMpc[arm] = new TH2F( Form("mxReco_%s_coaExMpc",arm==0?"S":"N"), Form("mxReco_%s_coaExMpc;MPCEX Energy; MPC Energy",arm==0?"S":"N"),50,0,.250,100,0,35);
    fList->Add( fHHits[arm] );
    fList->Add( fHcoaN[arm] );
    fList->Add( fHcoaEn[arm] );
    fList->Add( fHcoaET[arm] );
    fList->Add( fHcoaEL[arm] );
    fList->Add( fHcoaPh[arm] );
    fList->Add( fHcoaEt[arm] );
    fList->Add( fHcoaSPh[arm] );
    fList->Add( fHcoaSEt[arm] );
    fList->Add( fHcoaPty[arm] );
    fList->Add( fHcoaEff[arm] );
    fList->Add( fHcoaSla[arm] );
    fList->Add( fHcoaSta[arm] );
    fList->Add( fHcoaHit[arm] );
    fList->Add( fHcoaEtEn[arm] );
    fList->Add( fHcoaS[arm] );
    fList->Add( fHcoaExMpc[arm] );
    fHuniN[arm] = new TH1F( Form("mxReco_%s_uniN",arm==0?"S":"N"), Form("mxReco_%s_uniN;NO of UNIONS",arm==0?"S":"N"), 100,-0.5,99.5);
    fHuniEn[arm] = new TH1F( Form("mxReco_%s_uniEn",arm==0?"S":"N"), Form("mxReco_%s_uniEn;ENERGY  GEV",arm==0?"S":"N"), 100,0,maxe);
    fHuniET[arm] = new TH1F( Form("mxReco_%s_uniET",arm==0?"S":"N"), Form("mxReco_%s_uniET;ENERGY_{T}  GEV",arm==0?"S":"N"), 100,0,maxe);
    fHuniEL[arm] = new TH1F( Form("mxReco_%s_uniEL",arm==0?"S":"N"), Form("mxReco_%s_uniEL;ENERGY_{L}  GEV",arm==0?"S":"N"), 100,0,maxe);
    fHuniDP[arm] = new TH1F( Form("mxReco_%s_uniDP",arm==0?"S":"N"), Form("mxReco_%s_uniDP;THETA_{O}  RAD",arm==0?"S":"N"), 100,0,TMath::TwoPi());
    fHuniMa[arm] = new TH1F( Form("mxReco_%s_uniMa",arm==0?"S":"N"), Form("mxReco_%s_uniMa;MASS  GEV C^{-2}",arm==0?"S":"N"), 100,0,0.5);
    fHuniPh[arm] = new TH1F( Form("mxReco_%s_uniPh",arm==0?"S":"N"), Form("mxReco_%s_uniPh;PHI  RAD",arm==0?"S":"N"), 100,0,TMath::TwoPi());
    fHuniEt[arm] = new TH1F( Form("mxReco_%s_uniEt",arm==0?"S":"N"), Form("mxReco_%s_uniEt;ETA",arm==0?"S":"N"), 100,-5,+5);
    fList->Add( fHuniN[arm] );
    fList->Add( fHuniEn[arm] );
    fList->Add( fHuniET[arm] );
    fList->Add( fHuniEL[arm] );
    fList->Add( fHuniDP[arm] );
    fList->Add( fHuniMa[arm] );
    fList->Add( fHuniPh[arm] );
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
  fHvx->Fill( r->GetVertexX() );
  fHvy->Fill( r->GetVertexY() );
  fHvz->Fill( r->GetVertexZ() );
  
  for(int arm=0; arm!=2; ++arm) {
    //==>UNI
    fHcoaEff[arm]->SetMinimum(0);
    fHcoaS[arm]->SetMinimum(0);
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
	  fHcoaHit[arm]->Fill( lyr, mypty->N() );
	  if(minlyr<0) minlyr = lyr;
	  if(lyr<8) mpcexenergy+=mypty->Signal();
	  if(lyr == 8) mpcenergy=mypty->Signal();
	}
      }
      fHcoaPty[arm]->Fill(layhit);
      fHcoaSta[arm]->Fill(minlyr,coa[k]->GetEnergy());
      fHcoaSla[arm]->Fill(minlyr);
      fHcoaEtEn[arm]->Fill(coa[k]->GetEta(),coa[k]->GetEnergy());
      std::cout << mpcexenergy << " " << mpcenergy <<std::endl;
      fHcoaExMpc[arm]->Fill(mpcexenergy,mpcenergy);
      keeplay+=minlyr;
      for (int ll = minlyr; ll!=9; ++ll){
	fHcoaS[arm]->Fill(ll);
      }
    }

    for(int lyr=0; lyr!=9; ++lyr) {
      int glyr = arm*9 + lyr;
      //==>PTY
      fHptyN[arm][lyr]->Fill( r->GetNParties(glyr) );
      std::vector<mxParty*> pty = r->GetParties(glyr);
      for(int k=0; k!=r->GetNParties(glyr); ++k) {
	fHptyE[arm][lyr]->Fill( pty[k]->Signal() );
	fHptyX[arm][lyr]->Fill( pty[k]->GetX() );
	fHptyY[arm][lyr]->Fill( pty[k]->GetY() );
	fHptySX[arm][lyr]->Fill( pty[k]->GetCov(0) );
	fHptySY[arm][lyr]->Fill( pty[k]->GetCov(1) );
      }
      //==>HIT
      fHhitN[arm][lyr]->Fill( r->GetNHits(glyr) );
      std::vector<mxHit*> hit = r->GetHits(glyr);
      fHHits[arm]->Fill(lyr,r->GetNHits(glyr));
      for(int k=0; k!=r->GetNHits(glyr); ++k) {
	fHhitE[arm][lyr]->Fill( hit[k]->Signal() );
      }

    }
  }

}
