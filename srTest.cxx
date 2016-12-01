#include "stdlib.h"
#include <iostream>
#include <vector>
#include <algorithm>

#include "getClass.h"
#include "PHCompositeNode.h"
#include "RunHeader.h" 
#include "Fun4AllReturnCodes.h"
#include "Fun4AllServer.h"

#include "recoConsts.h"
#include "PHTimeStamp.h"

//#include "MpcExRawHit.h"
//#include "TMpcExHit.h"
//#include "TMpcExHitSet.h"
//#include "MpcExEventHeader.h"
//#include "TMpcExGeaHitContainer.h"
//#include "TMpcExGeaHit.h"

//#include "../calibration/mxCalibMaster.h"
//#include "../calibration/mxCalibDAu16.h"
//#include "../calibration/mxCalibBase.h"
//#include "../calibration/mxChipStatus.h"

//#include "../pr/mxReconstruction.h"
//#include "../pr/mxParty.h"

#include "TString.h"

#include "MpcMap.h"

#include "srTest.h"

using namespace std;
using namespace findNode;

//struct myOrder : public std::binary_function<TMpcExHit*,TMpcExHit*,bool> {
//public:
//  bool operator()(TMpcExHit* lhs, TMpcExHit* rhs){
//    return lhs->key() <= rhs->key();
//  }
//};

//====================================================
srTest::srTest( const char* name ) : 
  SubsysReco( name )
{
  printf("mcReco::CTOR\n");
}
//====================================================
int srTest::End(PHCompositeNode *topNode) {
  return EVENT_OK;
}
//====================================================
srTest::~srTest() {
}
//====================================================
int srTest::Init(PHCompositeNode* top_node) {
  printf("srTest::Init\n");
  Fun4AllServer *se = Fun4AllServer::instance();

  return EVENT_OK;
}
//====================================================
int srTest::InitRun(PHCompositeNode* top_node) {
  printf("srTest::InitRun\n");
  RunHeader *runhead = getClass<RunHeader> (top_node, "RunHeader");
  if(!runhead) { cout<<PHWHERE<<" exiting."<<endl; exit(1); }
  int runno = runhead->get_RunNumber();
  printf("srTest::InitRun || Run number %d\n",runno);

  MpcMap *mpcmap = getClass<MpcMap>(top_node, "MpcMap");
  if(mpcmap) {
    std::cout << " got MPC map!" << std::endl;
    const int MAXCH = 576;
    float mpc_x[MAXCH], mpc_y[MAXCH], mpc_z[MAXCH];
    for(int fee_ch=0; fee_ch!=MAXCH; ++fee_ch) {
      mpc_x[fee_ch] = mpcmap->getX(fee_ch);
      mpc_y[fee_ch] = mpcmap->getY(fee_ch);
      mpc_z[fee_ch] = mpcmap->getZ(fee_ch);
    }
    std::cout << Form("\n mpcx[%d] = { ",MAXCH);
    for(int i=0; i!=MAXCH; ++i) {
      if(i%18==0) std::cout << std::endl;
      std::cout << Form("%.1f, ",mpc_x[i]);
    }
    std::cout << Form("\n mpcy[%d] = { ",MAXCH);
    for(int i=0; i!=MAXCH; ++i) {
      if(i%18==0) std::cout << std::endl;
      std::cout << Form("%.1f, ",mpc_y[i]);
    }
    std::cout << Form("\n mpcz[%d] = { ",MAXCH);
    for(int i=0; i!=MAXCH; ++i) {
      if(i%18==0) std::cout << std::endl;
      std::cout << Form("%.1f, ",mpc_z[i]);
    }
  }




  return EVENT_OK; 
}
//====================================================
int srTest::process_event(PHCompositeNode* top_node) {
  /////////////////
  // reading mpcex data
  //MpcExRawHit *mMpcExRawHits = getClass<MpcExRawHit>(top_node, "MpcExRawHit");
  //if(!mMpcExRawHits) return ABORTEVENT;
  //TMpcExHitSet<myOrder> rawcontainer(mMpcExRawHits);
  //for(TMpcExHitSet<myOrder>::const_iterator itr=rawcontainer.get_iterator();
  //    itr!=rawcontainer.end();
  //    ++itr) {
  //  TMpcExHit *raw_hit = (*itr);
  //  if(!raw_hit) continue;
  //}
  /////////////////

  return EVENT_OK;
}
