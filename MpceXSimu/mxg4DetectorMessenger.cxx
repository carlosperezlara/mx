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

  fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/MPCEXsim/det/stepMax",this);
  fStepMaxCmd->SetGuidance("Define a step max");
  fStepMaxCmd->SetParameterName("stepMax",false);
  fStepMaxCmd->SetUnitCategory("Length");
  fStepMaxCmd->AvailableForStates(G4State_Idle);
}

mxg4DetectorMessenger::~mxg4DetectorMessenger() {
  delete fStepMaxCmd;
  delete fMXDirectory;
  delete fDetDirectory;
}

void mxg4DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue) {
  if( command == fStepMaxCmd ) {
    fDetectorConstruction
      ->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
  }   
}
