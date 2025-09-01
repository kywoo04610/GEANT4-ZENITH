#ifndef PMSENSITIVEDETECTOR_HH
#define PMSENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"

class PMSensitiveDetector : public G4VSensitiveDetector {
public:
    explicit PMSensitiveDetector(G4String name);
    ~PMSensitiveDetector() override;

    void Initialize(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
    void EndOfEvent(G4HCofThisEvent*) override;

private:
    G4double fTotalEnergyDeposited = 0.;
};

#endif
