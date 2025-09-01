#include "PMPhysicsList.hh"

#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"

// --- 핵심: 하드로닉 물리 추가 ---
#include "G4HadronElasticPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"

// (선택) 저에너지 중성자까지 정밀히 보려면 HP 사용 가능:
// #include "G4HadronPhysicsFTFP_BERT_HP.hh"

#include "G4SystemOfUnits.hh"

PMPhysicsList::PMPhysicsList()
: G4VModularPhysicsList()
{
  // 추적 컷 기본값 (너무 작으면 느려짐)
  defaultCutValue = 1.0*mm;

  // 1) 전자기
  RegisterPhysics(new G4EmStandardPhysics());

  // 2) 붕괴 (π±, μ± 등)
  RegisterPhysics(new G4DecayPhysics());

  // 3) 하드로닉 (핵탄성 + 비탄성: 프로톤이 공기원자핵과 충돌해 π 생성)
  RegisterPhysics(new G4HadronElasticPhysics());
  RegisterPhysics(new G4HadronPhysicsFTFP_BERT());
  // HP 쓰려면 위 줄 대신 아래 줄:
  // RegisterPhysics(new G4HadronPhysicsFTFP_BERT_HP());

  // (참고) RadioactiveDecayPhysics는 방사성 붕괴용이라 여기선 필수 아님
}

PMPhysicsList::~PMPhysicsList() {}
