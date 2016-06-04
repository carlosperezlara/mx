//#include <stdlib>
#include <iostream>
#include <vector>
#include <algorithm>

#include "phMath.h"

#include "mxGeometry.h"
#include "mxHit.h"
#include "mxParty.h"
#include "mxCoalition.h"
#include "mxReconstruction.h"

struct GreaterSignal
{
  template<class T>
  bool operator()(T const &a, T const &b) const { return a->Signal() > b->Signal(); }
};

//========
mxReconstruction::mxReconstruction() {
  // ctor
  std::cout << "mxReconstruction:: ctor" << std::endl;
  for(int i=0; i!=18; ++i) {
    fNHit[i] = 0;
    fNPty[i] = 0;
  }
  for(int i=0; i!=2; ++i) {
    fNCoa[i] = 0;
  }
  fGeo = new mxGeometry();
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
  for(int i=0; i!=2; ++i)
    fNCoa[i] = 0;
  fV[0] = 0.;
  fV[1] = 0.;
  fV[2] = 0.;
}
//========
mxReconstruction::~mxReconstruction() {
  // dtor
  for(int i=0; i!=18; ++i) {
    for(int j=0; j!=fNHit[i]; ++j)
      delete fHit[i].at(j);
    for(int j=0; j!=fNPty[i]; ++j)
      delete fPty[i].at(j);
  }
  for(int i=0; i!=2; ++i) {
    for(int j=0; j!=fNCoa[i]; ++j)
      delete fCoa[i].at(j);
  }
}
//========
void mxReconstruction::DumpParties() {
  std::cout << "=============================" << std::endl;
  std::cout << "mxReconstruction::DumpParties" << std::endl;
  int nn=0;
  mxParty *pty;
  mxHit *hit;
  for(int i=0; i!=18; ++i) {
    int n = fNPty[i];
    nn += n;
    std::cout << "  Layer " << i << " || Nparties " << n << std::endl;
    for(int j=0; j!=n; ++j) {
      pty = fPty[i].at(j);
      std::cout << "  ||" << j << "|| x y " << pty->X() << " " << pty->Y() << " || sgn " << pty->Signal() << std::endl;
      int m = pty->N();
      for(int k=0; k!=m; ++k) {
	hit = pty->GetHit(k);
	float x = fGeo->X(hit->Idx());
	float y = fGeo->Y(hit->Idx());
	std::cout << "       ||" << k << "|| x y " << x << " " << y << " || sgn " << hit->Signal() << std::endl;
      }
    }
  }
  std::cout << "Total Parties: " << nn << std::endl;
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
  for(int lyr=0; lyr!=18; ++lyr) std::sort(fHit[lyr].begin(),fHit[lyr].end(),GreaterSignal());
  Parties();
  for(int lyr=0; lyr!=18; ++lyr) std::sort(fPty[lyr].begin(),fPty[lyr].end(),GreaterSignal());
  Coalitions();
}
//========
void mxReconstruction::Parties() {
  // making local parties
  mxHit *hit;
  mxParty *pty;
  for(int lyr=0; lyr!=18; ++lyr) {
    // seeding
    //for(int mh=0; mh!=fNHit[lyr]; ++mh) {
    //  hit = (mxHit*) fHit[lyr]->At( mh );
    //  hit->SetAssigned( false );
    //}
    // building
    //if(lyr!=12) continue;
    for(int mh=0; mh!=fNHit[lyr]; ++mh) {
      hit = (mxHit*) fHit[lyr].at( mh );
      //if(hit->IsAssigned()) continue;
      int idx = hit->Idx();
      float x = fGeo->X( idx );
      float y = fGeo->Y( idx );
      bool append = false;
      //std::cout << "  hit no " << mh << " in " << x << " " << y << std::endl;
      for(int mp=0; mp!=fNPty[lyr]; ++mp) {
	pty = (mxParty*) fPty[lyr].at( mp );
	float test = pty->Test(x,y);
	//std::cout << "    pty no " << mp << "||=> " << pty->X() << " " << pty->Y() << " || " << pty->N() << " || stddev " << sqrt(pty->Cov(0)) << " " << sqrt(pty->Cov(1)) << " || test " << test << std::endl;
	if(test<3) {
	  append = true;
	  //std::cout << "    >>party update<< before:" << pty->X() << " " << pty->Y();
	  break;
	}
      }
      if(!append) {
	int nmax = fPty[lyr].size();
	if(fNPty[lyr]>nmax-1) {
	  pty = new mxParty();
	  fPty[lyr].push_back(pty);
	} else pty = fPty[lyr].at( fNPty[lyr] );
	pty->Reset();
	fNPty[lyr]++;
	//std::cout << "    >>NEW PARTY<<  ";
      }
      pty->Fill( hit, x, y );
      //std::cout << " now " << pty->X() << " " << pty->Y() << std::endl;
    }
  }
}
//========
void mxReconstruction::Coalitions() {
  // forming global coalitions
  float Z[18] = {-203.982, -204.636, -205.29, -205.944, -206.598, -207.252, -207.906, -208.560, -500,
                 +203.982, +204.636, +205.29, +205.944, +206.598, +207.252, +207.906, +208.560, +500};

  mxParty *pty;
  mxCoalition *coa;
  for(int arm=0; arm!=2; ++arm) {
    // dissociating previous unions
    for(int lyr=arm*9; lyr!=(arm+1)*9; ++lyr)
      for(int mp=0; mp!=fNPty[lyr]; ++mp) {
	pty = fPty[lyr].at(mp);
	pty->SetAssigned( false );
      }
    // call for coalition formation
    for(int lyr=arm*9+8; lyr!=arm*9+1; --lyr) {
      //std::cout << "lyr " << lyr << " | npty " << fNPty[lyr] << std::endl;
      for(int mp=0; mp!=fNPty[lyr]; ++mp) {
	pty = fPty[lyr].at(mp);
        if(pty->IsAssigned()) continue;
        // seeding
	int nmax = fCoa[arm].size();
        if(fNCoa[arm]>nmax-1) {
          coa = new mxCoalition();
          fCoa[arm].push_back(coa);
        } else coa = fCoa[arm].at( fNCoa[arm] );
	coa->Reset();
	float phi = _phi( pty->X(), pty->Y() );
	float eta = _eta( pty->X(), pty->Y(), Z[lyr] );
	//std::cout << " seeding pty " << mp << " with " << phi << " " << eta  << std::endl;
	int coalyr = lyr%9;
        coa->Fill( coalyr, pty, phi, eta );
	//std::cout << " >>>NEW COALITION<<< " << coa->Phi() << " " << coa->Eta() << std::endl;
	++fNCoa[arm];
        // check for allignments
        for(int inc=lyr-1; inc!=arm*9-1; --inc) {
	  //std::cout << "  checking lyr " << inc << " | npty " << fNPty[inc] << std::endl;
	  for(int np=0; np!=fNPty[inc]; ++np) {
            pty = fPty[inc].at(np);
	    if(pty->IsAssigned()) continue;
	    phi = _phi( pty->X(), pty->Y() );
	    eta = _eta( pty->X(), pty->Y(), Z[lyr] );
	    float test = coa->Test( phi, eta );
	    //std::cout << "    ptyno " << np << " with " << phi << " " << eta << " || test " << test << std::endl;
            if( test < 3) {
              coa->Fill( inc, pty, phi, eta );
	      break;
	    }
          }
	  //std::cout << " >update< " << coa->Phi() << " " << coa->Eta() << std::endl;
	}
      }
    }
    // DONE with arm
  }
}
