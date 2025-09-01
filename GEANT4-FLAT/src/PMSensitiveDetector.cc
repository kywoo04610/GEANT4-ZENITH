#include "PMSensitiveDetector.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4SDManager.hh"
#include "PMEventAction.hh"
#include "G4EventManager.hh"

PMSensitiveDetector::PMSensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    fTotalEnergyDeposited = 0.;
}

PMSensitiveDetector::~PMSensitiveDetector() {}

void PMSensitiveDetector::Initialize(G4HCofThisEvent*)
{
    fTotalEnergyDeposited = 0.;
}

G4bool PMSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    auto* track = step->GetTrack();
    auto* def   = track->GetDefinition();

    // μ±만 기록
    if (def != G4MuonPlus::Definition() && def != G4MuonMinus::Definition())
        return false;

    // Detector 경계 진입일 때만 기록 (중복 방지)
    auto* pre = step->GetPreStepPoint();
    if (pre->GetStepStatus() != fGeomBoundary) return false;

    auto* touch = pre->GetTouchable();
    int detID   = touch ? touch->GetCopyNumber() : -1;
    int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    auto pos = pre->GetPosition();
    auto mom = pre->GetMomentum();
    double time = pre->GetGlobalTime();
    double Ek   = track->GetKineticEnergy();

    auto* m = G4AnalysisManager::Instance();
    m->FillNtupleIColumn(0, 0, eventID);
    m->FillNtupleIColumn(0, 1, detID);
    m->FillNtupleDColumn(0, 2, pos.x());
    m->FillNtupleDColumn(0, 3, pos.y());
    m->FillNtupleDColumn(0, 4, pos.z());
    m->FillNtupleDColumn(0, 5, time);
    m->FillNtupleDColumn(0, 6, Ek);
    m->FillNtupleDColumn(0, 7, mom.x());
    m->FillNtupleDColumn(0, 8, mom.y());
    m->FillNtupleDColumn(0, 9, mom.z());
    m->AddNtupleRow(0);

    // deposit energy 합산 (optional)
    double edep = step->GetTotalEnergyDeposit();
    if (edep > 0) fTotalEnergyDeposited += edep;

    auto* evAction = static_cast<PMEventAction*>(
        G4EventManager::GetEventManager()->GetUserEventAction());
    if(evAction) {
        evAction->AddDetectorHit(detID, Ek);
    }

    return true;
}

void PMSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    auto* m = G4AnalysisManager::Instance();
    m->FillH1(0, fTotalEnergyDeposited);
}
