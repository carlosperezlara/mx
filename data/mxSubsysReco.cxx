#include "stdlib.h"
#include <iostream>
#include <vector>
#include <algorithm>

#include "TH1F.h"
#include "TH2F.h"
#include "THnSparse.h"

#include "getClass.h"
#include "PHCompositeNode.h"
#include "RunHeader.h" 
#include "Fun4AllReturnCodes.h"
#include "Fun4AllServer.h"

#include "recoConsts.h"
#include "PHTimeStamp.h"

#include "MpcExRawHit.h"
#include "TMpcExHit.h"
#include "TMpcExHitSet.h"
#include "MpcExEventHeader.h"
#include "TMpcExGeaHitContainer.h"
#include "TMpcExGeaHit.h"

#include "../calibration/mxCalibMaster.h"
#include "../calibration/mxCalibDAu16.h"
#include "../calibration/mxCalibBase.h"
#include "../calibration/mxChipStatus.h"

#include "../pr/mxReconstruction.h"
#include "../pr/mxParty.h"

#include "mxSubsysReco.h"

using namespace std;
using namespace findNode;

struct myOrder : public std::binary_function<TMpcExHit*,TMpcExHit*,bool> {
public:
  bool operator()(TMpcExHit* lhs, TMpcExHit* rhs){
    return lhs->key() <= rhs->key();
  }
};

