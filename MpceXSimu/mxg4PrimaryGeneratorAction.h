// origin: S.Karthas (Aug 2016)

#ifndef mxg4PrimaryGeneratorAction_h
#define mxg4PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;

class mxg4PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  mxg4PrimaryGeneratorAction();    
  virtual ~mxg4PrimaryGeneratorAction();
  
  virtual void GeneratePrimaries(G4Event* );
  
  G4GeneralParticleSource* GetParticleGun() {return fParticleGun;}
  void SetRandomFlag(G4bool );
  
private:
  G4GeneralParticleSource* fParticleGun;
};

#endif
