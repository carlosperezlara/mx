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
// $Id: EventAction.hh 75214 2013-10-29 16:04:42Z gcosmo $
//
/// \file EventAction.hh
/// \brief Definition of the EventAction class

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"

#include "globals.hh"
#include <vector>

/// Event action class

class EventAction : public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();
  
  virtual void  BeginOfEventAction(const G4Event* );
  virtual void    EndOfEventAction(const G4Event* );
  std::vector<G4int>& GetMinipads() { return minipads; }
  std::vector<G4double>& GetMiniEnergies() { return minienergies; }
  std::vector<G4int>& GetCrystals() { return crystals; }
  std::vector<G4double>& GetMPCEnergies() { return mpcenergies; }
  void ClearVectors(){minipads.clear();
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif