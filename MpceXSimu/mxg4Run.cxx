// origin: S.Karthas (Aug 2016) 

#include "G4Event.hh"
#include "G4SDManager.hh"

#include "mxg4Run.h"
#include "mxg4TrackerHit.h"
#include "mxg4DetectorConstruction.h"

mxg4Run::mxg4Run() :
  G4Run(),
  si_ene(0),
  mpc_ene(0),
  SIHCID(-1),
  MPCHCID(-1) {
}

void mxg4Run::RecordEvent(const G4Event* evt) {
  //Forward call to base class
  G4Run::RecordEvent(evt);
  mxg4DetectorConstruction det;
  if ( SIHCID == -1|| MPCHCID == -1) {
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    if (!det.IsMinis()){SIHCID = sdManager->GetCollectionID("TrackerChamberSD/TrackerHitsCollection");}
    if (det.IsMinis()){SIHCID = sdManager->GetCollectionID("MinipadSD/MinipadHitsCollection");}
    MPCHCID = sdManager->GetCollectionID("MPCSD/MPCHitsCollection");
  }
  G4HCofThisEvent* hce = evt->GetHCofThisEvent();
  if(!hce) {
    G4ExceptionDescription msg;
    msg << "No hits collection of this event found.\n";
    G4Exception("Run::RecordEvent()","Code001", JustWarning, msg);
    return;
  }
  const TrackerHitsCollection* siHC = static_cast<const TrackerHitsCollection*>(hce->GetHC(SIHCID));
  const TrackerHitsCollection* mpcHC = static_cast<const TrackerHitsCollection*>(hce->GetHC(MPCHCID));
  if( !siHC || !mpcHC ) {
    G4ExceptionDescription msg;
    msg << "Some of hits collections of this event not found.\n";
    G4Exception("Run::RecordEvent()","Code001", JustWarning, msg);
    return;
  }
  G4double si = 0.;
  G4double mpc = 0.;
  for(size_t i=0;i<siHC->GetSize();i++) {
    mxg4TrackerHit* hit = (*siHC)[i];
    si += hit->GetEdep();
  }
  for(size_t i=0;i<mpcHC->GetSize();i++) {
    mxg4TrackerHit* hit = (*mpcHC)[i];
    mpc += hit->GetEdep();
  }
  mpc_ene += mpc;
  si_ene += si;
}

void mxg4Run::Merge(const G4Run*) {
  /* const Run* localRun = static_cast<const Run*>(aRun);
  em_ene += localRun->GetEmEnergy();
  had_ene += localRun->GetHadEnergy();
  shower_shape += localRun->GetShowerShape();
  //Forward call to base-class
  G4Run::Merge(localRun);*/
}
