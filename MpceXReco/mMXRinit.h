#ifndef __mMXRinit_HH__
#define __mMXRinit_HH__

#include <fstream>
#include <map>
#include "SubsysReco.h"
#include "TString.h"

class PHCompositeNode;
class mxCalibMaster;
class mxData;

enum kCalibMode{ kStatic = 0, kDynamic };

class mMXRinit: public SubsysReco {
 public:
  mMXRinit( const char* name = "mMXRinit" );
  virtual ~mMXRinit();

  bool PassEventCuts(PHCompositeNode*);
  virtual int Init(PHCompositeNode*);
  virtual int InitRun(PHCompositeNode*);
  virtual int process_event(PHCompositeNode*);
  virtual int End(PHCompositeNode *topNode);
  void SetSkipSouth() {fSkipSouth = true;}
  void SetSkipNorth() {fSkipNorth = true;}
  void SetRangeVertex(float a, float b) {fMinVertex=a; fMaxVertex=b;}
  void SetRangeCentrality(float a, float b) {fMinCentrality=a; fMaxCentrality=b;}
  void SetTriggerMode1(TString a) {fTrigger1=a;}
  void SetTriggerMode2(TString b) {fTrigger2=b;}
  void SetCalibMode(int v) {fCalibMode=v;}
  void SetCalibStaticLevel(int v) {fCalibStaticLevel=v;}

 protected:
  int fCalibMode;
  int fCalibStaticLevel;
  bool fSkipSouth;
  bool fSkipNorth;
  mxCalibMaster *fCal;
  mxData *fData;
  std::map<int,int> fMPCIDX;
  float fMinVertex;
  float fMaxVertex;
  float fMinCentrality;
  float fMaxCentrality;
  TString fTrigger1;
  TString fTrigger2;
};

#endif /* __mMXRinit_H__ */ 
