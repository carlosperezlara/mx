// origin: S.Karthas (Aug 2016)

#ifndef mxg4TrackerSD_h
#define mxg4TrackerSD_h 1

#include "G4VSensitiveDetector.hh"

#include "mxg4TrackerHit.h"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class mxg4TrackerSD : public G4VSensitiveDetector {
 public:
  mxg4TrackerSD(const G4String& name, 
		const G4String& hitsCollectionName);
  virtual ~mxg4TrackerSD();
  
  virtual void   Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);
  
 private:
  TrackerHitsCollection* fHitsCollection;
};

#endif
