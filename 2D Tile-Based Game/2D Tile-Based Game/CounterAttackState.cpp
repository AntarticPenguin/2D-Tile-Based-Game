#include "ComponentSystem.h"
#include "CounterAttackState.h"
#include "Character.h"

CounterAttackState::CounterAttackState()
{

}


CounterAttackState::~CounterAttackState()
{

}

void CounterAttackState::Init(Character* character)
{
	State::Init(character);
}

void CounterAttackState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}
}

void CounterAttackState::Render()
{
	State::Render();
}

void CounterAttackState::Release()
{
	State::Release();
}

void CounterAttackState::Reset()
{
	State::Reset();
}

void CounterAttackState::Start()
{
	State::Start();
	_curState = eStateType::ET_COUNTERATTACK;

	sComponentMsgParam msgParam;
	msgParam.sender = (Component*)_character;
	msgParam.receiver = _character->GetCounterTarget();
	msgParam.message = L"Attack";
	msgParam.attackPoint = _character->GetAttackPoint();
	ComponentSystem::GetInstance().SendMessageToComponent(msgParam);

	_nextState = eStateType::ET_MOVE;
}

void CounterAttackState::Stop()
{
	State::Stop();
}