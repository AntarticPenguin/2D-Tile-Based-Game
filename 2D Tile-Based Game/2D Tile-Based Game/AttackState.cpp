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

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ResetTarget();
		_character->ChangeState(_nextState);
		return;
	}
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
	_curState = eStateType::ET_ATTACK;

	sComponentMsgParam msgParam;
	msgParam.sender = (Component*)_character;
	msgParam.receiver = _character->GetTarget();
	msgParam.message = L"Attack";
	msgParam.attackPoint = _character->GetAttackPoint();
	//ComponentSystem::GetInstance().SendMessageToComponent(msgParam);

	_nextState = eStateType::ET_IDLE;
}

void AttackState::Stop()
{
	State::Stop();
}