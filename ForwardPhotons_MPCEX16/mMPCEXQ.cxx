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
#include <qcQ.h>

#include "TMpcExHitContainer.h"
#include "TMpcExHit.h"
#include "MpcExEventHeader.h"

#include "mMPCEXQ.h"

using namespace std;
using namespace findNode;

//====================================================
mMPCEXQ::mMPCEXQ( const char* name ) : 
  SubsysReco( name ),
  fGeo( NULL ),
  fName( name ),
  fCentMin( 0.0 ),
  fCentMax( 80.0 ),
  fVertexMin( -100 ),
  fVertexMax( +100 ),
  fEvents( NULL ),
  fEnergy( NULL ),
  fVertex( NULL ),
  fCentrality( NULL ),
  fEXRejectedByEnergy( NULL ),
  fMPCRejectedByEnergy( NULL )
{
  printf("mcReco::CTOR\n");

  for(int i=0; i!=9; ++i) {
    fQ[i] = NULL;
    fEneLyr[i] = NULL;
    fXYhits[i] = NULL;
    fQxDist[i] = NULL;
    fQyDist[i] = NULL;
    fPsi2[i] = NULL;
    if(i>1) continue;
    fQbbc[i] = NULL;
    fBbcEneTim[i] = NULL;
    fBbcXY[i] = NULL;
  }

  fMPCIDX.clear();
  fBBCcalib = new BbcCalib();
  fBBCgeo = new BbcGeo();
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
int mMPCEXQ::End(PHCompositeNode *topNode) {
  return EVENT_OK;
}
//====================================================
mMPCEXQ::~mMPCEXQ() {
  if(fGeo) delete fGeo;
  if(fBBCcalib) delete fBBCcalib;
  if(fBBCgeo) delete fBBCgeo;
}
//====================================================
int mMPCEXQ::Init(PHCompositeNode* top_node) {
  printf("mMPCEXQ::Init\n");
  Fun4AllServer *se = Fun4AllServer::instance();

  fEvents = new TH1F(Form("%s_Events",fName.Data()),"Events",7,-0.5,+6.5);
  se->registerHisto( ((TH1*) (fEvents) ) );

  fEnergy = new TH2F(Form("%s_Energy",fName.Data()),"Energy",49152,-0.5,+49151.5,100,0,10000);
  se->registerHisto( ((TH2F*) (fEnergy) ) );
  fEnergy->GetXaxis()->SetBinLabel(1,"All Raw");
  fEnergy->GetXaxis()->SetBinLabel(2,"Triggered");
  fEnergy->GetXaxis()->SetBinLabel(3,"Detector Integrity");
  fEnergy->GetXaxis()->SetBinLabel(4,"Selected");

  fVertex = new TH1F(Form("%s_Vertex",fName.Data()),"Vertex",100,-100,+100);
  se->registerHisto( ((TH1F*) (fVertex) ) );

  fCentrality = new TH1F(Form("%s_Centrality",fName.Data()),"Centrality",100,-0.5,99.5);
  se->registerHisto( ((TH1F*) (fCentrality) ) );

  fMPCRejectedByEnergy = new TH1F(Form("%s_MPCRejectedByEnergy",fName.Data()),"MPCRejectedByEnergy",  416,-0.5,  415.5);
  se->registerHisto( ((TH1F*) (fMPCRejectedByEnergy) ) );

  fEXRejectedByEnergy  = new TH1F(Form("%s_EXRejectedByEnergy",fName.Data()), "EXRejectedByEnergy", 49152,-0.5,49151.5);
  se->registerHisto( ((TH1F*) (fEXRejectedByEnergy) ) );

  for(int i=0; i!=2; ++i) {
    fQbbc[i] = new qcQ(2);
    fBbcEneTim[i] = new TH2F(Form("%s_bbcET_%d",fName.Data(),i),
			 Form("bbcET_%d",i),
			 1000,-1,30,1000,-1,+30);
    se->registerHisto( ((TH2F*) (fBbcEneTim[i]) ) );
    fBbcXY[i] = new TH2F(Form("%s_bbcXY_%d",fName.Data(),i),
			 Form("bbcXY_%d",i),
			 100,-230,+230,100,-230,+230);
    se->registerHisto( ((TH2F*) (fBbcXY[i]) ) );
  }

  for(int i=0; i!=9; ++i) {
    fQ[i] = new qcQ(2);
    fXYhits[i] = new TH2F(Form("%s_XYhits_%d",fName.Data(),i),
			  Form("XYhits_%d",i),
			  100,-23,+23,100,-23,+23);
    se->registerHisto( ((TH2F*) (fXYhits[i]) ) );
    fQxDist[i] = new TH1F(Form("%s_QxDist_%d",fName.Data(),i),
			  Form("QxDist_%d",i),1000,-1,1);
    se->registerHisto( ((TH1F*) (fQxDist[i]) ) );
    fQyDist[i] = new TH1F(Form("%s_QyDist_%d",fName.Data(),i),
			  Form("QyDist_%d",i),1000,-1,1);
    se->registerHisto( ((TH1F*) (fQyDist[i]) ) );
    float energ = 30000;
    if(i==8) energ = 100;
    fEneLyr[i] = new TH1F(Form("%s_EneLyr_%d",fName.Data(),i),
			  Form("EneLyr_%d",i),1000,0,energ);
    se->registerHisto( ((TH1F*) (fEneLyr[i]) ) );
  }
  for(int i=0; i!=9; ++i) {
    fPsi2[i] = new TH2F( Form("%s_Psi2_%d",fName.Data(),i),
			 Form("Psi2_%d",i),
			 300, 0, TMath::Pi(), 300, 0, TMath::Pi() );
    se->registerHisto( ((TH2F*) (fPsi2[i]) ) );
  }

  fGeo = new mxGeometry();

  return EVENT_OK;
}
//====================================================
int mMPCEXQ::InitRun(PHCompositeNode* top_node) {
  printf("mMPCEXQ::InitRun\n");
  recoConsts *rc = recoConsts::instance();
  fBBCcalib->restore(rc->get_TimeStamp(), 4002);

  return EVENT_OK; 
}
//====================================================
bool mMPCEXQ::PassEventCuts(PHCompositeNode* top_node) {
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
int mMPCEXQ::process_event(PHCompositeNode* top_node) {
  static int nev = 0;
  if(nev%10000==0) {
    std::cout << Form("mMPCEXQ::process_event %d events", nev) << std::endl;
  }
  nev++;

  fEvents->Fill(0); // all raw
  TriggerHelper *myTH = new TriggerHelper(top_node);
  bool trig = myTH->trigScaled("BBCLL1(>0 tubes) novertex"); //CHANGE TO NARROW VERTEX
  trig = trig || myTH->trigScaled("BBCLL1(>0 tubes)_central_narrowvtx"); //CHANGE TO NARROW VERTEX
  delete myTH;
  if( !trig ) return ABORTEVENT;
  fEvents->Fill(1); // all triggered
  if(!PassEventCuts(top_node)) return ABORTEVENT;
  fEvents->Fill(2); // mpcex integrity

  //ReactionPlaneObject *rp = findNode::getClass<ReactionPlaneObject>(top_node, "ReactionPlaneObject");
  //cout << "BBC ";
  //cout << rp->getBBCrp10() << " ";
  //cout << rp->getBBCrp11() << " ";
  //cout << rp->getBBCrp12() << " ";
  //cout << rp->getBBCrp00() << " ";
  //cout << rp->getBBCrp01() << " ";
  //cout << rp->getBBCrp02() << endl;

  //cout << "CNT ";
  //cout << rp->getCNTrp10() << " ";
  //cout << rp->getCNTrp11() << " ";
  //cout << rp->getCNTrp12() << endl;
  //cout << rp->getCNTrp00() << " ";
  //cout << rp->getCNTrp01() << " ";
  //cout << rp->getCNTrp02() << endl;

  //cout << "MVD ";
  //cout << rp->getMVDrp10() << " ";
  //cout << rp->getMVDrp11() << " ";
  //cout << rp->getMVDrp12() << " ";
  //cout << rp->getMVDrp00() << " ";
  //cout << rp->getMVDrp01() << " ";
  //cout << rp->getMVDrp02() << endl;

  //cout << "SMD ";
  //cout << rp->getSMDrp10() << " ";
  //cout << rp->getSMDrp11() << " ";
  //cout << rp->getSMDrp12() << endl;

  //cout << "FCL ";
  //cout << rp->getFCLrp10() << " ";
  //cout << rp->getFCLrp11() << " ";
  //cout << rp->getFCLrp12() << endl;

  PHGlobal *phglobal = getClass<PHGlobal> (top_node, "PHGlobal");
  if(!phglobal) return ABORTEVENT;
  TMpcExHitContainer *mMpcExHits = getClass<TMpcExHitContainer>(top_node, "TMpcExHitContainer");
  if(!mMpcExHits) return ABORTEVENT;
  mpcRawContainer *mpcraw2 = getClass<mpcRawContainer>(top_node,"MpcRaw2");
  if(!mpcraw2) return ABORTEVENT;
  BbcRaw *bbcraw = getClass<BbcRaw>(top_node,"BbcRaw");
  if(!bbcraw) return ABORTEVENT;

  if( phglobal->getBbcZVertex() < fVertexMin ) return EVENT_OK;
  if( phglobal->getBbcZVertex() > fVertexMax ) return EVENT_OK;
  if( phglobal->getCentrality() < fCentMin ) return EVENT_OK;
  if( phglobal->getCentrality() > fCentMax ) return EVENT_OK;
  fEvents->Fill(3); // further event cuts
  fVertex->Fill( phglobal->getBbcZVertex() );
  fCentrality->Fill( phglobal->getCentrality() );

  for(int i=0; i!=9; ++i) fQ[i]->Reset();
  for(int i=0; i!=2; ++i) fQbbc[i]->Reset();
  float enelimit[8] = {2000,2000,2500,2500,3000,3000,3500,3500};

  //cout << mMpcExHits->size() << endl;
  for(unsigned int ihit=0; ihit!=mMpcExHits->size(); ++ihit) {
    TMpcExHit *hit = mMpcExHits->getHit(ihit);
    unsigned int key = hit->key();
    float energy = hit->combined();
    if(key>=24576) continue;
    int layer = fGeo->LyrIdx(key);
    if(energy<100) {
      fEXRejectedByEnergy->Fill( key );
      continue;
    }
    if(energy>enelimit[layer]) {
      fEXRejectedByEnergy->Fill( key );
      continue;
    }
    fEnergy->Fill(key,energy);
    fEneLyr[layer]->Fill( energy );
    fXYhits[layer]->Fill( fGeo->X(key), fGeo->Y(key), energy );
    float phi = TMath::Pi() + TMath::ATan2(-fGeo->Y(key),-fGeo->X(key));
    //cout << " ==> " << layer << " " << phi << " " << energy << endl;
    fQ[layer]->Fill( phi, energy );
  }

  for(unsigned int i=0; i!=mpcraw2->size(); ++i) {
    mpcRawContent *raw = mpcraw2->getTower(i);
    int chnmpc = raw->get_ch();
    int idxmpc = (*fMPCIDX.find(chnmpc)).second;
    int key = 49152+idxmpc;
    float adc = raw->get_adc();
    float ene = adc;
    int layer = fGeo->LyrIdx(key);
    if(layer!=8) continue;
    if(ene<0.5) {
      fMPCRejectedByEnergy->Fill( idxmpc );
      continue;
    }
    if(ene>30) {
      fMPCRejectedByEnergy->Fill( idxmpc );
      continue;
    }
    fEneLyr[layer]->Fill( ene );
    fXYhits[layer]->Fill( fGeo->X(key), fGeo->Y(key), ene );
    float phi = TMath::Pi() + TMath::ATan2(-fGeo->Y(key),-fGeo->X(key));
    //cout << idxmpc << " " << phi << " " << ene << endl;
    fQ[layer]->Fill( phi, ene );
  }

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
      fBbcEneTim[0]->Fill(charge,time0);
      fBbcXY[0]->Fill(bbc_x,bbc_y,charge);
      fQbbc[0]->Fill(bbc_phi,charge);
    } else {
      if(time0<-9999) continue;
      if(time0>+9999) continue;
      if(charge<-9999) continue;
      if(charge>+9999) continue;
      fBbcEneTim[1]->Fill(charge,time0);
      fBbcXY[1]->Fill(bbc_x,bbc_y,charge);
      fQbbc[1]->Fill(bbc_phi,charge);
    }
    // ================
  }


  //cout << fQ[8]->NP() << " " << fQ[8]->Psi()<< endl;

  for(int i=0; i!=9; ++i) {
    fPsi2[i]->Fill(fQ[i]->Psi(),fQbbc[0]->Psi());
    fQxDist[i]->Fill(fQ[i]->X()/fQ[i]->M(),fQ[i]->M());
    fQyDist[i]->Fill(fQ[i]->Y()/fQ[i]->M(),fQ[i]->M());
  }

  return EVENT_OK;
}
//======
