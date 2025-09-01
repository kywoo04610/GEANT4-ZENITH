#ifndef PMDETECTORCONSTRUCTION_HH
#define PMDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

class PMDetectorConstruction : public G4VUserDetectorConstruction {
public:
    PMDetectorConstruction();
    virtual ~PMDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

private:
    G4LogicalVolume* fLogicDet = nullptr;
    virtual void ConstructSDandField();
};

#endif
