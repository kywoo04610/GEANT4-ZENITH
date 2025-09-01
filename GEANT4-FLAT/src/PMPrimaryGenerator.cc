#include "PMPrimaryGenerator.hh"

#include "G4SystemOfUnits.hh"
#include "G4MuonMinus.hh"   // 필요시 MuonPlus.hh
#include "Randomize.hh"

PMPrimaryGenerator::PMPrimaryGenerator() {
    fParticleGun = new G4ParticleGun(1);

    // 입자 정의: muon-
    fParticleGun->SetParticleDefinition(G4MuonMinus::Definition());

    // 에너지: 3 GeV
    fParticleGun->SetParticleEnergy(3.0*GeV);

    // 방향: 수직 하향
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
}

PMPrimaryGenerator::~PMPrimaryGenerator() {
    delete fParticleGun;
}

void PMPrimaryGenerator::GeneratePrimaries(G4Event* anEvent) {
    // 1 m³ 박스 윗면 (z=+0.5 m), x,y는 균일 분포 [-0.5 m, +0.5 m]
    double x = (G4UniformRand() - 0.5)*1.0*m;
    double y = (G4UniformRand() - 0.5)*1.0*m;
    double z = +0.5*m;

    fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));

    fParticleGun->GeneratePrimaryVertex(anEvent);
}
