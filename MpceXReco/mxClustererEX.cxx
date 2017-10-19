//=========================
// written by Carlos Perez 
//=========================
#include <iostream>
#include <vector>
#include "mxGeometry.h"
#include "mxData.h"
#include "mxHit.h"
#include "mxClustererEX.h"

struct GreaterSignal
{
  template<class T>
  bool operator()(T const &a, T const &b) const { return a->Signal() > b->Signal(); }
};


mxClustererEX::mxClustererEX() :  mxClusterer() {
}
//===============
mxClustererEX::~mxClustererEX() {
}
//===============
void mxClustererEX::Make_ClustersPadRow(int lyr) {
  if(lyr==8||lyr==17) return;
  int nn = fData->GetNHits( lyr );
  std::vector<mxHit*> hits = fData->GetHits( lyr );
  // 1: inflate layer info
  mxHit *sensor[24][4][32];
  for(int i=0; i!=24; ++i)
    for(int j=0; j!=4; ++j)
      for(int k=0; k!=32; ++k)
	sensor[i][j][k] = NULL;
  mxHit *hitI;
  for(int i=0; i<nn; ++i) {
    hitI = hits.at(i);
    int idxI = hitI->Idx();
    int senI = fGeo->PS_Idx2Sen(idxI);
    int rowI = fGeo->PS_Idx2Row(idxI);
    int colI = fGeo->PS_Idx2Col(idxI);
    sensor[senI][rowI][colI] = hitI;
  }
  // 2: sending the information to clusterer
  for(int senI=0; senI!=24; ++senI)
    Make_ClustersPadRowSensor(lyr,sensor[senI]);
}
//===============
void mxClustererEX::Make_ClustersPadRowSensor(int lyr, mxHit* hit[4][32]) {
  //std::cout << "Make_ClustersPadRowSensor" << std::endl;
  for(int rowI=0; rowI!=4; ++rowI) {
    float col[32];
    for(int colI=0; colI!=32; ++colI) {
      col[colI] = 0;
      if(hit[rowI][colI])
	col[colI] = hit[rowI][colI]->Signal();
      //if(bad) col[colI] = -1;
    }
    //std::cout << " row " << rowI << std::endl;
    // 1: finding leaders
    float thr = 0;
    // 1.1: seeking maximum
    for(int colI=0; colI!=32; ++colI) {
      if(col[colI]>thr) thr = col[colI];
    }
    // 1.2: rejecting no talent
    float kLeader_MinimumEnergy = 1e-6;
    if(thr<kLeader_MinimumEnergy) continue;
    // 1.3: identifying leaders
    float kLeader_ConstantFraction = 0.3;
    thr *= kLeader_ConstantFraction;
    std::vector<int> leader;
    for(int colI=0; colI!=32; ++colI) {
      if(col[colI]>thr) leader.push_back(colI);
    }
    //std::cout << "  found " << leader.size() << " leaders" << std::endl;
    // 2: party around leader
    std::vector<int> boundaries;
    boundaries.push_back(0);
    // 2.1: determine boundaries for party
    unsigned int nleaders = leader.size();
    if(nleaders>1) {
      for(unsigned int l=0; l<nleaders-1; ++l) {
	int L0 = leader[l];
	int L1 = leader[l+1];
	if( (L1-L0)==1 ) continue;
	float E0 = col[L0];
	float E1 = col[L1];
	int delta = (L1-L0)*E0/(E0+E1); //weighted delta
	if(delta==0) delta=1; //minimum comvention for ranges [,)
	boundaries.push_back( L0 + delta );
      }
    }
    boundaries.push_back(32);
    //std::cout << "  made " << boundaries.size() << " boundaries" << std::endl;
    // 2.2: pushing parties to fData
    for(unsigned int b=0; b!=boundaries.size()-1; ++b) {
      mxCluster *clu = fData->PushNewCluster(lyr);
      //std::cout << "  -> cluster " << std::endl;
      for(int h=boundaries[b]; h!=boundaries[b+1]; ++h) {
	if(!hit[rowI][h]) continue;
	int idx = hit[rowI][h]->Idx();
	//std::cout << "    * " << idx << ": " << hit[rowI][h]->Signal() << std::endl;
	clu->Fill( hit[rowI][h], fGeo->X(idx), fGeo->Y(idx) );
      }
    }
  }
}
