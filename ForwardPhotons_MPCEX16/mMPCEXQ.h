#ifndef __mMPCEXQ_HH__
#define __mMPCEXQ_HH__

#include <fstream>
#include <map>
#include "SubsysReco.h"
#include "TString.h"

class qcQ;
class TH1F;
class TH2F;
class mxGeometry;
class TriggerHelper;
class PHCompositeNode;
class BbcCalib;
class BbcGeo;

class mMPCEXQ: public SubsysReco {
 public:
  mMPCEXQ( const char* name = "mcounter" );
  virtual ~mMPCEXQ();

  virtual int Init(PHCompositeNode*);
  virtual int InitRun(PHCompositeNode*);
  virtual int process_event(PHCompositeNode*);
  virtual int End(PHCompositeNode *topNode);
  bool PassEventCuts(PHCompositeNode* top_node);
  void Centrality(float m, float M) {fCentMin=m; fCentMax=M;}
  void Vertex(float m, float M) {fVertexMin=m; fVertexMax=M;}

 protected:
  std::map<int,int> fMPCIDX;
  mxGeometry *fGeo;
  BbcCalib *fBBCcalib;
  BbcGeo *fBBCgeo;
  TString fName;
  float fCentMin;
  float fCentMax;
  float fVertexMin;
  float fVertexMax;

  qcQ *fQ[9];
  qcQ *fQbbc[2];
  TH1F *fEvents;
  TH2F *fEnergy;
  TH1F *fVertex;
  TH1F *fCentrality;
  TH1F *fEXRejectedByEnergy;
  TH1F *fMPCRejectedByEnergy;
  TH1F *fEneLyr[9];
  TH2F *fXYhits[9];
  TH2F *fBbcEneTim[2];
  TH2F *fBbcXY[2];
  TH1F *fQxDist[9];
  TH1F *fQyDist[9];
  TH2F *fPsi2[9];

};

#endif /* __mMPCEXQ_H__ */ 
