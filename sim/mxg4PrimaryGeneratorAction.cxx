// origin: S.Karthas (Aug 2016)

#include "mxg4PrimaryGeneratorAction.h"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "Randomize.hh"
#include "time.h"

mxg4PrimaryGeneratorAction::mxg4PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction() {
  G4int nofParticles = 1;
  fParticleGun = new G4GeneralParticleSource();
  fParticleGun->SetNumberOfParticles(nofParticles);
}

mxg4PrimaryGeneratorAction::~mxg4PrimaryGeneratorAction() {
  delete fParticleGun;
}

void mxg4PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
