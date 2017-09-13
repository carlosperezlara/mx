#ifndef __mMPCEXCounter_HH__
#define __mMPCEXCounter_HH__

#include <fstream>
#include <map>
#include "SubsysReco.h"

class TH1F;
class TriggerHelper;
class PHCompositeNode;

class mMPCEXCounter: public SubsysReco {
 public:
  mMPCEXCounter( const char* name = "mcounter" );
  virtual ~mMPCEXCounter();

  virtual int Init(PHCompositeNode*);
  virtual int InitRun(PHCompositeNode*);
  virtual int process_event(PHCompositeNode*);
  virtual int End(PHCompositeNode *topNode);
  bool PassEventCuts(PHCompositeNode* top_node);
  void selectMPCA(PHCompositeNode* top_node,TriggerHelper *myTH);
  void selectMPCB(PHCompositeNode* top_node,TriggerHelper *myTH);
  void selectMB(PHCompositeNode* top_node,TriggerHelper *myTH);

 protected:
  TH1F *fScaleValues;
  TH1F *fTriggeredStatsMB;
  TH1F *fTriggeredStatsMPCB;
  TH1F *fTriggeredStatsMPCA;
  TH1F *fVertexMB;
  TH1F *fVertexMPCB;
  TH1F *fVertexMPCA;
};

#endif /* __mMPCEXCounter_H__ */ 
