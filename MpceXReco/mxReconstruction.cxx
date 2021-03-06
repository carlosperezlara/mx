//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include <iostream>
#include <vector>
#include <algorithm>
#include "phMath.h"

#include "TString.h"

#include "mxGeometry.h"
#include "mxHit.h"
#include "mxCluster.h"
#include "mxCandidate.h"
#include "mxDecayCandidate.h"
#include "mxReconstruction.h"

struct GreaterSignal
{
  template<class T>
  bool operator()(T const &a, T const &b) const { return a->Signal() > b->Signal(); }
};

struct LowerKey
{
  template<class T>
  bool operator()(T const &a, T const &b) const { return a->Idx() < b->Idx(); }
};

struct MostContributors
{
  template<class T>
  bool operator()(T const &a, T const &b) const { return a->N() > b->N(); }
};

//========
mxReconstruction::mxReconstruction() :
  fDebug(0),
  fPtyAlg(0),
  fCoaAlg(0),
  fPtyAlgPadRow_thr(0.15),
  fPtyAlgMPCBreaker_thr(0.10),
  fPtyAlgMPCBreaker_NCrystals(1),
  fCoaAlgSeed6_nc(1) {
  // ctor
  std::cout << "mxReconstruction:: ctor" << std::endl;
  for(int i=0; i!=18; ++i) {
    fNHit[i] = 0;
    fNPty[i] = 0;
  }
  for(int i=0; i!=2; ++i) {
    fNCoa[i] = 0;
    fNCoaPreEvent[i] = 0;
    fNUni[i] = 0;
  }
  fGeo = mxGeometry::Instance();
  fV[0] = 0.;
  fV[1] = 0.;
  fV[2] = 0.;
}
//========
void mxReconstruction::Reset() {
  for(int i=0; i!=18; ++i) {
    fNHit[i] = 0;
    fNPty[i] = 0;
  }
  for(int i=0; i!=2; ++i) {
    fNCoa[i] = 0;
    fNUni[i] = 0;
  }
  fV[0] = 0.;
  fV[1] = 0.;
  fV[2] = 0.;
}
//========
mxReconstruction::~mxReconstruction() {
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
    for(unsigned int j=0; j!=fUni[i].size(); ++j)
      delete fUni[i].at(j);
  }
}
//========
void mxReconstruction::DumpHits() {
  std::cout << "=============================" << std::endl;
  std::cout << "mxReconstruction::DumpHits" << std::endl;
  mxHit *hit;
  int nn=0;
  for(int i=0; i!=18; ++i) {
    int n = fNHit[i];
    std::cout << "  Layer " << i << " || Nhits " << n << std::endl;
    nn += n;
    for(int j=0; j!=n; ++j) {
      hit = fHit[i].at(j);
      std::cout << "  ||" << j << "|| index "  << hit->Idx() << " signal " << hit->Signal() << std::endl;
    }
  }
  std::cout << "Total Hits: " << nn << std::endl;
  std::cout << "=============================" << std::endl;
}
//========
void mxReconstruction::DumpMPCLayer(int lyr) {
  int inikey = 0;
  int finkey = 196;
  if(lyr>0) {
    inikey = 196;
    finkey = 416;
  }
  float asgn[416] = {0};
  int nn;
  mxHit *hit;
  nn = fNHit[8+lyr*9];
  for(int i=0; i!=nn; ++i) {
    hit = fHit[8+lyr*9].at(i);
    //std::cout << hit->Idx() << " ";
    //std::cout << hit->Signal() << std::endl;
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
	std::cout << "|##########|";
      } else {
	std::cout << Form("|%03d: ",idx[ix][iy]);
	if(sgn[ix][iy]>0.00) std::cout << Form(" %4.2f|",sgn[ix][iy]);
	else std::cout << "     |";
      }
    }
  }
  std::cout << std::endl;
}
//========
void mxReconstruction::DumpParties() {
  std::cout << "=============================" << std::endl;
  std::cout << "mxReconstruction::DumpParties" << std::endl;
  int nn=0;
  mxCluster *pty;
  mxHit *hit;
  for(int i=0; i!=18; ++i) {
    int n = fNPty[i];
    nn += n;
    std::cout << "  Layer " << i << " || Nparties " << n << std::endl;
    for(int j=0; j!=n; ++j) {
      pty = fPty[i].at(j);
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
void mxReconstruction::DumpCandidates(int lvl) {
  std::cout << "================================" << std::endl;
  std::cout << "mxReconstruction::DumpCandidates" << std::endl;
  mxCandidate *coa;
  mxCluster *pty;
  mxHit *hit;
  for(int i=0; i!=2; ++i) {
    int n = fNCoa[i];
    std::cout << "  Arm " << i << " || Ncoalitions " << n << std::endl;
    for(int j=0; j!=n; ++j) {
      coa = fCoa[i].at(j);
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
void mxReconstruction::DumpPreEventCandidates(int lvl) {
  std::cout << "========================================" << std::endl;
  std::cout << "mxReconstruction::DumpPreEventCandidates" << std::endl;
  mxCandidate *coa;
  mxCluster *pty;
  mxHit *hit;
  for(int i=0; i!=2; ++i) {
    int n = fNCoaPreEvent[i];
    std::cout << "  Arm " << i << " || Ncoalitions " << n << std::endl;
    for(int j=0; j!=n; ++j) {
      coa = fCoaPreEvent[i].at(j);
      std::cout << "    ||" << j << "|| phi theta "  << coa->GetPhi() << " " << coa->GetTheta() << " || pties " << coa->N() << " || sgn " << coa->Signal();
      std::cout << " || cphi ctheta cphitheta " << coa->GetCov(0) << " " << coa->GetCov(1) << " " << coa->GetCov(2);
      std::cout << " || pschi2prob " << coa->GetPSChi2Prob();
      std::cout << std::endl;
    }
  }
  std::cout << "=============================" << std::endl;
}
//========
void mxReconstruction::DumpUnions() {
  std::cout << "============================" << std::endl;
  std::cout << "mxReconstruction::DumpUnions" << std::endl;
  mxDecayCandidate *uni;
  mxCandidate *coa1, *coa2;
  for(int i=0; i!=2; ++i) {
    int n = fNUni[i];
    std::cout << "  Arm " << i << " || Nunions " << n << std::endl;
    for(int j=0; j!=n; ++j) {
      uni = fUni[i].at(j);
      std::cout << "    ||" << j << "|| ene asym mass "  << uni->GetEnergy() << " " << uni->GetEnergyAsymmetry() << " " << uni->GetMass() << std::endl;
      coa1 = uni->GetCandidate(0);
      coa2 = uni->GetCandidate(1);
      std::cout << "       | ene1 ene2 " << coa1->GetEnergy() << " " << coa2->GetEnergy() << std::endl;
    }
  }
  std::cout << "=============================" << std::endl;
}
//========
void mxReconstruction::DumpStats() {
  int n;
  std::cout << "===========================" << std::endl;
  std::cout << "mxReconstruction::DumpStats" << std::endl;
  std::cout << "  Hit:"; n=0;
  for(int i=0; i!=18; ++i) {
    n += fNHit[i];
    std::cout << " " << fNHit[i];
  } std::cout << " || Tot: " << n << std::endl;
  std::cout << "  Pty:"; n=0;
  for(int i=0; i!=18; ++i) {
    n += fNPty[i];
    std::cout << " " << fNPty[i];
  } std::cout << " || Tot: " << n << std::endl;
  std::cout << "  Coa:"; n=0;
  for(int i=0; i!=2; ++i) {
    n += fNCoa[i];
    std::cout << " " << fNCoa[i];
  }  std::cout << " || Tot: " << n << std::endl;
  std::cout << "===========================" << std::endl;
}
//========
float mxReconstruction::GetMultiplicity(int lyr) {
  float mult = 0;
  for(int mh=0; mh!=fNHit[lyr]; ++mh) {
    mxHit *hit = (mxHit*) fHit[lyr].at( mh );
    mult += hit->Signal();
  }
  return mult;
}
//========
void mxReconstruction::Fill(int idx, float sgn) {
  // filler
  int lyridx = fGeo->LyrIdx( idx );
  int nmax = fHit[lyridx].size();
  mxHit *hit;
  if(fNHit[lyridx]>nmax-1) {
    hit = new mxHit();
    fHit[lyridx].push_back( hit );
  } else hit = fHit[lyridx].at( fNHit[lyridx] );
  hit->Fill( idx, sgn );
  fNHit[lyridx]++;
}
//========
void mxReconstruction::Make() {
  // maker
  //DumpHits();
  Parties();
  //DumpParties();
  Candidates();
  //DumpCandidates();
  Unions();
}
//========
void mxReconstruction::Parties() {
  mxHit *hit;
  // dissociating previous parties
  for(int lyr=0; lyr!=18; ++lyr) {
    for(int mh=0; mh!=fNHit[lyr]; ++mh) {
      hit = (mxHit*) fHit[lyr].at( mh );
      hit->SetAssigned( false );
    }
  }
  switch(fPtyAlg) {
  case(0): // FOCAL ALLIANCE for EX and MPC
    for(int lyr=0; lyr!=18; ++lyr)
      Parties_ALGFocal(lyr);
    break;
  case(1): // PADROW ALLIANCE for EX nothig for MPC
    for(int lyr=0; lyr!=18; ++lyr)
      if(lyr==8||lyr==17) continue;
      else Parties_ALGPadRow(lyr);
    break;
  case(2): // PADROW ALLIANCE for EX and LAYER ALLIANCE for MPC
    Parties_ALGLayer(8);
    Parties_ALGLayer(17);
    for(int lyr=0; lyr!=18; ++lyr)
      Parties_ALGPadRow(lyr);
    break;
  case(3): // FOCAL ALLIANCE for EX and LAYER ALLIANCE for MPC
    Parties_ALGLayer(8);
    Parties_ALGLayer(17);
    for(int lyr=0; lyr!=18; ++lyr)
      Parties_ALGFocal(lyr);
    break;
  case(4): // MPCBreaker for MPC only
    Parties_ALGMPCBreaker(8);
    Parties_ALGMPCBreaker(17);
    break;
  }
}
//========
void mxReconstruction::Parties_ALGMPCBreaker(int lyr) {
  if(fDebug>20)
    std::cout << "> P_ALGMPCBraker for layer " << lyr << std::endl;
  if(lyr!=8&&lyr!=17) return; //do nothing
  float dx = fGeo->PbWO4_a0();
  float dy = fGeo->PbWO4_a1();
  // sort by energy
  std::sort(fHit[lyr].begin(),fHit[lyr].begin()+fNHit[lyr],GreaterSignal());
  // start loop from most energetic, then attach neighbours below threshold
  //std::cout << "  starting with parties " << fNPty[lyr] << std::endl;
  for(int mh=0; mh!=fNHit[lyr]; ++mh) {
    mxHit *hit = (mxHit*) fHit[lyr].at( mh );
    if(hit->IsAssigned()) continue;
    int idx = hit->Idx();
    float peak = hit->Signal();
    // create new party
    int nmax = fPty[lyr].size();
    mxCluster *pty;
    if(fNPty[lyr]>nmax-1) {
      pty = new mxCluster();
      pty->SetDxDy(dx,dy);
      fPty[lyr].push_back(pty);
    } else pty = fPty[lyr].at( fNPty[lyr] );
    fNPty[lyr]++;
    pty->Reset();
    float x = fGeo->X( idx );
    float y = fGeo->Y( idx );
    pty->Fill( hit, x, y );
    if(fDebug>20) {
      std::cout << "  hit no " << mh << " in x y " << x;
      std::cout << " " << y << " || signal " << hit->Signal()<< std::endl;
    }
    if(fPtyAlgMPCBreaker_NCrystals<2) continue;
    float thr = peak*fPtyAlgMPCBreaker_thr;
    int fourn[4];
    bool forceexit = false;
    /*
    bool added = true;
    while(added) { // unnecesarily slow (TODO: speed it up)
      if(fDebug>20)
	std::cout << " scanning party " << std::endl;
      added = false;
      if(forceexit) break;
      for(int nhp=0; nhp!=pty->N(); ++nhp) { // loop over party members
	if(forceexit) break;
	mxHit *hitp = (mxHit*) pty->GetHit(nhp);
    */
    mxHit *hitp = (mxHit*) pty->GetHit(0);
    int idxp = hitp->Idx();
    fGeo->PbWO4_GetNeighbours( idxp, fourn );
    if(fDebug>20) {
      std::cout << "  HIT " << idxp << " | NEIG ";
      std::cout << fourn[0] << " " << fourn[1] << " " << fourn[2];
      std::cout << " " << fourn[3] << std::endl;
      std::cout << "  scanning neighbours " << std::endl;
    }
    for(int mi=0; mi!=fNHit[lyr]; ++mi) { // loop over hits
      if(forceexit) break;
      if(fDebug>20) std::cout << "   scanning hit collection " << std::endl;
      mxHit *hitn = (mxHit*) fHit[lyr].at( mi );
      if(hitn->IsAssigned()) continue;
      if(hitn->Signal()<thr) continue;
      if(hitn->Signal()>peak) continue;
      int idxn = hitn->Idx();
      float xn = fGeo->X( idxn );
      float yn = fGeo->Y( idxn );
      for(int nn=0; nn!=4; ++nn) { // loop over neighbours
	if( fourn[nn] < 0 ) continue;
	if(idxn != fourn[nn]) continue;
	if(fDebug>20)
	  std::cout << "    new member at x y " << xn << " " << yn << std::endl;
	pty->Fill( hitn, xn, yn );
	//added = true;
	if(pty->N()==fPtyAlgMPCBreaker_NCrystals) forceexit = true;
	fourn[nn] = -1;
	break; // break loop when found
      } // end of loop neighbours
    } // end of loop hits
    /*
      } // end of loop pty members
      } // recursive while
    */
    if(pty->N()!=fPtyAlgMPCBreaker_NCrystals) {
      // failure, discarting
      for(int rb=0; rb!=pty->N(); ++rb) {
	mxHit *hhh = pty->GetHit(rb);
	hhh->SetAssigned( false ); // freeing hit
      }
      fNPty[lyr]--;
    }
  }
}
//========
void mxReconstruction::Parties_ALGLayer(int lyr) {
  if(fDebug>20)
    std::cout << "> P_ALGLayer for layer " << lyr << std::endl;
  float dx, dy;
  if(lyr==8||lyr==17) {
    dx = fGeo->PbWO4_a0();
    dy = fGeo->PbWO4_a1();
  } else {
    if((lyr%9)%2==0) {
      dx = fGeo->Si_a0();
      dy = fGeo->Si_a1();
    } else {
      dx = fGeo->Si_a1();
      dy = fGeo->Si_a0();
    }
  }

  mxCluster *pty;
  int nmax = fPty[lyr].size();
  if(fNPty[lyr]>nmax-1) {
    pty = new mxCluster();
    pty->SetDxDy(dx,dy);
    fPty[lyr].push_back(pty);
  } else pty = fPty[lyr].at( fNPty[lyr] );
  pty->Reset();

  mxHit *hit;
  for(int mh=0; mh!=fNHit[lyr]; ++mh) {
    hit = (mxHit*) fHit[lyr].at( mh );
    if(hit->IsAssigned()) continue;
    int idx = hit->Idx();
    float x = fGeo->X( idx );
    float y = fGeo->Y( idx );
    pty->Fill( hit, x, y );
    if(fDebug>20) {
      std::cout << "  HIT x " << x << " y " << y << " sgn " << hit->Signal();
      std::cout << " => Cluster x " << pty->GetX() << " y " << pty->GetY() << " sgn " << pty->Signal() << std::endl;
    }
  }
  if(pty->N()>0) {
    fNPty[lyr]++;
    if(fDebug>20)
      std::cout << "  PTY PUSHED" << std::endl;
  }
}
//========
void mxReconstruction::Parties_ALGFocal(int lyr) {
  if(fDebug>20)
    std::cout << "> P_ALGFocal for layer " << lyr << std::endl;
  std::sort(fHit[lyr].begin(),fHit[lyr].begin()+fNHit[lyr],GreaterSignal());
  // making local parties
  mxHit *hit;
  mxCluster *pty;
  // building
  float dx, dy;
  if(lyr==8||lyr==17) {
    dx = fGeo->PbWO4_a0();
    dy = fGeo->PbWO4_a1();
  } else {
    if((lyr%9)%2==0) {
      dx = fGeo->Si_a0();
      dy = fGeo->Si_a1();
    } else {
      dx = fGeo->Si_a1();
      dy = fGeo->Si_a0();
    }
  }
  if(fDebug>20)
    std::cout << "Layer " << lyr << " | dx " << dx << " dy " << dy << std::endl;
  for(int mh=0; mh!=fNHit[lyr]; ++mh) {
    hit = (mxHit*) fHit[lyr].at( mh );
    if(hit->IsAssigned()) continue;
    int idx = hit->Idx();
    float x = fGeo->X( idx );
    float y = fGeo->Y( idx );
    bool append = false;
    if(fDebug>20)
      std::cout << "  hit no " << mh << " in x y " << x << " " << y << " || signal " << hit->Signal()<< std::endl;
    for(int mp=0; mp!=fNPty[lyr]; ++mp) {
      pty = (mxCluster*) fPty[lyr].at( mp );
      float test = pty->Test(x,y);
      if(fDebug>20) {
	std::cout << "    pty no " << mp << "|| in x y " << pty->GetX() << " " << pty->GetY();
	std::cout << " || hits " << pty->N() << " || spreadX " << pty->GetSpreadX();
	std::cout << " spreadY " << pty->GetSpreadY() << " || test " << test << std::endl;
      }
      if(test<10) {
	append = true;
	if(fDebug>20)
	  std::cout << "    >>party update<< before: x=" << pty->GetX() << " y=" << pty->GetY();
	break;
      }
    }
    if(!append) {
      int nmax = fPty[lyr].size();
      if(fNPty[lyr]>nmax-1) {
	pty = new mxCluster();
	pty->SetDxDy(dx,dy);
	fPty[lyr].push_back(pty);
      } else pty = fPty[lyr].at( fNPty[lyr] );
      pty->Reset();
      fNPty[lyr]++;
      if(fDebug>20)
	std::cout << "    >>NEW PARTY<<  ";
    }
    pty->Fill( hit, x, y );
    if(fDebug>20)
      std::cout << " now x=" << pty->GetX() << " y=" << pty->GetY() <<" sgn=" <<pty->Signal() <<std::endl;
  }
}
//========
void mxReconstruction::Parties_ALGPadRow(int lyr) {
  if(fDebug>20)
    std::cout << "> P_ALGPadRow for layer " << lyr << std::endl;
  std::sort(fHit[lyr].begin(),fHit[lyr].begin()+fNHit[lyr],LowerKey());
  float dx, dy;
  if(lyr==8||lyr==17) {
    dx = fGeo->PbWO4_a0();
    dy = fGeo->PbWO4_a1();
  } else {
    if((lyr%9)%2==0) {
      dx = fGeo->Si_a0();
      dy = fGeo->Si_a1();
    } else {
      dx = fGeo->Si_a1();
      dy = fGeo->Si_a0();
    }
  }
  mxHit *buff[4][32];
  int cursen = -1;
  for(int r=0; r!=4; ++r) for(int c=0; c!=32; ++c) buff[r][c] = NULL;
  for(int mh=0; mh!=fNHit[lyr]; ++mh) {
    mxHit *hit = (mxHit*) fHit[lyr].at( mh );
    int idx = hit->Idx();
    if(fDebug>20)
      std::cout << "  WORKING " << idx << std::endl;
    int sen = fGeo->Si_Idx2Sen(idx);
    if(cursen==-1) cursen=sen;
    if(fDebug>20)
      std::cout << "  > Sensor: now " << sen << "  won " << cursen << std::endl;
    if(sen==cursen) {
      int row = fGeo->Si_Idx2Row(idx);
      int col = fGeo->Si_Idx2Col(idx);
      if(fDebug>20)
	std::cout << "  > Adding hit to buffer in [" << row << "," << col << "]" << std::endl;
      buff[row][col] = hit;
    }
    if( (sen!=cursen) || (mh==(fNHit[lyr]-1)) ) { //DUMP
      if(fDebug>20) {
	std::cout << " >> dumping buffer" << std::endl;
	for(int r=0; r!=4; ++r) {
	  for(int c=0; c!=32; ++c) {
	    std::cout << (buff[r][c]==NULL?0:1) << "|";
	  }
	  std::cout << std::endl;
	}
      }
      for(int r=0; r!=4; ++r) { //per row
	if(fDebug>20)
	  std::cout << "    finding leaders" << std::endl;
      	//finding leaders
      	float mm=0;
      	for(int c=0; c!=32; ++c)
      	  if( buff[r][c] )
      	    if( buff[r][c]->Signal()>mm ) mm = buff[r][c]->Signal();
      	mm = mm * fPtyAlgPadRow_thr; // set threshold
      	int nlead = 0;
      	int lead[32];
      	for(int c=0; c!=32; ++c)
      	  if( buff[r][c] )
      	    if( buff[r][c]->Signal()>mm ) lead[nlead++] = c;
      	//leaders found
	if(fDebug>20) {
	  std::cout << "    nleaders " << nlead << " => ";
	  for(int l=0; l!=nlead; ++l)
	    std::cout << lead[l] << " ";
	  std::cout << std::endl;
	  std::cout << "     finding boundaries" << std::endl;
	}
      	//finding boundaries
      	int ngroups = 0;
      	int blead[33];
      	if(nlead>0) {
      	  ngroups = 1;
      	  blead[0] = 0;
      	  for(int l=0; l<nlead-1; ++l) {
      	    int L0 = lead[l];
      	    int L1 = lead[l+1];
      	    if( (L1-L0)==1 ) continue;
      	    float E0 = buff[r][L0]->Signal();
      	    float E1 = buff[r][L1]->Signal();
      	    int delta = (L1-L0)*E0/(E0+E1); //weighted delta
      	    if(delta==0) delta=1; //minimum comvention for ranges [,)
      	    blead[ngroups] = L0 + delta;
      	    ngroups++;
      	  }
      	  blead[ngroups] = 32;
      	} //nlead
      	//boundaries found
	if(fDebug>20) {
	  std::cout << "     ngroups " << ngroups << " ==> boundaries ";
	  if(ngroups>0) {
	    for(int l=0; l!=ngroups+1; ++l)
	      std::cout << blead[l] << " ";
	  }
	  std::cout << std::endl;
	  std::cout << "    dumping into parties" << std::endl;
	}
      	//dumping into parties
      	mxCluster *pty;
      	for(int l=0; l!=ngroups; ++l) {
      	  int nmax = fPty[lyr].size();
      	  if(fNPty[lyr]>nmax-1) {
      	    pty = new mxCluster();
      	    pty->SetDxDy(dx,dy);
      	    fPty[lyr].push_back(pty);
      	  } else pty = fPty[lyr].at( fNPty[lyr] );
      	  fNPty[lyr]++;
      	  pty->Reset();
      	  for(int c=blead[l]; c!=blead[l+1]; ++c) {
      	    if(buff[r][c]) {
      	      float x = fGeo->X( buff[r][c]->Idx() );
      	      float y = fGeo->Y( buff[r][c]->Idx() );
      	      pty->Fill( buff[r][c], x, y);
      	    }
      	  }
      	}
	if(fDebug>20)
	  std::cout << "    end of damping" << std::endl;
      }// end of dump
      if(sen!=cursen) {
      	cursen = sen;
      	for(int r=0; r!=4; ++r) for(int c=0; c!=32; ++c) buff[r][c] = NULL; //reset
      	int row = fGeo->Si_Idx2Row(idx);
      	int col = fGeo->Si_Idx2Col(idx);
	if(fDebug>20)
	  std::cout << "  > Adding hit to new buffer in [" << row << "," << col << "]" << std::endl;
      	buff[row][col] = hit;
        if(mh==(fNHit[lyr]-1)) { // dump lonely
          mxCluster *pty;
          int nmax = fPty[lyr].size();
          if(fNPty[lyr]>nmax-1) {
            pty = new mxCluster();
            pty->SetDxDy(dx,dy);
            fPty[lyr].push_back(pty);
          } else pty = fPty[lyr].at( fNPty[lyr] );
          fNPty[lyr]++;
          pty->Reset();
          float x = fGeo->X( hit->Idx() );
          float y = fGeo->Y( hit->Idx() );
          pty->Fill( hit, x, y);
        } // dump lonely
      }
    }
  }
}
//========
void mxReconstruction::Candidates() {
  //std::cout << "COALITIONS " << std::endl;
  for(int lyr=0; lyr!=18; ++lyr) std::sort(fPty[lyr].begin(),fPty[lyr].begin()+fNPty[lyr],GreaterSignal());
  mxCluster *pty;
  // dissociating previous allignments
  for(int lyr=0; lyr!=18; ++lyr)
    for(int mp=0; mp!=fNPty[lyr]; ++mp) {
      pty = fPty[lyr].at(mp);
      pty->SetAssigned( false );
    }
  switch(fCoaAlg) {
  case(0):
    Candidates_ALGSeedMPC(); // MPC ==> EX0
    break;
  case(1):
    Candidates_ALGSeed6(); // EX6 ==> EX0 ==> EX7 | MPC
    break;
  }
}
//========
void mxReconstruction::Candidates_ALGSeedMPC() {
  if(fDebug>10)
    std::cout << "C_ALGSeedMPC" << std::endl;
  // 0 1 2 3 4 5 6 7 || 8
  // O<=================O
  // Build the coalition from MPC backwards
  // Parties cannot be shared among coalitions

  // forming global coalitions
  float Z[18], dz[18];
  for(int i=0;i!=18;++i) dz[i] = fGeo->Si_a2();
  dz[8] = fGeo->PbWO4_a2();
  dz[17] = fGeo->PbWO4_a2();
  //for(int i=0;i!=18;++i) Z[i] = fGeo->RZ(i);// + 0.5*dz[i];
  for(int i=0;i!=18;++i) Z[i] = fGeo->RZ(i) + 0.5*dz[i];

  mxCluster *pty;
  mxCandidate *coa;
  for(int arm=0; arm!=2; ++arm) {
    // call for coalition formation
    for(int lyr=arm*9+8; lyr!=arm*9+7; --lyr) {
      if(fDebug>10)
	std::cout << "lyr " << lyr << " | npty " << fNPty[lyr] << std::endl;
      for(int mp=0; mp!=fNPty[lyr]; ++mp) {
      	pty = fPty[lyr].at(mp);
        if(pty->IsAssigned()) continue;
        // seeding
      	int nmax = fCoa[arm].size();
        if(fNCoa[arm]>nmax-1) {
          coa = new mxCandidate();
          fCoa[arm].push_back(coa);
        } else
      	  coa = fCoa[arm].at( fNCoa[arm] );
      	coa->Reset();
      	float ephi=0, etheta=0;
      	float phi = _phi( pty->GetX()-fV[0], pty->GetY()-fV[1], ephi, pty->GetSpreadX(), pty->GetSpreadY() );
      	float theta = _theta( pty->GetX()-fV[0], pty->GetY()-fV[1], Z[lyr]-fV[2], etheta, pty->GetSpreadX(), pty->GetSpreadY(), dz[lyr] );
	if(fDebug>10) {
	  std::cout << " seeding pty " << mp << " with phi theta " << phi << " " << theta;
	  std::cout << " || ephi etheta " << ephi << " " << etheta << std::endl;
	}
      	int coalyr = lyr%9;
        coa->Fill( coalyr, pty, phi, theta );
	if(fDebug>10)
	  std::cout << " >>>NEW COALITION<<< with phi theta " << coa->GetPhi() << " " << coa->GetTheta() << std::endl;
      	++fNCoa[arm];
        // check for allignments
        for(int inc=lyr-1; inc!=arm*9-1; --inc) {
	  if(fDebug>10)
	    std::cout << "  checking lyr " << inc << " | npty " << fNPty[inc] << std::endl;
      	  for(int np=0; np!=fNPty[inc]; ++np) {
            pty = fPty[inc].at(np);
      	    if(pty->IsAssigned()) continue;
      	    phi = _phi( pty->GetX()-fV[0], pty->GetY()-fV[1], ephi, pty->GetSpreadX(), pty->GetSpreadY() );
      	    theta = _theta( pty->GetX()-fV[0], pty->GetY()-fV[1], Z[inc]-fV[2], etheta, pty->GetSpreadX(), pty->GetSpreadY(), dz[inc]  );
      	    float test = coa->Test( phi, theta, ephi, etheta );
	    if(fDebug>10) {
	      std::cout << "    ptyno " << np << " with phi theta " << phi << " " << theta << " || ephi etheta ";
	      std::cout << ephi << " " << etheta << " || test " << test << std::endl;
	    }
            if( test < 3) {
	      if(fDebug>10)
		std::cout << "    [compatible]  saving..." << std::endl;
              coa->Fill( inc, pty, phi, theta );
      	      break;
      	    }
          } // np
	  if(fDebug>10)
    	    std::cout << " >update< " << coa->GetPhi() << " " << coa->GetTheta() << std::endl;
        } //inc
      } //mp
    } //lyr
  } //arm
}
//========
void mxReconstruction::Candidates_ALGSeed6() {
  if(fDebug>10)
    std::cout << "C_ALGSeed6" << std::endl;
  // 0 1 2 3 4 5 6 7 || 8
  // O<==========O   ||
  //             =>O || X
  // Building coalitions using EX only
  // Candidate formation starts at layer 6 and move backwards
  // then continues to last layer.
  // Parties cannot be shared among coalitions
  // Matching of MPC afterwrds in *passive* mode (used for calibration)

  // forming global coalitions
  float Z[18], dz[18];
  for(int i=0;i!=18;++i) dz[i] = fGeo->Si_a2();
  dz[8] = fGeo->PbWO4_a2();
  dz[17] = fGeo->PbWO4_a2();
  for(int i=0;i!=18;++i) Z[i] = fGeo->RZ(i);// + 0.5*dz[i];
  mxCluster *pty;
  mxCandidate *coa;
  for(int arm=0; arm!=2; ++arm) {
    // call for coalition formation
    int lyr=arm*9+6;
    if(fDebug>10)
      std::cout << "lyr " << lyr << " | npty " << fNPty[lyr] << std::endl;
    for(int mp=0; mp!=fNPty[lyr]; ++mp) {
      pty = fPty[lyr].at(mp);
      if(pty->IsAssigned()) continue;
      // seeding
      int nmax = fCoa[arm].size();
      if(fNCoa[arm]>nmax-1) {
      	coa = new mxCandidate();
      	fCoa[arm].push_back(coa);
      } else
      	coa = fCoa[arm].at( fNCoa[arm] );
      coa->Reset();
      float ephi=0, etheta=0;
      float spreadxy = TMath::Max( pty->GetSpreadX(), pty->GetSpreadY() );
      float phi = _phi( pty->GetX()-fV[0], pty->GetY()-fV[1], ephi, spreadxy, spreadxy );
      float theta = _theta( pty->GetX()-fV[0], pty->GetY()-fV[1], Z[lyr]-fV[2], etheta, spreadxy, spreadxy, dz[lyr] );
      if(fDebug>10) {
	std::cout << " seeding pty " << mp << " with phi theta " << phi << " " << theta;
	std::cout << " || ephi etheta " << ephi << " " << etheta << std::endl;
      }
      int coalyr = lyr%9;
      coa->Fill( coalyr, pty, phi, theta );
      if(fDebug>10)
	std::cout << " >>>NEW COALITION<<< with phi theta " << coa->GetPhi() << " " << coa->GetTheta() << std::endl;
      ++fNCoa[arm];
      // check for allignments backwards
      for(int inc=lyr-1; inc!=arm*9-1; --inc) {
	if(fDebug>10)
	  std::cout << "  checking lyr " << inc << " | npty " << fNPty[inc] << std::endl;
      	for(int np=0; np!=fNPty[inc]; ++np) {
      	  pty = fPty[inc].at(np);
      	  if(pty->IsAssigned()) continue;
      	  phi = _phi( pty->GetX()-fV[0], pty->GetY()-fV[1], ephi, pty->GetSpreadX(), pty->GetSpreadY() );
      	  theta = _theta( pty->GetX()-fV[0], pty->GetY()-fV[1], Z[inc]-fV[2], etheta, pty->GetSpreadX(), pty->GetSpreadY(), dz[inc]  );
      	  float test = coa->Test( phi, theta, ephi, etheta );
	  if(fDebug>10) {
	    std::cout << "    ptyno " << np << " with phi theta " << phi << " " << theta << " || ephi etheta ";
	    std::cout << ephi << " " << etheta << " || test " << test << std::endl;
	  }
      	  int coainc = inc%9;
      	  if( test < 3 ) {
	    if(fDebug>10)
	      std::cout << "    [compatible]  saving..." << std::endl;
      	    coa->Fill( coainc, pty, phi, theta );
      	    break;
      	  }
      	} //np
	if(fDebug>10)
	  std::cout << " >update< " << coa->GetPhi() << " " << coa->GetTheta() << std::endl;
      } //inc
      // check for allignments forward
      for(int inc=lyr+1; inc!=(arm+1)*9-1; ++inc) {
	if(fDebug>10)
	  std::cout << "  checking lyr " << inc << " | npty " << fNPty[inc] << std::endl;
      	for(int np=0; np!=fNPty[inc]; ++np) {
      	  pty = fPty[inc].at(np);
      	  if(pty->IsAssigned()) continue;
      	  phi = _phi( pty->GetX()-fV[0], pty->GetY()-fV[1], ephi, pty->GetSpreadX(), pty->GetSpreadY() );
      	  theta = _theta( pty->GetX()-fV[0], pty->GetY()-fV[1], Z[inc]-fV[2], etheta, pty->GetSpreadX(), pty->GetSpreadY(), dz[inc]  );
      	  float test = coa->Test( phi, theta, ephi, etheta );
	  if(fDebug>10) {
	    std::cout << "    ptyno " << np << " with phi theta " << phi << " " << theta << " || ephi etheta " << ephi;
	    std::cout << " " << etheta << " || test " << test << std::endl;
	  }
	  int coainc = inc%9;
      	  if( test < 3 ) {
	    if(fDebug>10)
	      std::cout << "    [compatible]  saving..." << std::endl;
      	    coa->Fill( coainc, pty, phi, theta );
      	    break;
      	  }
      	} //np
	if(fDebug>10)
	  std::cout << " >update< " << coa->GetPhi() << " " << coa->GetTheta() << std::endl;
      } //inc
    } //mp
    std::sort(fCoa[arm].begin(),fCoa[arm].begin()+fNCoa[arm],MostContributors());
    for(int icoa=0; icoa!=fNCoa[arm]; ++icoa) {
      coa = fCoa[arm].at(icoa);
      mxCluster *mpc = SeekHitInEM(coa->GetPhi(), coa->GetTheta(), arm, fCoaAlgSeed6_nc);
      if(mpc) {
	coa->Fill( arm*9+8, mpc, coa->GetPhi(), coa->GetTheta() );
	if(fDebug>5)
	  std::cout << " >attach mpc< with " << mpc->N() << " hits" << std::endl;
      }
      float pp = ComputePSChi2Prob(arm,coa);
      coa->SetPSChi2Prob(pp);
    } //icoa
  } //arm
}
//========
void mxReconstruction::FillPreEvent() {
  if(fDebug>10)
    std::cout << "FillPreEvent" << std::endl;
  mxCandidate *coaI, *coaJ;
  for(int arm=0; arm!=2; ++arm) {
    // call for union formation
    fNCoaPreEvent[arm] = fNCoa[arm];
    for(int mi=0; mi!=fNCoa[arm]; ++mi) {
      coaI = (mxCandidate*) fCoa[arm].at( mi );
      if(mi<fCoaPreEvent[arm].size())
	coaJ = (mxCandidate*) fCoaPreEvent[arm].at( mi );
      else {
	coaJ = new mxCandidate();
	fCoaPreEvent[arm].push_back( coaJ );
      }
      coaJ->CopyFrom( coaI );
    }
  }
}
//========
void mxReconstruction::MixUnions() {
  // forming mixed unions
  mxCandidate *coaI, *coaJ;
  mxDecayCandidate *un;
  for(int arm=0; arm!=2; ++arm) {
    fNUni[arm] = 0;
    // call for union formation
    for(int mi=0; mi<fNCoa[arm]-1; ++mi) {
      coaI = (mxCandidate*) fCoa[arm].at( mi );
      //std::cout << coaI->GetEnergy() << " this_event" << std::endl;
      for(int mj=mi+1; mj<fNCoaPreEvent[arm]; ++mj) {
      	coaJ = (mxCandidate*) fCoaPreEvent[arm].at( mj );
	//std::cout << coaJ->GetEnergy() << " previous_event" << std::endl;
        int nmax = fUni[arm].size();
        if(fNUni[arm]>nmax-1) {
          un = new mxDecayCandidate();
          fUni[arm].push_back(un);
        } else un = fUni[arm].at( fNUni[arm] );
	un->Make(coaI,coaJ);
        ++fNUni[arm];
      }
    }
  }
}
//========
void mxReconstruction::Unions() {
  // forming global unions
  //for(int arm=0; arm!=2; ++arm) std::sort(fCoa[arm].begin(),fCoa[arm].begin()+fNCoa[arm],GreaterSignal());
  mxCandidate *coaI, *coaJ;
  mxDecayCandidate *un;
  for(int arm=0; arm!=2; ++arm) {
    fNUni[arm] = 0;
    // call for union formation
    if(fNCoa[arm]<2) continue;
    for(int mi=0; mi<fNCoa[arm]-1; ++mi) {
      coaI = (mxCandidate*) fCoa[arm].at( mi );
      for(int mj=mi+1; mj<fNCoa[arm]; ++mj) {
      	coaJ = (mxCandidate*) fCoa[arm].at( mj );
        int nmax = fUni[arm].size();
        if(fNUni[arm]>nmax-1) {
          un = new mxDecayCandidate();
          fUni[arm].push_back(un);
        } else un = fUni[arm].at( fNUni[arm] );
	un->Make(coaI,coaJ);
        ++fNUni[arm];
      }
    }
  }
}
//========
void mxReconstruction::FillPP(float energy, float eta, float phi, int pdg) {
  //fMCPart->Fill(energy,eta,phi,pdg);
}
//========
mxCluster* mxReconstruction::SeekHitInEM(float phi, float theta, int arm, int group) {
  if(fDebug>5)
    std::cout << "SeekHitInEM" << std::endl;
  int lyr = arm*9+8;
  float z = fGeo->RZ(lyr);
  float r = z*TMath::Tan(theta);
  float x = r*TMath::Cos(phi);
  float y = r*TMath::Sin(phi);
  if(fDebug>5)
    std::cout << " x y " << x << " " << y << std::endl;
  mxCluster *pty;
  float dx = fGeo->PbWO4_a0();
  float dy = fGeo->PbWO4_a1();
  if(fDebug>5)
    std::cout << " look if party is already created and attached to it" << std::endl;
  for(int mp=0; mp!=fNPty[lyr]; ++mp) {
    pty = (mxCluster*) fPty[lyr].at(mp);
    if(pty->IsAssigned()) continue;
    float xp = pty->GetX();
    float yp = pty->GetY();
    if( TMath::Abs(xp-x) > (dx/2+0.5) ) continue;
    if( TMath::Abs(yp-y) > (dy/2+0.5) ) continue;
    return pty;
  }
  if(fDebug>5)
    std::cout << " try to build one" << std::endl;
  std::sort(fHit[lyr].begin(),fHit[lyr].begin()+fNHit[lyr],GreaterSignal());
  mxHit *hit;
  pty = NULL;
  for(int mh=0; mh!=fNHit[lyr]; ++mh) {
    hit = (mxHit*) fHit[lyr].at( mh );
    if(hit->IsAssigned()) continue;
    int idx = hit->Idx();
    float xh = fGeo->X( idx );
    float yh = fGeo->Y( idx );
    if( TMath::Abs(xh-x) > (dx/2+0.5) ) continue;
    if( TMath::Abs(yh-y) > (dy/2+0.5) ) continue;
    if(fDebug>5)
      std::cout << " FOUND " << hit->Idx() << std::endl;
    int nmax = fPty[lyr].size();
    if(fNPty[lyr]>nmax-1) {
      pty = new mxCluster();
      pty->SetDxDy(dx,dy);
      fPty[lyr].push_back(pty);
    } else pty = fPty[lyr].at( fNPty[lyr] );
    pty->Reset();
    fNPty[lyr]++;
    pty->Fill( hit, xh, yh );
    if(group>2) { // find two best neighbors
      if(fDebug>5)
	std::cout << " attempt to find threesome" << std::endl;
      int nn[2] = {0,0};
      int idn[2][2] = { {-1,-1}, {-1,-1} };
      float sgnn[2][2] = { {0,0}, {0,0} };
      float xn[2][2] = { {0,0}, {0,0} };
      float yn[2][2] = { {0,0}, {0,0} };
      mxHit *ohit;
      for(int nh=mh; nh!=fNHit[lyr]; ++nh) { //loop over less energetic ones
	//for(int nh=0; nh!=fNHit[lyr]; ++nh) { //loop over less energetic ones
	ohit = (mxHit*) fHit[lyr].at( nh );
	if(ohit->IsAssigned()) continue;
	int oidx = ohit->Idx();
	float ox = fGeo->X( oidx );
	float oy = fGeo->Y( oidx );
	if(fDebug>5)
	  std::cout << " found one crystal with dx " << (xh-ox) << " dy " << (yh-oy) << " idx " << oidx << std::endl;
	if( (TMath::Abs(xh-ox)<(dx+0.5)) && 
	    (TMath::Abs(yh-oy)<0.1) ) { // horizontal neighbor
	  if(fDebug>5)
	    std::cout << " found neighbour in H" << std::endl;
	  idn[0][ nn[0] ] = nh;
	  xn[0][ nn[0] ] = ox;
	  yn[0][ nn[0] ] = oy;
	  sgnn[0][ nn[0]++ ] = ohit->Signal();
	} else
	if( (TMath::Abs(yh-oy)<(dy+0.5)) && 
	    (TMath::Abs(xh-ox)<0.1) ) { // vertical neighbor
	  if(fDebug>5)
	    std::cout << " found neighbour in V" << std::endl;
	  idn[1][ nn[1] ] = nh;
	  xn[1][ nn[1] ] = ox;
	  yn[1][ nn[1] ] = oy;
	  sgnn[1][ nn[1]++ ] = ohit->Signal();
	}
      }
      if(nn[0]>0) {
	int ij = 0;
	if( sgnn[0][1] > sgnn[0][0] ) ij = 1;
	pty->Fill( fHit[lyr].at( idn[0][ij] ), xn[0][ij], yn[0][ij] );
      }
      if(nn[1]>0) {
	int ij = 0;
	if( sgnn[1][1] > sgnn[1][0] ) ij = 1;
	pty->Fill( fHit[lyr].at( idn[1][ij] ), xn[1][ij], yn[1][ij] );
      }
    }
    break;
  }
  //if(group>3) { // increase mpc cluster by proximity
  //  std::sort(fHit[lyr].begin(),fHit[lyr].begin()+fNHit[lyr],GreaterSignal());
  //}
  return pty;
}
//========
float mxReconstruction::ComputePSChi2Prob(int arm, mxCandidate *coa) {
  mxCluster *pty = NULL;
  float ph0 = coa->GetPhi();
  float th0 = coa->GetTheta();
  float chi2 = 0;
  int ncl = 0;

  float Z[8];
  float dz[8];
  for(int i=0;i!=8;++i) dz[i] = fGeo->Si_a2();
  for(int i=0;i!=8;++i) Z[i] = fGeo->RZ(arm*9+i);// + 0.5*dz[i];

  for(int i=0; i!=8; ++i) {
    pty = coa->GetCluster(i);
    if(!pty) continue;
    ncl++;
    float ephi=0, ethi=0;
    float phi = _phi( pty->GetX(), pty->GetY(), ephi, pty->GetSpreadX(), pty->GetSpreadY() );
    float thi = _theta( pty->GetX(), pty->GetY(), Z[i], ethi, pty->GetSpreadX(), pty->GetSpreadY(), dz[i] );
    chi2 += (phi-ph0)/ephi*(phi-ph0)/ephi + (thi-th0)/ethi*(thi-th0)/ethi;
  }
  return TMath::Prob(chi2,ncl*2.0);
}
//========
void mxReconstruction::SetIdentificationAlgorithm(int combo) {
  switch(combo) {
  case(0):
    SetClusterAlgorithm(0); // FOCAL ALLIANCE for EX and MPC
    SetCandidateAlgorithm(0); // MPC ==> EX0
    break;
  case(1):
    SetClusterAlgorithm(1); // PADROW ALLIANCE for EX nothig for MPC
    SetCandidateAlgorithm(1); // EX6 ==> EX0 ==> EX7 | MPC
    SetPtyAlgPadRow_Threshold(0.15);
    SetCoaAlgSeed6_NCrystals(1); // MATCH MPC to ONE crystal
    break;
  case(2):
    SetClusterAlgorithm(2);// PADROW ALLIANCE for EX and LAYER ALLIANCE for MPC
    SetCandidateAlgorithm(1); // EX6 ==> EX0 ==> EX7 | MPC
    SetPtyAlgPadRow_Threshold(0.15);
    SetCoaAlgSeed6_NCrystals(1); // IRRELEVANT, WILL MATCH TO FULL MPC
    break;
  case(3):
    SetClusterAlgorithm(3);// FOCAL ALLIANCE for EX  and LAYER ALLIANCE for MPC
    SetCandidateAlgorithm(1); // EX6 ==> EX0 ==> EX7 | MPC
    SetPtyAlgPadRow_Threshold(0.15);
    SetCoaAlgSeed6_NCrystals(1); // IRRELEVANT, WILL MATCH TO FULL MPC
    break;
  case(4):
    SetClusterAlgorithm(1); // PADROW ALLIANCE for EX nothig for MPC
    SetCandidateAlgorithm(1); // EX6 ==> EX0 ==> EX7 | MPC
    SetPtyAlgPadRow_Threshold(0.15);
    SetCoaAlgSeed6_NCrystals(3); // MATCH MPC to THREE crystals
    break;
  case(5):
    SetClusterAlgorithm(4); // MPCBreaker for MPC only
    SetCandidateAlgorithm(0); // MPC ==> EX0
    break;
  }
}
