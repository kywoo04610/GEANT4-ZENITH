#include "PMDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Sphere.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SDManager.hh"
#include "PMSensitiveDetector.hh"

PMDetectorConstruction::PMDetectorConstruction() {}
PMDetectorConstruction::~PMDetectorConstruction() {}

G4VPhysicalVolume* PMDetectorConstruction::Construct() {
    const G4bool checkOverlaps = true;
    auto* nist = G4NistManager::Instance();

    // --- World (진공, 크게 설정) ---
    auto* worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    const G4double worldR  = 120.*km;   // atmosphere보다 크게

    auto* solidWorld = new G4Sphere("solidWorld", 0., worldR,
                                    0.*deg, 360.*deg, 0.*deg, 180.*deg);
    auto* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicalWorld");
    auto* physWorld  = new G4PVPlacement(nullptr, {}, logicWorld, "physWorld",
                                         nullptr, false, 0, checkOverlaps);

    // --- Atmosphere Sphere (Detector를 감싸는 공기) ---
    auto* air = nist->FindOrBuildMaterial("G4_AIR");
    const G4double atmR = 100.*km;

    auto* solidAtm = new G4Sphere("solidAtm", 0., atmR,
                                  0.*deg, 360.*deg, 0.*deg, 180.*deg);
    auto* logicAtm = new G4LogicalVolume(solidAtm, air, "logicalAtm");
    new G4PVPlacement(nullptr, {}, logicAtm, "physAtm",
                      logicWorld, false, 0, checkOverlaps);

    // --- Detector (중심에 박스) ---
    auto* detMat = nist->FindOrBuildMaterial("G4_Si");
    const G4double detXY = 10.*cm;
    const G4double detZ  = 1.*cm;

    auto* solidDetector = new G4Box("solidDetector",
                                    0.5*detXY, 0.5*detXY, 0.5*detZ);
    logicDetector = new G4LogicalVolume(solidDetector, detMat, "logicDetector");

    new G4PVPlacement(nullptr, {}, logicDetector, "physDetector",
                      logicAtm, false, 0, checkOverlaps);

    auto* detVis = new G4VisAttributes(G4Color(1.0,1.0,0.0));
    detVis->SetForceSolid(true);
    logicDetector->SetVisAttributes(detVis);

    ConstructSDandField();
    return physWorld;
}

void PMDetectorConstruction::ConstructSDandField() {
    auto* sensDet = new PMSensitiveDetector("SensitiveDetector");
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
    logicDetector->SetSensitiveDetector(sensDet);
}
