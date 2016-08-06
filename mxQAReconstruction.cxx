//#include <stdlib>
#include <iostream>
#include <vector>
#include <algorithm>

#include "TMath.h"
#include "TList.h"
#include "TH1F.h"
#include "TString.h"

#include "mxReconstruction.h"
#include "mxQAReconstruction.h"
#include "mxUnion.h"
#include "mxCoalition.h"
#include "mxParty.h"
#include "mxHit.h"

//========
mxQAReconstruction::mxQAReconstruction() {
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
    for(int lyr=0; lyr!=9; ++lyr) {
      fHhitN[arm][lyr] = new TH1F( Form("mxReco_%s%d_hitN",arm==0?"S":"N",lyr), Form("mxReco_%s%d_hitN;HITS",arm==0?"S":"N",lyr), 100,0,800);
      fHhitE[arm][lyr] = new TH1F( Form("mxReco_%s%d_hitE",arm==0?"S":"N",lyr), Form("mxReco_%s%d_hitE;GEV",arm==0?"S":"N",lyr), 100,0,1);
      fList->Add( fHhitN[arm][lyr] );
      fList->Add( fHhitE[arm][lyr] );

      fHptyN[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptyN",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptyN;PARTIES",arm==0?"S":"N",lyr), 100,0,150);
      fHptyE[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptyE",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptyE;GEV",arm==0?"S":"N",lyr), 100,0,3);
      fHptyX[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptyX",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptyX;CM",arm==0?"S":"N",lyr), 100,-20,+20);
      fHptyY[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptyY",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptyY;CM",arm==0?"S":"N",lyr), 100,-20,+20);
      fHptySX[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptySX",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptySX;CM^2",arm==0?"S":"N",lyr), 100,0,2);
      fHptySY[arm][lyr] = new TH1F( Form("mxReco_%s%d_ptySY",arm==0?"S":"N",lyr), Form("mxReco_%s%d_ptySY;CM^2",arm==0?"S":"N",lyr), 100,0,2);
      fList->Add( fHptyN[arm][lyr] );
      fList->Add( fHptyE[arm][lyr] );
      fList->Add( fHptyX[arm][lyr] );
      fList->Add( fHptyY[arm][lyr] );
      fList->Add( fHptySX[arm][lyr] );
      fList->Add( fHptySY[arm][lyr] );
    }

    fHcoaN[arm] = new TH1F( Form("mxReco_%s_coaN",arm==0?"S":"N"), Form("mxReco_%s_coaN;COALITIONS",arm==0?"S":"N"), 100,0,500);
    fHcoaEn[arm] = new TH1F( Form("mxReco_%s_coaEn",arm==0?"S":"N"), Form("mxReco_%s_coaEn;GEV",arm==0?"S":"N"), 100,0,3);
    fHcoaET[arm] = new TH1F( Form("mxReco_%s_coaET",arm==0?"S":"N"), Form("mxReco_%s_coaET;GEV",arm==0?"S":"N"), 100,0,0.3);
    fHcoaEL[arm] = new TH1F( Form("mxReco_%s_coaEL",arm==0?"S":"N"), Form("mxReco_%s_coaEL;GEV",arm==0?"S":"N"), 100,0,3);
    fHcoaPh[arm] = new TH1F( Form("mxReco_%s_coaPh",arm==0?"S":"N"), Form("mxReco_%s_coaPh;RAD",arm==0?"S":"N"), 100,0,TMath::TwoPi());
    fHcoaEt[arm] = new TH1F( Form("mxReco_%s_coaEt",arm==0?"S":"N"), Form("mxReco_%s_coaEt;ETA",arm==0?"S":"N"), 100,-5,+5);
    fHcoaSPh[arm] = new TH1F( Form("mxReco_%s_coaSPh",arm==0?"S":"N"), Form("mxReco_%s_coaSPh;RAD^2",arm==0?"S":"N"), 100,0,2);
    fHcoaSEt[arm] = new TH1F( Form("mxReco_%s_coaSEt",arm==0?"S":"N"), Form("mxReco_%s_coaSEt;ETA^2",arm==0?"S":"N"), 100,0,1);
    fList->Add( fHcoaN[arm] );
    fList->Add( fHcoaEn[arm] );
    fList->Add( fHcoaET[arm] );
    fList->Add( fHcoaEL[arm] );
    fList->Add( fHcoaPh[arm] );
    fList->Add( fHcoaEt[arm] );
    fList->Add( fHcoaSPh[arm] );
    fList->Add( fHcoaSEt[arm] );

    fHuniN[arm] = new TH1F( Form("mxReco_%s_uniN",arm==0?"S":"N"), Form("mxReco_%s_uniN;UNIONS",arm==0?"S":"N"), 100,0,1e4);
    fHuniEn[arm] = new TH1F( Form("mxReco_%s_uniEn",arm==0?"S":"N"), Form("mxReco_%s_uniEn;GEV",arm==0?"S":"N"), 100,0,10);
    fHuniET[arm] = new TH1F( Form("mxReco_%s_uniET",arm==0?"S":"N"), Form("mxReco_%s_uniET;GEV",arm==0?"S":"N"), 100,0,1);
    fHuniEL[arm] = new TH1F( Form("mxReco_%s_uniEL",arm==0?"S":"N"), Form("mxReco_%s_uniEL;GEV",arm==0?"S":"N"), 100,0,10);
    fHuniDP[arm] = new TH1F( Form("mxReco_%s_uniDP",arm==0?"S":"N"), Form("mxReco_%s_uniDP;RAD",arm==0?"S":"N"), 100,0,TMath::TwoPi());
    fHuniMa[arm] = new TH1F( Form("mxReco_%s_uniMa",arm==0?"S":"N"), Form("mxReco_%s_uniMa;GEV/C^2",arm==0?"S":"N"), 100,0,0.5);
    fHuniPh[arm] = new TH1F( Form("mxReco_%s_uniPh",arm==0?"S":"N"), Form("mxReco_%s_uniPh;RAD",arm==0?"S":"N"), 100,0,TMath::TwoPi());
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
      for(int k=0; k!=r->GetNHits(glyr); ++k) {
	fHhitE[arm][lyr]->Fill( hit[k]->Signal() );
      }

    }
  }

}
