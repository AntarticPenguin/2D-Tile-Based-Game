#include "IdleState.h"
#include "Character.h"

#include "GameSystem.h"
#include "UISystem.h"

IdleState::IdleState()
{

}

IdleState::~IdleState()
{

}

void IdleState::Init(Character* character)
{
	State::Init(character);
}

void IdleState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	if (false == _character->IsLive())
		return;

	if (eComponentType::PLAYER == _character->GetType() && GameSystem::GetInstance().IsRightMouseDown())
	{
		if (UISystem::GetInstance().IsMenuOn())
			UISystem::GetInstance().TurnOffMenu();
	}

	_character->ChargeBehavior(deltaTime);
}

void IdleState::Start()
{
	State::Start();

	_curState = eStateType::IDLE;
}

void IdleState::Stop()
{
	State::Stop();
}