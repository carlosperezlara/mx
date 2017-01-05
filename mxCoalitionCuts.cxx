//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include "mxCoalitionCuts.h"
#include "mxCoalition.h"

//========
mxCoalitionCuts::mxCoalitionCuts():
  fPS_minNHits(0),
  fPS_minSignal(0),
  fPS_minChi2Prob(0) {
  // ctor
  for(int i=0; i!=9; ++i) fHitLayer[i] = false;
}
//========
mxCoalitionCuts::~mxCoalitionCuts() {
  // dtor
}
//========
mxCoalitionCuts::mxCoalitionCuts(const mxCoalitionCuts &src) {
  // copy ctor
  fPS_minNHits = src.fPS_minNHits;
  fPS_minSignal = src.fPS_minSignal;
  fPS_minChi2Prob = src.fPS_minChi2Prob;
}
//========
mxCoalitionCuts& mxCoalitionCuts::operator=(const mxCoalitionCuts &src) {
  // asgmnt operator
  if(&src!=this) {
    fPS_minNHits = src.fPS_minNHits;
    fPS_minSignal = src.fPS_minSignal;
    fPS_minChi2Prob = src.fPS_minChi2Prob;
  }
  return *this;
}
//========
bool mxCoalitionCuts::PassesCuts(mxCoalition *coa) {
  if(fPS_minNHits > coa->NPreShower() ) return false;
  if(fPS_minSignal > coa->SignalPreShower() ) return false;
  if(fPS_minChi2Prob > coa->GetPSChi2Prob() ) return false;
  for(int i=0; i!=9; ++i)
    if(fHitLayer[i])
      if(!coa->IsHitLayer(i)) return false;
  return true;
}
