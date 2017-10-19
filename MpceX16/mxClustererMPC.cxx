//=========================
// written by Carlos Perez 
//=========================
#include <iostream>
#include "mxGeometry.h"
#include "mxData.h"
#include "mxClustererMPC.h"

struct GreaterSignal
{
  template<class T>
  bool operator()(T const &a, T const &b) const { return a->Signal() > b->Signal(); }
};


mxClustererMPC::mxClustererMPC() :  mxClusterer() {
}
//===============
mxClustererMPC::~mxClustererMPC() {
}
//===============
void mxClustererMPC::Make_Clusters5Cross(int arm) {
  if(arm!=0&&arm!=1) return;
  const int nDIM = 4;
  int lyr = 8+arm*9;
  int nn = fData->GetNHits( lyr );
  std::vector<mxHit*> hits = fData->GetHits( lyr );
  int n8[nDIM];
  mxHit *hitI, *hitJ;
  for(int i=0; i<nn; ++i) {
    hitI = hits.at(i);
    int idxI = hitI->Idx();
    fGeo->PbWO4_GetNeighbours(idxI, n8);
    // 1: check if neighbours are possible
    bool no8=true;
    for(int k=0; k!=nDIM; ++k) if(n8[k]!=-1) no8 = false;
    if(no8) continue;
    fParty->Reset();
    fParty->Fill( hitI, fGeo->X(idxI), fGeo->Y(idxI) );
    // 2: check if idxI is max
    bool isM=true;
    for(int j=0; j<nn; ++j) {
      if(i==j) continue;
      hitJ = hits.at(j);
      int idxJ = hitJ->Idx();
      for(int k=0; k!=nDIM; ++k) {
	if(idxJ!=n8[k]) continue;
	if( hitJ->Signal() > hitI->Signal() ) {
	  isM = false;
	  break;
	}
	fParty->Fill( hitJ, fGeo->X(idxJ), fGeo->Y(idxJ) );
      }
    }
    if(!isM) continue;
    // 3: add to clusters
    mxCluster *addclu = fData->PushNewCluster(lyr);
    addclu->CopyFrom( fParty );
  }
}
//===============
void mxClustererMPC::Make_Clusters9Square(int arm) {
  if(arm!=0&&arm!=1) return;
  const int nDIM = 4;
  int lyr = 8+arm*9;
  int nn = fData->GetNHits( lyr );
  //std::cout << " " << nn << std::endl;
  std::vector<mxHit*> hits = fData->GetHits( lyr );
  //std::sort(hits.begin(),hits.begin()+nn,GreaterSignal());
  int n8[nDIM];
  mxHit *hitI, *hitJ;
  for(int i=0; i<nn; ++i) {
    hitI = hits.at(i);
    int idxI = hitI->Idx();
    //std::cout << "I " << idxI-49152 << " || S " << hitI->Signal() << std::endl;
    fGeo->PbWO4_GetNeighbours(idxI, n8);
    // 1: check if neighbours are possible
    bool no8=true;
    for(int k=0; k!=nDIM; ++k) if(n8[k]!=-1) no8 = false;
    if(no8) {
      //std::cout << "  (rejected by acceptance)" << std::endl;
      continue;
    }
    fParty->Reset();
    fParty->Fill( hitI, fGeo->X(idxI), fGeo->Y(idxI) );
    // 2: check if idxI is max
    bool isM=true;
    for(int j=0; j<nn; ++j) {
      if(i==j) continue;
      hitJ = hits.at(j);
      int idxJ = hitJ->Idx();
      for(int k=0; k!=nDIM; ++k) {
	if(idxJ!=n8[k]) continue;
	if( hitJ->Signal() > hitI->Signal() ) {
	  isM = false;
	  break;
	}
	//std::cout << " -> I " << idxJ-49152 << " || S " << hitJ->Signal() << std::endl;
	fParty->Fill( hitJ, fGeo->X(idxJ), fGeo->Y(idxJ) );
      }
    }
    if(!isM) {
      //std::cout << "  (rejected by not maximum)" << std::endl;
      continue;
    }
    // 3: add to clusters
    mxCluster *addclu = fData->PushNewCluster(lyr);
    addclu->CopyFrom( fParty );
  }
}
