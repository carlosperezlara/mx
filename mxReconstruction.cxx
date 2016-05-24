//#include <stdlib>
#include <iostream>
#include <vector>

#include "phMath.h"

#include "mxGeometry.h"
#include "mxHit.h"
#include "mxParty.h"
#include "mxCoalition.h"
#include "mxReconstruction.h"

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
void mxReconstruction::DumpStats() {
  std::cout << "===========================" << std::endl;
  std::cout << "mxReconstruction::DumpStats" << std::endl;
  std::cout << "  Hit:"; for(int i=0; i!=18; ++i) std::cout << " " << fNHit[i]; std::cout << std::endl;
std::cout << "  Pty:"; for(int i=0; i!=18; ++i) std::cout << " " << fNPty[i]; std::cout << std::endl;
std::cout << "  Coa:"; for(int i=0; i!=2; ++i) std::cout << " " << fNCoa[i]; std::cout << std::endl;
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
  Parties();
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
    for(int mh=0; mh!=fNHit[lyr]; ++mh) {
      hit = (mxHit*) fHit[lyr].at( mh );
      //if(hit->IsAssigned()) continue;
      int idx = hit->Idx();
      float x = fGeo->X( idx );
      float y = fGeo->Y( idx );
      bool append = false;
      //std::cout << "hit no " << mh << std::endl;
      //std::cout << x << " " << y << std::endl;
      for(int mp=0; mp!=fNPty[lyr]; ++mp) {
	pty = (mxParty*) fPty[lyr].at( mp );
	float test = pty->Test(x,y);
	//std::cout << " " << mp << "||=> " << pty->X() << " " << pty->Y() << " || " << test << std::endl;
	if(test<3) {
	  append = true;
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
      }
      pty->Fill( hit, x, y );
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
      std::cout << "lyr " << lyr << " | npty " << fNPty[lyr] << std::endl;
      for(int mp=0; mp!=fNPty[lyr]; ++mp) {
	pty = fPty[lyr].at(mp);
        if(pty->IsAssigned()) continue;
        // seeding
	std::cout << " seeding pty " << mp << std::endl;
	int nmax = fCoa[arm].size();
        if(fNCoa[arm]>nmax-1) {
          coa = new mxCoalition();
          fCoa[arm].push_back(coa);
        } else coa = fCoa[arm].at( fNCoa[arm] );
	coa->Reset();
	float phi = _phi( pty->X(), pty->Y() );
	float eta = _eta( pty->X(), pty->Y(), Z[lyr] );
        coa->Fill( lyr, pty, phi, eta );
        // check for allignments
        for(int inc=lyr-1; inc!=arm*9-1; --inc) {
	  std::cout << "  checking lyr " << inc << " | npty " << fNPty[inc] << std::endl;
	  for(int np=0; np!=fNPty[inc]; ++np) {
            pty = fPty[inc].at(np);
	    if(pty->IsAssigned()) continue;
	    phi = _phi( pty->X(), pty->Y() );
	    eta = _eta( pty->X(), pty->Y(), Z[lyr] );
	    float test = coa->Test( phi, eta );
	    std::cout << "    ptyno " << np << " | test " << test << std::endl;
            if( test < 3)
              coa->Fill( inc, pty, phi, eta );
          }
	}
      }
    }
    // DONE with arm
  }
}
