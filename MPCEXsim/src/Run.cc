//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//

#include "Run.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "TrackerHit.hh"
#include "DetectorConstruction.hh"

Run::Run() :
  G4Run(),
  si_ene(0),
  mpc_ene(0),
  SIHCID(-1),
  MPCHCID(-1)
{ }

void Run::RecordEvent(const G4Event* evt)
{
  //Forward call to base class
  G4Run::RecordEvent(evt);
  DetectorConstruction det;

  if ( SIHCID == -1|| MPCHCID == -1) {
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    if (!det.IsMinis()){SIHCID = sdManager->GetCollectionID("TrackerChamberSD/TrackerHitsCollection");}
    if (det.IsMinis()){SIHCID = sdManager->GetCollectionID("MinipadSD/MinipadHitsCollection");}
    MPCHCID = sdManager->GetCollectionID("MPCSD/MPCHitsCollection");
  }
  G4HCofThisEvent* hce = evt->GetHCofThisEvent();
  if (!hce) {
    G4ExceptionDescription msg;
    msg << "No hits collection of this event found.\n";
    G4Exception("Run::RecordEvent()",
		"Code001", JustWarning, msg);
    return;

  }
  const TrackerHitsCollection* siHC =
    static_cast<const TrackerHitsCollection*>(hce->GetHC(SIHCID));
  const TrackerHitsCollection* mpcHC =
    static_cast<const TrackerHitsCollection*>(hce->GetHC(MPCHCID));
  if ( !siHC || !mpcHC )
    {
      G4ExceptionDescription msg;
      msg << "Some of hits collections of this event not found.\n";
      G4Exception("Run::RecordEvent()",
		  "Code001", JustWarning, msg);
      return;
      
    }
  G4double si = 0.;
  G4double mpc = 0.;
  
  for (size_t i=0;i<siHC->GetSize();i++)
    {
      TrackerHit* hit = (*siHC)[i];
      si += hit->GetEdep();
    }
  for (size_t i=0;i<mpcHC->GetSize();i++)
    {
      TrackerHit* hit = (*mpcHC)[i];
      mpc += hit->GetEdep();
    }
  mpc_ene += mpc;
  si_ene += si;
}

void Run::Merge(const G4Run* aRun)
{
  /* const Run* localRun = static_cast<const Run*>(aRun);
  em_ene += localRun->GetEmEnergy();
  had_ene += localRun->GetHadEnergy();
  shower_shape += localRun->GetShowerShape();
  //Forward call to base-class
  G4Run::Merge(localRun);*/
}
