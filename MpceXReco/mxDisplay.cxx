//=========================
// written by Carlos Perez 
//=========================
#include <iostream>
#include <vector>
#include "TString.h"
#include "mxHit.h"
#include "mxCluster.h"
#include "mxCandidate.h"
#include "mxDecayCandidate.h"
#include "mxGeometry.h"
#include "mxData.h"
#include "mxDisplay.h"

//========
mxDisplay::mxDisplay() {
  fData = NULL;
  fGeo = mxGeometry::Instance();
}
//========
mxDisplay::~mxDisplay() {
}
//========
void mxDisplay::DumpHits() {
  std::cout << "=============================" << std::endl;
  std::cout << "mxDisplay::DumpHits" << std::endl;
  mxHit *hit;
  int nn=0;
  for(int i=0; i!=18; ++i) {
    int n = fData->GetNHits(i);
    std::cout << "  Layer " << i << " || Nhits " << n << std::endl;
    nn += n;
    std::vector<mxHit*> hits = fData->GetHits(i);
    for(int j=0; j!=n; ++j) {
      hit = hits.at(j);
      std::cout << "  ||" << j << "|| index "  << hit->Idx() << " signal " << hit->Signal() << std::endl;
    }
  }
  std::cout << "Total Hits: " << nn << std::endl;
  std::cout << "=============================" << std::endl;
}
//========
void mxDisplay::DumpEXSensor(int lyr,int sen) {
  if(lyr==8||lyr==17) return;
  int   aidx[4][32] = {{0}};
  float asgn[4][32] = {{0}};
  int nn;
  mxHit *hit;
  nn = fData->GetNHits(lyr);
  std::vector<mxHit*> hits = fData->GetHits(lyr);
  for(int i=0; i!=nn; ++i) {
    hit = hits.at(i);
    int idxI = hit->Idx();
    int senI = fGeo->PS_Idx2Sen(idxI);
    if(sen!=senI) continue;
    int row = fGeo->PS_Idx2Row(idxI);
    int col = fGeo->PS_Idx2Col(idxI);
    aidx[row][col] = idxI;
    asgn[row][col] = hit->Signal();
  }
  std::cout << "############################################" << std::endl;
  std::cout << Form("########  LYR %02d  ########  SEN %02d  ########", lyr, sen) << std::endl;
  int rord[4] = {1,0,2,3};
  for(int r=0; r!=4; ++r) {
    for(int c=0; c!=32; ++c)
      std::cout << Form("|%05d", aidx[rord[r]][c]);
    std::cout << "|" << std::endl;
    for(int c=0; c!=32; ++c)
      std::cout << Form("|%5.0f", asgn[rord[r]][c]*1e+6);
    std::cout << "|" << std::endl;
  }
  std::cout << "############################################" << std::endl;
}
//========
void mxDisplay::DumpMPCLayer(int arm) {
  int inikey = 0;
  int finkey = 196;
  if(arm>0) {
    inikey = 196;
    finkey = 416;
    return;
  }
  float asgn[416] = {0};
  int nn;
  mxHit *hit;
  nn = fData->GetNHits(8+arm*9);
  std::vector<mxHit*> hits = fData->GetHits(8+arm*9);
  for(int i=0; i!=nn; ++i) {
    hit = hits.at(i);
    std::cout << hit->Idx() << "->" << hit->Idx()-49152 << " ";
    std::cout << hit->Signal() << std::endl;
    asgn[ hit->Idx()-49152 ] = hit->Signal();
  }
  int idx[18][18];
  float sgn[18][18];
  for(int iy=0; iy!=18; ++iy) {
    for(int ix=0; ix!=18; ++ix) {
      idx[ix][iy] = -1;
      sgn[ix][iy] = -1;
    }
  }
  for(int i=inikey; i!=finkey; ++i) {
    float x = fGeo->X(49152 + i);
    float y = fGeo->Y(49152 + i);
    // works for south, not for north
    int ix = (x+19.7)/2.2;
    int iy = (y+19.5)/2.2;
    idx[ix][17-iy] = i;
    sgn[ix][17-iy] = asgn[i];
  }
  for(int iy=0; iy!=18; ++iy) {
    std::cout << std::endl;
    for(int ix=0; ix!=18; ++ix) {
      if(idx[ix][iy]<0) {
	std::cout << "|########|";
      } else {
	std::cout << Form("|%03d:",idx[ix][iy]);
	if(sgn[ix][iy]>0.00) std::cout << Form(" %3.0f|",sgn[ix][iy]);
	else std::cout << "    |";
      }
    }
  }
  std::cout << std::endl;
}
//========
void mxDisplay::DumpClusters() {
  std::cout << "=============================" << std::endl;
  std::cout << "mxDisplay::DumpClusters" << std::endl;
  int nn=0;
  mxCluster *pty;
  mxHit *hit;
  for(int i=0; i!=18; ++i) {
    int n = fData->GetNClusters(i);
    nn += n;
    std::cout << "  Layer " << i << " || Nparties " << n << std::endl;
    std::vector<mxCluster*> ptys = fData->GetClusters(i);
    for(int j=0; j!=n; ++j) {
      pty = ptys.at(j);
      std::cout << "    ||" << j << "|| x y "  <<pty->GetX() << " " << pty->GetY() << " || hits " << pty->N() << " || sgn " << pty->Signal();
      std::cout << " || cx cy cxy " << pty->GetCov(0) << " " << pty->GetCov(1) << " " << pty->GetCov(2);
      std::cout << " || spx spy " << pty->GetSpreadX() << " " << pty->GetSpreadY() << std::endl;
      int m = pty->N();
      for(int k=0; k!=m; ++k) {
      	hit = pty->GetHit(k);
      	if(!hit) continue;
      	float x = fGeo->X(hit->Idx());
      	float y = fGeo->Y(hit->Idx());
      	std::cout << "       ||" << k << "|| key x y " << hit->Idx() <<" "<< x << " " << y << " || sgn " << hit->Signal() << std::endl;
      }
    }
  }
  std::cout << "Total Parties: " << nn << std::endl;
  std::cout << "=============================" << std::endl;
}
//========
void mxDisplay::DumpCandidates(int lvl) {
  std::cout << "================================" << std::endl;
  std::cout << "mxDisplay::DumpCandidates" << std::endl;
  mxCandidate *coa;
  mxCluster *pty;
  mxHit *hit;
  for(int i=0; i!=2; ++i) {
    int n = fData->GetNCandidates(i);
    std::cout << "  Arm " << i << " || Ncoalitions " << n << std::endl;
    std::vector<mxCandidate*> coas = fData->GetCandidates(i);
    for(int j=0; j!=n; ++j) {
      coa = coas.at(j);
      std::cout << "    ||" << j << "|| phi theta "  << coa->GetPhi() << " " << coa->GetTheta() << " || pties " << coa->N() << " || sgn " << coa->Signal();
      std::cout << " || cphi ctheta cphitheta " << coa->GetCov(0) << " " << coa->GetCov(1) << " " << coa->GetCov(2);
      std::cout << " || pschi2prob " << coa->GetPSChi2Prob();
      std::cout << std::endl;
      if(lvl<2) continue;
      for(int k=0; k!=9; ++k) {
      	pty = coa->GetCluster(k);
      	std::cout << "       ||" << k;
      	if(!pty) {
      	  std::cout << std::endl;
      	  continue;
      	}
      	float x = pty->GetX();
      	float y = pty->GetY();
      	std::cout << "|| x y " << x << " " << y << " || sgn " << pty->Signal() << " || hits " << pty->N() << std::endl;
	if(lvl<3) continue;
        for(int l=0; l!=pty->N(); ++l) {
          hit = pty->GetHit(l);
          std::cout << "         ||" << l;
          if(!hit) {
            std::cout << std::endl;
            continue;
          }
          std::cout << "|| idx sgn " << hit->Idx() << " " << hit->Signal() << std::endl;
        }
      }
    }
  }
  std::cout << "=============================" << std::endl;
}
//========
void mxDisplay::DumpPreEventCandidates(int lvl) {
  std::cout << "========================================" << std::endl;
  std::cout << "mxDisplay::DumpPreEventCandidates" << std::endl;
  mxCandidate *coa;
  for(int i=0; i!=2; ++i) {
    int n = fData->GetNCandidatesPreEvent(i);
    std::cout << "  Arm " << i << " || Ncoalitions " << n << std::endl;
    std::vector<mxCandidate*> coas = fData->GetCandidatesPreEvent(i);
    for(int j=0; j!=n; ++j) {
      coa = coas.at(j);
      std::cout << "    ||" << j << "|| phi theta "  << coa->GetPhi() << " " << coa->GetTheta() << " || pties " << coa->N() << " || sgn " << coa->Signal();
      std::cout << " || cphi ctheta cphitheta " << coa->GetCov(0) << " " << coa->GetCov(1) << " " << coa->GetCov(2);
      std::cout << " || pschi2prob " << coa->GetPSChi2Prob();
      std::cout << std::endl;
    }
  }
  std::cout << "=============================" << std::endl;
}
//========
void mxDisplay::DumpDecayCandidates() {
  std::cout << "============================" << std::endl;
  std::cout << "mxDisplay::DumpDecayCandidates" << std::endl;
  mxDecayCandidate *uni;
  mxCandidate *coa1, *coa2;
  for(int i=0; i!=2; ++i) {
    int n = fData->GetNDecayCandidates(i);
    std::cout << "  Arm " << i << " || Nunions " << n << std::endl;
    std::vector<mxDecayCandidate*> unis = fData->GetDecayCandidates(i);
    for(int j=0; j!=n; ++j) {
      uni = unis.at(j);
      std::cout << "    ||" << j << "|| ene asym mass "  << uni->GetEnergy() << " " << uni->GetEnergyAsymmetry() << " " << uni->GetMass() << std::endl;
      coa1 = uni->GetCandidate(0);
      coa2 = uni->GetCandidate(1);
      std::cout << "       | ene1 ene2 " << coa1->GetEnergy() << " " << coa2->GetEnergy() << std::endl;
    }
  }
  std::cout << "=============================" << std::endl;
}
//========
void mxDisplay::DumpStats() {
  int n;
  std::cout << "===========================" << std::endl;
  std::cout << "mxDisplay::DumpStats" << std::endl;
  std::cout << "  Hit:"; n=0;
  for(int i=0; i!=18; ++i) {
    n += fData->GetNHits(i);
    std::cout << " " << fData->GetNHits(i);
    if(i==8) std::cout << "  | ";
  } std::cout << "  || Tot: " << n << std::endl;
  std::cout << "  Pty:"; n=0;
  for(int i=0; i!=18; ++i) {
    n += fData->GetNClusters(i);
    std::cout << " " << fData->GetNClusters(i);
    if(i==8) std::cout << "  | ";
  } std::cout << "  || Tot: " << n << std::endl;
  std::cout << "  Coa:"; n=0;
  for(int i=0; i!=2; ++i) {
    n += fData->GetNCandidates(i);
    std::cout << " " << fData->GetNCandidates(i);
  }  std::cout << " || Tot: " << n << std::endl;
  std::cout << "===========================" << std::endl;
}
