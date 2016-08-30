// origin: S.Karthas (Aug 2016)

#ifndef mxg4DetectorMessenger_h
#define mxg4DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class mxg4DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

class mxg4DetectorMessenger: public G4UImessenger
{
  public:
    mxg4DetectorMessenger(mxg4DetectorConstruction* );
    virtual ~mxg4DetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    mxg4DetectorConstruction*  fDetectorConstruction;
    G4UIdirectory*           fMXDirectory;
    G4UIdirectory*           fDetDirectory;
    G4UIcmdWithAString*      fTargMatCmd;
    G4UIcmdWithAString*      fChamMatCmd;
    G4UIcmdWithAString*      fMpcMatCmd;
    G4UIcmdWithAString*      fMiniMatCmd;
    G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
};

#endif
