#include "PMPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"

// Hadronic
#include "G4HadronElasticPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"

#include "G4SystemOfUnits.hh"

PMPhysicsList::PMPhysicsList() {
  // EM & Decay
  RegisterPhysics(new G4EmStandardPhysics());
  RegisterPhysics(new G4DecayPhysics());

  // Hadronic full set
  RegisterPhysics(new G4HadronElasticPhysics());
  RegisterPhysics(new G4HadronPhysicsFTFP_BERT());
  RegisterPhysics(new G4StoppingPhysics());
  RegisterPhysics(new G4IonPhysics());

  SetDefaultCutValue(1.0*mm);
}

PMPhysicsList::~PMPhysicsList() {}
