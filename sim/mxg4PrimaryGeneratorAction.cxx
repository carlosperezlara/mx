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
  //  fParticleGun = new G4ParticleGun(nofParticles);
  fParticleGun = new G4GeneralParticleSource();
  fParticleGun->SetNumberOfParticles(nofParticles);
  
  // default particle kinematic
  /*
  G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle("gamma");//"proton");
  
  G4double rand = G4UniformRand();
  G4double Ekin = 5.5 - rand;
  G4double phi = G4UniformRand()*twopi;
  G4double eta = 3.6 - G4UniformRand()*0.3;
  G4double theta = 2.0*std::atan(std::exp(-eta));
  
  std::cout << "eta = " << eta << " theta = " << theta << " Ekin = " << Ekin<< " phi = "<< phi << std::endl; 
*/
  //fGPS->GetParticleEnergy()->;
  /*
  G4double pz = Ekin*(std::exp(eta)-1)/(std::exp(eta)+1);
  G4double mass = particleDefinition->GetPDGMass();
  G4double p = sqrt(Ekin*Ekin-pz*pz - mass*mass);

  G4double px = p*std::sin(phi);
  G4double py = p*std::cos(phi);
  G4double pp = px*px + py*py + pz*pz;

  px = px/sqrt(pp); py = py/sqrt(pp); pz = pz/sqrt(pp);
  
 
 fParticleGun->SetParticleMomentum(sqrt(pp)*GeV);
  */
  /*
  G4double angle = (G4UniformRand()-0.5)*sigmaAngle;
  G4double pp =
    momentum+(G4UniformRand()-0.5)*sigmaMomentum;
  G4double mass = particleDefinition->GetPDGMass();
  G4double Ekin = sqrt(pp*pp+mass*mass)-mass;
  fparticleGun->SetParticleEnergy(Ekin);
  */
  //G4double angle = pi/2.0;
  //fParticleGun->SetParticleDefinition(particleDefinition);

  //  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,std::sin(pi/4.0),std::cos(pi/4.0)));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,-1*std::sin(pi/45.0),std::cos(pi/45.0)));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(std::sin(phi)*std::sin(theta),std::cos(phi)*std::sin(theta),std::cos(theta)));
  //fParticleGun->SetParticleEnergy(Ekin*GeV);
  
}

mxg4PrimaryGeneratorAction::~mxg4PrimaryGeneratorAction() {
  delete fParticleGun;
}

void mxg4PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore.
  /*
  G4double worldZHalfLength = 0;
  G4LogicalVolume* worldLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Box* worldBox = NULL;
  if ( worldLV ) worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  if ( worldBox ) worldZHalfLength = worldBox->GetZHalfLength();
  else  {
    G4cerr << "World volume of box not found." << G4endl;
    G4cerr << "Perhaps you have changed geometry." << G4endl;
    G4cerr << "The gun will be place in the center." << G4endl;
  }

  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));//-worldZHalfLength));
  */
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
