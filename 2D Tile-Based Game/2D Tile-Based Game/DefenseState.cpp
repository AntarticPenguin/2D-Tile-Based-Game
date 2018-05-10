#include "DefenseState.h"
#include "Character.h"

DefenseState::DefenseState()
{

}

DefenseState::~DefenseState()
{

}

void DefenseState::Init(Character* character)
{
	State::Init(character);
}

void DefenseState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}
}

void DefenseState::Render()
{
	State::Render();
}

void DefenseState::Release()
{
	State::Release();
}

void DefenseState::Reset()
{
	State::Reset();
}

void DefenseState::Start()
{
	State::Start();

	_curState = eStateType::DEFENSE;
	
	int defenseRate = (rand() % 100) + 1;
	
	if (0 != (defenseRate % 5))
	{
		int attackPoint = _character->GetAttackedPoint();
		_character->DecreaseHP(attackPoint);
	}

	if(false == _character->IsLive())
	{
		_character->SetCanMove(true);
		_character->MoveStop();
		_nextState = eStateType::DEAD;
	}
	else
	{
		_nextState = eStateType::IDLE;
	}
}

void DefenseState::Stop()
{
	State::Stop();
}