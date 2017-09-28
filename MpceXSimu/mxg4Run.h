// origin: S.Karthas (Aug 2016)

#ifndef mxg4RUN_HH
#define mxg4RUN_HH

#include "G4Run.hh"

class G4Event;

class mxg4Run : public G4Run {
public:
  mxg4Run();
  virtual ~mxg4Run() {};

  virtual void RecordEvent(const G4Event*);
  virtual void Merge(const G4Run*);
  G4double GetSiEnergy() const { return si_ene; }
  G4double GetMPCEnergy() const { return mpc_ene; }
  
private:
  G4double si_ene;
  G4double mpc_ene;
  G4int SIHCID;
  G4int MPCHCID;
};
#endif
