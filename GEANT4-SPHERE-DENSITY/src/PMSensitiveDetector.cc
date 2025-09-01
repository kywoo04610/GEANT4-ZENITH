#include "PMSensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

PMSensitiveDetector::PMSensitiveDetector(G4String name)
: G4VSensitiveDetector(std::move(name)) {}

PMSensitiveDetector::~PMSensitiveDetector() {}

void PMSensitiveDetector::Initialize(G4HCofThisEvent*) {
    fTotalEnergyDeposited = 0.;
}

G4bool PMSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
    auto* track = step->GetTrack();
    const auto pdg = track->GetDefinition()->GetPDGEncoding();

    // μ±만 카운트, 볼륨 "첫 스텝"에서만 1회 기록
    if ((pdg == 13 || pdg == -13) && step->IsFirstStepInVolume()) {
        auto* analysis = G4AnalysisManager::Instance();
        const G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

        const auto pre = step->GetPreStepPoint();
        const G4ThreeVector pos = pre->GetPosition();
        const G4double time = pre->GetGlobalTime();

        const G4ThreeVector dir = track->GetMomentumDirection().unit();
        const G4double cosTheta = dir.z(); // detector 법선(+z) 기준

        const G4double KE = track->GetKineticEnergy(); // 기록 단위: 내부 단위(기본 MeV)

        // Ntuple: eventID, x,y,z, time, cosTheta, KE
        analysis->FillNtupleIColumn(0, 0, eventID);
        analysis->FillNtupleDColumn(0, 1, pos.x());
        analysis->FillNtupleDColumn(0, 2, pos.y());
        analysis->FillNtupleDColumn(0, 3, pos.z());
        analysis->FillNtupleDColumn(0, 4, time);
        analysis->FillNtupleDColumn(0, 5, cosTheta);
        analysis->FillNtupleDColumn(0, 6, KE);
        analysis->AddNtupleRow();

        // 필요시: 중복 방지를 위해 여기서 track stop 하지 않음 (통과/에너지 손실 관측 가능)
    }

    // 총 dE 적산(참고용)
    const G4double edep = step->GetTotalEnergyDeposit();
    if (edep > 0.) fTotalEnergyDeposited += edep;

    return true;
}

void PMSensitiveDetector::EndOfEvent(G4HCofThisEvent*) {
    // 필요하면 히스토그램 추가 가능. 여기서는 루트 파일 용량 줄이기 위해 출력 생략.
    // G4cout << "Edep in detector: " << fTotalEnergyDeposited << G4endl;
}
