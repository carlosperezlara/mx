// origin: S.Karthas (Aug 2016) 

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "mxg4TrackerSD.h"

mxg4TrackerSD::mxg4TrackerSD(const G4String& name,
			     const G4String& hitsCollectionName) 
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL) {
  collectionName.insert(hitsCollectionName);
}

mxg4TrackerSD::~mxg4TrackerSD() {
}

void mxg4TrackerSD::Initialize(G4HCofThisEvent* hce) {
  fHitsCollection = new TrackerHitsCollection(SensitiveDetectorName, collectionName[0]); 
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 
}

G4bool mxg4TrackerSD::ProcessHits(G4Step* aStep, 
				  G4TouchableHistory*) {
  // energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit();

  if (edep==0.) return false;

  mxg4TrackerHit* newHit = new mxg4TrackerHit();

  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()
                                               ->GetCopyNumber());
  newHit->SetEdep(edep);
  newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());

  fHitsCollection->insert( newHit );

  //newHit->Print();

  return true;
}

void mxg4TrackerSD::EndOfEvent(G4HCofThisEvent*) {
  if ( verboseLevel>1 ) { 
     G4int nofHits = fHitsCollection->entries();
     G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits 
            << " hits in the tracker chambers: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
}
