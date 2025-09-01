#ifndef PMPRIMARYGENERATOR_HH
#define PMPRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

class PMPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
    PMPrimaryGenerator();
    ~PMPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event*);

private:
    G4ParticleGun* fParticleGun;
};

#endif
