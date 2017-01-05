//=========================
// written by Carlos Perez
// 2015-2016
//=========================
#ifndef __mxCoalitionCuts_HH__
#define __mxCoalitionCuts_HH__

class mxCoalition;

class mxCoalitionCuts {
 public:
  mxCoalitionCuts();
  mxCoalitionCuts(const mxCoalitionCuts&);
  mxCoalitionCuts& operator=(const mxCoalitionCuts&);
  virtual ~mxCoalitionCuts();
  bool PassesCuts( mxCoalition* );
  void Set_PS_minNHits(int v) {fPS_minNHits=v;}
  void Set_PS_minSignal(float v) {fPS_minSignal=v;}
  void Set_HitLayer(int v) {fHitLayer[v]=true;}
  void Set_PS_minChi2Prob(float v) {fPS_minChi2Prob=v;}

 protected:
  int fPS_minNHits;
  float fPS_minSignal;
  float fPS_minChi2Prob;
  bool fHitLayer[9];
};
#endif /* __mxCoalitionCuts_H__ */
