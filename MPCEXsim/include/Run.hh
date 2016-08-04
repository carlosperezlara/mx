// \brief Definition of Run class

#ifndef RUN_HH
#define RUN_HH

#include "G4Run.hh"

class G4Event;

//=================================
// Exercise 1 Step 1:
// Create a user-defined Run class
// This class accumulates the energy
// in Hadronic and Em calorimeters for the
// entire run. Energy is taken from hits.
// Add two datamembers that will hold the accumulated energy
// As an optimization, instead of using the hits collection
// names to search them, let's use their ID. Add two integers
// that holds IDs.

class Run : public G4Run {
public:
  Run();
  virtual ~Run() {};

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
