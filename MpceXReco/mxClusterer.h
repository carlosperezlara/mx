//=========================
// written by Carlos Perez 
//=========================
#ifndef __mxClusterer_HH__
#define __mxClusterer_HH__

class mxGeometry;
class mxData;
class mxCluster;

class mxClusterer {
 public:
  mxClusterer();
  virtual ~mxClusterer();
  void Link2Data(mxData *dat) {fData=dat;}

 protected:
  mxGeometry *fGeo;
  mxData *fData;
  mxCluster *fParty;
};

#endif /* __mxClusterer_H__ */ 
