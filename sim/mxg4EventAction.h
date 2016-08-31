// origin: S.Karthas (Aug 2016)

#ifndef mxg4EventAction_h
#define mxg4EventAction_h 1

#include "G4UserEventAction.hh"

#include "globals.hh"
#include <vector>

class mxg4EventAction : public G4UserEventAction
{
public:
  mxg4EventAction();
  virtual ~mxg4EventAction();
  
  virtual void BeginOfEventAction(const G4Event* );
  virtual void EndOfEventAction(const G4Event* );
  std::vector<G4int>& GetMinipads() { return minipads; }
  std::vector<G4double>& GetMiniEnergies() { return minienergies; }
  std::vector<G4int>& GetCrystals() { return crystals; }
  std::vector<G4double>& GetMPCEnergies() { return mpcenergies; }
  void ClearVectors(){
    minipads.clear();
    minienergies.clear();
    crystals.clear();
    mpcenergies.clear();
  };
private:
  G4int fMPCEXHCID;
  G4int fMPCHCID;
  G4int fMINIHCID;
  
  std::vector<G4int> minipads;
  std::vector<G4double> minienergies;
  std::vector<G4int> crystals;
  std::vector<G4double> mpcenergies;
};

#endif
