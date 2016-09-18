// origin: S.Karthas (Aug 2016) 

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "time.h"

#include "mxg4RunAction.h"
#include "mxg4EventAction.h"
#include "mxg4Analysis.h"
#include "mxg4Run.h"

mxg4RunAction::mxg4RunAction()
  : G4UserRunAction() {
  // set printing event number per each 100 events
  G4RunManager::GetRunManager()->SetPrintProgress(1000);     
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(4);
  G4cout << "Using " << analysisManager->GetType() << G4endl;
}

mxg4RunAction::~mxg4RunAction() {
  delete G4AnalysisManager::Instance();
}

void mxg4RunAction::BeginOfRunAction(const G4Run* /*run*/) {
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");

  if(1) {
    long seeds[2];
    time_t systime = time(NULL);
    seeds[0] = (long) systime;
    seeds[1] = (long) (systime*G4UniformRand());
    G4Random::setTheSeeds(seeds);
    G4Random::showEngineStatus();
  } else {
    G4Random::showEngineStatus();
  }

  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  const mxg4EventAction* constEventAction = static_cast<const mxg4EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction());
  mxg4EventAction* eventAction = const_cast<mxg4EventAction*>(constEventAction);

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
  analysisManager->OpenFile();/*
  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  G4long seed = time(NULL);
  CLHEP::HepRandom::setTheSeed(seed);*/
}

void mxg4RunAction::EndOfRunAction(const G4Run* run) {
  std::cout << "Entered EndofRunAction" << std::endl;
  G4Random::showEngineStatus();

  const mxg4Run* myrun = dynamic_cast<const mxg4Run*>(run);
  if( myrun ) {
    G4int nEvets = myrun->GetNumberOfEvent();
    if( nEvets < 1 ) {
      G4ExceptionDescription msg;
      msg << "Run consists of 0 events";
      G4Exception("RunAction::EndOfRunAction()","Code001", JustWarning, msg);
      nEvets=1;
    }
    G4double si_ene = myrun->GetSiEnergy();
    //G4cout<<"Run["<<myrun->GetRunID()<<"] With: "<<nEvets<<"Events\n"
    //	  <<" <Si_em>="<<G4BestUnit(si_ene/nEvets,"Energy")<<G4endl;//<<"\n"
  } else {
    G4ExceptionDescription msg;
    msg << "Run is not of correct type, skypping analysis via RunAction";
    G4Exception("RunAction::EndOfRunAction()","Code001", JustWarning, msg);
  }

  // Write and close output file
  // save histograms & ntuple

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}

G4Run* mxg4RunAction::GenerateRun() {
  return new mxg4Run;
}

