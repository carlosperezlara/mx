#include "stdlib.h"
#include <iostream>
#include <vector>
#include <algorithm>

#include "TMath.h"
#include "TList.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"

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
#include "MpcMap.h"

#include "mpcRawContainer.h"
#include "mpcRawContent.h"
#include "MpcExRawHit.h"
#include "TMpcExHit.h"
#include "TMpcExHitSet.h"
#include "MpcExEventHeader.h"

#include "mxCalibMaster.h"
#include "mxCalibDAu16.h"
#include "mxCalibBaseSiW.h"

#include "mxReconstruction.h"
#include "mxQAReconstruction.h"

#include "PHMXData.h"
#include "mSubsysReco.h"

using namespace std;
using namespace findNode;

struct myOrder : public std::binary_function<TMpcExHit*,TMpcExHit*,bool> {
public:
  bool operator()(TMpcExHit* lhs, TMpcExHit* rhs){
    return lhs->key() <= rhs->key();
  }
};

//====================================================
mSubsysReco::mSubsysReco( const char* name ) : 
  SubsysReco( name ),
  fDoQA(true),
  fCal(NULL),
  fRec(NULL),
  fQA(NULL),
  fList(NULL)
{
  printf("mcReco::CTOR\n");
  for(int i=0; i!=2; ++i) {
    fHstk[i] = NULL;
    fHpar[i] = NULL;
    fHsph[i] = NULL;
    fHcid[i] = NULL;
  }
  fHcry = NULL;
  fHcrytofS = NULL;
  fHcryeneS = NULL;
  fHcrytofN = NULL;
  fHcryeneN = NULL;
}
//====================================================
int mSubsysReco::End(PHCompositeNode *topNode) {
  return EVENT_OK;
}
//====================================================
mSubsysReco::~mSubsysReco() {
  if(fCal) delete fCal;
  if(fRec) delete fRec;
  if(fQA) delete fQA;
  if(fList) delete fList;
}
//====================================================
int mSubsysReco::Init(PHCompositeNode* top_node) {
  printf("mSubsysReco::Init\n");

  Fun4AllServer *se = Fun4AllServer::instance();

  if(fDoQA) {
    fQA = new mxQAReconstruction();
    for(int i=0; i!=fQA->GetList()->GetEntries(); ++i)
      se->registerHisto( ((TH1F*) (fQA->GetList()->At(i))) );
  }

  fList = new TList();
  fList->SetOwner();
  for(int i=0; i!=2; ++i) {
    fHstk[i] = new TH1F( Form("mxDet_Hstk%d",i), Form("mxDet_Hstk%d",i),  6,-0.5,5.5);
    fHpar[i] = new TH1F( Form("mxDet_Hpar%d",i), Form("mxDet_Hpar%d",i),120,-0.5,120-0.5);
    fHsph[i] = new TH1F( Form("mxDet_Hsph%d",i), Form("mxDet_Hsph%d",i),120,-0.5,520-0.5);
    fHcid[i] = new TH2F( Form("mxDet_Hcid%d",i), Form("mxDet_Hcid%d",i), 16,-0.5,15.5,47,-0.5,46.5);
    fList->Add(fHstk[i]);
    fList->Add(fHpar[i]);
    fList->Add(fHsph[i]);
    fList->Add(fHcid[i]);
  }
  fHcry = new TH3F( "mxDet_Hcry", "mxDet_Hcry", 576,-0.5,575.5, 100,0,50, 100,0,15 );
  fHcryeneS = new TH1F( "mxDet_HcryeneS", "mxDet_HcryeneS", 202, -1, 100 );
  fHcrytofS = new TH1F( "mxDet_HcrytofS", "mxDet_HcrytofS", 100, 0, 20 );
  fHcryeneN = new TH1F( "mxDet_HcryeneN", "mxDet_HcryeneN", 202, -1, 100 );
  fHcrytofN = new TH1F( "mxDet_HcrytofN", "mxDet_HcrytofN", 100, 0, 20 );
  fList->Add(fHcry);
  fList->Add(fHcryeneS);
  fList->Add(fHcrytofS);
  fList->Add(fHcryeneN);
  fList->Add(fHcrytofN);
  for(int i=0; i!=fList->GetEntries(); ++i)
    se->registerHisto( ((TH1F*) (fList->At(i))) );

  return EVENT_OK;
}
//====================================================
int mSubsysReco::InitRun(PHCompositeNode* top_node) {
  printf("mSubsysReco::InitRun\n");

  PHNodeIterator nodeIter(top_node);
  PHCompositeNode *dstNode = static_cast<PHCompositeNode*>(nodeIter.findFirst("PHCompositeNode","DST"));
  if (dstNode == NULL) {
    dstNode = new PHCompositeNode("DST");
    top_node->addNode(dstNode);
  }
  PHMXData *mxdata = new PHMXData();
  PHIODataNode<PHMXData> *hitNode = new PHIODataNode<PHMXData>(mxdata,"MX","PHMXData");
  dstNode->addNode(hitNode);
  fRec = mxdata->GetReconstruction();

  RunHeader *runhead = getClass<RunHeader> (top_node, "RunHeader");
  if(!runhead) { cout<<PHWHERE<<" exiting."<<endl; exit(1); }
  int runno = runhead->get_RunNumber();

  printf("mSubsysReco::InitRun || Run number %d\n",runno);
  fCal = new mxCalibDAu16();
  if(!fCal) return ABORTEVENT;

  return EVENT_OK; 
}
//====================================================
void mSubsysReco::FillHistos(int lev, PHCompositeNode* top_node) {
  MpcExEventHeader *mMpcExEventHeader = getClass<MpcExEventHeader>(top_node,"MpcExEventHeader");
  if(!mMpcExEventHeader) return;

  fHstk[lev]->Fill( mMpcExEventHeader->getStack() );
  fHpar[lev]->Fill( mMpcExEventHeader->getPARSTTime(0) ); // 0-15
  fHsph[lev]->Fill( mMpcExEventHeader->getStatephase(0) ); // 0-15

  int myCHPMap[2][8][48];
  for(unsigned int i=0; i!=mMpcExEventHeader->getCellIDsSize(); ++i) {
    int arm = mMpcExEventHeader->getCellIDsArm(i);
    int pkt = mMpcExEventHeader->getCellIDsPkt(i);
    int chp = mMpcExEventHeader->getCellIDsSVXID(i);
    myCHPMap[arm][pkt][chp] = mMpcExEventHeader->getCellIDsValue(i);
  }

  for(unsigned int i=0; i!=16; ++i)
    fHcid[lev]->Fill( i, myCHPMap[i/8][i%8][0] ); // 0-47

  return;
}
//====================================================
bool mSubsysReco::PassEventCuts(PHCompositeNode* top_node) {
  MpcExEventHeader *mMpcExEventHeader = getClass<MpcExEventHeader>(top_node,"MpcExEventHeader");
  if(!mMpcExEventHeader) return false;

  /////////////////
  // check stack
  // 1) select only stack==1
  if(mMpcExEventHeader->getStack() != 1) return false;

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
int mSubsysReco::process_event(PHCompositeNode* top_node) {
  FillHistos(0,top_node);
  if(!PassEventCuts(top_node)) return ABORTEVENT;
  FillHistos(1,top_node);

  fRec->Reset();

  PHGlobal *phglobal = getClass<PHGlobal> (top_node, "PHGlobal");
  if(!phglobal) return false;
  fRec->SetVertex( 0, 0, phglobal->getBbcZVertex() );

  /*
  VtxOut *vtxout = getClass<VtxOut> (top_node, "VtxOut");
  if(!vtxout) return false;
  PHPoint vx = vtxout->get_Vertex();
  fRec->SetVertex( vx.getX(), vx.getY(), vx.getZ() );
  */

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

    float hi_adc = raw_hit->high() - fCal->GetPHMu()->Get(key);
    float lo_adc = raw_hit->low()  - fCal->GetPLMu()->Get(key);
    float hires = hi_adc * fCal->GetLMPV()->Get(key)*0.14/20.;
    float lores = lo_adc * fCal->GetLHft()->Get(key) * fCal->GetLMPV()->Get(key)*0.14/20.;

    float ene = hires; // 0.5*(hires+lores); // hires<mThrhld?hires:lores;
    float enecut = TMath::Abs(fCal->GetPHSg()->Get(key))*5 *0.14/20 ; // fCal->GetLMPV()->Get(key) - 1*fCal->GetLSgm()->Get(key);
    if(ene>enecut) fRec->Fill(key,ene);
  }
  /////////////////
  // reading mpc data
  mpcRawContainer *mpcraw2 = findNode::getClass<mpcRawContainer>(top_node,"MpcRaw2");
  if(!mpcraw2) return ABORTEVENT;
  for (unsigned int i=0; i!=mpcraw2->size(); ++i) {
    mpcRawContent *raw = mpcraw2->getTower(i);
    int key = raw->get_ch();
    float tof = raw->get_sample();
    float ene = raw->get_adc();
    //fRec->Fill(49152+key,ene);
    if(key<288) {
      fHcrytofS->Fill( tof );
      fHcryeneS->Fill( ene );
    } else {
      fHcrytofN->Fill( tof );
      fHcryeneN->Fill( ene );
    }
    fHcry->Fill( key, ene, tof );
  }
  /////////////////

  fRec->Make();

  //for(int gl=0; gl!=18; ++gl)
  //  std::cout << fRec->GetNHits(gl) << " ";
  //std::cout << std::endl;

  if(fDoQA) fQA->Make(fRec);

  return EVENT_OK;
}
