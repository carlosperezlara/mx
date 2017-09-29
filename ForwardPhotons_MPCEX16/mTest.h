#ifndef __mTest_HH__
#define __mTest_HH__

#include "SubsysReco.h"

class TStopwatch;
class mxData;
class mxDisplay;
class mxClustererMPC;
class mxClustererEX;
class mxGeometry;
class TH1F;

class mTest: public SubsysReco {
 public:
  mTest( const char* name = "mTest" );
  virtual ~mTest();

  virtual int Init(PHCompositeNode*);
  virtual int InitRun(PHCompositeNode*);
  virtual int process_event(PHCompositeNode*);
  virtual int End(PHCompositeNode *topNode);

 protected:
  TStopwatch *fTime;
  mxData *fData;
  mxClustererMPC *fClustererMPC;
  mxClustererEX *fClustererEX;
  mxDisplay *fDisplay;
  mxGeometry *fGeo;

  TH1F *fEnergyPad[16][24];
};

#endif /* __mTest_H__ */ 
