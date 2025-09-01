#include "PMPrimaryGenerator.hh"
#include "G4Proton.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

PMPrimaryGenerator::PMPrimaryGenerator() {
    fParticleGun = new G4ParticleGun(1);
    fParticleGun->SetParticleDefinition(G4Proton::Proton());
    fParticleGun->SetParticleEnergy(50.*GeV); // Cosmic ray proton
}

PMPrimaryGenerator::~PMPrimaryGenerator() {
    delete fParticleGun;
}

void PMPrimaryGenerator::GeneratePrimaries(G4Event* evt) {
    const G4double Rgen = 101.*km;  // Atmosphere (100 km) 바깥에서 시작

    // Uniform 방향 (구 표면 위 무작위 점)
/*
   u = cosθ ∈ [-1,1], φ ∈ [0,2π]
   rhat = (√(1-u²)cosφ, √(1-u²)sinφ, u)
*/
    const G4double u   = 2.*G4UniformRand() - 1.;
    const G4double phi = twopi*G4UniformRand();
    const G4double s   = std::sqrt(1.-u*u);

    G4ThreeVector rhat(s*std::cos(phi), s*std::sin(phi), u);

    // 위치 = 반경 Rgen 구 위
    G4ThreeVector pos = Rgen * rhat;

    // 방향 = Detector(0,0,0) - pos
    G4ThreeVector dir = (-pos).unit();

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(dir);
    fParticleGun->GeneratePrimaryVertex(evt);
}
