#pragma once
#include "G4VNestedParameterisation.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include <vector>

struct IsoAtmCfg {
    G4double R_earth;
    G4double h0;
    G4double hTop;
    G4double T;
    G4double P0;
    G4double M;
    G4double g;
    G4int    N;
    G4ThreeVector earthCenter; // 반드시 호출측에서 설정
};

class G4Sphere;
class G4Material;
class G4VPhysicalVolume;

class IsothermalAtmosphereParam : public G4VNestedParameterisation {
public:
    explicit IsothermalAtmosphereParam(const IsoAtmCfg& c);
    ~IsothermalAtmosphereParam() override = default;

    void ComputeDimensions(G4Sphere& solid, G4int copyNo, const G4VPhysicalVolume*) const override;
    void ComputeTransformation(G4int, G4VPhysicalVolume*) const override {}

    G4Material* ComputeMaterial(G4int copyNo, G4VPhysicalVolume*, const G4VTouchable*) override;
    G4Material* ComputeMaterial(G4VPhysicalVolume*, G4int copyNo, const G4VTouchable*);

    G4Material* GetMaterial(G4int idx) const override;

    G4int GetNumberOfMaterials() const override { return (G4int)mats_.size(); }
    G4int GetNumberOfLayers() const { return (G4int)hBound_.size() - 1; }

    G4ThreeVector EarthCenter() const { return cfg_.earthCenter; }

private:
    IsoAtmCfg cfg_;
    G4double H_;
    G4double X0_, Xtop_;
    std::vector<G4double> hBound_;
    std::vector<G4Material*> mats_;

    void BuildLayerBoundsEqualX();
    void BuildMaterials();
    G4double PressureAt(G4double h) const;
    G4double DensityAt(G4double h) const;
};
