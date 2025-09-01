#include "PMActionInitialization.hh"
#include "PMEventAction.hh"

PMActionInitialization::PMActionInitialization()
{}

PMActionInitialization::~PMActionInitialization()
{}

void PMActionInitialization::BuildForMaster() const
{
    // 마스터 스레드: RunAction만 등록
    SetUserAction(new PMRunAction());
}

void PMActionInitialization::Build() const
{
    // 워커 스레드: Primary + RunAction
    SetUserAction(new PMPrimaryGenerator());
    SetUserAction(new PMRunAction());
    SetUserAction(new PMEventAction());

    // 필요하다면 여기서 EventAction / SteppingAction 추가 가능
}
