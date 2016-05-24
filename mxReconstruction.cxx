//#include <stdlib>

#include "TClonesArray.h"

#include "mxGeometry.h"
#include "mxHit.h"
#include "mxParty.h"
#include "mxCoalition.h"
#include "mxReconstruction.h"

//========
mxReconstruction::mxReconstruction() {
  // ctor
  for(int i=0; i!=18; ++i) {
    fHit[i] = new TClonesArray("mxHit",20);
    fPty[i] = new TClonesArray("mxParty",10);
    fNHit[i] = 0;
    fNPty[i] = 0;
  }
  for(int i=0; i!=2; ++i) {
    fCoa[i] = new TClonesArray("mxCoalition",10);
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
    delete fHit[i];
    delete fPty[i];
  }
  for(int i=0; i!=2; ++i) {
    delete fCoa[i];
  }
}
//========
void mxReconstruction::Fill(int idx, float sgn) {
  // filler
  int lyridx = fGeo->LyrIdx( idx );
  int nmax = fHit[lyridx]->GetEntriesFast();
  if(fNHit[lyridx]>nmax-1) fHit[lyridx]->Expand( 2*nmax );
  mxHit *hit = (mxHit*) fHit[lyridx]->At( fNHit[lyridx] );
  if(!hit) hit = (mxHit*) fHit[lyridx]->ConstructedAt( fNHit[lyridx] );
  hit->Fill( idx, sgn );
  fNHit[lyridx]++;
}
//========
void mxReconstruction::Make() {
  // maker
  Parties();
  //Coalitions();
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
      hit = (mxHit*) fHit[lyr]->At( mh );
      //if(hit->IsAssigned()) continue;
      int idx = hit->Idx();
      float x = fGeo->X( idx );
      float y = fGeo->Y( idx );
      bool append = false;
      for(int mp=0; mp!=fNPty[lyr]; ++mp) {
	pty = (mxParty*) fPty[lyr]->At( mp );
	if(pty->Test(x,y)<3) {
	  append = true;
	  break;
	}
      }
      if(!append) {
	int nmax = fPty[lyr]->GetEntriesFast();
	if(fNPty[lyr]>nmax-1) fPty[lyr]->Expand( 2*nmax );
	pty = (mxParty*) fPty[lyr]->ConstructedAt( fNPty[lyr] );
	fNPty[lyr]++;
      }
      pty->Fill( hit, x, y );
      hit->SetAssigned( true );
    }
  }
}
//========
void mxReconstruction::Coalitions() {
  /*
  // forming global coalitions
  mxParty *pty;
  mxCoalition *coa;
  for(int arm=0; arm!=2; ++arm) {
    // dissociating previous unions
    for(int lyr=arm*9; lyr!=(arm+1)*9; ++lyr)
      for(int mp=0; mp!=fNPty[lyr]; ++mp) {
	pty = fPty->At(mp);
	pty->SetAssigned( false );
      }
    // call for coalition formation
    for(int lyr=arm*9+8; lyr!=arm*9+1; --lyr)
      for(int mp=0; mp!=fNPty[lyr]; ++mp) {
	pty = fPty->At(mp);
        if(pty->IsAssigned()) continue;
        // seeding
        coa = new mxCoalition();
        fCoa[arm]->add( coa );
        coa->Fill( pty, fV[0], fV[1], fV[2] );
	pty->SetAssigned( true );
        // check for allignments
        for(int inc=lyr-1; inc!=arm*9-1; --inc)
	  for(int np=0; np!=fNPty[inc]; ++j) {
            if(assig[inc][ pties[inc][j] ]) continue;
            pty = pool->get( pties[inc][j] );
            if( test->Test( pty, fV[0], fV[1], fV[2] ) < 3) {
              test->Fill( pty, fV[0], fV[1], fV[2] );
              assig[inc][ pties[inc][j] ] = true;
              cout << " " << inc << " ";
            }
          }
        cout << endl;
      }
    // DONE with arm
  }
  */
}