//====================================================
mxSubsysReco::mxSubsysReco( const char* name ) : 
  SubsysReco( name ),
  fDebug(NULL),
  fQA(0),
  fCal(NULL),
  fRec(new mxReconstruction())
{
  printf("mcReco::CTOR\n");
}
//====================================================
int mxSubsysReco::End(PHCompositeNode *topNode) {
  return EVENT_OK;
}
//====================================================
mxSubsysReco::~mxSubsysReco() {
  if(fCal) delete fCal;
  if(fRec) delete fRec;
}
//====================================================
int mxSubsysReco::Init(PHCompositeNode* top_node) {
  printf("mxSubsysReco::Init\n");
  Fun4AllServer *se = Fun4AllServer::instance();

  Int_t bin[3] = {49152,200,200};
  Double_t lim[3] = {-0.5,-0.5,-0.5};
  Double_t liM[3] = {49151.5,199.5,199.5};
  fEvents = new TH1F("mxSubsysReco_EVENTS","mxSubsysReco_EVENTS",10,-0.5,9.5);
  se->registerHisto(fEvents);

  switch(fQA) {
  case(1):
    // frequency of locked chips
    fQAbadchp = new TH1F("mxSubsysReco_chpbad","mxSubsysReco_chpbad;chpIDX",768,-0.5,767.5);
    se->registerHisto(fQAbadchp);
    // frequency of locked chips per chain
    fQAbadchpperchn = new TH2F("mxSubsysReco_chpbadperchain","mxSubsysReco_chpbadperchain;chnIDX;Nchips",64,-0.5,63.5,12,-0.5,11.5);
    se->registerHisto(fQAbadchpperchn);
    // cellid for good chips
    fQAgoodchpcid = new TH2F("mxSubsysReco_chpgoodcid","mxSubsysReco_chpgoodcid;chpIDX;CllID",768,-0.5,767.5,47,-0.5,46.5);
    se->registerHisto(fQAgoodchpcid);
    break;
  case(2):
    // hi adc
    fQAadchi = new TH2F("mxSubsysReco_adchi","mxSubsysReco_adchi;key;ADChi",49152,-0.5,49151.5,256,-0.5,255.5);
    se->registerHisto(fQAadchi);
    // lo raw adc
    fQAadclo = new TH2F("mxSubsysReco_adclo","mxSubsysReco_adclo;key;ADClo",49152,-0.5,49151.5,256,-0.5,255.5);
    se->registerHisto(fQAadclo);
    // hi adc pty
    fQAadchipty = new TH2F("mxSubsysReco_adchipty","mxSubsysReco_adchipty;key;ADChi",49152,-0.5,49151.5,236,-0.5,235.5);
    se->registerHisto(fQAadchipty);
    break;
  case(3):
    // hi lo correlation
    fQAadchilo = new THnSparseF("mxSubsysReco_adchilo","mxSubsysReco_adchilo;key;ADChi;ADClo",3,bin,lim,liM);
    se->registerHisto(fQAadchilo);
    // hi lo ratio
    fQAadchilor = new TH2F("mxSubsysReco_adchilor","mxSubsysReco_adchilor;key;ADChi/ADClo",49152,-0.5,49151.5,200,0,10);
    se->registerHisto(fQAadchilor);
    break;
  }
  return EVENT_OK;
}
//====================================================
int mxSubsysReco::InitRun(PHCompositeNode* top_node) {
  printf("mxSubsysReco::InitRun\n");
  RunHeader *runhead = getClass<RunHeader> (top_node, "RunHeader");
  if(!runhead) { cout<<PHWHERE<<" exiting."<<endl; exit(1); }
  int runno = runhead->get_RunNumber();

  if(fDebug) printf("mxSubsysReco::InitRun || Run number %d\n",runno);
  fCal = new mxCalibDAu16();
  if(!fCal) return ABORTEVENT;

  return EVENT_OK; 
}
//====================================================
bool mxSubsysReco::PassEventCuts(PHCompositeNode* top_node) {
  MpcExEventHeader *mMpcExEventHeader = getClass<MpcExEventHeader>(top_node,"MpcExEventHeader");
  if(!mMpcExEventHeader) return false;
  fEvents->Fill(0);

  /////////////////
  // select single-buffered
  if(mMpcExEventHeader->getStack() != 1) return false;
  fEvents->Fill(1);

  /////////////////
  // check PARtimes
  bool badpar = false;
  int par[16]; // stores partimes; one per packet
  for(int i=0; i!=16; ++i) par[i] = mMpcExEventHeader->getPARSTTime(i);
  for(int i=1; i!=8; ++i)  if(par[i]!=par[0]) badpar = true; // all south should have the same
  if(badpar) return false;
  for(int i=9; i!=16; ++i) if(par[i]!=par[8]) badpar = true; // all north should have the same
  if(badpar) return false;
  fEvents->Fill(2);
  int corpar = TMath::Abs(par[8]-par[0]);
  if(corpar>1) return false;
  if(par[0]>28&&par[0]<48) return false;
  if(par[8]>28&&par[8]<48) return false;
  fEvents->Fill(3);

  /////////////////
  // check ChipStatus
  mxChipStatus *myChpStatus = fCal->GetChpS();
  int mySVXMap[2][8][48]; // svxmap
  for(unsigned int i=0; i!=mMpcExEventHeader->getCellIDsSize(); ++i) {
    int arm = mMpcExEventHeader->getCellIDsArm(i);
    int pkt = mMpcExEventHeader->getCellIDsPkt(i);
    int svx = mMpcExEventHeader->getCellIDsSVXID(i);
    mySVXMap[arm][pkt][svx] = mMpcExEventHeader->getCellIDsValue(i);
  }
  myChpStatus->Read(mySVXMap);
  if(fQA==1) { // quality assurance for chips
    int nbpchn[64];
    for(int i=0; i!=64; ++i) nbpchn[i] = 0;
    for(int idx=0; idx!=768; ++idx)
      if(myChpStatus->IsBad(idx)) {
	fQAbadchp->Fill(idx);
	int chn = idx/12;
	++nbpchn[chn];
      } else {
	fQAgoodchpcid->Fill( idx, myChpStatus->CellID(idx) );
      }
    for(int chn=0; chn!=64; ++chn)
      fQAbadchpperchn->Fill( chn, nbpchn[chn] );
  }
  return true;
}
//====================================================
int mxSubsysReco::process_event(PHCompositeNode* top_node) {
  if(!PassEventCuts(top_node)) return ABORTEVENT;
  fRec->Reset();

  /////////////////
  // reading mpcex data
  MpcExRawHit *mMpcExRawHits = getClass<MpcExRawHit>(top_node, "MpcExRawHit");
  if(!mMpcExRawHits) return ABORTEVENT;
  TMpcExHitSet<myOrder> rawcontainer(mMpcExRawHits);
  for(TMpcExHitSet<myOrder>::const_iterator itr=rawcontainer.get_iterator();
      itr!=rawcontainer.end();
      ++itr) {
    TMpcExHit *raw_hit = (*itr);
    if(!raw_hit) continue;
    unsigned int key = raw_hit->key();
    //int arm = key/24576;
    //int pkt = (key%24576)/3072;
    //int chipmap = key%3072;
    //int svx = chipmap/64;
    //int idxsvx = arm*8*48 + pkt*48 + svx;
    //int layer = fGeo->LYR(key);

    // rejection of known detector problems
    // if(myChpStatus->IsArmLock(idxsvx)) continue;
    // if(myChpStatus->IsBad(idxsvx)) continue;
    // 
    //
    //
    int hi_adc = raw_hit->high(); // RAW ADC HIGH
    hi_adc -= fCal->GetPHMu()->Get(key); // subtracting its pedestal
    int lo_adc = raw_hit->low(); // RAW ADC LOW
    lo_adc -= fCal->GetPLMu()->Get(key); // subtracting its pedestal
    float ene, minene;
    ene = hi_adc; // FIXME!
    minene = TMath::Abs(fCal->GetPHSg()->Get(key))*5;
    if(ene>minene) fRec->Fill(key,ene);

    //
    if(fQA==2) fQAadchi->Fill( key, hi_adc );
    if(fQA==2) fQAadclo->Fill( key, lo_adc );
    if(fQA==3) {
      Double_t filla[3] = { key, hi_adc, lo_adc };
      fQAadchilo->Fill( filla );
      if((hi_adc>30)&&(hi_adc<150)&&(lo_adc>10))
	fQAadchilor->Fill( key, hi_adc/lo_adc );
    }
    //
  }
  /////////////////

  fRec->Make();
  fRec->DumpStats();
  //fRec->DumpParties();

  // COINCIDENCE LOGIC
  if(fQA==2) {
    vector<mxParty*> pty;
    for(int lyr=0; lyr!=18; ++lyr) {
      if(lyr==8||lyr==17) continue; // reserved for MPC
      pty = fRec->GetParties( lyr );
      for(unsigned int j=0; j!=pty.size()/4; ++j) { // loop over number of parties (first quartiles)
	for(int k=0; k!=pty[j]->N(); ++k) { // loop over number of hits in party
	  int key = pty[j]->GetHit(k)->Idx();
	  float sgn = pty[j]->GetHit(k)->Signal();
	  fQAadchipty->Fill( key, sgn );
	}
      }
    }
  }

  return EVENT_OK;
}
