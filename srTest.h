#ifndef __srTest_HH__
#define __srTest_HH__

#include "SubsysReco.h"

class PHCompositeNode;

class srTest: public SubsysReco {
 public:
  srTest( const char* name = "srTest" );
  virtual ~srTest();

  virtual int Init(PHCompositeNode*);
  virtual int InitRun(PHCompositeNode*);
  virtual int process_event(PHCompositeNode*);
  virtual int End(PHCompositeNode *topNode);
};

#endif /* __srTest_H__ */ 
