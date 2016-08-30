// origin: S.Karthas (Aug 2016)

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

#include "mxg4DetectorConstruction.h"
#include "mxg4DetectorMessenger.h"
#include "mxg4TrackerSD.h"

#include "../mxGeometry.h"
#include "../mxGeometry.cxx"

G4ThreadLocal 
G4GlobalMagFieldMessenger* mxg4DetectorConstruction::fMagFieldMessenger = 0;

mxg4DetectorConstruction::mxg4DetectorConstruction()
:G4VUserDetectorConstruction(), 
 fNbOfChambers(0),
 fLogicTarget(NULL),
 fLogicChambersil(NULL),
 fLogicCrystal(NULL),
 fCrystalWrap(NULL),
 fLogicMinipads(NULL),
 fTargetMaterial(NULL),
 fLayerMaterial(NULL),
 fMPCMaterial(NULL),
 fStepLimit(NULL),
 ftoggleMinipad(true),
 fCheckOverlaps(false)
{
  fMessenger = new mxg4DetectorMessenger(this);

  fNbOfChambers = 192*2;
  fLogicTarget = new G4LogicalVolume*[fNbOfChambers];
  fLogicChambersil = new G4LogicalVolume*[fNbOfChambers];
  fLogicCrystal = new G4LogicalVolume*[288*2];
  fCrystalWrap = new G4OpticalSurface*[288*2];
  fLogicMinipads = new G4LogicalVolume*[49152];
}

mxg4DetectorConstruction::~mxg4DetectorConstruction()
{
  delete [] fLogicTarget;
  delete [] fLogicChambersil;
  delete [] fLogicCrystal;
  delete [] fCrystalWrap;
  delete [] fLogicMinipads;
  delete fStepLimit;
  delete fMessenger;
}

G4VPhysicalVolume* mxg4DetectorConstruction::Construct()
{
  DefineMaterials();
  return DefineVolumes();
}

