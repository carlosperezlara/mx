#ifndef __mMPCTunner_HH__
#define __mMPCTunner_HH__

#include <map>

#include <SubsysReco.h>
#include <TString.h>

class TList;
class TFile;
class TTree;
class mxEventCuts;

class mMPCTunner: public SubsysReco {
 public:
  mMPCTunner( const char* name = "mMPCTunner" );
  virtual ~mMPCTunner();

  virtual int Init(PHCompositeNode*);
  virtual int InitRun(PHCompositeNode*);
  virtual int process_event(PHCompositeNode*);
  virtual int End(PHCompositeNode *topNode);

 protected:
  void PrepareTree(TString name);
  void WriteTree();

  mxEventCuts *fEventCuts;
  TFile *fDSTfile;
  TTree *fTree;
  Float_t fVertexZ;
  Float_t fBBCS;
  Float_t fBBCN;
  UChar_t fTrigger;
  std::map<int,int> fMPCIDX;
  Float_t fMPCAdc[416];
};

#endif /* __mMPCTunner_H__ */ 
