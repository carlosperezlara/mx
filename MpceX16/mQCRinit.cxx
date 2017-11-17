#include "stdlib.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

#include "TMath.h"
#include "TList.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "getClass.h"
#include "PHCompositeNode.h"
#include "RunHeader.h" 
#include "Fun4AllReturnCodes.h"
#include "Fun4AllServer.h"

#include "recoConsts.h"
#include "PHTimeStamp.h"

#include "PHGlobal.h"
#include "VtxOut.h"
#include "PHPoint.h"
#include "TriggerHelper.h"
#include "ReactionPlaneObject.h"

#include "mpcRawContainer.h"
#include "mpcRawContent.h"

#include "BbcRaw.h"
#include "BbcCalib.hh"
#include "BbcGeo.hh"

#include "mxGeometry.h"
#include "PHMXData.h"
#include "mxData.h"
#include "mxHit.h"
#include <qcQ.h>

#include "TMpcExHitContainer.h"
#include "TMpcExHit.h"
#include "MpcExEventHeader.h"

#include "mQCRinit.h"

using namespace std;
using namespace findNode;

//====================================================
mQCRinit::mQCRinit( const char* name ) : 
  SubsysReco( name ),
  fGeo( mxGeometry::Instance() ),
  fCalib( NULL ),
  fName( name ),
  fQA(false),
  fEvents( NULL ),
  fCentrality( NULL )
{
  printf("qcReco::CTOR\n");
  fBBCcalib = new BbcCalib();
  fBBCgeo = new BbcGeo();
  for(int arm=0; arm!=2; ++arm) {
    fQbbc[arm] = new qcQ(2);
    fQmpc[arm] = new qcQ(2);
    for(int det=0; det!=8; ++det) {
      fHMultBBC[arm][det/4][det%4] = NULL;
      fHPsi2Com[arm][det/4][det%4] = NULL;
      fHPsi2Cob[arm][det/4][det%4] = NULL;
      fHPsi2Reb[arm][det/4][det%4] = NULL;
      for(int ord=0; ord!=10; ++ord) {
	fQex[ord][arm][det/4][det%4] = new qcQ(ord+1);
	fHPsiex[ord][arm][det/4][det%4] = NULL;
	fHPsiexN[ord][arm][det/4][det%4] = NULL;
	fHPsiexM[ord][arm][det/4][det%4] = NULL;
	fHQexNorm[ord][arm][det/4][det%4] = NULL;
	fHQex[ord][arm][det/4][det%4][0] = NULL;
	fHQex[ord][arm][det/4][det%4][1] = NULL;
      }
    }
    for(int pair=0; pair!=16; ++pair) {
      fHPsi2Rex[arm][pair] = NULL;
      fHPsi2Cox[arm][pair] = NULL;
    }
  }
  int iEXPair_IOC[16][2] = { {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {1,0}, {1,0}, {1,0},
			     {0,0}, {0,0}, {0,0}, {1,1}, {1,1}, {1,1} };
  int iEXPair_DET[16][2] = { {0,0}, {1,1}, {2,2}, {3,3}, {0,1}, {1,2}, {2,3}, {0,1}, {1,2}, {2,3},
			     {0,1}, {1,2}, {2,3}, {0,1}, {1,2}, {2,3} };
  for(int i=0; i!=16; ++i) {
    fEXPair_IOC[i][0] =  iEXPair_IOC[i][0];
    fEXPair_IOC[i][1] =  iEXPair_IOC[i][1];
    fEXPair_DET[i][0] =  iEXPair_DET[i][0];
    fEXPair_DET[i][1] =  iEXPair_DET[i][1];
  }

}
//====================================================
int mQCRinit::End(PHCompositeNode *topNode) {
  return EVENT_OK;
}
//====================================================
mQCRinit::~mQCRinit() {
  if(fBBCcalib) delete fBBCcalib;
  if(fBBCgeo) delete fBBCgeo;
  for(int arm=0; arm!=2; ++arm) {
    delete fQbbc[arm];
    delete fQmpc[arm];
    for(int ord=0; ord!=10; ++ord) {
      for(int det=0; det!=8; ++det) {
	delete fQex[ord][arm][det/4][det%4];
      }
    }
  }
}
//====================================================
int mQCRinit::Init(PHCompositeNode* top_node) {
  printf("mQCRinit::Init\n");
  Fun4AllServer *se = Fun4AllServer::instance();

  fEvents = new TH1F(Form("%s_Events",fName.Data()),"Events",7,-0.5,+6.5);
  se->registerHisto( ((TH1*) (fEvents) ) );
  fEvents->GetXaxis()->SetBinLabel(1,"All Raw");
  fEvents->GetXaxis()->SetBinLabel(2,"Data Reached");
  fEvents->GetXaxis()->SetBinLabel(3,"Centrality Cut");
  fEvents->GetXaxis()->SetBinLabel(4,"Futher Event Cuts");

  fCentrality = new TH2F(Form("%s_Centrality",fName.Data()),"Centrality",100,0,100,100,0,250);
  se->registerHisto( ((TH2F*) (fCentrality) ) );

  if(fQA) {
    for(int arm=0; arm!=2; ++arm) {
      if(arm==1) continue;
      for(int det=0; det!=8; ++det) {
	fHMultBBC[arm][det/4][det%4] = new TH2F( Form("MultBBC_ARM%d_DET%d%c", arm, det%4, det/4==0?'I':'O'),
						 Form("MultBBC_ARM%d_DET%d%c;EX;BBC", arm, det%4, det/4==0?'I':'O'),
						 100, 0, 1500, 100, 0, 250 );
	fHPsi2Com[arm][det/4][det%4] = new TH2F( Form("Psi%dexPsi%dmp_ARM%d_DET%d%c", 2, 2, arm, det%4, det/4==0?'I':'O'),
						 Form("Psi%dexPsi%dmp_ARM%d_DET%d%c;EX;MPC", 2, 2, arm, det%4, det/4==0?'I':'O'),
						 30, 0, TMath::TwoPi()/(2), 30, 0, TMath::TwoPi()/(2) );
	fHPsi2Cob[arm][det/4][det%4] = new TH2F( Form("Psi%dexPsi%dbb_ARM%d_DET%d%c", 2, 2, arm, det%4, det/4==0?'I':'O'),
						 Form("Psi%dexPsi%dbb_ARM%d_DET%d%c;EX;BBC", 2, 2, arm, det%4, det/4==0?'I':'O'),
						 30, 0, TMath::TwoPi()/(2), 30, 0, TMath::TwoPi()/(2) );
	fHPsi2Reb[arm][det/4][det%4] = new TH2F( Form("Psi2Reb_ARM%d_BBC_DET%d%c", arm, det%4, det/4==0?'I':'O'),
						 Form("Psi2Rex_ARM%d_BBC_DET%d%c", arm, det%4, det/4==0?'I':'O'),
						 20, -0.5, 99.5, 100, 0, +1 );
	se->registerHisto( ((TH2F*) fHMultBBC[arm][det/4][det%4] ) );
	se->registerHisto( ((TH2F*) fHPsi2Com[arm][det/4][det%4] ) );
	se->registerHisto( ((TH2F*) fHPsi2Cob[arm][det/4][det%4] ) );
	se->registerHisto( ((TH2F*) fHPsi2Reb[arm][det/4][det%4] ) );
	for(int ord=0; ord!=10; ++ord) {
	  fHPsiex[ord][arm][det/4][det%4] = new TH2F( Form("Psi%dex_ARM%d_DET%d%c", ord+1, arm, det%4, det/4==0?'I':'O'),
						      Form("Psi%dex_ARM%d_DET%d%c", ord+1, arm, det%4, det/4==0?'I':'O'),
						      20, -0.5, 99.5, 360, 0, TMath::TwoPi()/(ord+1) );
	  fHPsiexN[ord][arm][det/4][det%4] = new TH2F( Form("NPsi%dex_ARM%d_DET%d%c", ord+1, arm, det%4, det/4==0?'I':'O'),
						       Form("NPsi%dex_ARM%d_DET%d%c", ord+1, arm, det%4, det/4==0?'I':'O'),
						       100,0,1500, 360, 0, TMath::TwoPi()/(ord+1) );
	  fHPsiexM[ord][arm][det/4][det%4] = new TH2F( Form("MPsi%dex_ARM%d_DET%d%c", ord+1, arm, det%4, det/4==0?'I':'O'),
						       Form("MPsi%dex_ARM%d_DET%d%c", ord+1, arm, det%4, det/4==0?'I':'O'),
						       100,0,500, 360, 0, TMath::TwoPi()/(ord+1) );
	  fHQexNorm[ord][arm][det/4][det%4] = new TH2F( Form("QNorm%dex_ARM%d_DET%d%c", ord+1, arm, det%4, det/4==0?'I':'O'),
							Form("QNorm%dex_ARM%d_DET%d%c", ord+1, arm, det%4, det/4==0?'I':'O'),
							20, -0.5, 99.5, 360, 0, 7 );
	  fHQex[ord][arm][det/4][det%4][0] = new TH2F( Form("Qx%dex_ARM%d_DET%d%c", ord+1, arm, det%4, det/4==0?'I':'O'),
						       Form("Qx%dex_ARM%d_DET%d%c", ord+1, arm, det%4, det/4==0?'I':'O'),
						       20, -0.5, 99.5, 360, -1,+1 );
	  fHQex[ord][arm][det/4][det%4][1] = new TH2F( Form("Qy%dex_ARM%d_DET%d%c", ord+1, arm, det%4, det/4==0?'I':'O'),
						       Form("Qy%dex_ARM%d_DET%d%c", ord+1, arm, det%4, det/4==0?'I':'O'),
						       20, -0.5, 99.5, 360, -1, +1 );
	  se->registerHisto( ((TH2F*) (fHPsiex[ord][arm][det/4][det%4]) ) );
	  se->registerHisto( ((TH2F*) (fHPsiexN[ord][arm][det/4][det%4]) ) );
	  se->registerHisto( ((TH2F*) (fHPsiexM[ord][arm][det/4][det%4]) ) );
	  se->registerHisto( ((TH2F*) (fHQexNorm[ord][arm][det/4][det%4]) ) );
	  se->registerHisto( ((TH2F*) (fHQex[ord][arm][det/4][det%4][0]) ) );
	  se->registerHisto( ((TH2F*) (fHQex[ord][arm][det/4][det%4][1]) ) );
	}
      }
      for(int pair=0; pair!=16; ++pair) {
	TString det0 = Form("%d%c", fEXPair_DET[pair][0], fEXPair_IOC[pair][0]==0?'I':'O' );
	TString det1 = Form("%d%c", fEXPair_DET[pair][1], fEXPair_IOC[pair][1]==0?'I':'O' );
	fHPsi2Rex[arm][pair] = new TH2F( Form("Psi2Rex_ARM%d_PAIR%d_DET%s_DET%s", arm, pair, det0.Data(), det1.Data()),
					 Form("Psi2Rex_ARM%d_PAIR%d_DET%s_DET%s", arm, pair, det0.Data(), det1.Data()),
					 20, -0.5, 99.5, 100, 0, +1 );
	fHPsi2Cox[arm][pair] = new TH2F( Form("Psi2Cox_ARM%d_PAIR%d_DET%s_DET%s", arm, pair, det0.Data(), det1.Data()),
					 Form("Psi2Cox_ARM%d_PAIR%d_DET%s_DET%s", arm, pair, det0.Data(), det1.Data()),
					 30, 0, TMath::TwoPi()/(2), 30, 0, TMath::TwoPi()/(2) );
	se->registerHisto( ((TH2F*) (fHPsi2Rex[arm][pair]) ) );
	se->registerHisto( ((TH2F*) (fHPsi2Cox[arm][pair]) ) );
      }
    }
  }

  return EVENT_OK;
}
//====================================================
int mQCRinit::InitRun(PHCompositeNode* top_node) {
  printf("mQCRinit::InitRun\n");
  recoConsts *rc = recoConsts::instance();
  fBBCcalib->restore(rc->get_TimeStamp(), 4002);

  return EVENT_OK; 
}
//====================================================
int mQCRinit::process_event(PHCompositeNode* top_node) {
  static int nev = 0;
  if(nev%10000==0) {
    std::cout << Form("mQCRinit::process_event %d events", nev) << std::endl;
  }
  nev++;

  fEvents->Fill(0); // all raw
  PHGlobal *phglobal = getClass<PHGlobal> (top_node, "PHGlobal");
  if(!phglobal) return ABORTEVENT;
  BbcRaw *bbcraw = getClass<BbcRaw> (top_node,"BbcRaw");
  if(!bbcraw) return ABORTEVENT;
  PHMXData *phmxdata = getClass<PHMXData> (top_node,"MXR");
  if(!phmxdata) return ABORTEVENT;
  mxData *mxdata = phmxdata->GetData();
  fEvents->Fill(1); // datareached
  float cc =  phglobal->getCentrality();
  if(cc<0) return ABORTEVENT;
  fEvents->Fill(2); // centralitycut

  fEvents->Fill(3); // further event cuts
  float bbcm[2];
  bbcm[0] = phglobal->getBbcChargeS();
  bbcm[1] = phglobal->getBbcChargeN();
  fCentrality->Fill(cc,bbcm[0]);

  for(int ord=0; ord!=10; ++ord) {
    for(int arm=0; arm!=2; ++arm) {
      fQbbc[arm]->Reset();
      fQmpc[arm]->Reset();
      for(int det=0; det!=8; ++det) {
	fQex[ord][arm][det/4][det%4]->Reset();
      }
    }
  }

  //==== EX
  std::vector<mxHit*> hits;
  for(int lyr=0; lyr!=8; ++lyr) {
    hits = mxdata->GetHits(lyr);
    //std::cout << "LYR " << lyr << " HITS " << mxdata->GetNHits(lyr) << std::endl;
    for(int ht=0; ht!=mxdata->GetNHits(lyr); ++ht) {
      mxHit *hit = hits.at(ht);
      int key = hit->Idx();
      int ioc = fGeo->PS_SenRing( fGeo->PS_Idx2Sen(key) );
      float energy = hit->Signal()*1e3; // in MeV
      if(energy<0.1||energy>1.0) continue;
      float phi = TMath::Pi() + TMath::ATan2(-fGeo->Y(key),-fGeo->X(key));
      for(int ord=0; ord!=10; ++ord)
	fQex[ord][0][ioc][lyr/2]->Fill( phi, energy );
    }
  }
  //==== MPC
  hits = mxdata->GetHits(8);
  for(int ht=0; ht!=mxdata->GetNHits(8); ++ht) {
    mxHit *hit = hits.at(ht);
    int key = hit->Idx();
    float energy = hit->Signal();
    if(energy<0.5||energy>30) continue;
    float phi = TMath::Pi() + TMath::ATan2(-fGeo->Y(key),-fGeo->X(key));
    fQmpc[0]->Fill( phi, energy );
  }
  //==== BBC
  for(int ipmt=0;ipmt!=128;++ipmt){
    short adc    = bbcraw->get_Adc(ipmt);
    short tdc    = bbcraw->get_Tdc0(ipmt);
    float time0  = fBBCcalib->getHitTime0(ipmt,tdc,adc);
    float charge = fBBCcalib->getCharge(ipmt,adc);
    float bbc_x  = fBBCgeo->getX(ipmt);
    float bbc_y  = fBBCgeo->getY(ipmt);
    float bbc_phi= TMath::Pi() + TMath::ATan2(-bbc_y,-bbc_x);
    // ===== CUTS =====
    if(ipmt<64) {
      if(time0<-10) continue;
      if(time0>+9999) continue;
      if(charge<-10) continue;
      if(charge>+9999) continue;
      fQbbc[0]->Fill(bbc_phi,charge);
    } else {
      if(time0<-9999) continue;
      if(time0>+9999) continue;
      if(charge<-9999) continue;
      if(charge>+9999) continue;
      fQbbc[1]->Fill(bbc_phi,charge);
    }
    // ================
  }


  if(fQA) {
    float psix[10][2][4];
    for(int ord=0; ord!=10; ++ord)
      for(int det=0; det!=8; ++det)
	psix[ord][det/4][det%4] = -999;

    for(int det=0; det!=8; ++det) {
      qcQ *q = fQex[0][0][det/4][det%4]; //any order will do
      if(q->NP()<5) continue;
      fHMultBBC[0][det/4][det%4]->Fill(q->NP(),bbcm[0]);
    }

    for(int ord=0; ord!=10; ++ord) {
      for(int det=0; det!=8; ++det) {
	qcQ *q = fQex[ord][0][det/4][det%4];
	if(q->NP()<5) continue;
	//std::cout << " DET " << det << " ENTRIES " << q->M() << std::endl;
	psix[ord][det/4][det%4] = q->Psi();
	fHPsiex[ord][0][det/4][det%4]->Fill( cc, psix[ord][det/4][det%4] );
	fHPsiexN[ord][0][det/4][det%4]->Fill( q->NP(), psix[ord][det/4][det%4] );
	fHPsiexM[ord][0][det/4][det%4]->Fill( q->M(), psix[ord][det/4][det%4] );
	fHQexNorm[ord][0][det/4][det%4]->Fill( cc, TMath::Sqrt(q->ModulusSquared()/q->M()), q->M() );
	fHQex[ord][0][det/4][det%4][0]->Fill( cc, q->X()/q->M(), q->M() );
	fHQex[ord][0][det/4][det%4][1]->Fill( cc, q->Y()/q->M(), q->M() );
      }
    }
    for(int pair=0; pair!=16; ++pair) {
      int ioc0 = fEXPair_IOC[pair][0];
      int ioc1 = fEXPair_IOC[pair][1];
      int det0 = fEXPair_DET[pair][0];
      int det1 = fEXPair_DET[pair][1];
      qcQ *a = fQex[1][0][ioc0][det0];
      qcQ *b = fQex[1][0][ioc1][det1];
      float psia = psix[1][ioc0][det0];
      float psib = psix[1][ioc1][det1];
      if(a->NP()<5) continue;
      if(b->NP()<5) continue;
      float ab_x = a->X()*b->X();
      float ab_y = a->Y()*b->Y();
      float ab = TMath::Sqrt( a->ModulusSquared()*b->ModulusSquared() );
      float ab_w = a->M()*b->M();
      fHPsi2Rex[0][pair]->Fill( cc, TMath::Abs(ab_x+ab_y)/ab, ab_w );
      if( (cc<50)&&(cc>30) ) {
	fHPsi2Cox[0][pair]->Fill( psia, psib );
      }
    }
    for(int det=0; det!=8; ++det) {
      qcQ *a = fQex[1][0][det/4][det%4];
      if(a->NP()<5) continue;
      qcQ *b = fQbbc[0];
      float ab_x = a->X()*b->X();
      float ab_y = a->Y()*b->Y();
      float ab = TMath::Sqrt( a->ModulusSquared()*b->ModulusSquared() );
      float ab_w = a->M()*b->M();
      fHPsi2Reb[0][det/4][det%4]->Fill( cc, TMath::Abs(ab_x+ab_y)/ab, ab_w );
      float psixd = psix[1][det/4][det%4];
      if( (cc<50)&&(cc>30) ) {
	float psim = fQmpc[0]->Psi();
	float psib = fQbbc[0]->Psi();

	fHPsi2Com[0][det/4][det%4]->Fill( psixd, psim );
	fHPsi2Cob[0][det/4][det%4]->Fill( psixd, psib );
      }
    }
  }

  return EVENT_OK;
}
//======
