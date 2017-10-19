//=========================
// written by Carlos Perez 
//=========================
#include <iostream>
#include <vector>
#include <algorithm>
#include "TString.h"
#include "mxHit.h"
#include "mxCluster.h"
#include "mxCandidate.h"
#include "mxDecayCandidate.h"
#include "mxGeometry.h"
#include "mxData.h"

//========
mxData::mxData() {
  // ctor
  std::cout << "mxData:: ctor" << std::endl;
  for(int i=0; i!=18; ++i) {
    fNHit[i] = 0;
    fNPty[i] = 0;
  }
  for(int i=0; i!=2; ++i) {
    fNCoa[i] = 0;
    fNCoaPreEvent[i] = 0;
    fNUni[i] = 0;
  }
  fV[0] = 0.;
  fV[1] = 0.;
  fV[2] = 0.;
  fGeo = mxGeometry::Instance();
}
//========
void mxData::Reset() {
  ClearHits();
  ClearClusters();
  ClearCandidates();
  ClearDecayCandidates();
  fV[0] = 0.;
  fV[1] = 0.;
  fV[2] = 0.;
}
//========
mxData::~mxData() {
  // dtor
  for(int i=0; i!=18; ++i) {
    for(unsigned int j=0; j!=fHit[i].size(); ++j){
      delete fHit[i].at(j);
    }
    for(unsigned int j=0; j!=fPty[i].size(); ++j)
      delete fPty[i].at(j);
  }
  for(int i=0; i!=2; ++i) {
    for(unsigned int j=0; j!=fCoa[i].size(); ++j)
      delete fCoa[i].at(j);
    for(unsigned int j=0; j!=fCoaPreEvent[i].size(); ++j)
      delete fCoaPreEvent[i].at(j);
    for(unsigned int j=0; j!=fUni[i].size(); ++j)
      delete fUni[i].at(j);
  }
}
//========
float mxData::GetMultiplicity(int lyr) {
  float mult = 0;
  for(int mh=0; mh!=fNHit[lyr]; ++mh) {
    mxHit *hit = (mxHit*) fHit[lyr].at( mh );
    mult += hit->Signal();
  }
  return mult;
}
//========
void mxData::Fill(int idx, float sgn) {
  // filler
  int lyridx = fGeo->LyrIdx( idx );
  mxHit *hit = PushNewHit(lyridx);
  hit->Fill( idx, sgn );
}
//========
void mxData::SaveCandidatesToPreEvent() {
  mxCandidate *coaI;
  for(int arm=0; arm!=2; ++arm) {
    for(int mi=0; mi!=fNCoa[arm]; ++mi) {
      coaI = (mxCandidate*) fCoa[arm].at( mi );
      mxCandidate *coaJ = PushNewCandidatePreEvent(arm);
      coaJ->CopyFrom( coaI );
    }
  }
}
//========
mxHit* mxData::PushNewHit(int lyr) {
  mxHit *ret;
  if(fNHit[lyr]<int(fHit[lyr].size()))
    ret = (mxHit*) fHit[lyr].at( fNHit[lyr] );
  else {
    ret = new mxHit();
    fHit[lyr].push_back( ret );
  }
  ret->Reset();
  ++fNHit[lyr];
  return ret;
}
//========
mxCluster* mxData::PushNewCluster(int lyr) {
  mxCluster *ret;
  if(fNPty[lyr]<int(fPty[lyr].size()))
    ret = (mxCluster*) fPty[lyr].at( fNPty[lyr] );
  else {
    ret = new mxCluster();
    fPty[lyr].push_back( ret );
  }
  ret->Reset();
  ++fNPty[lyr];
  return ret;
}
//========
mxCandidate* mxData::PushNewCandidate(int arm) {
  mxCandidate *ret;
  if(fNCoa[arm]<int(fCoa[arm].size()))
    ret = (mxCandidate*) fCoa[arm].at( fNCoa[arm] );
  else {
    ret = new mxCandidate();
    fCoa[arm].push_back( ret );
  }
  ret->Reset();
  ++fNCoa[arm];
  return ret;
}
//========
mxCandidate* mxData::PushNewCandidatePreEvent(int arm) {
  mxCandidate *ret;
  if(fNCoaPreEvent[arm]<int(fCoaPreEvent[arm].size()))
    ret = (mxCandidate*) fCoaPreEvent[arm].at( fNCoaPreEvent[arm] );
  else {
    ret = new mxCandidate();
    fCoaPreEvent[arm].push_back( ret );
  }
  ret->Reset();
  ++fNCoaPreEvent[arm];
  return ret;
}
//========
mxDecayCandidate* mxData::PushNewDecayCandidate(int arm) {
  mxDecayCandidate *ret;
  if(fNUni[arm]<int(fUni[arm].size()))
    ret = (mxDecayCandidate*) fUni[arm].at( fNUni[arm] );
  else {
    ret = new mxDecayCandidate();
    fUni[arm].push_back( ret );
  }
  ret->Reset();
  ++fNUni[arm];
  return ret;
}
