// origin: S.Karthas (Aug 2016)

#include "mxg4DetectorMessenger.h"
#include "mxg4DetectorConstruction.h"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

mxg4DetectorMessenger::mxg4DetectorMessenger(mxg4DetectorConstruction* Det)
 : G4UImessenger(),
   fDetectorConstruction(Det) {
  fMXDirectory = new G4UIdirectory("/MPCEXsim/");
  fMXDirectory->SetGuidance("UI commands specific to this example.");

  fDetDirectory = new G4UIdirectory("/MPCEXsim/det/");
  fDetDirectory->SetGuidance("Detector construction control");

  fTargMatCmd = new G4UIcmdWithAString("/MPCEXsim/det/setTargetMaterial",this);
  fTargMatCmd->SetGuidance("Select Material of the Target.");
  fTargMatCmd->SetParameterName("choice",false);
  fTargMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fChamMatCmd = new G4UIcmdWithAString("/MPCEXsim/det/setChamberMaterial",this);
  fChamMatCmd->SetGuidance("Select Material of the Chamber.");
  fChamMatCmd->SetParameterName("choice",false);
  fChamMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fMpcMatCmd = new G4UIcmdWithAString("/MPCEXsim/det/setMpcMaterial",this);
  fMpcMatCmd->SetGuidance("Select Material of the MPC.");
  fMpcMatCmd->SetParameterName("choice",false);
  fMpcMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fMiniMatCmd = new G4UIcmdWithAString("/MPCEXsim/det/setMinipadMaterial",this);
  fMiniMatCmd->SetGuidance("Select Material of the Minipads.");
  fMiniMatCmd->SetParameterName("choice",false);
  fMiniMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/MPCEXsim/det/stepMax",this);
  fStepMaxCmd->SetGuidance("Define a step max");
  fStepMaxCmd->SetParameterName("stepMax",false);
  fStepMaxCmd->SetUnitCategory("Length");
  fStepMaxCmd->AvailableForStates(G4State_Idle);
}

mxg4DetectorMessenger::~mxg4DetectorMessenger() {
  delete fTargMatCmd;
  delete fChamMatCmd;
  delete fMpcMatCmd;
  delete fMiniMatCmd;
  delete fStepMaxCmd;
  delete fMXDirectory;
  delete fDetDirectory;
}

void mxg4DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue) {
  if( command == fTargMatCmd )
   { fDetectorConstruction->SetTargetMaterial(newValue);}

  if( command == fChamMatCmd )
   { fDetectorConstruction->SetChamberMaterial(newValue);}
  if( command == fChamMatCmd )
    { fDetectorConstruction->SetMpcMaterial(newValue);}

  if( command == fMiniMatCmd )
    { fDetectorConstruction->SetMinipadMaterial(newValue);}
  
  if( command == fStepMaxCmd ) {
    fDetectorConstruction
      ->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
  }   
}
