#include "ComponentSystem.h"
#include "AttackState.h"
#include "Character.h"

AttackState::AttackState()
{

}

AttackState::~AttackState()
{

}

void AttackState::Init(Character* character)
{
	State::Init(character);
}

void AttackState::Update(float deltaTime)
{
	State::Update(deltaTime);

	_character->ResetTarget();
	_character->ChangeState(eStateType::ET_IDLE);
}

void AttackState::Render()
{
	State::Render();
}

void AttackState::Release()
{
	State::Release();
}

void AttackState::Reset()
{
	State::Reset();
}

void AttackState::Start()
{
	State::Start();

	sComponentMsgParam msgParam;
	msgParam.sender = (Component*)_character;
	msgParam.receiver = _character->GetTarget();
	msgParam.message = L"Attack";
	msgParam.attackPoint = _character->GetAttackPoint();
	ComponentSystem::GetInstance().SendMessageToComponent(msgParam);
}

void AttackState::Stop()
{
	State::Stop();
}