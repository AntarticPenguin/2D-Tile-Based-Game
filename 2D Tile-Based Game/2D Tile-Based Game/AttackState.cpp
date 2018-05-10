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

	if (eStateType::NONE != _nextState)
	{
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
	_curState = eStateType::ATTACK;

	std::vector<Component*> targets = _character->GetTargets();
	sComponentMsgParam msgParam;

	if (1 == targets.size())
	{
		msgParam.sender = (Component*)_character;
		msgParam.receiver = targets[0];
		msgParam.message = L"Attack";
		msgParam.attackPoint = _character->GetAttackPoint();
		ComponentSystem::GetInstance().SendMessageToComponent(msgParam);
	}
	else if(1 < targets.size())
	{
		for (int i = 0; i < targets.size(); i++)
		{
			msgParam.sender = (Component*)_character;
			msgParam.receiver = targets[i];
			msgParam.message = L"Attack";
			msgParam.attackPoint = _character->GetAttackPoint();
			ComponentSystem::GetInstance().SendMessageToComponent(msgParam);
		}
	}

	_character->SetCanBattle(false);

	_nextState = eStateType::IDLE;
}

void AttackState::Stop()
{
	State::Stop();
	_character->DecreaseBehaviorPoint(2);
	_character->ResetTargets();
	_character->SetTargetTileCell(NULL);
}