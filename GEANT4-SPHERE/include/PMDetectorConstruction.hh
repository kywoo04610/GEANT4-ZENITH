#ifndef PMDETECTORCONSTRUCTION_HH
#define PMDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"

class G4VPhysicalVolume;

class PMSensitiveDetector;
class IsothermalAtmosphereParam;

class PMDetectorConstruction : public G4VUserDetectorConstruction {
public:
    PMDetectorConstruction();
    ~PMDetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;

private:
    G4LogicalVolume* logicDetector = nullptr;
    G4LogicalVolume* fAtmMotherLV  = nullptr; // 대기 모체 LV 보관

    void BuildIsothermalAtmosphere(G4LogicalVolume* worldLV);
    void ConstructSDandField() override;
};

#endif
