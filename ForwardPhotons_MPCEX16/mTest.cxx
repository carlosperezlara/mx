#include "stdlib.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

#include "getClass.h"
#include "PHCompositeNode.h"
#include "Fun4AllReturnCodes.h"
#include "Fun4AllServer.h"

#include "recoConsts.h"
#include "PHTimeStamp.h"

#include "PHGlobal.h"

#include "mxData.h"
#include "mxClustererMPC.h"
#include "mxClustererEX.h"
#include "mxDisplay.h"
#include "mxGeometry.h"

#include "PHMXData.h"
#include "mTest.h"

#include "TStopwatch.h"
#include "TH1F.h"

#include "mxCandidateCuts.h"

using namespace std;
using namespace findNode;

//====================================================
mTest::mTest( const char* name ) : 
  SubsysReco( name ),
  fTime(new TStopwatch),
  fData(NULL),
  fClustererMPC(new mxClustererMPC),
  fClustererEX(new mxClustererEX),
  fDisplay(new mxDisplay),
  fGeo(mxGeometry::Instance())
{
  printf("mcReco::CTOR\n");
  for(int i=0; i!=16; ++i)
    for(int j=0; j!=24; ++j)
      fEnergyPad[i][j] = NULL;
}
//====================================================
int mTest::End(PHCompositeNode *topNode) {
  return EVENT_OK;
}
//====================================================
mTest::~mTest() {
  if(fClustererMPC) delete fClustererMPC;
  if(fClustererEX) delete fClustererEX;
  if(fDisplay) delete fDisplay;
}
//====================================================
int mTest::Init(PHCompositeNode* top_node) {
  printf("mTest::Init\n");
  Fun4AllServer *se = Fun4AllServer::instance();
  for(int i=0; i!=16; ++i)
    for(int j=0; j!=24; ++j) {
      fEnergyPad[i][j] = new TH1F( Form("EnergyPad_LYR%02d_SEN%02d",i,j), Form("EnergyPad_LYR%02d_SEN%02d",i,j), 5000,0,5000);
      se->registerHisto( ((TH1*) (fEnergyPad[i][j]) ) );
    }
  return EVENT_OK;
}
//====================================================
int mTest::InitRun(PHCompositeNode* top_node) {
  printf("mTest::InitRun\n");
  PHMXData *mxdata = findNode::getClass<PHMXData>(top_node,"MXR");
  if(mxdata == NULL) {
    printf("mTest::InitRun exit0\n");
    exit(0);
  }
  fData = mxdata->GetData();
  fClustererMPC->Link2Data( fData );
  fClustererEX->Link2Data( fData );
  fDisplay->Link2Data( fData );
  return EVENT_OK; 
}
//====================================================
int mTest::process_event(PHCompositeNode* top_node) {
  static int nevs = 0;
  if(nevs==0) {
    fTime->Reset();
    fTime->Start();
  }
  if(nevs>5000) {
    fTime->Stop();
    float vec = nevs / fTime->RealTime();
    std::cout << " nevts per sec " << vec << std::endl;
    nevs=0;
  }
  nevs++;
  //printf("mTest::process_event\n");
  //fDisplay->DumpStats();

  //fDisplay->DumpMPCLayer(0);
  //fClustererMPC->Make_Clusters3x3(0);
  //fDisplay->DumpEXSensor(0,0);
  //fClustererEX->Make_ClustersPadRow(0);
  //fDisplay->DumpMPCLayer(1);
  //fDisplay->DumpHits();
  //fDisplay->DumpClusters();
  for(int lyr=0; lyr!=18; ++lyr) {
    if(lyr==8||lyr==17) continue;
    int nn = fData->GetNHits( lyr );
    std::vector<mxHit*> hits = fData->GetHits( lyr );
    mxHit *hitI;
    for(int i=0; i<nn; ++i) {
      hitI = hits.at(i);
      int idxI = hitI->Idx();
      int senI = fGeo->PS_Idx2Sen(idxI);
      int lyrI = fGeo->PS_Idx2Lyr(idxI);
      if(lyrI>7) lyrI = lyrI-1;
      if(lyrI<0||lyrI>15) continue;
      fEnergyPad[lyrI][senI]->Fill( hitI->Signal()*1E+6 );
    }
  }
  return EVENT_OK;
}
