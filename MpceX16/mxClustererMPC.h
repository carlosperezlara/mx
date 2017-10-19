//=========================
// written by Carlos Perez 
//=========================
#ifndef __mxClustererMPC_HH__
#define __mxClustererMPC_HH__

#include "mxClusterer.h"
class mxGeometry;
class mxData;

class mxClustererMPC : public mxClusterer {
 public:
  mxClustererMPC();
  virtual ~mxClustererMPC();

  void Make_Clusters5Cross(int);
  void Make_Clusters9Square(int);

};

#endif /* __mxClustererMPC_H__ */ 
