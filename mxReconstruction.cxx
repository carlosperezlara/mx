//#include <stdlib>
#include <iostream>
#include <vector>
#include <algorithm>

#include "phMath.h"

#include "mxGeometry.h"
#include "mxHit.h"
#include "mxParty.h"
#include "mxCoalition.h"
#include "mxUnion.h"
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
    fNUni[i] = 0;
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
    for(int j=0; j!=int(fHit[i].size()); ++j){
      delete fHit[i].at(j);
    }
    for(int j=0; j!=int(fPty[i].size()); ++j){
      delete fPty[i].at(j);
    }
    fHit[i].clear();
    fPty[i].clear();
  }
  for(int i=0; i!=2; ++i) {
    fNCoa[i] = 0;
    fNUni[i] = 0;
    for(int j=0; j!=int(fCoa[i].size()); ++j){
      delete fCoa[i].at(j);
    }
    for(int j=0; j!=int(fUni[i].size()); ++j){
      delete fUni[i].at(j);
      }
    fCoa[i].clear();
    fUni[i].clear();
  }
  fV[0] = 0.;
  fV[1] = 0.;
  fV[2] = 0.;
}
//========
mxReconstruction::~mxReconstruction() {
  // dtor
  for(int i=0; i!=18; ++i) {
    for(int j=0; j!=fNHit[i]; ++j){

      delete fHit[i].at(j);
    }
    for(int j=0; j!=fNPty[i]; ++j)
      delete fPty[i].at(j);
  }
  for(int i=0; i!=2; ++i) {
    for(int j=0; j!=fNCoa[i]; ++j)
      delete fCoa[i].at(j);
    for(int j=0; j!=fNUni[i]; ++j)
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
    int nhit = fNHit[i];
    for(int j=0; j!=n; ++j) {
      pty = fPty[i].at(j);
      std::cout << "  ||" << j << "|| x y "  <<pty->GetX() << " " << pty->GetY() << " || hits " << pty->N() << " || sgn " << pty->Signal() << " || dx dy " << pty->GetSpreadX() << " " << pty->GetSpreadY() << std::endl;
      int m = pty->N();
      for(int k=0; k!=m; ++k) {
	hit = pty->GetHit(k);
	if(!hit) std::cout << "CORRUPTION. Crashing..." << std::endl;
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
  //for(int arm=0; arm!=2; ++arm) std::sort(fCoa[arm].begin(),fCoa[arm].end(),GreaterSignal());
  //Unions();
}
//========
void mxReconstruction::Parties() {
  // making local parties
  mxHit *hit;
  mxParty *pty;
  for(int lyr=0; lyr!=18; ++lyr) {
    // seeding
    for(int mh=0; mh!=fNHit[lyr]; ++mh) {
      hit = (mxHit*) fHit[lyr].at( mh );
      hit->SetAssigned( false );
    }
    // building
    float dx, dy;
    if(lyr==8||lyr==17) {
      dx = fGeo->PWO4_a0();
      dy = fGeo->PWO4_a1();
    } else {
      if((lyr%9)%2==0) {
	dx = fGeo->Si_a0();
	dy = fGeo->Si_a1();
      } else {
	dx = fGeo->Si_a1();
	dy = fGeo->Si_a0();
      }
    }
    //std::cout << "Layer " << lyr << " | dx " << dx << " dy " << dy << std::endl;
    for(int mh=0; mh!=fNHit[lyr]; ++mh) {
      hit = (mxHit*) fHit[lyr].at( mh );
      if(hit->IsAssigned()) continue;
      int idx = hit->Idx();
      float x = fGeo->X( idx );
      float y = fGeo->Y( idx );
      bool append = false;
      //std::cout << "  hit no " << mh << " in x y " << x << " " << y << " || signal " << hit->Signal()<< std::endl;
      for(int mp=0; mp!=fNPty[lyr]; ++mp) {
	pty = (mxParty*) fPty[lyr].at( mp );
	float test = pty->Test(x,y);
	//std::cout << "    pty no " << mp << "|| in x y " << pty->GetX() << " " << pty->GetY();
	//std::cout << " || hits " << pty->N() << " || spreadX " << pty->GetSpreadX();
	//std::cout << " spreadY " << pty->GetSpreadY() << " || test " << test << std::endl;
	if(test<10) {
	  append = true;
	  //std::cout << "    >>party update<< before: x=" << pty->GetX() << " y=" << pty->GetY();
	  break;
	}
      }
      if(!append) {
	int nmax = fPty[lyr].size();
	if(fNPty[lyr]>nmax-1) {
	  pty = new mxParty();
	  pty->SetDxDy(dx,dy);
	  fPty[lyr].push_back(pty);
	} else pty = fPty[lyr].at( fNPty[lyr] );
	pty->Reset();
	fNPty[lyr]++;
	//std::cout << "    >>NEW PARTY<<  ";
      }
      pty->Fill( hit, x, y );
      //std::cout << " now x=" << pty->GetX() << " y=" << pty->GetY() <<" sgn=" <<pty->Signal() <<std::endl;
    }
  }
}
//========
void mxReconstruction::Coalitions() {
  // forming global coalitions
  float Z[18], dz[18];
  for(int i=0;i!=18;++i) dz[i] = fGeo->Si_a2();
  dz[8] = fGeo->PWO4_a2();
  dz[17] = fGeo->PWO4_a2();
  for(int i=0;i!=18;++i) Z[i] = fGeo->RZ(i);// + 0.5*dz[i];

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
    //for(int lyr=arm*9+8; lyr!=arm*9+1; --lyr) {
    for(int lyr=arm*9+8; lyr!=arm*9+7; --lyr) {
      //std::cout << "lyr " << lyr << " | npty " << fNPty[lyr] << std::endl;
      for(int mp=0; mp!=fNPty[lyr]; ++mp) {
	pty = fPty[lyr].at(mp);
        if(pty->IsAssigned()) continue;
        // seeding
	int nmax = fCoa[arm].size();
        if(fNCoa[arm]>nmax-1) {
          coa = new mxCoalition();
          fCoa[arm].push_back(coa);
        } else
	  coa = fCoa[arm].at( fNCoa[arm] );
	coa->Reset();
	float ephi, eeta;
	float phi = _phi( pty->GetX(), pty->GetY(), ephi, pty->GetSpreadX(), pty->GetSpreadY() );
	float eta = _eta( pty->GetX(), pty->GetY(), Z[lyr], eeta, pty->GetSpreadX(), pty->GetSpreadY(), dz[lyr] );
	//std::cout << " seeding pty " << mp << " with phi eta " << phi << " " << eta  << " || ephi eeta " << ephi << " " << eeta << std::endl;
	int coalyr = lyr%9;
        coa->Fill( coalyr, pty, phi, eta );
	//std::cout << " >>>NEW COALITION<<< with phi eta " << coa->GetPhi() << " " << coa->GetEta() << std::endl;
	++fNCoa[arm];
        // check for allignments
        for(int inc=lyr-1; inc!=arm*9-1; --inc) {
	  //std::cout << "  checking lyr " << inc << " | npty " << fNPty[inc] << std::endl;
	  for(int np=0; np!=fNPty[inc]; ++np) {
            pty = fPty[inc].at(np);
	    if(pty->IsAssigned()) continue;
	    phi = _phi( pty->GetX(), pty->GetY(), ephi, pty->GetSpreadX(), pty->GetSpreadY() );
	    eta = _eta( pty->GetX(), pty->GetY(), Z[inc], eeta, pty->GetSpreadX(), pty->GetSpreadY(), dz[inc]  );
	    float test = coa->Test( phi, eta, ephi, eeta );
	    //std::cout << "    ptyno " << np << " with phi eta " << phi << " " << eta << " || ephi eeta " << ephi << " " << eeta << " || test " << test << std::endl;
            if( test < 3) {
	      //std::cout << "    [compatible]  saving..." << std::endl;
              coa->Fill( inc, pty, phi, eta );
	      break;
	    }
          }
	  //std::cout << " >update< " << coa->GetPhi() << " " << coa->GetEta() << std::endl;
	}
      }
    }
    // DONE with arm
  }
}
//========
void mxReconstruction::Unions() {
  // forming global unions
  mxCoalition *coaI, *coaJ;
  mxUnion *un;
  for(int arm=0; arm!=2; ++arm) {
    // call for union formation
    if(fNCoa[arm]<2) continue;
    for(int mi=0; mi!=fNCoa[arm]-1; ++mi) {
      coaI = (mxCoalition*) fCoa[arm].at( mi );
      for(int mj=mi+1; mj!=fNCoa[arm]; ++mj) {
	coaJ = (mxCoalition*) fCoa[arm].at( mj );
        int nmax = fUni[arm].size();
        if(fNUni[arm]>nmax-1) {
          un = new mxUnion();
          fUni[arm].push_back(un);
        } else un = fUni[arm].at( fNUni[arm] );
	un->Make(coaI,coaJ);
        ++fNUni[arm];
      }
    }
    // DONE with arm
  }
}
