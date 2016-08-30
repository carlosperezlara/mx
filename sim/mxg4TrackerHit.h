// origin: S.Karthas (Aug 2016)

#ifndef mxg4TrackerHit_h
#define mxg4TrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

class mxg4TrackerHit : public G4VHit
{
  public:
    mxg4TrackerHit();
    mxg4TrackerHit(const mxg4TrackerHit&);
    virtual ~mxg4TrackerHit();

    const mxg4TrackerHit& operator=(const mxg4TrackerHit&);
    G4int operator==(const mxg4TrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);
    virtual void Draw();
    virtual void Print();
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetChamberNb(G4int chamb)      { fChamberNb = chamb; };
    void SetEdep     (G4double de)      { fEdep = de; };
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
    G4int GetTrackID() const     { return fTrackID; };
    G4int GetChamberNb() const   { return fChamberNb; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };

  private:

      G4int         fTrackID;
      G4int         fChamberNb;
      G4double      fEdep;
      G4ThreeVector fPos;
};

typedef G4THitsCollection<mxg4TrackerHit> TrackerHitsCollection;

extern G4ThreadLocal G4Allocator<mxg4TrackerHit>* TrackerHitAllocator;

inline void* mxg4TrackerHit::operator new(size_t)
{
  if(!TrackerHitAllocator)
      TrackerHitAllocator = new G4Allocator<mxg4TrackerHit>;
  return (void *) TrackerHitAllocator->MallocSingle();
}

inline void mxg4TrackerHit::operator delete(void *hit)
{
  TrackerHitAllocator->FreeSingle((mxg4TrackerHit*) hit);
}

#endif
