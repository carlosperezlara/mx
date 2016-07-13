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
// $Id: B2EventAction.cc 75214 2013-10-29 16:04:42Z gcosmo $
//
/// \file B2EventAction.cc
/// \brief Implementation of the B2EventAction class

#include "B2EventAction.hh"
#include "B2TrackerHit.hh"
#include "Analysis.hh"
#include "B2RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2EventAction::B2EventAction()
  : G4UserEventAction(),
    fMPCEXHCID(-1),
    fMPCHCID(-1)
{
  // set printing per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2EventAction::~B2EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2EventAction::BeginOfEventAction(const G4Event*)
{
  if (fMPCEXHCID==-1) {
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    fMPCEXHCID = sdManager->GetCollectionID("TrackerChamberSD/TrackerHitsCollection");
    fMPCHCID  = sdManager->GetCollectionID("MPCSD/MPCHitsCollection");
  }
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2EventAction::EndOfEventAction(const G4Event* event)
{
  // get number of stored trajectories

  /*  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  */
  // periodic printing

  /*
  G4int eventID = event->GetEventID();
  if ( eventID < 100 || eventID % 100 == 0) {
    G4cout << ">>> Event: " << eventID  << G4endl;
    if ( trajectoryContainer ) {
      G4cout << "    " << n_trajectories
             << " trajectories stored in this event." << G4endl;
    }
    G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
    G4cout << "    "  
           << hc->GetSize() << " hits stored in this event" << G4endl;
  }
  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  if (!hce)
    {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found.\n";
      G4Exception("EventAction::EndOfEventAction()","Code001", JustWarning, msg);
      return;
    } 
  */
  
 
  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  if (!hce)
    {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found.\n";
      G4Exception("EventAction::EndOfEventAction()","Code001", JustWarning, msg);
      return;
    }           
  // Get hits collections 
  B2TrackerHitsCollection* mpcexHC = static_cast<B2TrackerHitsCollection*>(hce->GetHC(fMPCEXHCID));
  B2TrackerHitsCollection* mpcHC = static_cast<B2TrackerHitsCollection*>(hce->GetHC(fMPCHCID));

  if (!mpcexHC){
    G4ExceptionDescription msg;
    msg << "Some of hits collections of this event not found.\n";
    G4Exception("EventAction::EndOfEventAction()",
		"Code001", JustWarning, msg);
    return;
  }

  // Fill Histograms and ntuples

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  G4int n_hit = mpcHC->entries();
  //analysisManager->FillH1(0, n_hit);
  
  G4cout << "mpchit: " <<n_hit << G4endl;
  analysisManager->FillNtupleIColumn(1,n_hit);

  n_hit = mpcexHC->entries();
  G4cout << "mpcexhit: " <<n_hit << G4endl;
  analysisManager->FillNtupleIColumn(0,n_hit);
  G4int totalMPCEXHit = 0;
  G4double totalMPCEXE = 0.;
  //  B2RunAction* runaction = new B2RunAction();
  G4int count = 0;
  //std::vector<G4double> ene;
  //std::vector<G4int> mini;
  for (G4int i=0; i<n_hit; i++){
    B2TrackerHit* hit = (*mpcexHC)[i];
    G4double eDep = hit->GetEdep();
    G4int minnum = hit->GetChamberNb();
    if (eDep>0.){
      //       analysisManager->FillNtupleIColumn(4, i);
      // analysisManager->FillNtupleDColumn(5, eDep);
      //fEnergies.push_back(eDep);
      //fMinipads.push_back(i);
      //runaction->SetMinipads(i);
      //runaction->SetEnergies(eDep);
      // std::cout << eDep << std::endl;
      minipads.push_back(minnum);
      energies.push_back(eDep);
      if (i%50 == 0 ){      std::cout << i<< std::endl;}
      //analysisManager->FillNtupleIColumn(4,i);
      //analysisManager->FillNtupleDColumn(5,eDep);
      totalMPCEXHit++;
      totalMPCEXE += eDep;
      count ++;
    }
  }
  // analysisManager->FillNtupleIColumn(4,mini);
  // analysisManager->FillNtupleDColumn(5,ene);
  analysisManager->FillNtupleDColumn(2, totalMPCEXE);
  std::cout << "Going to add Rows" << std::endl;
  analysisManager->AddNtupleRow();
  //  runaction->PrintVector();
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
