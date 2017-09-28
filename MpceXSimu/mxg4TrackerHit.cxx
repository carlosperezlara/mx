// origin: S.Karthas (Aug 2016) 

#include <iomanip>

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include "mxg4TrackerHit.h"

G4ThreadLocal G4Allocator<mxg4TrackerHit>* TrackerHitAllocator=0;

mxg4TrackerHit::mxg4TrackerHit()
 : G4VHit(),
   fTrackID(-1),
   fChamberNb(-1),
   fEdep(0.),
   fPos(G4ThreeVector()) {
}

mxg4TrackerHit::~mxg4TrackerHit() {
}

mxg4TrackerHit::mxg4TrackerHit(const mxg4TrackerHit& right)
  : G4VHit() {
  fTrackID   = right.fTrackID;
  fChamberNb = right.fChamberNb;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
}

const mxg4TrackerHit& mxg4TrackerHit::operator=(const mxg4TrackerHit& right) {
  fTrackID   = right.fTrackID;
  fChamberNb = right.fChamberNb;
  fEdep      = right.fEdep;
  fPos       = right.fPos;

  return *this;
}

G4int mxg4TrackerHit::operator==(const mxg4TrackerHit& right) const {
  return ( this == &right ) ? 1 : 0;
}

void mxg4TrackerHit::Draw() {
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) {
    G4Circle circle(fPos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

void mxg4TrackerHit::Print() {
  G4cout
     << "  trackID: " << fTrackID << " chamberNb: " << fChamberNb
     << "Edep: "
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << " Position: "
     << std::setw(7) << G4BestUnit( fPos,"Length")
     << G4endl;
}
