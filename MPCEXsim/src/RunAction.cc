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
// $Id: RunAction.cc 75214 2013-10-29 16:04:42Z gcosmo $
//
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"
#include "EventAction.hh"
#include "Analysis.hh"
#include "Run.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
  : G4UserRunAction()
{
  
  // set printing event number per each 100 events
  G4RunManager::GetRunManager()->SetPrintProgress(1000);     
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(4);
  G4cout << "Using " << analysisManager->GetType() << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  const EventAction* constEventAction = static_cast<const EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction());
  EventAction* eventAction = const_cast<EventAction*>(constEventAction);

  eventAction->ClearVectors();
  
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("MPCEXfile");

  analysisManager->CreateNtuple("MPCEXfile", "Hits");
  analysisManager->CreateNtupleIColumn("MPCEXHits");  // column Id = 0
  analysisManager->CreateNtupleIColumn("MPCHits");  // column Id = 1
  analysisManager->CreateNtupleDColumn("MPCEXEnergy"); // column Id = 2
  analysisManager->CreateNtupleDColumn("MPCEnergy"); // column Id = 3
  analysisManager->CreateNtupleIColumn("MPCEXHitminipads",eventAction->GetMinipads()); // column Id = 4
  analysisManager->CreateNtupleDColumn("MPCEXEnergysens",eventAction->GetMiniEnergies()); // column Id = 5
  analysisManager->CreateNtupleIColumn("MPCHitcrystals",eventAction->GetCrystals()); // column Id = 6
  analysisManager->CreateNtupleDColumn("MPCEnergysens",eventAction->GetMPCEnergies()); // column Id = 7
  
  analysisManager->FinishNtuple();
   
  // Open an output file
  // The default file name is set in RunAction::RunAction(),
  // it can be overwritten in a macro
  analysisManager->OpenFile();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  std::cout << "Entered EndofRunAction" << std::endl;
  const Run* myrun = dynamic_cast<const Run*>(run);
  if ( myrun )
    {
      G4int nEvets = myrun->GetNumberOfEvent();
      if ( nEvets < 1 )
	{
	  G4ExceptionDescription msg;
	  msg << "Run consists of 0 events";
	  G4Exception("RunAction::EndOfRunAction()",
		      "Code001", JustWarning, msg);
	  nEvets=1;
	}
      G4double si_ene = myrun->GetSiEnergy();
      G4cout<<"Run["<<myrun->GetRunID()<<"] With: "<<nEvets<<"Events\n"
	    <<" <Si_em>="<<G4BestUnit(si_ene/nEvets,"Energy")<<G4endl;//<<"\n"
    } else {
    G4ExceptionDescription msg;
    msg << "Run is not of correct type, skypping analysis via RunAction";
    G4Exception("RunAction::EndOfRunAction()",
		"Code001", JustWarning, msg);
  }

  // Write and close output file
  // save histograms & ntuple

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}

G4Run* RunAction::GenerateRun() {
  return new Run;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
