// origin: S.Karthas (Aug 2016)

#ifndef mxg4RunAction_h
#define mxg4RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <vector>

class G4Run;

class mxg4RunAction : public G4UserRunAction {
 public:
  mxg4RunAction();
  virtual ~mxg4RunAction();
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);
  virtual G4Run* GenerateRun();
};

#endif
