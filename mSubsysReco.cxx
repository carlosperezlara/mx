#include "stdlib.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

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
#include "mxCalibBaseSiW.h"
#include "mxCalibBasePbWO4.h"
#include "mxDB.cc"

#include "mxReconstruction.h"
#include "mxQAReconstruction.h"

#include "PHMXData.h"
#include "mSubsysReco.h"

#include "mxCoalitionCuts.h"

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
  fDoQA(false),
  fCal(NULL),
  fRec(NULL),
  fQA(NULL),
  fFlush(false),
  fList(NULL),
  fCheckMpcRaw2(true),
  fCheckMpcExRawHit(true),
  fNSigmaCut(0),
  fByPassEXCalibration(false),
  fAlgorithmCombo(0),
  fNoCuts(NULL),
  fCalibrationCuts(NULL)
{
  printf("mcReco::CTOR\n");
  fFileOut.open( "dataflow.hit" );
  for(int i=0; i!=2; ++i) {
    fHstk[i] = NULL;
    fHpar[i] = NULL;
    fHsph[i] = NULL;
    fHcid[i] = NULL;
    fHadc[i] = NULL;
    fHlhf[i] = NULL;
    fHcry[i] = NULL;
  }

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
int mSubsysReco::End(PHCompositeNode *topNode) {
  fFileOut.close();
  return EVENT_OK;
}
//====================================================
mSubsysReco::~mSubsysReco() {
  if(fCal) delete fCal;
  //if(fRec) delete fRec; // deleted by Node
  if(fQA) delete fQA;
  if(fList) delete fList;
  if(fNoCuts) delete fNoCuts;
  if(fCalibrationCuts) delete fCalibrationCuts;
}
//====================================================
int mSubsysReco::Init(PHCompositeNode* top_node) {
  printf("mSubsysReco::Init\n");

  Fun4AllServer *se = Fun4AllServer::instance();

  fNoCuts = new mxCoalitionCuts("mSRNoCuts");
  //fNoCuts->SetQA();
  //fNoCuts->GetList()->SetOwner(false);
  //for(int i=0; i!=fNoCuts->GetList()->GetEntries(); ++i)
  //  se->registerHisto( ((TH1*) (fNoCuts->GetList()->At(i))) );

  fCalibrationCuts = new mxCoalitionCuts("mSRCalibrationCuts");
  //fCalibrationCuts->SetQA();
  fCalibrationCuts->Set_HitLayer(5);
  fCalibrationCuts->Set_HitLayer(6);
  fCalibrationCuts->Set_HitLayer(7);
  //fCalibrationCuts->Set_HitLayer(8);
  fCalibrationCuts->Set_PS_minChi2Prob(0.2);
  //fCalibrationCuts->GetList()->SetOwner(false);
  //for(int i=0; i!=fCalibrationCuts->GetList()->GetEntries(); ++i)
  //  se->registerHisto( ((TH1*) (fCalibrationCuts->GetList()->At(i))) );

  if(fDoQA) {
    fQA = new mxQAReconstruction();
    fQA->GetList()->SetOwner(false);
    for(int i=0; i!=fQA->GetList()->GetEntries(); ++i)
      se->registerHisto( ((TH1*) (fQA->GetList()->At(i))) );
  }

  fList = new TList();
  //fList->SetOwner(); // histos passed to mannager
  if(fCheckMpcExRawHit) {
    for(int i=0; i!=2; ++i) {
      fHstk[i] = new TH1F( Form("mxDet_Hstk%d",i), Form("mxDet_Hstk%d",i),  6,-0.5,5.5);
      fHpar[i] = new TH1F( Form("mxDet_Hpar%d",i), Form("mxDet_Hpar%d",i),120,-0.5,120-0.5);
      fHsph[i] = new TH1F( Form("mxDet_Hsph%d",i), Form("mxDet_Hsph%d",i),120,-0.5,520-0.5);
      fHcid[i] = new TH2F( Form("mxDet_Hcid%d",i), Form("mxDet_Hcid%d",i), 16,-0.5,15.5,47,-0.5,46.5);
      fHadc[i] = new TH2F( Form("mxDet_Hadc%d",i), Form("mxDet_Hadc%d",i),49152,-0.5,49151.5,100,-20.5,79.5 );
      fHlhf[i] = new TH2F( Form("mxDet_Hlhf%d",i), Form("mxDet_Hlhf%d",i),49152,-0.5,49151.5,50,0.0,0.5 );
      fList->Add(fHstk[i]);
      fList->Add(fHpar[i]);
      fList->Add(fHsph[i]);
      fList->Add(fHcid[i]);
      fList->Add(fHadc[i]);
      fList->Add(fHlhf[i]);
    }
  }
  if(fCheckMpcRaw2) {
    for(int i=0; i!=2; ++i) {
      fHcry[i] = new TH3F( Form("mxDet_Hcry%d",i), Form("mxDet_Hcry%d",i), 416,-0.5,415.5, 100,0,50, 100,0,15 );
      fList->Add(fHcry[i]);
    }
  }

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
  switch(fAlgorithmCombo) {
  case(0):
    fRec->SetPartyAlgorithm(0);
    fRec->SetCoalitionAlgorithm(0);
    break;
  case(1):
    fRec->SetPartyAlgorithm(1);
    fRec->SetCoalitionAlgorithm(1);
    fRec->SetPtyAlg1_Threshold(0.15);
    break;
  }

  RunHeader *runhead = getClass<RunHeader> (top_node, "RunHeader");
  if(!runhead) { cout<<PHWHERE<<" exiting."<<endl; exit(1); }
  int runno = runhead->get_RunNumber();

  printf("mSubsysReco::InitRun || Run number %d\n",runno);
  fCal = new mxCalibMaster();
  mxDB::read(runno,fCal);
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
  //Fun4AllServer *se = Fun4AllServer::instance();
  //se->Print();
  //exit(0);

  //std::cout << "mSubsysReco::process_event" << std::endl;
  static int nev = 0;
  if(nev%10000==0) {
    std::cout << Form("mSubsysReco::process_event %d events", nev) << std::endl;
  }
  nev++;
  if(fCheckMpcExRawHit) FillHistos(0,top_node);
  if(!PassEventCuts(top_node)) return ABORTEVENT;
  if(fCheckMpcExRawHit) FillHistos(1,top_node);

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
  float buffE[49728];
  int buffK[49728];
  int nbuff = 0;
  MpcExRawHit *mMpcExRawHits = getClass<MpcExRawHit>(top_node, "MpcExRawHit");
  if(!mMpcExRawHits) return ABORTEVENT;
  TMpcExHitSet<myOrder> rawcontainer(mMpcExRawHits);
  for(TMpcExHitSet<myOrder>::const_iterator itr=rawcontainer.get_iterator();
      itr!=rawcontainer.end();
      ++itr) {
    TMpcExHit *raw_hit = (*itr);
    if(!raw_hit) continue;
    unsigned int key = raw_hit->key();
    if( fCal->IsBadKey(key) ) continue;
    float hi_adc = raw_hit->high() - fCal->GetPHMu()->Get(key);
    float lo_adc = raw_hit->low()  - fCal->GetPLMu()->Get(key);
    float lmpv = 147.0/fCal->GetLMPV()->Get(key); // in keV;
    float lhft = fCal->GetLHft()->Get(key);
    float enecut = TMath::Max( (fCal->GetLMPV()->Get(key) - fNSigmaCut*fCal->GetLSgm()->Get(key)) * 1e-6 , 1e-6);
    if(fByPassEXCalibration) {
      //temporal solution towards energy calibration
      int pkt = key/3072;
      lmpv = 147.0/15.0;
      if(pkt==2||pkt==6) lmpv = 147.0/23.0;
      if(pkt==10||pkt==14) lmpv = 147.0/23.0;
      enecut = 1e-6; // 1 keV
    }
    float hires = (hi_adc * lmpv) * 1e-6; // in GeV
    float lores = (lo_adc / lhft * lmpv) * 1e-6; // in GeV
    if(fCheckMpcExRawHit) {
      fHadc[0]->Fill(key,hi_adc);
      if(hi_adc>40 && hi_adc<150)
      	fHlhf[0]->Fill(key,lo_adc/hi_adc);
    }
    if( fCal->IsBadKey(key) ) continue;
    bool useHi = hi_adc<150;
    float ene = useHi?hires:lores;
    if(ene>enecut) {
      fRec->Fill(key,ene);
      buffE[nbuff] = ene;
      buffK[nbuff] = key;
      ++nbuff;
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
    int key = 49152+idxmpc;
    float tau = fCal->GetTau()->Get(key);
    float gam = fCal->GetGam()->Get(key);
    float tof = raw->get_sample();
    float adc = raw->get_adc();
    float ene = adc;
    float cti = tof-tau;
    fRec->Fill(key,ene);
    buffE[nbuff] = adc;
    buffK[nbuff] = key;
    ++nbuff;
    if(fCheckMpcRaw2) {
      fHcry[0]->Fill( float(idxmpc), adc, tof );
      fHcry[1]->Fill( float(idxmpc), ene, 7+cti );
    }
  }
  /////////////////

  if(fFlush) {
    fFileOut << nbuff << std::endl;
    for(int i=0; i!=nbuff; ++i) {
      fFileOut << buffK[i] << " " << buffE[i] << std::endl;
    }
  }
  fRec->Make();
  //fRec->DumpStats();
  //fRec->DumpParties();

  for(int arm=0; arm!=2; ++arm) {
    int n = fRec->GetNCoalitions(arm);
    for(int i=0; i!=n; ++i) {
      mxCoalition *coa = fRec->GetCoalition(arm,i);
      if(!coa) continue;
      fNoCuts->PassesCuts(coa);
    }
  }

  if(fCheckMpcExRawHit) {
    // selecting only hits that make a coalition
    bool keyfiltered[49152];
    for(int k=0; k!=49152; ++k) keyfiltered[k] = false;
    // init buffering
    mxCoalition *coa;
    for(int arm=0; arm!=2; ++arm) {
      int n = fRec->GetNCoalitions(arm);
      for(int i=0; i!=n; ++i) {
	coa = fRec->GetCoalition(arm,i);
	if(!coa) continue;
	if( fCalibrationCuts->PassesCuts(coa) ) {
	//if( fNoCuts->PassesCuts(coa) ) {
	  for(int hl=0; hl!=8; ++hl) { //leaving out MPC
	    mxParty *pty = coa->GetParty(hl);
	    if(!pty) continue;
	    for(int ht=0; ht!=pty->N(); ++ht) {
	      mxHit *hit = pty->GetHit(ht);
	      if(!hit) continue;
	      unsigned int key = hit->Idx();
	      if(key<0 || key>49151) continue;
	      keyfiltered[key] = true;
	    }
	  }
	}
      }
    }
    // end of buffering and begining of booking
    for(TMpcExHitSet<myOrder>::const_iterator itr=rawcontainer.get_iterator();
	itr!=rawcontainer.end();
	++itr) {
      TMpcExHit *raw_hit = (*itr);
      if(!raw_hit) continue;
      unsigned int key = raw_hit->key();
      if(key<0 || key>49151) continue;
      if( !keyfiltered[key] ) continue;
      float hi_adc = raw_hit->high() - fCal->GetPHMu()->Get(key);
      fHadc[1]->Fill(key,hi_adc);
    }
    // done
  }

  //std::cout << "Starting QA..." << std::endl;
  if(fDoQA) fQA->Make(fRec);
  //std::cout << "mSubsysReco::EndOdProcesses" << std::endl;
  return EVENT_OK;
}
