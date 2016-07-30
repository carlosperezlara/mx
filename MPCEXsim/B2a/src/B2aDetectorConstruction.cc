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
// $Id: B2aDetectorConstruction.cc 87359 2014-12-01 16:04:27Z gcosmo $
//
/// \file B2aDetectorConstruction.cc
/// \brief Implementation of the B2aDetectorConstruction class
 
#include "B2aDetectorConstruction.hh"
#include "B2aDetectorMessenger.hh"
#include "B2TrackerSD.hh"

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
#include "/Users/stacykarthas/mx/mxGeometry.h"
#include "/Users/stacykarthas/mx/mxGeometry.cxx"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4ThreadLocal 
G4GlobalMagFieldMessenger* B2aDetectorConstruction::fMagFieldMessenger = 0;

B2aDetectorConstruction::B2aDetectorConstruction()
:G4VUserDetectorConstruction(), 
 fNbOfChambers(0),
 fLogicTarget(NULL), fLogicChambersil(NULL), fLogicCrystal(NULL), fCrystalWrap(NULL), fLogicMinipads(NULL),
 fTargetMaterial(NULL), fLayerMaterial(NULL), fMPCMaterial(NULL), 
 fStepLimit(NULL), ftoggleMinipad(true),
 fCheckOverlaps(false)
{
  fMessenger = new B2aDetectorMessenger(this);

  fNbOfChambers = 192*2;
  fLogicTarget = new G4LogicalVolume*[fNbOfChambers];
  fLogicChambersil = new G4LogicalVolume*[fNbOfChambers];
  fLogicCrystal = new G4LogicalVolume*[188*2];
  fCrystalWrap = new G4OpticalSurface*[188*2];
  fLogicMinipads = new G4LogicalVolume*[49152];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
B2aDetectorConstruction::~B2aDetectorConstruction()
{
  delete [] fLogicTarget;
  delete [] fLogicChambersil;
  delete [] fLogicCrystal;
  delete [] fCrystalWrap;
  delete [] fLogicMinipads;
  delete fStepLimit;
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* B2aDetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::DefineMaterials()
{
  // Material definition 

  G4NistManager* nistManager = G4NistManager::Instance();

  // Air defined using NIST Manager
  nistManager->FindOrBuildMaterial("G4_AIR");
  
  // Tungsten defined using NIST Manager
  fTargetMaterial  = nistManager->FindOrBuildMaterial("G4_W");

  // Silicon defined using NIST Manager
  fLayerMaterial = nistManager->FindOrBuildMaterial("G4_Si");

  // PbWO4 defined using NIST Manager
  fMPCMaterial = nistManager->FindOrBuildMaterial("G4_PbWO4");
  
  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B2aDetectorConstruction::DefineVolumes()
{
  G4Material* air  = G4Material::GetMaterial("G4_AIR");

  // Sizes of the principal geometrical components (solids)
  
  G4double chamberSpacing = 80*cm; // from chamber center to center!

  G4double chamberWidth = 20.0*cm; // width of the chambers
  G4double targetLength =  5.0*cm; // full length of Target
  
  //  G4double trackerLength = (fNbOfChambers+1)*chamberSpacing;

  G4double trackerLength = 230.0*cm;
  //  G4double worldLength = 1.2 * (2*targetLength + trackerLength);

  
  G4double worldLength = 1.2*(trackerLength);
  
  G4double targetRadius  = 0.5*targetLength;   // Radius of Target
  targetLength = 0.5*targetLength;             // Half length of the Target  
  G4double trackerSize   = 0.5*trackerLength;  // Half length of the Tracker
  
  // Definitions of Solids, Logical Volumes, Physical Volumes

  // To select MPCEX and MPC geometry

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

  // Target
  /*
  G4ThreeVector positionTarget = G4ThreeVector(0,0,-(targetLength+trackerSize));

  G4Tubs* targetS
    = new G4Tubs("target",0.,targetRadius,targetLength,0.*deg,360.*deg);
  fLogicTarget
    = new G4LogicalVolume(targetS, fTargetMaterial,"Target",0,0,0);
  new G4PVPlacement(0,               // no rotation
                    positionTarget,  // at (x,y,z)
                    fLogicTarget,    // its logical volume
                    "Target",        // its name
                    worldLV,         // its mother volume
                    false,           // no boolean operations
                    0,               // copy number
                    fCheckOverlaps); // checking overlaps 

  G4cout << "Target is " << 2*targetLength/cm << " cm of "
         << fTargetMaterial->GetName() << G4endl;
  */
  // Tracker
 
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

  // Visualization attributes

  G4VisAttributes* boxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  G4VisAttributes* siliconVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  G4VisAttributes* minipadVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  G4VisAttributes* mpcVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  
  worldLV      ->SetVisAttributes(boxVisAtt);
  //  fLogicTarget ->SetVisAttributes(boxVisAtt);
  trackerLV    ->SetVisAttributes(boxVisAtt);

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
       G4Exception("B2aDetectorConstruction::DefineVolumes()",
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
    
  
    
    //  G4double rmax =  rmaxFirst + copyNo * rmaxIncr;

    //  G4Tubs* chamberS
    //    = new G4Tubs("Chamber_solid", 0, rmax, halfWidth, 0.*deg, 360.*deg);

    //  fLogicChamber[copyNo] =
    //         new G4LogicalVolume(chamberS,fLayerMaterial,"Chamber_LV",0,0,0);

    // fLogicChamber[copyNo]->SetVisAttributes(chamberVisAtt);

      new G4PVPlacement(0,                            // no rotation
                        G4ThreeVector(Xposition*cm,Yposition*cm,Zposition*cm), // at (x,y,z)
                        fLogicTarget[copyNo],        // its logical volume
                        "Target_PV",                 // its name
                        trackerLV,                    // its mother  volume
                        false,                        // no boolean operations
                        copyNo,                       // copy number
                        fCheckOverlaps);              // checking overlaps 

  }
  if (ftoggleMinipad == false){
    for (G4int copyNo=0; copyNo < fNbOfChambers; copyNo++) {//Silicon
      G4double Zposition = mgeo->W_Z(copyNo);
      if (Zposition>0){
	//      G4double Zposition = mgeo->W_Z(copyNo)-0.02;//firstPosition + copyNo * chamberSpacing;
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
      
      
      
      //  G4double rmax =  rmaxFirst + copyNo * rmaxIncr;
      
      //  G4Tubs* chamberS
      //    = new G4Tubs("Chamber_solid", 0, rmax, halfWidth, 0.*deg, 360.*deg);
      
      //  fLogicChamber[copyNo] =
      //         new G4LogicalVolume(chamberS,fLayerMaterial,"Chamber_LV",0,0,0);
      
      // fLogicChamber[copyNo]->SetVisAttributes(chamberVisAtt);
      
      new G4PVPlacement(0,                            // no rotation
			G4ThreeVector(Xposition*cm,Yposition*cm,Zposition*cm), // at (x,y,z)
			fLogicChambersil[copyNo],        // its logical volume
			"Chambersil_PV",                 // its name
			trackerLV,                    // its mother  volume
			false,                        // no boolean operations
			copyNo,                       // copy number
			fCheckOverlaps);              // checking overlaps
      
    }
  }
  if (ftoggleMinipad == true){
    for (G4int copyNo=0; copyNo < 49152; copyNo++) {//Silicon

      G4double Zposition = mgeo->Z(copyNo);//firstPosition + copyNo * chamberSpacing;
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



	      //  G4double rmax =  rmaxFirst + copyNo * rmaxIncr;

	      //  G4Tubs* chamberS
	      //    = new G4Tubs("Chamber_solid", 0, rmax, halfWidth, 0.*deg, 360.*deg);

	      //  fLogicChamber[copyNo] =
	      //         new G4LogicalVolume(chamberS,fLayerMaterial,"Chamber_LV",0,0,0);

	      // fLogicChamber[copyNo]->SetVisAttributes(chamberVisAtt);

	      new G4PVPlacement(0,                            // no rotation
				G4ThreeVector(Xposition*cm,Yposition*cm,Zposition*cm), // at (x,y,z)
				fLogicMinipads[copyNo],        // its logical volume
				"Minipad_PV",                 // its name
				trackerLV,                    // its mother  volume
				false,                        // no boolean operations
				copyNo,                       // copy number
				false);              // checking overlaps

    }
  }

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
	    
	    //  G4double rmax =  rmaxFirst + copyNo * rmaxIncr;

	    //  G4Tubs* chamberS
	    //    = new G4Tubs("Chamber_solid", 0, rmax, halfWidth, 0.*deg, 360.*deg);

	    //  fLogicChamber[copyNo] =
	    //         new G4LogicalVolume(chamberS,fLayerMaterial,"Chamber_LV",0,0,0);

	    // fLogicChamber[copyNo]->SetVisAttributes(chamberVisAtt);

	    G4VPhysicalVolume* crys = new G4PVPlacement(0,                            // no rotation
			      G4ThreeVector(mpcx*cm,mpcy*cm,mpcz*cm), // at (x,y,z)
			      fLogicCrystal[mid],        // its logical volume
			      "Crystal_PV",                 // its name
			      trackerLV,                    // its mother  volume
			      false,                        // no boolean operations
			      mid,                       // copy number
			      fCheckOverlaps);              // checking overlaps
	    
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
  
  
  
  /*  for (G4int copyNo=0; copyNo<192; copyNo++) {//North Arm

    G4double Zposition = mgeo->W_Z(192+copyNo);
    G4double Yposition = mgeo->W_Y(192+copyNo);
    G4double Xposition = mgeo->W_X(192+copyNo);
    //G4double rmax =  rmaxFirst + copyNo * rmaxIncr;

    G4Box* Sensor = new G4Box("Sensor_Si",dx/2.0*cm,dy/2.0*cm,dz/2.0*cm);
    fLogicChamber[copyNo] =
      new G4LogicalVolume(Sensor,fLayerMaterial,"Sensor_Si_LV",0,0,0);
    fLogicChamber[copyNo]->SetVisAttributes(siliconVisAtt);

		new G4PVPlacement(0,                            // no rotation
				  G4ThreeVector(Xposition,Yposition,Zposition), // at (x,y,z)
				  fLogicChamber[copyNo],        // its logical volume
				  "Sensor_Si_PV",                 // its name
				  trackerLV,                    // its mother  volume
				  false,                        // no boolean operations
				  copyNo,                       // copy number
				  fCheckOverlaps);              // checking overlaps

  }
  
  */
  // Example of User Limits
  //
  // Below is an example of how to set tracking constraints in a given
  // logical volume
  //
  // Sets a max step length in the tracker region, with G4StepLimiter

  G4double maxStep = 0.5*dz;
  fStepLimit = new G4UserLimits(maxStep);
  trackerLV->SetUserLimits(fStepLimit);
 
  /// Set additional contraints on the track, with G4UserSpecialCuts
  ///
  G4double maxLength = 260.0*cm, maxTime = 100*ns, minEkin = 6*MeV;
  trackerLV->SetUserLimits(new G4UserLimits(maxStep,
					    maxLength,
					    maxTime,
					    minEkin));

  // Always return the physical world

  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void B2aDetectorConstruction::ConstructSDandField()
{
  // Sensitive detectors

  G4String trackerChamberSDname = "B2/TrackerChamberSD";
  B2TrackerSD* aTrackerSD = new B2TrackerSD(trackerChamberSDname,
                                            "TrackerHitsCollection");
  G4String mpcSDname = "B2/MPCSD";
  B2TrackerSD* ampcSD = new B2TrackerSD(mpcSDname,
					    "MPCHitsCollection");

  G4String minipadSDname = "B2/MinipadSD";
  B2TrackerSD* aminipadSD = new B2TrackerSD(minipadSDname,"MinipadHitsCollection");
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
 
void B2aDetectorConstruction::SetTargetMaterial(G4String materialName)
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

void B2aDetectorConstruction::SetChamberMaterial(G4String materialName)
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
	    // if (fLogicChamber[copyNo]) fLogicChamber[copyNo]->
	    //				    SetMaterial(fTargetMaterial);
        }
	/*for (G4int copyNomp = 0; copyNomp < 49152; copyNomp++) {
	  if (fLogicMinipads[copyNo]) fLogicMinipads[copyNo]->
					  SetMaterial(fLayerMaterial);
					  }*/
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
  /*  if (fTargetMaterial != pttoMaterial) {
    if ( pttoMaterial ) {
      fTargetMaterial = pttoMaterial;
      for (G4int copyNo=0; copyNo<fNbOfChambers; copyNo++) {
	if (fLogicChamber[copyNo]) fLogicChamber[copyNo]->
					SetMaterial(fTargetMaterial);
	// if (fLogicChamber[copyNo]) fLogicChamber[copyNo]->
	//                              SetMaterial(fTargetMaterial);
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
    }*/
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::SetMinipadMaterial(G4String materialName)
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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::SetMpcMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

    G4Material* pttoMaterial =
      nistManager->FindOrBuildMaterial(materialName);

    if (fTargetMaterial != pttoMaterial) {
      if ( pttoMaterial ) {
	fTargetMaterial = pttoMaterial;
	for (G4int copyNo=0; copyNo<288*2; copyNo++) {

	  if (fLogicCrystal[copyNo]) fLogicCrystal[copyNo]->SetMaterial(fMPCMaterial);
	}
	        G4cout
		  << G4endl
		  << "----> The mpc is made of " << materialName << G4endl;
      } else {
	        G4cout
		  << G4endl
		  << "-->  WARNING from SetTargetMaterial : "
		  << materialName << " not found" << G4endl;
      }
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
  fCheckOverlaps = checkOverlaps;
}  
