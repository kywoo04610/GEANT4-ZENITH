#include "PMRunAction.hh"
#include <sstream>

PMRunAction::PMRunAction() {
    auto* analysis = G4AnalysisManager::Instance();

    // 간단히 Ntuple 하나: Muon hits at detector
    analysis->CreateNtuple("Muons", "Muon hits at detector");
    analysis->CreateNtupleIColumn("eventID");
    analysis->CreateNtupleDColumn("x");
    analysis->CreateNtupleDColumn("y");
    analysis->CreateNtupleDColumn("z");
    analysis->CreateNtupleDColumn("time");
    analysis->CreateNtupleDColumn("cosTheta");
    analysis->CreateNtupleDColumn("KE"); // kinetic energy (MeV/GeV 단위는 기록 값 그대로)
    analysis->FinishNtuple();
}

PMRunAction::~PMRunAction() {}

void PMRunAction::BeginOfRunAction(const G4Run* run) {
    auto* analysis = G4AnalysisManager::Instance();
    std::stringstream ss; ss << run->GetRunID();
    analysis->OpenFile("output_run" + ss.str() + ".root");
}

void PMRunAction::EndOfRunAction(const G4Run* run) {
    auto* analysis = G4AnalysisManager::Instance();
    analysis->Write();
    analysis->CloseFile();
    G4cout << "Finishing run " << run->GetRunID() << G4endl;
}
