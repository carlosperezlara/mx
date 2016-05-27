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

#include "mxCalibMaster.h"
#include "mxCalibDAu16.h"
#include "mxCalibBase.h"
#include "mxChipStatus.h"

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
  fRec(NULL)
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
}
//====================================================
int mxSubsysReco::Init(PHCompositeNode* top_node) {
  printf("mxSubsysReco::Init\n");
  Fun4AllServer *se = Fun4AllServer::instance();

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
    break;
  case(3):
    // hi lo correlation
    fQAadchilo = new THnSparse("mxSubsysReco_adchilo","mxSubsysReco_adchilo;key;ADChi;ADClo",49152,-0.5,49151.5,200,-0.5,199.5,200,-0.5,199.5);
    se->registerHisto(fQAadchilo);
    // hi lo ratio
    fQAadchilor = new THnSparse("mxSubsysReco_adchilor","mxSubsysReco_adchilor;key;ADChi/ADClo",49152,-0.5,49151.5,200,0.10);
    se->registerHisto(fQAadchilo);
    break;
  case(4):
    // hi adc pty
    fQAadchipty = new TH2F("mxSubsysReco_adchipty","mxSubsysReco_adchipty;key;ADChi",49152,-0.5,49151.5,236,-0.5,235.5);
    se->registerHisto(fQAadchi);
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
int mxSubsysReco::process_event(PHCompositeNode* top_node) {
  MpcExEventHeader *mMpcExEventHeader = getClass<MpcExEventHeader>(top_node,"MpcExEventHeader");
  if(!mMpcExEventHeader) return ABORTEVENT;

  /////////////////
  // select single-buffered
  if(mMpcExEventHeader->getStack() != 1) return ABORTEVENT;

  /////////////////
  // select appropriate PARtimes

  /////////////////
  // tag problematic chips
  //mxChipStatus *myChpStatus = fCal->GetChpS();
  //myChpStatus->Read(top_node);
  /////////////////

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
    int arm = key/24576;
    int pkt = (key%24576)/3072;
    int chipmap = key%3072;
    int svx = chipmap/64;
    int idxsvx = arm*8*48 + pkt*48 + svx;
    int layer = fGeo->LYR(key);

    // rejection of known detector problems
    // if(myChpStatus->IsArmLock(idxsvx)) continue;
    // if(myChpStatus->IsBad(idxsvx)) continue;
    // 
    //
    //
    int hi_adc = raw_hit->high(); // RAW ADC HIGH
    hi_adc -= fCal->GetPHMu()->Get(key); // subtracting its pedestal
    if(fQA==2) fQAadchi->Fill( key, hi_adc );
    //
    int lo_adc = raw_hit->low(); // RAW ADC LOW
    lo_adc -= fCal->GetPLMu()->Get(key); // subtracting its pedestal
    if(fQA==2) fQAadclo->Fill( key, lo_adc );
    //
    if(fQA==4) fQAadchilo->Fill( key, hi_adc, lo_adc );
    //
  }
  /////////////////

  // COINCIDENCE LOGIC
  //if(fQA==3) fQAadchipty->Fill( key, hi_adc );

  return EVENT_OK;
}
