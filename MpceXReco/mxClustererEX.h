//=========================
// written by Carlos Perez 
//=========================
#ifndef __mxClustererEX_HH__
#define __mxClustererEX_HH__

#include "mxClusterer.h"
class mxGeometry;
class mxHit;
class mxData;

class mxClustererEX : public mxClusterer {
 public:
  mxClustererEX();
  virtual ~mxClustererEX();

  void Make_ClustersPadRow(int);
  void Make_ClustersPadRowSensor(int,mxHit* a[4][32]);

};

#endif /* __mxClustererEX_H__ */ 
