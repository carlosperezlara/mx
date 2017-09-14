#include "TMath.h"

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

#include <TriggerHelper.h>
#include <TrigLvl1.h>

#include "mpcRawContainer.h"
#include "mpcRawContent.h"
#include "MpcExRawHit.h"
#include "MpcExEventHeader.h"

#include "mxSafetyCuts.h"

using namespace std;
using namespace findNode;

//====================================================
mxSafetyCuts::mxSafetyCuts()
{
  printf("mxSafetyCuts::CTOR\n");
}
//====================================================
mxSafetyCuts::~mxSafetyCuts() {
}
//====================================================
UChar_t mxSafetyCuts::PassSafetyCuts(PHCompositeNode* top_node) {
  PHGlobal *phglobal = getClass<PHGlobal> (top_node, "PHGlobal");
  if(!phglobal) return 0;
  if( TMath::Abs( phglobal->getBbcZVertex() ) > 10 ) return 0;

  MpcExEventHeader *mMpcExEventHeader = getClass<MpcExEventHeader>(top_node,"MpcExEventHeader");
  if(!mMpcExEventHeader) return 0;

  UChar_t trigg = 0;
  TriggerHelper *myTH = new TriggerHelper(top_node); 
  trigg += myTH->trigScaled("BBCLL1(>0 tubes) novertex")?1:0;
  trigg += myTH->trigScaled("BBCLL1(>0 tubes) narrowvtx")?2:0;
  trigg += myTH->trigScaled("BBCLL1(>0 tubes)_central_narrowvtx")?4:0;
  trigg += myTH->trigScaled("MPC_N_A")?8:0;
  trigg += myTH->trigScaled("MPC_S_A")?16:0;
  trigg += myTH->trigScaled("MPC_N_B")?32:0;
  trigg += myTH->trigScaled("MPC_S_B")?64:0;
  delete myTH;

  /////////////////
  // check stack
  // 1) select only stack==1
  if(mMpcExEventHeader->getStack() != 1) return 0;

  /////////////////
  // check PARtimes
  bool badpar = false;
  int par[16]; // stores partimes; one per packet
  for(int i=0; i!=16; ++i) par[i] = mMpcExEventHeader->getPARSTTime(i);
  // 2) all south packets should have the same partime
  for(int i=1; i!=8; ++i)  if(par[i]!=par[0]) badpar = true; // all south should have the same
  if(badpar) return 0;
  // 3) all north packets should have the same partime
  for(int i=9; i!=16; ++i) if(par[i]!=par[8]) badpar = true; // all north should have the same
  if(badpar) return 0;
  // 4) partime must be equal for south and north
  int corpar = TMath::Abs(par[8]-par[0]);
  if(corpar>1) return 0;
  // 5) partime ranges where adc signal is untrustable
  if(par[0]>28&&par[0]<48) return 0;
  if(par[8]>28&&par[8]<48) return 0;

  /////////////////
  // check statephase
  bool badsph = false;
  int sph[16]; // stores statephase; one per packet
  for(int i=0; i!=16; ++i) sph[i] = mMpcExEventHeader->getStatephase(i) & 0xfff;
  // 6) all south packets should have the same statephase
  for(int i=1; i!=8; ++i)  if(sph[i]!=sph[0]) badsph = true; // all south should have the same
  if(badsph) return 0;
  // 7) all north packets should have the same statephase
  for(int i=9; i!=16; ++i) if(sph[i]!=sph[8]) badsph = true; // all north should have the same
  if(badsph) return 0;
  // 8) statephase must be equal for south and north
  int corsph = TMath::Abs(sph[8]-sph[0]);
  if(corsph>1) return 0;

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
      if( myCHPMap[0][pkt][chp] != cids ) return 0;
      if( myCHPMap[1][pkt][chp] != cidn ) return 0;
    }
  }

  return trigg;
}
