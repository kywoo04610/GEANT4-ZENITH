#include "PMDetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SDManager.hh"
#include "PMSensitiveDetector.hh"

PMDetectorConstruction::PMDetectorConstruction() {}
PMDetectorConstruction::~PMDetectorConstruction() {}

G4VPhysicalVolume* PMDetectorConstruction::Construct() {
    auto* nist = G4NistManager::Instance();
    const G4bool check = true;

    // Materials
    auto* air     = nist->FindOrBuildMaterial("G4_AIR");
    auto* groundM = nist->FindOrBuildMaterial("G4_CONCRETE");
    auto* detM    = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    // World (5 m cube)
    G4double worldSize = 5.0*m;
    auto* sWorld = new G4Box("sWorld", 0.5*worldSize, 0.5*worldSize, 0.5*worldSize);
    auto* lWorld = new G4LogicalVolume(sWorld, air, "lWorld");
    auto* pWorld = new G4PVPlacement(nullptr, {}, lWorld, "pWorld", nullptr, false, 0, check);

    // LabAir (1 m³ cube, 중심이 (0,0,0))
    G4double labSize = 1.0*m;
    auto* sLab = new G4Box("sLab", 0.5*labSize, 0.5*labSize, 0.5*labSize);
    auto* lLab = new G4LogicalVolume(sLab, air, "lLab");
    new G4PVPlacement(nullptr, {}, lLab, "pLab", lWorld, false, 0, check);

    // Ground slab (10 cm thick) at bottom of LabAir (z = -0.5m)
    G4double groundTh = 10.*cm;
    auto* sGround = new G4Box("sGround", 0.5*labSize, 0.5*labSize, 0.5*groundTh);
    auto* lGround = new G4LogicalVolume(sGround, groundM, "lGround");
    G4double groundZ = -0.5*labSize + 0.5*groundTh;
    new G4PVPlacement(nullptr, {0,0,groundZ}, lGround, "pGround", lLab, false, 0, check);

    // Detector size (18.40 × 18.40 × 2.70 cm³)
    G4double detX = 18.40*cm;
    G4double detY = 18.40*cm;
    G4double detZ = 2.70*cm;
    auto* sDet = new G4Box("sDet", 0.5*detX, 0.5*detY, 0.5*detZ);
    fLogicDet = new G4LogicalVolume(sDet, detM, "lDet");

    // Detector1: ground slab 위
    G4double det1Z = groundZ + 0.5*groundTh + 0.5*detZ;
    new G4PVPlacement(nullptr, {0,0,det1Z}, fLogicDet, "pDet1", lLab, false, 1, check);

    // Detector2: Detector1 위 280 mm
    G4double det2Z = det1Z + 280.*mm;
    new G4PVPlacement(nullptr, {0,0,det2Z}, fLogicDet, "pDet2", lLab, false, 2, check);

    // Visualization
    lGround->SetVisAttributes(new G4VisAttributes(G4Color(0.3,0.8,0.4,0.6)));
    auto* detVis = new G4VisAttributes(G4Color(1.0,1.0,0.0,0.9));
    detVis->SetForceSolid(true);
    fLogicDet->SetVisAttributes(detVis);

    ConstructSDandField();
    return pWorld;
}

void PMDetectorConstruction::ConstructSDandField() {
    auto* sd = new PMSensitiveDetector("MuonCounterSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(sd);
    if (fLogicDet) fLogicDet->SetSensitiveDetector(sd);
}
