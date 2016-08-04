//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: PrimaryGeneratorAction.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "Randomize.hh"
#include "time.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction()
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic

  G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle("gamma");//"proton");
  
  G4double rand = G4UniformRand();
  G4double Ekin = 5.5 - rand;
  G4double phi = G4UniformRand()*twopi;
  G4double eta = 3.6 - G4UniformRand()*0.3;
  G4double theta = 2.0*std::atan(std::exp(-eta));
  
  std::cout << "eta = " << eta << " theta = " << theta << " Ekin = " << Ekin<< " phi = "<< phi << std::endl; 
  
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
  fParticleGun->SetParticleDefinition(particleDefinition);

  //  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,std::sin(pi/4.0),std::cos(pi/4.0)));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,-1*std::sin(pi/45.0),std::cos(pi/45.0)));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(std::sin(phi)*std::sin(theta),std::cos(phi)*std::sin(theta),std::cos(theta)));
  fParticleGun->SetParticleEnergy(Ekin*GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore.

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

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......