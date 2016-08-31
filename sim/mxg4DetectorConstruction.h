// origin: S.Karthas (Aug 2016)

#ifndef mxg4DetectorConstruction_h
#define mxg4DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "tls.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;
class G4GlobalMagFieldMessenger;
class G4OpticalSurface;
class mxg4DetectorMessenger;

class mxg4DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  mxg4DetectorConstruction();
  virtual ~mxg4DetectorConstruction();
  
public:
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
  
  void SetTargetMaterial(G4String);
  void SetChamberMaterial(G4String);
  void SetMinipadMaterial(G4String);
  void SetMpcMaterial(G4String);
  void SetMaxStep(G4double);
  void SetCheckOverlaps(G4bool);
  bool IsMinis() {return ftoggleMinipad;}

private:
  void DefineMaterials();
  G4VPhysicalVolume *DefineVolumes();

  G4int fNbOfEXSensors;
  G4LogicalVolume **fLV_EXW;
  G4LogicalVolume **fLV_EXSiSen;
  G4LogicalVolume **fLV_PWO4;
  G4OpticalSurface **fOS_PWO4wrap;
  G4LogicalVolume **fLV_EXSi;
  G4Material *fMaterial_W;
  G4Material *fMaterial_Si;
  G4Material *fMaterial_PWO4;
  G4UserLimits *fStepLimit;
  mxg4DetectorMessenger*  fMessenger;

  static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger; 
  G4bool ftoggleMinipad;
  G4bool fCheckOverlaps;
};

#endif
