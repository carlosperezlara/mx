// origin: S.Karthas (Aug 2016)
// mods: C.Perez (Aug 2016)

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
 fNbOfEXSensors(0),
 fLV_EXW(NULL),
 fLV_EXSiSen(NULL),
 fLV_PWO4(NULL),
 fOS_PWO4wrap(NULL),
 fLV_EXSi(NULL),
 fStepLimit(NULL),
 ftoggleMinipad(true),
 fCheckOverlaps(false) {
  fMessenger = new mxg4DetectorMessenger(this);

  fNbOfEXSensors = 2*8*24; // arms x layers x sensorsPerLayer
  fLV_EXW = new G4LogicalVolume*[fNbOfEXSensors];
  fLV_EXSiSen = new G4LogicalVolume*[fNbOfEXSensors];
  fLV_PWO4 = new G4LogicalVolume*[288*2];
  fOS_PWO4wrap = new G4OpticalSurface*[288*2];
  fLV_EXSi = new G4LogicalVolume*[49152];
}

mxg4DetectorConstruction::~mxg4DetectorConstruction() {
  delete [] fLV_EXW;
  delete [] fLV_EXSiSen;
  delete [] fLV_PWO4;
  delete [] fOS_PWO4wrap;
  delete [] fLV_EXSi;
  delete fStepLimit;
  delete fMessenger;
}

G4VPhysicalVolume* mxg4DetectorConstruction::Construct() {
  return DefineVolumes();
}

