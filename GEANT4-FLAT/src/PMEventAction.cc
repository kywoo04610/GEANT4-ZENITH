#include "PMEventAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

#include <sstream>
#include <limits>

PMEventAction::PMEventAction() {}
PMEventAction::~PMEventAction() {}

void PMEventAction::BeginOfEventAction(const G4Event*)
{
    fDetectorsHit.clear();
    fMuonEnergies.clear();
}

void PMEventAction::EndOfEventAction(const G4Event*)
{
    auto* m = G4AnalysisManager::Instance();
    int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    // Coincidence 조건: 두 개 이상 detector
    if (fDetectorsHit.size() >= 2) {
        // detCount
        int detCount = (int)fDetectorsHit.size();

        // detIDs 문자열 (ex: "1,2,3")
        std::stringstream ss;
        for (size_t i=0; i<fDetectorsHit.size(); i++) {
            ss << fDetectorsHit[i];
            if (i+1 < fDetectorsHit.size()) ss << ",";
        }
        G4String detIDs = ss.str();

        // mean, min, max
        double meanEk = 0.;
        double minEk = std::numeric_limits<double>::max();
        double maxEk = -1.0;
        for (auto e : fMuonEnergies) {
            meanEk += e;
            if (e < minEk) minEk = e;
            if (e > maxEk) maxEk = e;
        }
        meanEk /= fMuonEnergies.size();

        // Coincidence ntuple (index=1)
        m->FillNtupleIColumn(1, 0, eventID);
        m->FillNtupleIColumn(1, 1, detCount);
        m->FillNtupleSColumn(1, 2, detIDs);
        m->FillNtupleDColumn(1, 3, meanEk);
        m->FillNtupleDColumn(1, 4, minEk);
        m->FillNtupleDColumn(1, 5, maxEk);
        m->AddNtupleRow(1);
    }
}

void PMEventAction::AddDetectorHit(G4int detID, G4double energy)
{
    fDetectorsHit.push_back(detID);
    fMuonEnergies.push_back(energy);
}
