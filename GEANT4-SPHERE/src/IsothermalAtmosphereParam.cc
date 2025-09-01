#include "IsothermalAtmosphereParam.hh"

#include "G4Sphere.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <sstream>
#include <cmath>

IsothermalAtmosphereParam::IsothermalAtmosphereParam(const IsoAtmCfg& c) : cfg_(c) {
  const G4double Rgas = Avogadro * k_Boltzmann;
  H_   = (Rgas * cfg_.T) / (cfg_.M * cfg_.g);
  X0_  = cfg_.P0 / cfg_.g;
  Xtop_= X0_ * std::exp(-(cfg_.hTop - cfg_.h0)/H_);
  BuildLayerBoundsEqualX();
  BuildMaterials();
}

void IsothermalAtmosphereParam::BuildLayerBoundsEqualX() {
  hBound_.resize(cfg_.N + 1);
  for (int j=0; j<=cfg_.N; ++j) {
    // 등간격 컬럼 깊이로 경계 설정: Xj = X0 - (X0 - Xtop) * j/N
    const G4double Xj = X0_ - (X0_ - Xtop_) * ( (double)j / (double)cfg_.N );
    // 역함수: h = h0 - H ln(Xj/X0)
    hBound_[j] = cfg_.h0 - H_ * std::log(Xj / X0_);
  }
}

G4double IsothermalAtmosphereParam::PressureAt(G4double h) const {
  return cfg_.P0 * std::exp( -(h - cfg_.h0)/H_ );
}

G4double IsothermalAtmosphereParam::DensityAt(G4double h) const {
  const G4double Rgas = Avogadro * k_Boltzmann;
  return (cfg_.M/(Rgas * cfg_.T)) * PressureAt(h);
}

void IsothermalAtmosphereParam::BuildMaterials() {
  mats_.resize(cfg_.N);
  auto* nist = G4NistManager::Instance();
  for (int i=0; i<cfg_.N; ++i) {
    const G4double hMid = 0.5*(hBound_[i] + hBound_[i+1]);
    const G4double Pmid = PressureAt(hMid);
    const G4double T    = cfg_.T;

    std::ostringstream nm; nm << "AirLayer_" << i;
    // T, P에서 밀도가 자동 계산되는 가스 재질 생성
    mats_[i] = nist->ConstructNewGasMaterial(nm.str(), "G4_AIR", T, Pmid);
  }
}

void IsothermalAtmosphereParam::ComputeDimensions(G4Sphere& sph,
    const G4int copyNo, const G4VPhysicalVolume*) const {
  const G4double rmin = cfg_.R_earth + hBound_[copyNo];
  const G4double rmax = cfg_.R_earth + hBound_[copyNo+1];
  sph.SetInnerRadius(rmin);
  sph.SetOuterRadius(rmax);
  sph.SetStartPhiAngle(0.*CLHEP::deg);
  sph.SetDeltaPhiAngle(360.*CLHEP::deg);
  sph.SetStartThetaAngle(0.*CLHEP::deg);
  sph.SetDeltaThetaAngle(180.*CLHEP::deg);
}

G4Material* IsothermalAtmosphereParam::ComputeMaterial(G4int copyNo,
                                                       G4VPhysicalVolume* /*currentVol*/,
                                                       const G4VTouchable* /*touch*/) {
  return mats_[copyNo];
}

G4Material* IsothermalAtmosphereParam::ComputeMaterial(G4VPhysicalVolume* /*currentVol*/,
                                                       G4int copyNo,
                                                       const G4VTouchable* /*touch*/) {
  return mats_[copyNo];
}

G4Material* IsothermalAtmosphereParam::GetMaterial(G4int idx) const {
  return mats_[idx];
}