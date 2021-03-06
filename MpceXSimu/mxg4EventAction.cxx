// origin: S.Karthas (Aug 2016) 

#include "Randomize.hh"
#include "time.h"
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

#include "mxg4EventAction.h"
#include "mxg4TrackerHit.h"
#include "mxg4Analysis.h"
#include "mxg4RunAction.h"
#include "mxg4DetectorConstruction.h"

mxg4EventAction::mxg4EventAction()
  : G4UserEventAction(),
    fMPCEXHCID(-1),
    fMPCHCID(-1),
    fMINIHCID(-1) {
  G4RunManager::GetRunManager()->SetPrintProgress(1);
}

mxg4EventAction::~mxg4EventAction() {
}

void mxg4EventAction::BeginOfEventAction(const G4Event*) {
  ClearVectors();
  if (fMPCEXHCID==-1) {
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    mxg4DetectorConstruction det;
    sdManager->ListTree();
    if (!det.IsMinis()) {
      std::cout << "I didn't want to enter this loop" << std::endl;
      fMPCEXHCID = sdManager->GetCollectionID("TrackerChamberSD/TrackerHitsCollection");
    }
    fMPCHCID  = sdManager->GetCollectionID("MPCSD/MPCHitsCollection");
    if (det.IsMinis()) {
      fMINIHCID = sdManager->GetCollectionID("MinipadSD/MinipadHitsCollection");
    }
  }
}

void mxg4EventAction::EndOfEventAction(const G4Event* event) {
  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  G4PrimaryVertex* primaryVertex = event->GetPrimaryVertex();
  G4int npart = primaryVertex->GetNumberOfParticle();
  std::cout << "******** PRIMARIES " << npart << " *******" << std::endl;
  for(int i=0; i!=npart; ++i) {
    G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary(i);
    std::cout << "PART => " << primaryParticle->GetPDGcode() << std::endl;
  }
  G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary(0);
  G4double ke = primaryParticle->GetKineticEnergy();
  // G4double px = primaryParticle->GetMomentum().getX();
  // G4double py = primaryParticle->GetMomentum().getY();
  // G4double pz = primaryParticle->GetMomentum().getZ();
  G4double peta = primaryParticle->GetMomentum().getEta();
  G4double pphi = primaryParticle->GetMomentum().getPhi();
  G4int trackid = primaryParticle->GetTrackID();
  G4int pdgid = primaryParticle->GetPDGcode();
  
  std::cout<< ke << " " << trackid <<" "<< peta <<" "<< pphi << std::endl;
  if(!hce) {
    G4ExceptionDescription msg;
    msg << "No hits collection of this event found.\n";
    G4Exception("EventAction::EndOfEventAction()","Code001", JustWarning, msg);
    return;
  }
  mxg4DetectorConstruction det;
  TrackerHitsCollection* mpcexHC;
  // Get hits collections 
  if(!det.IsMinis()) {
    mpcexHC = static_cast<TrackerHitsCollection*>(hce->GetHC(fMPCEXHCID));
  } else {
    mpcexHC = static_cast<TrackerHitsCollection*>(hce->GetHC(fMINIHCID));
  }
  TrackerHitsCollection* mpcHC = static_cast<TrackerHitsCollection*>(hce->GetHC(fMPCHCID));
  if(!mpcexHC) {
    G4ExceptionDescription msg;
    msg << "Some of hits collections of this event not found.\n";
    G4Exception("EventAction::EndOfEventAction()",
		"Code001", JustWarning, msg);
    return;
  }

  //============================
  // Fill Histograms and ntuples
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  G4int n_mpchit = mpcHC->entries();
  //G4cout << "mpchit: " <<n_mpchit << G4endl;
  analysisManager->FillNtupleIColumn(1,n_mpchit);

  G4int  n_mpcexhit = mpcexHC->entries();
  //G4cout << "mpcexhit: " <<n_mpcexhit << G4endl;
  analysisManager->FillNtupleIColumn(0,n_mpcexhit);

  G4int totalMPCEXHit = 0;
  G4double totalMPCEXE = 0.;
  G4int totalMPCHit = 0;
  G4double totalMPCE = 0.;

  G4int count = 0;
  G4double miniarray[49152] = {0.0};

  for(G4int i=0; i<n_mpcexhit; i++) {
    mxg4TrackerHit* hit = (*mpcexHC)[i];
    G4double eDep = hit->GetEdep();
    G4int extrackID = hit->GetTrackID();
    G4int minnum = hit->GetChamberNb();
    if(eDep>0.) {
      // if (extrackID <100 )std::cout <<"Track ID is " <<extrackID<< std::endl;
      miniarray[minnum] += eDep;
      // if (i%50 == 0 ){std::cout << i<< std::endl;}
      //      totalMPCEXHit++;
      //  totalMPCEXE += eDep;
      count ++;
    }
  }
  for(G4int j = 0; j<49152; j++) {
    if(miniarray[j] > 0.010) { //0.0 1.5
      minipads.push_back(j);
      minienergies.push_back(miniarray[j]);
      totalMPCEXHit++;
      totalMPCEXE += miniarray[j];
    }      
  }
  G4double mpcarray[416] = {0.0};
  for(G4int i=0; i<n_mpchit; i++) {
    mxg4TrackerHit* mpchit = (*mpcHC)[i];
    G4double mpceDep = mpchit->GetEdep();
    G4int mpctrackID = mpchit->GetTrackID();
    G4int crystalnum = mpchit->GetChamberNb();
    if(mpceDep>0.0) {
      mpcarray[crystalnum] += mpceDep;
      //if (mpctrackID < 100) std::cout << "Track ID is " <<mpctrackID<< std::endl;
      //      if (i%50 == 0 ){      std::cout << i<< std::endl;}
      // totalMPCHit++;
      //totalMPCE += mpceDep;
      count ++;
    }
  }
  for(G4int j = 0; j<416; j++) {
    if(mpcarray[j] > 1) { //DEFAULT should be 1!!! 0.0 1.0 10.0 100.0
       crystals.push_back(j);
       mpcenergies.push_back(mpcarray[j]);
       totalMPCHit++;
       totalMPCE += mpcarray[j];
    }
  }
  analysisManager->FillNtupleDColumn(2, totalMPCEXE);
  analysisManager->FillNtupleDColumn(3, totalMPCE);
  analysisManager->FillNtupleDColumn(8, ke);
  analysisManager->FillNtupleDColumn(9, pphi);
  analysisManager->FillNtupleDColumn(10, peta);
  analysisManager->FillNtupleIColumn(11, pdgid);
  analysisManager->AddNtupleRow();
  //  runaction->PrintVector();
}  