void mxg4DetectorConstruction::DefineMaterials()
{
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_AIR");
  fTargetMaterial  = nistManager->FindOrBuildMaterial("G4_W");
  fLayerMaterial = nistManager->FindOrBuildMaterial("G4_Si");
  fMPCMaterial = nistManager->FindOrBuildMaterial("G4_PbWO4");
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

G4VPhysicalVolume* mxg4DetectorConstruction::DefineVolumes() {
  G4Material* air  = G4Material::GetMaterial("G4_AIR");

  G4double chamberSpacing = 80*cm; // from chamber center to center!
  G4double chamberWidth = 20.0*cm; // width of the chambers
  G4double targetLength =  5.0*cm; // full length of Target
  
  G4double trackerLength = 230.0*cm;
  
  G4double worldLength = 1.2*(trackerLength);
  G4double targetRadius  = 0.5*targetLength;   // Radius of Target
  targetLength = 0.5*targetLength;             // Half length of the Target  
  G4double trackerSize   = 0.5*trackerLength;  // Half length of the Tracker
  
  mxGeometry *mgeo = new mxGeometry();

  float dx, dy, dz;
  dx = mgeo->W_a0(); dy = mgeo->W_a1(); dz = mgeo->W_a2();
  
  // World

  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);

  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;

  G4Box* worldS
    = new G4Box("world",                                    //its name
                worldLength/2.0,worldLength/2.0,worldLength); //its size
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,   //its solid
                 air,      //its material
                 "World"); //its name
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // 
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(), // at (0,0,0)
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother  volume
                 false,           // no boolean operations
                 0,               // copy number
                 fCheckOverlaps); // checking overlaps
  
  // Tracker
  /* 
  G4ThreeVector positionTracker = G4ThreeVector(0,0,0);

  G4Box* trackerS
    = new G4Box("tracker",dx*6.0*cm,dy*6.0*cm,260.0*cm);
  G4LogicalVolume* trackerLV
    = new G4LogicalVolume(trackerS, air, "Tracker",0,0,0);  
  new G4PVPlacement(0,               // no rotation
                    positionTracker, // at (x,y,z)
                    trackerLV,       // its logical volume
                    "Tracker",       // its name
                    worldLV,         // its mother  volume
                    false,           // no boolean operations
                    0,               // copy number
                    fCheckOverlaps); // checking overlaps 
  */
  
  G4ThreeVector positionEXS = G4ThreeVector(0,0,-205*cm);//mgeo->W_Z(96)*cm);
  G4Box* EXSS = new G4Box("mpcexs",dx*6.0*cm,dy*6.0*cm,dz*24.0*cm);
  G4LogicalVolume* EXSLV = new G4LogicalVolume(EXSS, air, "MpcexS",0,0,0);
  new G4PVPlacement(0,
		    positionEXS,
		    EXSLV,
		    "MpcexS",
		    worldLV,
		    false,
		    0,
		    0);

  G4ThreeVector positionEXN = G4ThreeVector(0,0,205*cm);//mgeo->W_Z(288)*cm);
  G4Box* EXNS = new G4Box("mpcexn",dx*6.0*cm,dy*6.0*cm,dz*24.0*cm);
  G4LogicalVolume* EXNLV = new G4LogicalVolume(EXNS, air, "MpcexN",0,0,0);
  new G4PVPlacement(0,
		    positionEXN,
		    EXNLV,
		    "MpcexN",
		    worldLV,
		    false,
		    0,
		    0);

  G4ThreeVector positionMPCS = G4ThreeVector(0,0,(-220-mgeo->PWO4_a2()*0.5)*cm);//(mgeo->Z(49152+50)-mgeo->PWO4_a2()*0.5)*cm);
  G4Box* MPCS = new G4Box("mpcs",dx*6.0*cm,dy*6.0*cm,mgeo->PWO4_a2()*0.5*cm+1*cm);
  G4LogicalVolume* MPCSLV = new G4LogicalVolume(MPCS, air, "MpcS",0,0,0);
  new G4PVPlacement(0,
		    positionMPCS,
		    MPCSLV,
		    "MpcS",
		    worldLV,
		    false,
		    0,
		    0);

  G4ThreeVector positionMPCN = G4ThreeVector(0,0,(220+mgeo->PWO4_a2()*0.5)*cm);//(mgeo->Z(49152+310)+mgeo->PWO4_a2()*0.5)*cm);
  G4Box* MPCN = new G4Box("mpcn",dx*6.0*cm,dy*6.0*cm,mgeo->PWO4_a2()*0.5*cm+1*cm);
  G4LogicalVolume* MPCNLV = new G4LogicalVolume(MPCN, air, "MpcN",0,0,0);
  new G4PVPlacement(0,
		    positionMPCN,
		    MPCNLV,
		    "MpcN",
		    worldLV,
		    false,
		    0,
		    0);
  
  // Visualization attributes

  G4VisAttributes* boxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  G4VisAttributes* siliconVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  G4VisAttributes* minipadVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  G4VisAttributes* mpcVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  
  worldLV      ->SetVisAttributes(boxVisAtt);
  //  fLogicTarget ->SetVisAttributes(boxVisAtt);
  // trackerLV    ->SetVisAttributes(boxVisAtt);
  EXNLV        ->SetVisAttributes(boxVisAtt);
  EXSLV        ->SetVisAttributes(boxVisAtt);
  MPCNLV       ->SetVisAttributes(siliconVisAtt);
  MPCSLV       ->SetVisAttributes(siliconVisAtt);
  // Tracker segments

  G4cout << "There are " << fNbOfChambers << " chambers in the tracker region. "
         << G4endl
         << "The chambers are " << chamberWidth/cm << " cm of "
         << fLayerMaterial->GetName() << G4endl
         << "The distance between chamber is " << chamberSpacing/cm << " cm" 
         << G4endl;

  
  G4double firstPosition = -trackerSize + chamberSpacing;
  G4double firstLength   = trackerLength/10;
  G4double lastLength    = trackerLength;

  G4double halfWidth = 0.5*chamberWidth;
  G4double rmaxFirst = 0.5 * firstLength;

  G4double rmaxIncr = 0.0;
  if( fNbOfChambers > 0 ){
    rmaxIncr =  0.5 * (lastLength-firstLength)/(fNbOfChambers-1);
    if (chamberSpacing  < chamberWidth) {
       G4Exception("DetectorConstruction::DefineVolumes()",
                   "InvalidSetup", FatalException,
                   "Width>Spacing");
    }
  }
  

  for (G4int copyNo=0; copyNo < fNbOfChambers; copyNo++) {//Tungsten
    
    G4double Zposition = mgeo->W_Z(copyNo);//firstPosition + copyNo * chamberSpacing;
    G4double Xposition = mgeo->W_X(copyNo);
    G4double Yposition = mgeo->W_Y(copyNo);

    //dx = mgeo->Si_a0(); dy = mgeo->Si_a1(); dz = mgeo->Si_a2();
    if (Zposition < 0 ) Zposition = Zposition - dz/2.0;
    if (Zposition > 0 ) Zposition = Zposition + dz/2.0;
    G4Box* Sensor = new G4Box("Target_W",dx/2.0*cm,dy/2.0*cm,dz/2.0*cm);
    
    fLogicTarget[copyNo] =
      new G4LogicalVolume(Sensor,fTargetMaterial,"Target_LV",0,0,0);

    fLogicTarget[copyNo]->SetVisAttributes(chamberVisAtt);
    if (Zposition < 0){
      new G4PVPlacement(0,                            // no rotation
			G4ThreeVector(Xposition*cm,Yposition*cm,Zposition*cm+205*cm), // at (x,y,z)
			fLogicTarget[copyNo],        // its logical volume
			"Target_PV",                 // its name
			// trackerLV,                    // its mother  volume
			EXSLV,
			false,                        // no boolean operations
			copyNo,                       // copy number
			fCheckOverlaps);              // checking overlaps 
    }
    else{
      new G4PVPlacement(0,                            // no rotation
			G4ThreeVector(Xposition*cm,Yposition*cm,Zposition*cm-205*cm), // at (x,y,z)
			fLogicTarget[copyNo],        // its logical volume
			"Target_PV",                 // its name
			// trackerLV,                    // its mother  volume
			EXNLV,
			false,                        // no boolean operations
			copyNo,                       // copy number
			fCheckOverlaps);              // checking overlaps
      
    }
  }
  //If we want micromodules, not minipads
  /*  if (ftoggleMinipad == false){

    for (G4int copyNo=0; copyNo < fNbOfChambers; copyNo++) {//Silicon

      G4double Zposition = mgeo->W_Z(copyNo);

      if (Zposition>0){
	Zposition = Zposition - 0.02*2;
      }

      if (Zposition<0){
	Zposition = Zposition + 0.02*2;
      }

      G4double Xposition = mgeo->W_X(copyNo);
      G4double Yposition = mgeo->W_Y(copyNo);
      
      dx = mgeo->W_a0(); dy = mgeo->W_a1(); dz = mgeo->Si_a2();
            
      G4Box* Sensor = new G4Box("Sensor_Si",dx/2.0*cm,dy/2.0*cm,dz/2.0*cm);
      
      fLogicChambersil[copyNo] =
	new G4LogicalVolume(Sensor,fLayerMaterial,"Chambersil_LV",0,0,0);
      
      fLogicChambersil[copyNo]->SetVisAttributes(siliconVisAtt);
            
      new G4PVPlacement(0,                            // no rotation
			G4ThreeVector(Xposition*cm,Yposition*cm,Zposition*cm), // at (x,y,z)
			fLogicChambersil[copyNo],        // its logical volume
			"Chambersil_PV",                 // its name
			trackerLV,                    // its mother  volume
			false,                        // no boolean operations
			copyNo,                       // copy number
			fCheckOverlaps);              // checking overlaps
      
    }
    }*/
  //If we want minipad separation
  if (ftoggleMinipad == true){

    for (G4int copyNo=0; copyNo < 49152; copyNo++) {//Silicon
      G4double Zposition = mgeo->Z(copyNo);
      G4double Xposition = mgeo->X(copyNo);
      G4double Yposition = mgeo->Y(copyNo);
      
      if((mgeo->LyrIdx(copyNo)%9)%2==0) {
	dx = mgeo->Si_a0(); dy = mgeo->Si_a1(); dz = mgeo->Si_a2();
      }
      else {
	dx = mgeo->Si_a1(); dy = mgeo->Si_a0(); dz = mgeo->Si_a2();
      }
      
      G4Box* Minipad = new G4Box("Minipad",dx/2.0*cm,dy/2.0*cm,dz/2.0*cm);

      fLogicMinipads[copyNo] =
	new G4LogicalVolume(Minipad,fLayerMaterial,"Minipad_LV",0,0,0);
      
      fLogicMinipads[copyNo]->SetVisAttributes(minipadVisAtt);
      if (Zposition < 0){
	new G4PVPlacement(0,                            // no rotation
			  G4ThreeVector(Xposition*cm,Yposition*cm,Zposition*cm+205*cm), // at (x,y,z)
			  fLogicMinipads[copyNo],        // its logical volume
			  "Minipad_PV",                 // its name
			  //trackerLV,                    // its mother  volume
			  EXSLV,
			  false,                        // no boolean operations
			  copyNo,                       // copy number
			  false);              // checking overlaps 
      }
      else{
	new G4PVPlacement(0,                            // no rotation
			  G4ThreeVector(Xposition*cm,Yposition*cm,Zposition*cm-205*cm), // at (x,y,z)
			  fLogicMinipads[copyNo],        // its logical volume
			  "Minipad_PV",                 // its name
			  //trackerLV,                    // its mother  volume
			  EXNLV,
			  false,                        // no boolean operations
			  copyNo,                       // copy number
			  false);              // checking overlaps
      }
      
    }
  }

  //MPC Construction
  for (G4int mid=0; mid < 288*2; mid++) {//MPC

    int k = mid+49152;
    G4double mpcz = mgeo->Z(k);//firstPosition + copyNo * chamberSpacing;
    G4double mpcx = mgeo->X(k);
    G4double mpcy = mgeo->Y(k);
    
    if (mpcz < 10 && mpcz>-10) continue; //skip holes

    dx = mgeo->PWO4_a0(); dy = mgeo->PWO4_a1(); dz = mgeo->PWO4_a2();
    
    if (mpcz < 0 ) mpcz = mpcz - dz/2;
    if (mpcz > 0 ) mpcz = mpcz + dz/2;
    G4Box* Crystal = new G4Box("Crystal_Pb",dx/2.0*cm,dy/2.0*cm,dz/2.0*cm);

    fLogicCrystal[mid] =
      new G4LogicalVolume(Crystal,fMPCMaterial,"Crystal_LV",0,0,0);
    
    fLogicCrystal[mid]->SetVisAttributes(mpcVisAtt);
    
    fCrystalWrap[mid] = new G4OpticalSurface("CrystalWrap");
    G4VPhysicalVolume* crys;
    if (mpcz<0){
      crys = new G4PVPlacement(0,                            // no rotation
			       G4ThreeVector(mpcx*cm,mpcy*cm,mpcz*cm+220*cm+mgeo->PWO4_a2()*0.5*cm), // at (x,y,z)
			       fLogicCrystal[mid],        // its logical volume
			       "Crystal_PV",                 // its name
			       //trackerLV,                    // its mother  volume
			       MPCSLV,
			       false,                        // no boolean operations
			       mid,                       // copy number
			       fCheckOverlaps);              // checking overlaps
    }
    else{
      crys = new G4PVPlacement(0,                            // no rotation
			       G4ThreeVector(mpcx*cm,mpcy*cm,mpcz*cm-220*cm-mgeo->PWO4_a2()*0.5*cm), // at (x,y,z)
			       fLogicCrystal[mid],        // its logical volume
			       "Crystal_PV",                 // its name
			       //trackerLV,                    // its mother  volume
			       MPCNLV,
			       false,                        // no boolean operations
			       mid,                       // copy number
			       fCheckOverlaps);              // checking overlaps
    }
    
    new G4LogicalBorderSurface("CrystalWrap",crys,worldPV,fCrystalWrap[mid]);
    fCrystalWrap[mid]->SetType(dielectric_LUT);
    fCrystalWrap[mid]->SetModel(LUT);
    fCrystalWrap[mid]->SetFinish(polishedtyvekair);
    
    const G4int NUM = 2;
    G4double pp[NUM] = {2.0*eV, 3.5*eV};
    G4double reflectivity[NUM] = {0.97,0.97};
    G4double efficiency[NUM] = {0.0,0.0};
    
    G4MaterialPropertiesTable* CrystWrapProperty = new G4MaterialPropertiesTable();
    
    CrystWrapProperty->AddProperty("REFLECTIVITY",pp,reflectivity,NUM);
    CrystWrapProperty->AddProperty("EFFICIENCY",pp,efficiency,NUM);
    fCrystalWrap[mid]->SetMaterialPropertiesTable(CrystWrapProperty);
  }
  
  
  
  // Example of User Limits
  //
  // Below is an example of how to set tracking constraints in a given
  // logical volume
  //
  // Sets a max step length in the tracker region, with G4StepLimiter

  G4double maxStep = 0.5*dz;
  fStepLimit = new G4UserLimits(maxStep);
  //trackerLV->SetUserLimits(fStepLimit);
 
  /// Set additional contraints on the track, with G4UserSpecialCuts
  G4double maxLength = 260.0*cm, maxTime = 100*ns;
  /* trackerLV->SetUserLimits(new G4UserLimits(maxStep,
					    maxLength,
					    maxTime,
					    minEkin));*/
  /*
  for (int n = 0; n < 49152; n++){
    fLogicMinipads[n]->SetUserLimits(new G4UserLimits(0.5*mgeo->Si_a2(),
						      0.5*mgeo->Si_a2(),
						      maxTime,
						      0.5*MeV ));}*/
 
  /*  G4double mxcut = 1.5*MeV, mpccut = 100*MeV;
  EXSLV->SetUserLimits(new G4UserLimits(20*(mgeo->Si_a2())*cm,                                     
					maxLength,
					maxTime,
					mxcut));
  EXNLV->SetUserLimits(new G4UserLimits(20*(mgeo->Si_a2())*cm,
					//25*(mgeo->Si_a2())*cm,
					maxLength,
					maxTime,
					mxcut));
  MPCSLV->SetUserLimits(new G4UserLimits(0.5*(mgeo->PWO4_a2())*cm,
					 // 0.5*(mgeo->PWO4_a2())*cm,
					 maxLength,
					 maxTime,
					 mpccut));
  MPCNLV->SetUserLimits(new G4UserLimits(0.5*(mgeo->PWO4_a2())*cm,
					 //0.5*(mgeo->PWO4_a2())*cm,
					 maxLength,
					 maxTime,
					 mpccut));*/
  
  /*								 
  for (int q = 0; q < 288*2; q++){
    fLogicCrystal[q]->SetUserLimits(new G4UserLimits(0.5*mgeo->PWO4_a2(),
						     0.5*mgeo->PWO4_a2(),
						     maxTime,
						     6*MeV));}
  */
  
  // Always return the physical world
  
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void mxg4DetectorConstruction::ConstructSDandField() {
  // Sensitive detectors

  G4String trackerChamberSDname = "MPCEXsim/TrackerChamberSD";
  mxg4TrackerSD* aTrackerSD = new mxg4TrackerSD(trackerChamberSDname,
                                            "TrackerHitsCollection");
  G4String mpcSDname = "MPCEXsim/MPCSD";
  mxg4TrackerSD* ampcSD = new mxg4TrackerSD(mpcSDname,
					    "MPCHitsCollection");

  G4String minipadSDname = "MPCEXsim/MinipadSD";
  mxg4TrackerSD* aminipadSD = new mxg4TrackerSD(minipadSDname,"MinipadHitsCollection");
  // Setting aTrackerSD to all logical volumes with the same name 
  // of "Chamber_LV".
  //  SetSensitiveDetector("Chamber_LV", aTrackerSD, true);
  if (!ftoggleMinipad){SetSensitiveDetector("Chambersil_LV", aTrackerSD, true);}

  SetSensitiveDetector("Crystal_LV", ampcSD, true);

  if (ftoggleMinipad){
    SetSensitiveDetector("Minipad_LV",aminipadSD, true);
  }

  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector();
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void mxg4DetectorConstruction::SetTargetMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

  G4Material* pttoMaterial = 
              nistManager->FindOrBuildMaterial(materialName);

  if (fTargetMaterial != pttoMaterial) {
     if ( pttoMaterial ) {
        fTargetMaterial = pttoMaterial;
	for (G4int copyNo=0; copyNo<fNbOfChambers; copyNo++) {
	  
	  if (fLogicTarget[copyNo]) fLogicTarget[copyNo]->SetMaterial(fTargetMaterial);
	}
	G4cout 
          << G4endl 
          << "----> The target is made of " << materialName << G4endl;
     } else {
        G4cout 
          << G4endl 
          << "-->  WARNING from SetTargetMaterial : "
          << materialName << " not found" << G4endl;
     }
  }
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void mxg4DetectorConstruction::SetChamberMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

  G4Material* pttoMaterial =
              nistManager->FindOrBuildMaterial(materialName);

  if (fLayerMaterial != pttoMaterial) {
     if ( pttoMaterial ) {
        fLayerMaterial = pttoMaterial;
        for (G4int copyNo=0; copyNo<fNbOfChambers; copyNo++) {
            if (fLogicChambersil[copyNo]) fLogicChambersil[copyNo]->
                                            SetMaterial(fLayerMaterial);
        }
        G4cout 
          << G4endl 
          << "----> The chambers are made of " << materialName << G4endl;
     } else {
        G4cout 
          << G4endl 
          << "-->  WARNING from SetChamberMaterial : "
          << materialName << " not found" << G4endl;
     }
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void mxg4DetectorConstruction::SetMinipadMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();
  
  G4Material* pttoMaterial =
    nistManager->FindOrBuildMaterial(materialName);
  
  if (fLayerMaterial != pttoMaterial) {
    if ( pttoMaterial ) {
      fLayerMaterial = pttoMaterial;
      for (G4int copyNomp = 0; copyNomp < 49152; copyNomp++) {
	if (fLogicMinipads[copyNomp]) fLogicMinipads[copyNomp]->
					SetMaterial(fLayerMaterial);
      }
      G4cout
	<< G4endl
	<< "----> The minipadss are made of " << materialName << G4endl;
    } else {
      G4cout
	<< G4endl
	<< "-->  WARNING from SetMinipadMaterial : "
	<< materialName << " not found" << G4endl;
    }
  }
}

void mxg4DetectorConstruction::SetMpcMaterial(G4String materialName) {
  G4NistManager* nistManager = G4NistManager::Instance();
  G4Material* pttoMaterial = nistManager->FindOrBuildMaterial(materialName);
  if(!pttoMaterial) {
    G4cout << G4endl << "-->  WARNING from SetTargetMaterial : " << materialName << " not found" << G4endl;
    return;
  }
  if(fTargetMaterial != pttoMaterial) {
    fTargetMaterial = pttoMaterial;
    for(G4int copyNo=0; copyNo<288*2; copyNo++) {
      if(fLogicCrystal[copyNo])
	fLogicCrystal[copyNo]->SetMaterial(fMPCMaterial);
    }
    G4cout << G4endl << "----> The mpc is made of " << materialName << G4endl;
  }
}

void mxg4DetectorConstruction::SetMaxStep(G4double maxStep) {
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

void mxg4DetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps) {
  fCheckOverlaps = checkOverlaps;
}
