#include "PMRunAction.hh"

PMRunAction::PMRunAction()
{
    auto* m = G4AnalysisManager::Instance();

    // 히스토그램 (총 deposit energy 모니터링용)
    m->CreateH1("Edep", "Energy deposit", 100, 0., 10.*MeV);

    // Muon용 ntuple
    m->CreateNtuple("Muons", "Muon hits on detectors");
    m->CreateNtupleIColumn("eventID"); // 0
    m->CreateNtupleIColumn("detID");   // 1
    m->CreateNtupleDColumn("x");       // 2
    m->CreateNtupleDColumn("y");       // 3
    m->CreateNtupleDColumn("z");       // 4
    m->CreateNtupleDColumn("t");       // 5 (ns)
    m->CreateNtupleDColumn("Ek");      // 6 (MeV)
    m->CreateNtupleDColumn("px");      // 7
    m->CreateNtupleDColumn("py");      // 8
    m->CreateNtupleDColumn("pz");      // 9
    m->FinishNtuple(0);

    // Coincidence용 ntuple
    m->CreateNtuple("Coincidence", "Events with >=2 detectors hit");
    m->CreateNtupleIColumn("eventID");
    m->CreateNtupleIColumn("detCount");
    m->CreateNtupleSColumn("detIDs");
    m->CreateNtupleDColumn("meanEk");
    m->CreateNtupleDColumn("minEk");
    m->CreateNtupleDColumn("maxEk");
    m->FinishNtuple(1);
}

PMRunAction::~PMRunAction() {}

void PMRunAction::BeginOfRunAction(const G4Run* run)
{
    auto* m = G4AnalysisManager::Instance();
    std::stringstream ss;
    ss << run->GetRunID();
    m->OpenFile("output" + ss.str() + ".root");
}

void PMRunAction::EndOfRunAction(const G4Run* run)
{
    auto* m = G4AnalysisManager::Instance();
    m->Write();
    m->CloseFile();
    G4cout << "Finishing run " << run->GetRunID() << G4endl;
}