G4VPhysicalVolume* mxg4DetectorConstruction::DefineVolumes() {
  G4Material *material_W;
  G4Material *material_Si;
  G4Material *material_PWO4;

  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_AIR");
  material_W  = nistManager->FindOrBuildMaterial("G4_W");
  material_Si = nistManager->FindOrBuildMaterial("G4_Si");
  material_PWO4 = nistManager->FindOrBuildMaterial("G4_PbWO4");
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  G4Material* air  = G4Material::GetMaterial("G4_AIR");
  G4double worldLength = 1.2*230.0*cm;
  
  mxGeometry *mgeo = new mxGeometry();

  float dx, dy, dz;
  dx = mgeo->W_a0(); dy = mgeo->W_a1(); dz = mgeo->W_a2();

  //=======================
  // World
  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);
  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;
  G4Box* worldS = new G4Box("world",                                    //its name
			    worldLength/2.0,worldLength/2.0,worldLength); //its size
  G4LogicalVolume* worldLV = new G4LogicalVolume(worldS,   //its solid
						 air,      //its material
						 "World"); //its name
  G4VPhysicalVolume* worldPV = new G4PVPlacement(0,               // no rotation
						 G4ThreeVector(), // at (0,0,0)
						 worldLV,         // its logical volume
						 "World",         // its name
						 0,               // its mother  volume
						 false,           // no boolean operations
						 0,               // copy number
						 fCheckOverlaps); // checking overlaps
  
  G4ThreeVector positionEXS = G4ThreeVector(0,0,-205*cm);//mgeo->W_Z(96)*cm);
  G4Box* EXSS = new G4Box("mpcexs",dx*6.0*cm,dy*6.0*cm,dz*24.0*cm);
  G4LogicalVolume* EXSLV = new G4LogicalVolume(EXSS, air, "MpcexS",0,0,0);
  new G4PVPlacement(0, positionEXS, EXSLV, "MpcexS", worldLV, false, 0, 0);

  G4ThreeVector positionEXN = G4ThreeVector(0,0,205*cm);//mgeo->W_Z(288)*cm);
  G4Box* EXNS = new G4Box("mpcexn",dx*6.0*cm,dy*6.0*cm,dz*24.0*cm);
  G4LogicalVolume* EXNLV = new G4LogicalVolume(EXNS, air, "MpcexN",0,0,0);
  new G4PVPlacement(0, positionEXN, EXNLV, "MpcexN", worldLV, false, 0, 0);

  G4ThreeVector positionMPCS = G4ThreeVector(0,0,(-220-mgeo->PWO4_a2()*0.5)*cm);//(mgeo->Z(49152+50)-mgeo->PWO4_a2()*0.5)*cm);
  G4Box* MPCS = new G4Box("mpcs",dx*6.0*cm,dy*6.0*cm,mgeo->PWO4_a2()*0.5*cm+1*cm);
  G4LogicalVolume* MPCSLV = new G4LogicalVolume(MPCS, air, "MpcS",0,0,0);
  new G4PVPlacement(0, positionMPCS, MPCSLV, "MpcS", worldLV, false, 0, 0);

  G4ThreeVector positionMPCN = G4ThreeVector(0,0,(220+mgeo->PWO4_a2()*0.5)*cm);//(mgeo->Z(49152+310)+mgeo->PWO4_a2()*0.5)*cm);
  G4Box* MPCN = new G4Box("mpcn",dx*6.0*cm,dy*6.0*cm,mgeo->PWO4_a2()*0.5*cm+1*cm);
  G4LogicalVolume* MPCNLV = new G4LogicalVolume(MPCN, air, "MpcN",0,0,0);
  new G4PVPlacement(0, positionMPCN, MPCNLV, "MpcN", worldLV, false, 0, 0);
  
  // Visualization attributes
  G4VisAttributes* boxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  G4VisAttributes* siliconVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  G4VisAttributes* minipadVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  G4VisAttributes* mpcVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  
  //worldLV->SetVisAttributes(boxVisAtt);
  //EXNLV->SetVisAttributes(boxVisAtt);
  //EXSLV->SetVisAttributes(boxVisAtt);
  //MPCNLV->SetVisAttributes(siliconVisAtt);
  //MPCSLV->SetVisAttributes(siliconVisAtt);
  worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  EXNLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  EXSLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  MPCNLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  MPCSLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  //=======================
  // EX TUNGSTEN
  for(G4int copyNo=0; copyNo < fNbOfEXSensors; copyNo++) {
    G4double Zposition = mgeo->W_Z(copyNo);
    G4double Xposition = mgeo->W_X(copyNo);
    G4double Yposition = mgeo->W_Y(copyNo);
    dx = mgeo->W_a0();
    dy = mgeo->W_a1();
    dz = mgeo->W_a2();
    if(Zposition<0) {
      Zposition = Zposition - dz/2.0;
    } else {
      Zposition = Zposition + dz/2.0;
    }
    G4Box* Sensor = new G4Box("Target_W",dx/2.0*cm,dy/2.0*cm,dz/2.0*cm);
    fLV_EXW[copyNo] = new G4LogicalVolume(Sensor,material_W,"Target_LV",0,0,0);
    fLV_EXW[copyNo]->SetVisAttributes(chamberVisAtt);
    if(Zposition<0) {
      new G4PVPlacement(0,                            // no rotation
			G4ThreeVector(Xposition*cm,Yposition*cm,Zposition*cm+205*cm), // at (x,y,z)
			fLV_EXW[copyNo],        // its logical volume
			"Target_PV",                 // its name
			EXSLV,
			false,                        // no boolean operations
			copyNo,                       // copy number
			fCheckOverlaps);              // checking overlaps 
    } else {
      new G4PVPlacement(0,                            // no rotation
			G4ThreeVector(Xposition*cm,Yposition*cm,Zposition*cm-205*cm), // at (x,y,z)
			fLV_EXW[copyNo],        // its logical volume
			"Target_PV",                 // its name
			EXNLV,
			false,                        // no boolean operations
			copyNo,                       // copy number
			fCheckOverlaps);              // checking overlaps
    }
  }

  //=======================
  // EX SILICON
  if(ftoggleMinipad == false){
    for(G4int copyNo=0; copyNo < fNbOfEXSensors; copyNo++) { // loop on EXSensors
      G4double Zposition = mgeo->W_Z(copyNo);
      if(Zposition>0) {
	Zposition = Zposition - 0.02*2;
      } else {
	Zposition = Zposition + 0.02*2;
      }
      G4double Xposition = mgeo->W_X(copyNo);
      G4double Yposition = mgeo->W_Y(copyNo);
      dx = mgeo->W_a0();
      dy = mgeo->W_a1();
      dz = mgeo->Si_a2();
      G4Box* Sensor = new G4Box("Sensor_Si",dx/2.0*cm,dy/2.0*cm,dz/2.0*cm);
      fLV_EXSiSen[copyNo] = new G4LogicalVolume(Sensor,material_Si,"Chambersil_LV",0,0,0);
      //fLV_EXSiSen[copyNo]->SetVisAttributes(siliconVisAtt);
      fLV_EXSiSen[copyNo]->SetVisAttributes(G4VisAttributes::GetInvisible());
      if(Zposition < 0) {
	new G4PVPlacement(0,                            // no rotation
			  G4ThreeVector(Xposition*cm,Yposition*cm,Zposition*cm), // at (x,y,z)
			  fLV_EXSiSen[copyNo],        // its logical volume
			  "Chambersil_PV",                 // its name
			  EXSLV,                    // its mother  volume
			  false,                        // no boolean operations
			  copyNo,                       // copy number
			  fCheckOverlaps);              // checking overlaps
      } else {
	new G4PVPlacement(0,                            // no rotation
			  G4ThreeVector(Xposition*cm,Yposition*cm,Zposition*cm), // at (x,y,z)
			  fLV_EXSiSen[copyNo],        // its logical volume
			  "Chambersil_PV",                 // its name
			  EXNLV,                    // its mother  volume
			  false,                        // no boolean operations
			  copyNo,                       // copy number
			  fCheckOverlaps);              // checking overlaps
      }
    } // end of loop on EXSensors
  } else {
    for (G4int copyNo=0; copyNo < 49152; copyNo++) { // loop on EXMinipads
      G4double Zposition = mgeo->Z(copyNo);
      G4double Xposition = mgeo->X(copyNo);
      G4double Yposition = mgeo->Y(copyNo);
      if((mgeo->LyrIdx(copyNo)%9)%2==0) {
	dx = mgeo->Si_a0();
	dy = mgeo->Si_a1();
      } else {
	dx = mgeo->Si_a1();
	dy = mgeo->Si_a0();
      }
      dz = mgeo->Si_a2();
      G4Box* Minipad = new G4Box("Minipad",dx/2.0*cm,dy/2.0*cm,dz/2.0*cm);
      fLV_EXSi[copyNo] = new G4LogicalVolume(Minipad,material_Si,"Minipad_LV",0,0,0);
      fLV_EXSi[copyNo]->SetVisAttributes(minipadVisAtt);
      if(Zposition < 0) {
	new G4PVPlacement(0,                            // no rotation
			  G4ThreeVector(Xposition*cm,Yposition*cm,Zposition*cm+205*cm), // at (x,y,z)
			  fLV_EXSi[copyNo],        // its logical volume
			  "Minipad_PV",                 // its name
			  EXSLV,
			  false,                        // no boolean operations
			  copyNo,                       // copy number
			  false);              // checking overlaps 
      } else {
	new G4PVPlacement(0,                            // no rotation
			  G4ThreeVector(Xposition*cm,Yposition*cm,Zposition*cm-205*cm), // at (x,y,z)
			  fLV_EXSi[copyNo],        // its logical volume
			  "Minipad_PV",                 // its name
			  EXNLV,
			  false,                        // no boolean operations
			  copyNo,                       // copy number
			  false);              // checking overlaps
      }
    } // end of loop on EXMinipads
  }

  //=======================
  // MPC CRYSTALS
  for(G4int mid=0; mid < 288*2; mid++) {//MPC
    int k = mid+49152;
    G4double mpcz = mgeo->Z(k);//firstPosition + copyNo * chamberSpacing;
    G4double mpcx = mgeo->X(k);
    G4double mpcy = mgeo->Y(k);
    if(mpcz < 10 && mpcz>-10) continue; //skip holes
    dx = mgeo->PWO4_a0();
    dy = mgeo->PWO4_a1();
    dz = mgeo->PWO4_a2();
    if (mpcz < 0 ) mpcz = mpcz - dz/2;
    if (mpcz > 0 ) mpcz = mpcz + dz/2;
    G4Box* Crystal = new G4Box("Crystal_Pb",dx/2.0*cm,dy/2.0*cm,dz/2.0*cm);
    fLV_PWO4[mid] = new G4LogicalVolume(Crystal,material_PWO4,"Crystal_LV",0,0,0);
    fLV_PWO4[mid]->SetVisAttributes(mpcVisAtt);
    fOS_PWO4wrap[mid] = new G4OpticalSurface("CrystalWrap");
    G4VPhysicalVolume* crys;
    if(mpcz<0) {
      crys = new G4PVPlacement(0,                            // no rotation
			       G4ThreeVector(mpcx*cm,mpcy*cm,mpcz*cm+220*cm+mgeo->PWO4_a2()*0.5*cm), // at (x,y,z)
			       fLV_PWO4[mid],        // its logical volume
			       "Crystal_PV",                 // its name
			       MPCSLV,
			       false,                        // no boolean operations
			       mid,                       // copy number
			       fCheckOverlaps);              // checking overlaps
    } else {
      crys = new G4PVPlacement(0,                            // no rotation
			       G4ThreeVector(mpcx*cm,mpcy*cm,mpcz*cm-220*cm-mgeo->PWO4_a2()*0.5*cm), // at (x,y,z)
			       fLV_PWO4[mid],        // its logical volume
			       "Crystal_PV",                 // its name
			       MPCNLV,
			       false,                        // no boolean operations
			       mid,                       // copy number
			       fCheckOverlaps);              // checking overlaps
    }
    new G4LogicalBorderSurface("CrystalWrap",crys,worldPV,fOS_PWO4wrap[mid]);
    fOS_PWO4wrap[mid]->SetType(dielectric_LUT);
    fOS_PWO4wrap[mid]->SetModel(LUT);
    fOS_PWO4wrap[mid]->SetFinish(polishedtyvekair);
    const G4int NUM = 2;
    G4double pp[NUM] = {2.0*eV, 3.5*eV};
    G4double reflectivity[NUM] = {0.97,0.97};
    G4double efficiency[NUM] = {0.0,0.0};
    G4MaterialPropertiesTable* CrystWrapProperty = new G4MaterialPropertiesTable();
    CrystWrapProperty->AddProperty("REFLECTIVITY",pp,reflectivity,NUM);
    CrystWrapProperty->AddProperty("EFFICIENCY",pp,efficiency,NUM);
    fOS_PWO4wrap[mid]->SetMaterialPropertiesTable(CrystWrapProperty);
  }
  
  G4double maxStep = 0.5*dz;
  fStepLimit = new G4UserLimits(maxStep);
 
  // Always return the physical world
  return worldPV;
}

void mxg4DetectorConstruction::ConstructSDandField() {
  // Sensitive detectors
  G4String trackerChamberSDname = "MPCEXsim/TrackerChamberSD";
  mxg4TrackerSD *aTrackerSD = new mxg4TrackerSD(trackerChamberSDname,"TrackerHitsCollection");
  G4String minipadSDname = "MPCEXsim/MinipadSD";
  mxg4TrackerSD *aminipadSD = new mxg4TrackerSD(minipadSDname,"MinipadHitsCollection");
  G4String mpcSDname = "MPCEXsim/MPCSD";
  mxg4TrackerSD *ampcSD = new mxg4TrackerSD(mpcSDname, "MPCHitsCollection");
  if(!ftoggleMinipad) {
    SetSensitiveDetector("Chambersil_LV", aTrackerSD, true);
  } else {
    SetSensitiveDetector("Minipad_LV",aminipadSD, true);
  }
  SetSensitiveDetector("Crystal_LV", ampcSD, true);

  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector();
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
    // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

void mxg4DetectorConstruction::SetMaxStep(G4double maxStep) {
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

void mxg4DetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps) {
  fCheckOverlaps = checkOverlaps;
}
