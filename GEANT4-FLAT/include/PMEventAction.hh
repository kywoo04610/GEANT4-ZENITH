#ifndef PMEVENTACTION_HH
#define PMEVENTACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>

class PMEventAction : public G4UserEventAction
{
public:
    PMEventAction();
    virtual ~PMEventAction();

    virtual void BeginOfEventAction(const G4Event*) override;
    virtual void EndOfEventAction(const G4Event*) override;

    // SensitiveDetector에서 호출
    void AddDetectorHit(G4int detID, G4double energy);

private:
    std::vector<G4int>    fDetectorsHit;   // 이벤트 동안 히트한 detector ID
    std::vector<G4double> fMuonEnergies;   // 각 히트의 Ek
};

#endif
