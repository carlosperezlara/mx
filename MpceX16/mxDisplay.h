//=========================
// written by Carlos Perez 
//=========================
#ifndef __mxDisplay_HH__
#define __mxDisplay_HH__

class mxGeometry;
class mxData;

class mxDisplay {
 public:
  mxDisplay();
  ~mxDisplay();
  inline void Link2Data(mxData *dat) {fData = dat;}

  void DumpStats();
  void DumpHits();
  void DumpClusters();
  void DumpCandidates(int lvl=1);
  void DumpPreEventCandidates(int lvl=1);
  void DumpDecayCandidates();
  void DumpMPCLayer(int);
  void DumpEXSensor(int,int);

 private:
  mxGeometry *fGeo;
  mxData *fData;
};

#endif /* __mxDisplay_H__ */ 
