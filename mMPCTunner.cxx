#include "stdlib.h"
#include <iostream>
#include <map>

#include "TFile.h"
#include "TTree.h"

#include "getClass.h"
#include "PHCompositeNode.h"

#include "Fun4AllReturnCodes.h"
#include "Fun4AllServer.h"

#include "PHGlobal.h"
#include "VtxOut.h"
#include "PHPoint.h"
#include "MpcMap.h"
#include "MpcCalib.h"

#include "mpcRawContainer.h"
#include "mpcRawContent.h"
#include "MpcExRawHit.h"
#include "MpcExEventHeader.h"

#include "mMPCTunner.h"
#include "mxEventCuts.h"

using namespace std;
using namespace findNode;

//====================================================
mMPCTunner::mMPCTunner( const char* name ) : 
  SubsysReco( name ),
  fEventCuts( new mxEventCuts() ),
  fDSTfile(NULL),
  fTree(NULL),
  fVertexZ(0),
  fBBCS(0),
  fBBCN(0),
  fTrigger(0)
{
  printf("mcReco::CTOR\n");
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
  PrepareTree( Form("T_%s",name) );
}
//====================================================
int mMPCTunner::End(PHCompositeNode *topNode) {
  WriteTree();
  return EVENT_OK;
}
//====================================================
mMPCTunner::~mMPCTunner() {
  delete fEventCuts;
  if(fDSTfile) delete fDSTfile;
}
//====================================================
int mMPCTunner::Init(PHCompositeNode* top_node) {
  printf("mMPCTunner::Init\n");
  return EVENT_OK;
}
//====================================================
void mMPCTunner::PrepareTree(TString name) {
  fDSTfile = new TFile(name.Data(),"RECREATE");
  fTree = new TTree("MPC","quickanalysis");
  fTree->SetAutoSave(10000);
  fTree->Branch("trigger",&fTrigger,"trigger/b");
  fTree->Branch("vertexZ",&fVertexZ,"vertexZ/F");
  fTree->Branch("bbcS",&fBBCS,"bbcS/F");
  fTree->Branch("bbcN",&fBBCN,"bbcN/F");
  fTree->Branch("adc",fMPCAdc,"adc[416]/F");
}
//====================================================
void mMPCTunner::WriteTree() {
  fDSTfile->cd();
  fDSTfile->Write();
  fDSTfile->Close();
}
//====================================================
int mMPCTunner::InitRun(PHCompositeNode* top_node) {
  printf("mMPCTunner::InitRun\n");

  MpcCalib *mpccalib = findNode::getClass<MpcCalib>(top_node, "MpcCalib");
  if(mpccalib) {
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
    cout << "calib[416] = {" << endl;
    for(int i=0; i!=416; ++i) {
      cout << ", " << mpccalib->get_adc_gain(mpcidx[i]);
    }
    cout << "}" << endl;
  }
  return EVENT_OK; 
}
//====================================================
int mMPCTunner::process_event(PHCompositeNode* top_node)
{
  fTrigger = fEventCuts->PassEventCuts(top_node);
  if(fTrigger==0) return ABORTEVENT;

  PHGlobal *phglobal = getClass<PHGlobal> (top_node, "PHGlobal");
  if(!phglobal) return ABORTEVENT;
  fVertexZ = phglobal->getBbcZVertex();
  fBBCS = phglobal->getBbcChargeS();
  fBBCN = phglobal->getBbcChargeN();

  mpcRawContainer *mpcraw2 = findNode::getClass<mpcRawContainer>(top_node,"MpcRaw2");
  if(!mpcraw2) return ABORTEVENT;
  for(int i=0; i!=416; ++i) fMPCAdc[i] = 0;

  for(unsigned int i=0; i!=mpcraw2->size(); ++i) {
    mpcRawContent *raw = mpcraw2->getTower(i);
    int chnmpc = raw->get_ch();
    int idxmpc = (*fMPCIDX.find(chnmpc)).second;
    //float tof = raw->get_sample();
    Float_t adc = raw->get_adc();
    fMPCAdc[idxmpc] = adc;
  }

  fTree->Fill();
  return EVENT_OK;
}
