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
//#include "HadCalorimeterHit.hh"
//#include "EmCalorimeterHit.hh"
#include "B2TrackerHit.hh"
#include "B2aDetectorConstruction.hh"

Run::Run() :
  G4Run(),
  si_ene(0),
  mpc_ene(0),
  //shower_shape(0),
  SIHCID(-1),
  MPCHCID(-1)
{ }


//=================================
// Exercise 1 Step 2:
// Get the hits collection from the event
// For the EM and HAD calorimeter sensitive detectors
// Accumulate on them
// Do not forget to forward the call to the base class RecordEvent
// method

void Run::RecordEvent(const G4Event* evt)
{
  //Forward call to base class
  G4Run::RecordEvent(evt);
  B2aDetectorConstruction det;

  if ( SIHCID == -1|| MPCHCID == -1) {
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    if (!det.IsMinis()){SIHCID = sdManager->GetCollectionID("TrackerChamberSD/TrackerHitsCollection");}//EMcalorimeter/EMcalorimeterColl");
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
  const B2TrackerHitsCollection* siHC =
    static_cast<const B2TrackerHitsCollection*>(hce->GetHC(SIHCID));
  const B2TrackerHitsCollection* mpcHC =
    static_cast<const B2TrackerHitsCollection*>(hce->GetHC(MPCHCID));
  //          const HadCalorimeterHitsCollection* hadHC =
      //    static_cast<const HadCalorimeterHitsCollection*>(hce->GetHC(HCHCID));
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
	  
	  // G4double had = 0;
	  for (size_t i=0;i<siHC->GetSize();i++)
	    {
	      B2TrackerHit* hit = (*siHC)[i];
	      si += hit->GetEdep();
	    }
	  for (size_t i=0;i<mpcHC->GetSize();i++)
	    {
	      B2TrackerHit* hit = (*mpcHC)[i];
	      mpc += hit->GetEdep();
	    }
	  mpc_ene += mpc;
	  si_ene += si;
	  // if ( had+em > 0 )
	  //  shower_shape += ( em/(had+em) );
}

//=================================
// Exercise 1 Step 3:
// Implement a Merge method.
// The meaning of this will become clear
// When Multi-threading is discussed.
// This method of a MASTER instance is called
// By all worker threads passing WORKER run as
// parameter. Get the energy from WORKER run and
// Accumulate in the MASTER
// Do not forget to forward the call to the base class
// method to accumulate the number of processed events!
void Run::Merge(const G4Run* aRun)
{
  /* const Run* localRun = static_cast<const Run*>(aRun);
  em_ene += localRun->GetEmEnergy();
  had_ene += localRun->GetHadEnergy();
  shower_shape += localRun->GetShowerShape();
  //Forward call to base-class
  G4Run::Merge(localRun);*/
}
