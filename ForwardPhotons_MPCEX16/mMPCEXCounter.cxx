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

#include "MpcExEventHeader.h"

#include "mMPCEXCounter.h"

using namespace std;
using namespace findNode;

//====================================================
mMPCEXCounter::mMPCEXCounter( const char* name ) : 
  SubsysReco( name ),
  fScaleValues( NULL ),
  fTriggeredStatsMB( NULL ),
  fTriggeredStatsMPCA( NULL ),
  fTriggeredStatsMPCB( NULL ),
  fVertexMB( NULL ),
  fVertexMPCA( NULL ),
  fVertexMPCB( NULL )
{
  printf("mcReco::CTOR\n");
}
//====================================================
int mMPCEXCounter::End(PHCompositeNode *topNode) {
  return EVENT_OK;
}
//====================================================
mMPCEXCounter::~mMPCEXCounter() {
}
//====================================================
int mMPCEXCounter::Init(PHCompositeNode* top_node) {
  printf("mMPCEXCounter::Init\n");
  Fun4AllServer *se = Fun4AllServer::instance();


  fScaleValues = new TH1F("scalevalues","1 + getLevel1Scaledown",7,-0.5,+6.5);
  se->registerHisto( ((TH1*) (fScaleValues) ) );

  fTriggeredStatsMB = new TH1F("triggeredStatsMB","triggeredStatsMB",7,-1.5,+5.5);
  se->registerHisto( ((TH1*) (fTriggeredStatsMB) ) );

  fTriggeredStatsMPCA = new TH1F("triggeredStatsMPCA","triggeredStatsMPCA",7,-1.5,+5.5);
  se->registerHisto( ((TH1*) (fTriggeredStatsMPCA) ) );
  fTriggeredStatsMPCB = new TH1F("triggeredStatsMPCB","triggeredStatsMPCB",7,-1.5,+5.5);
  se->registerHisto( ((TH1*) (fTriggeredStatsMPCB) ) );

  fVertexMB = new TH1F("vertexzMB","vertexZMB",300,-80,+80);
  se->registerHisto( ((TH1*) (fVertexMB) ) );
  fVertexMPCA = new TH1F("vertexzMPCA","vertexZMPCA",300,-80,+80);
  se->registerHisto( ((TH1*) (fVertexMPCA) ) );
  fVertexMPCB = new TH1F("vertexzMPCB","vertexZMPCB",300,-80,+80);
  se->registerHisto( ((TH1*) (fVertexMPCB) ) );
  return EVENT_OK;
}
//====================================================
int mMPCEXCounter::InitRun(PHCompositeNode* top_node) {
  printf("mMPCEXCounter::InitRun\n");

  TriggerHelper *myTH = new TriggerHelper(top_node); 
  fScaleValues->SetBinContent(1, 1+myTH->getLevel1Scaledown("BBCLL1(>0 tubes) novertex") );
  fScaleValues->SetBinContent(2, 1+myTH->getLevel1Scaledown("MPC_N_A") );
  fScaleValues->SetBinContent(3, 1+myTH->getLevel1Scaledown("MPC_N_B") );
  delete myTH;

  return EVENT_OK; 
}
//====================================================
bool mMPCEXCounter::PassEventCuts(PHCompositeNode* top_node) {
  MpcExEventHeader *mMpcExEventHeader = getClass<MpcExEventHeader>(top_node,"MpcExEventHeader");
  if(!mMpcExEventHeader) return false;

  /////////////////
  // check stack
  // 1) select only stack==1
  if(mMpcExEventHeader->getStack() != 1) return false;

  return true; // skip the rest of the checks for the time being

  /////////////////
  // check PARtimes
  bool badpar = false;
  int par[16]; // stores partimes; one per packet
  for(int i=0; i!=16; ++i) par[i] = mMpcExEventHeader->getPARSTTime(i);
  // 2) all south packets should have the same partime
  for(int i=1; i!=8; ++i)  if(par[i]!=par[0]) badpar = true; // all south should have the same
  if(badpar) return false;
  // 3) all north packets should have the same partime
  for(int i=9; i!=16; ++i) if(par[i]!=par[8]) badpar = true; // all north should have the same
  if(badpar) return false;
  // 4) partime must be equal for south and north
  int corpar = TMath::Abs(par[8]-par[0]);
  if(corpar>1) return false;
  // 5) partime ranges where adc signal is untrustable
  if(par[0]>28&&par[0]<48) return false;
  if(par[8]>28&&par[8]<48) return false;

  /////////////////
  // check statephase
  bool badsph = false;
  int sph[16]; // stores statephase; one per packet
  for(int i=0; i!=16; ++i) sph[i] = mMpcExEventHeader->getStatephase(i) & 0xfff;
  // 6) all south packets should have the same statephase
  for(int i=1; i!=8; ++i)  if(sph[i]!=sph[0]) badsph = true; // all south should have the same
  if(badsph) return false;
  // 7) all north packets should have the same statephase
  for(int i=9; i!=16; ++i) if(sph[i]!=sph[8]) badsph = true; // all north should have the same
  if(badsph) return false;
  // 8) statephase must be equal for south and north
  int corsph = TMath::Abs(sph[8]-sph[0]);
  if(corsph>1) return false;

  /////////////////
  // check ChipStatus
  int myCHPMap[2][8][48];
  for(unsigned int i=0; i!=mMpcExEventHeader->getCellIDsSize(); ++i) {
    int arm = mMpcExEventHeader->getCellIDsArm(i);
    int pkt = mMpcExEventHeader->getCellIDsPkt(i);
    int chp = mMpcExEventHeader->getCellIDsSVXID(i);
    myCHPMap[arm][pkt][chp] = mMpcExEventHeader->getCellIDsValue(i);
  }
  // 9) cellid must be equal within a packet
  for(int pkt=0; pkt!=8; ++pkt) {
    int cids = myCHPMap[0][pkt][0];
    int cidn = myCHPMap[1][pkt][0];
    for(int chp=1; chp!=48; ++chp) {
      if( myCHPMap[0][pkt][chp] != cids ) return false;
      if( myCHPMap[1][pkt][chp] != cidn ) return false;
    }
  }

  return true;
}
//====================================================
int mMPCEXCounter::process_event(PHCompositeNode* top_node) {
  static int nev = 0;
  if(nev%10000==0) {
    std::cout << Form("mMPCEXCounter::process_event %d events", nev) << std::endl;
  }
  nev++;

  fTriggeredStatsMB->Fill(0); // all raw
  fTriggeredStatsMPCA->Fill(0); // all raw
  fTriggeredStatsMPCB->Fill(0); // all raw
  if(!PassEventCuts(top_node)) return ABORTEVENT;
  fTriggeredStatsMB->Fill(1); // all good
  fTriggeredStatsMPCA->Fill(1); // all good
  fTriggeredStatsMPCB->Fill(1); // all good

  TriggerHelper *myTH = new TriggerHelper(top_node); 
  selectMB(top_node,myTH);
  selectMPCA(top_node,myTH);
  selectMPCB(top_node,myTH);
  delete myTH;
  return EVENT_OK;
}
//======
void mMPCEXCounter::selectMB(PHCompositeNode* top_node,TriggerHelper *myTH) {
  if( !myTH->trigScaled("BBCLL1(>0 tubes) novertex") ) return;
  fTriggeredStatsMB->Fill(2); // this sample
  //====
  if( myTH->trigLive("MPC_N_A") ) {
    fTriggeredStatsMB->Fill(3); // passed online flag
  }
  if( myTH->trigLive("MPC_N_B") ) {
    fTriggeredStatsMB->Fill(4); // passed online flag
  }
  //====
  PHGlobal *phglobal = getClass<PHGlobal> (top_node, "PHGlobal");
  if(!phglobal) return;
  fVertexMB->Fill( phglobal->getBbcZVertex() );

}
//======
void mMPCEXCounter::selectMPCA(PHCompositeNode* top_node,TriggerHelper *myTH) {
  if( !myTH->trigScaled("MPC_N_A") ) return;
  fTriggeredStatsMPCA->Fill(2); // this sample
  //====
  bool mb = false;
  mb = mb || myTH->trigLive("BBCLL1(>0 tubes) novertex");
  if(!mb) return;
  fTriggeredStatsMPCA->Fill(3); // passed online flag
  //====
  PHGlobal *phglobal = getClass<PHGlobal> (top_node, "PHGlobal");
  if(!phglobal) return;
  fVertexMPCA->Fill( phglobal->getBbcZVertex() );
}
//======
void mMPCEXCounter::selectMPCB(PHCompositeNode* top_node,TriggerHelper *myTH) {
  if( !myTH->trigScaled("MPC_N_B") ) return;
  fTriggeredStatsMPCB->Fill(2); // this sample
  //====
  bool mb = false;
  mb = mb || myTH->trigLive("BBCLL1(>0 tubes) novertex");
  if(!mb) return;
  fTriggeredStatsMPCB->Fill(3); // passed online flag
  //====
  PHGlobal *phglobal = getClass<PHGlobal> (top_node, "PHGlobal");
  if(!phglobal) return;
  fVertexMPCB->Fill( phglobal->getBbcZVertex() );
}
