#include "IdleState.h"
#include "Character.h"

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

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	if (false == _character->IsLive())
		return;

	_character->ChargeBehavior(deltaTime);
}

void IdleState::Start()
{
	State::Start();

	_curState = eStateType::ET_IDLE;
}

void IdleState::Stop()
{
	State::Stop();
}