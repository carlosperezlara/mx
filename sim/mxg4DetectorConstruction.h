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
  
  void SetTargetMaterial (G4String );
  void SetChamberMaterial(G4String );
  void SetMinipadMaterial(G4String );
  void SetMpcMaterial(G4String );
  void SetMaxStep (G4double );
  void SetCheckOverlaps(G4bool );
  bool IsMinis(){return ftoggleMinipad;};
  
private:
  void DefineMaterials();
  G4VPhysicalVolume* DefineVolumes();
  
  G4int fNbOfChambers;
  G4LogicalVolume**   fLogicTarget;     // pointer to the logical Target
  G4LogicalVolume**  fLogicChambersil;  // pointer to the logical Chamber       
  G4LogicalVolume**  fLogicCrystal;     // pointer to the logical MPC Crystal
  G4OpticalSurface** fCrystalWrap;      // Wrap for Crystals
  G4LogicalVolume**  fLogicMinipads;    // pointer to the logical Minipads
  G4Material*   fTargetMaterial;   // pointer to the target  material
  G4Material*   fLayerMaterial;    // pointer to the chamber material
  G4Material*   fMPCMaterial;      // pointer to the MPC material
  G4UserLimits* fStepLimit;        // pointer to user step limits
  
  mxg4DetectorMessenger*  fMessenger;   // messenger
  
  static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger; 
  G4bool  ftoggleMinipad;
  G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps 
};

#endif
