#ifndef PMACTIONINITIALIZATION_HH
#define PMACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"
#include "PMPrimaryGenerator.hh"
#include "PMRunAction.hh"
// 필요하다면 EventAction, SteppingAction 추가 가능

class PMActionInitialization : public G4VUserActionInitialization
{
public:
    PMActionInitialization();
    ~PMActionInitialization();

    virtual void BuildForMaster() const override;
    virtual void Build() const override;
};

#endif
