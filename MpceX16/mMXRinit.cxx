#include "stdlib.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

#include "getClass.h"
#include "PHCompositeNode.h"
#include "RunHeader.h" 
#include "Fun4AllReturnCodes.h"
#include "Fun4AllServer.h"

#include "TString.h"
#include "TRandom3.h"
#include "TH2F.h"

#include "recoConsts.h"
#include "PHTimeStamp.h"

#include "PHGlobal.h"
#include "VtxOut.h"
#include "PHPoint.h"
#include "MpcMap.h"

#include "mpcRawContainer.h"
#include "mpcRawContent.h"
#include "MpcExRawHit.h"
#include "MpcExEventHeader.h"
#include "TMpcExHitContainer.h"
#include "TMpcExHit.h"

#include "mxCalibMaster.h"
#include "mxCalibBaseSiW.h"
#include "mxCalibBasePbWO4.h"
#include "mxDB.cc"

#include "TriggerHelper.h"

#include "mxData.h"

#include "PHMXData.h"
#include "mMXRinit.h"

using namespace std;
using namespace findNode;

//====================================================
mMXRinit::mMXRinit( const char* name ) : 
  SubsysReco( name ),
  fSim( false ),
  fCalibMode(kDynamic),
  fSkipSouth(false),
  fSkipNorth(false),
  fCal(NULL),
  fData(NULL),
  fMinVertex(-100),
  fMaxVertex(+100),
  fMinCentrality(0),
  fMaxCentrality(100),
  fTrigger1("BBCLL1(>0 tubes) novertex"),
  fTrigger2("BBCLL1(>0 tubes)_central_narrowvtx"),
  fQAPS(false),
  fAdcHigh(NULL),
  fAdcLow(NULL),
  fAdcHighLow(NULL),
  fAdcL2H(NULL),
  fEnergyHigh(NULL),
  fEnergyLow(NULL)
{
  printf("mMXRinit::Ctor\n");
  fMPCIDX.clear();
  int mpcidx[416] = {
    13,14,15,19,20,21,22,23,26,27,28,29,30,31,33,34,35,36,37,38,
    39,40,41,42,43,44,45,46,47,49,50,51,52,53,54,56,57,58,59,60,
    61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,
    81,82,83,84,85,86,87,88,95,109,110,111,115,116,117,118,119,122,123,124,
    125,126,127,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,157,158,
    159,163,164,165,166,167,170,171,172,173,174,175,177,178,179,180,181,182,183,184,
    185,186,187,188,189,190,191,193,194,195,196,197,198,200,201,202,203,204,205,206,
    207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,
    227,228,229,230,231,232,239,253,254,255,259,260,261,262,263,266,267,268,269,270,
    271,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,
    293,294,295,299,300,301,302,303,306,307,308,309,310,311,313,314,315,316,317,318,
    319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,338,339,340,
    341,342,343,345,346,347,348,349,350,353,354,355,356,357,358,361,362,363,364,365,
    366,369,370,371,372,373,374,378,379,380,381,382,383,389,390,391,395,396,397,398,
    399,402,403,404,405,406,407,409,410,411,412,413,414,415,416,417,418,419,420,421,
    422,423,424,425,426,427,428,429,430,431,437,438,439,443,444,445,446,447,450,451,
    452,453,454,455,457,458,459,460,461,462,463,464,465,466,467,468,469,470,471,472,
    473,474,475,476,477,478,479,482,483,484,485,486,487,489,490,491,492,493,494,497,
    498,499,500,501,502,505,506,507,508,509,510,513,514,515,516,517,518,522,523,524,
    525,526,527,533,534,535,539,540,541,542,543,546,547,548,549,550,551,553,554,555,
    556,557,558,559,560,561,562,563,564,565,566,567,568,569,570,571,572,573,574,575};
  for(int i=0; i!=416; ++i) fMPCIDX.insert( std::make_pair( mpcidx[i], i) );
}
//====================================================
int mMXRinit::End(PHCompositeNode *topNode) {
  printf("mMXRinit::End\n");
  return EVENT_OK;
}
//====================================================
mMXRinit::~mMXRinit() {
  printf("mMXRinit::Dtor\n");
  if(fCal) delete fCal;
  if(fSim) printf("mMXRinit::SimFlag enabled\n");
}
//====================================================
int mMXRinit::Init(PHCompositeNode* top_node) {
  printf("mMXRinit::Init\n");
  Fun4AllServer *se = Fun4AllServer::instance();
  if(fQAPS) {
    fAdcHigh = new TH2F("AdcHigh","AdcHigh",49152,-0.5,49151.5,100,-20.5,79.5);
    fAdcLow = new TH2F("AdcLow","AdcLow",49152,-0.5,49151.5,100,-20.5,79.5);
    fAdcHighLow = new TH2F("AdcHighLow","AdcHighLow;High;Low",128,-50.5,255.5,128,-50.5,255.5);
    fAdcL2H = new TH2F("AdcL2H","AdcL2H",49152,-0.5,49151.5,100,0.1,0.5);
    fEnergyHigh = new TH2F("EnergyHigh","EnergyHigh;MeV",49152,-0.5,49151.5, 300, 0.0, 3.0);
    fEnergyLow = new TH2F("EnergyLow","EnergyLow;MeV",49152,-0.5,49151.5, 800, 1.0, 9.0);
    se->registerHisto( ((TH2F*) (fAdcHigh)) );
    se->registerHisto( ((TH2F*) (fAdcLow)) );
    se->registerHisto( ((TH2F*) (fAdcHighLow)) );
    se->registerHisto( ((TH2F*) (fAdcL2H)) );
    se->registerHisto( ((TH2F*) (fEnergyHigh)) );
    se->registerHisto( ((TH2F*) (fEnergyLow)) );
  }

  return EVENT_OK;
}
//====================================================
int mMXRinit::InitRun(PHCompositeNode* top_node) {
  printf("mMXRinit::InitRun\n");
  PHNodeIterator nodeIter(top_node);
  PHCompositeNode *dstNode = static_cast<PHCompositeNode*>(nodeIter.findFirst("PHCompositeNode","DST"));
  if (dstNode == NULL) {
    dstNode = new PHCompositeNode("DST");
    top_node->addNode(dstNode);
  }
  PHMXData *mxdata = new PHMXData();
  PHIODataNode<PHMXData> *hitNode = new PHIODataNode<PHMXData>(mxdata,"MXR","PHMXData");
  dstNode->addNode(hitNode);
  fData = mxdata->GetData();

  RunHeader *runhead = getClass<RunHeader> (top_node, "RunHeader");
  if(!runhead) { cout<<PHWHERE<<" exiting."<<endl; exit(1); }
  int runno = runhead->get_RunNumber();

  printf("mMXRinit::InitRun || Run number %d\n",runno);
  fCal = new mxCalibMaster();
  if(fCalibMode==kStatic) {
    //load from scratchfile
    mxDB::read(runno,fCal);
  }
  return EVENT_OK; 
}
//====================================================
bool mMXRinit::PassSimEventCuts(PHCompositeNode* top_node) {
  return true;
}
//====================================================
bool mMXRinit::PassDataEventCuts(PHCompositeNode* top_node) {
  TriggerHelper *myTH = new TriggerHelper(top_node);
  bool trig = myTH->trigScaled(fTrigger1.Data()); //CHANGE TO NARROW VERTEX
  trig = trig || myTH->trigScaled(fTrigger2.Data()); //CHANGE TO NARROW VERTEX
  delete myTH;
  if( !trig ) return ABORTEVENT;

  PHGlobal *phglobal = getClass<PHGlobal> (top_node, "PHGlobal");
  if(!phglobal) return false;
  if( phglobal->getBbcZVertex() < fMinVertex ) return ABORTEVENT;
  if( phglobal->getBbcZVertex() > fMaxVertex ) return ABORTEVENT;
  if( phglobal->getCentrality() < fMinCentrality ) return ABORTEVENT;
  if( phglobal->getCentrality() > fMaxCentrality ) return ABORTEVENT;

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
int mMXRinit::process_event(PHCompositeNode* top_node) {
  static int nev = 0;
  //std::cout << " HOLA " << nev << std::endl;
  if(nev%43556==0) {
    std::cout << Form("mMXRinit::process_event %d events", nev) << std::endl;
  }
  nev++;
  if(fSim) {
    if(!PassSimEventCuts(top_node)) return ABORTEVENT;
  } else {
    if(!PassDataEventCuts(top_node)) return ABORTEVENT;
  }
  fData->Reset();
  PHGlobal *phglobal = getClass<PHGlobal> (top_node, "PHGlobal");
  if(!phglobal) return false;
  fData->SetVertex( 0, 0, phglobal->getBbcZVertex() );

  /*
  VtxOut *vtxout = getClass<VtxOut> (top_node, "VtxOut");
  if(!vtxout) return false;
  PHPoint vx = vtxout->get_Vertex();
  fData->SetVertex( vx.getX(), vx.getY(), vx.getZ() );
  */

  /////////////////
  // reading mpcex data
  if(fCalibMode==kStatic) {
    MpcExRawHit *mMpcExRawHits = getClass<MpcExRawHit>(top_node, "MpcExRawHit");
    if(!mMpcExRawHits) return ABORTEVENT;
    //std::cout << "HITS " << mMpcExRawHits->getnhits() << std::endl;
    for(unsigned int ihit=0; ihit!=mMpcExRawHits->getnhits(); ++ihit) {
      unsigned int key = mMpcExRawHits->getOnlineKey(ihit);
      if(fSkipSouth&&key<24576) continue;
      if(fSkipNorth&&key>24575) continue;
      if( fCal->IsBadKey(key) ) continue;
      //std::cout << "HITS " << ihit << " OF " << mMpcExRawHits->getnhits() << std::endl;
      float pedmean_hi = fCal->GetPHMu()->Get(key);
      float pedmean_lo = fCal->GetPLMu()->Get(key);
      float hi_adc = mMpcExRawHits->gethadc(ihit) - pedmean_hi;
      float lo_adc = mMpcExRawHits->getladc(ihit)  - pedmean_lo;
      //if(hi_adc<-12) continue;
      //if(lo_adc<-12) continue;
      float pedshift_hi = fCal->GetPHSh()->Get(key);
      float pedshift_lo = fCal->GetPLSh()->Get(key);
      float hi_adc_corr = hi_adc + gRandom->Rndm() - pedshift_hi;
      float lo_adc_corr = lo_adc + gRandom->Rndm() - pedshift_lo;
      float hi_adc_max = 255-20 - pedmean_hi - pedshift_hi;
      float lo_adc_max = 255-20 - pedmean_lo - pedshift_lo;
      float lmpv = 147.0/fCal->GetLMPV()->Get(key); // in keV;
      float lhft = fCal->GetLHft()->Get(key);
      float hires = ( hi_adc_corr * lmpv) * 1e-6; // in GeV
      float lores = ( lo_adc_corr / lhft * lmpv) * 1e-6; // in GeV
      if(fQAPS) {
	fAdcHigh->Fill(key,hi_adc);
	fAdcLow->Fill(key,lo_adc);
	fAdcHighLow->Fill(hi_adc,lo_adc);
	if(hi_adc_corr>50&&hi_adc_corr<std::min(hi_adc_max,float(150))) {
	  fAdcL2H->Fill(key,lo_adc_corr/hi_adc_corr);
	}
	fEnergyHigh->Fill(key,hires*1e3); // in MeV
	fEnergyLow->Fill(key,lores*1e3); // in MeV
      }
      if( fCal->IsBadKey(key) ) continue;
      //float fNSigmaCut = 3.0;
      //float enecut = TMath::Max( (fCal->GetLMPV()->Get(key) - fNSigmaCut*fCal->GetLSgm()->Get(key)) * 1e-6 , 1e-6);
      bool useHi = hi_adc_corr < hi_adc_max;
      float ene = useHi?hires:lores;
      float enecut = 1e-4;//100 keV
      if(ene<enecut) continue;
      if(lo_adc_corr > lo_adc_max) {
	ene = ( lo_adc_max / lhft * lmpv) * 1e-6;
      }
      fData->Fill(key,ene);
    }
  } else {
    TMpcExHitContainer *mMpcExHits = getClass<TMpcExHitContainer>(top_node, "TMpcExHitContainer");
    if(!mMpcExHits) return ABORTEVENT;
    for(unsigned int ihit=0; ihit!=mMpcExHits->size(); ++ihit) {
      TMpcExHit *hit = mMpcExHits->getHit(ihit);
      unsigned int key = hit->key();
      float ene = hit->combined() * 1e-6; // in GeV
      if(fSkipSouth&&key<24576) continue;
      if(fSkipNorth&&key>24575) continue;
      fData->Fill(key,ene);
    }
  }

  /////////////////
  // reading mpc data
  mpcRawContainer *mpcraw2 = findNode::getClass<mpcRawContainer>(top_node,"MpcRaw2");
  if(!mpcraw2) return ABORTEVENT;
  for(unsigned int i=0; i!=mpcraw2->size(); ++i) {
    mpcRawContent *raw = mpcraw2->getTower(i);
    int chnmpc = raw->get_ch();
    int idxmpc = (*fMPCIDX.find(chnmpc)).second;
    if(fSkipSouth&&idxmpc<196) continue;
    if(fSkipNorth&&idxmpc>195) continue;
    int key = 49152+idxmpc;
    //float tau = fCal->GetTau()->Get(key);
    float gam = 1;//fCal->GetGam()->Get(key);
    //float tof = raw->get_sample();
    float adc = raw->get_adc();
    float ene = gam*adc;
    //std::cout << "MPC" << i << " " << adc << " " << gam << " " << ene << std::endl;
    //float cti = tof-tau;
    fData->Fill(key,ene);
  }
  /////////////////
  //std::cout << "mMXRinit::EndOdProcesses" << std::endl;
  return EVENT_OK;
}
