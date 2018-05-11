#include "Monster.h"

#include "IdleState.h"
#include "DefenseState.h"
#include "DeadState.h"

#include "Font.h"

Monster::Monster(std::wstring name, std::wstring scriptName, std::wstring spriteFileName) :
	Character(name, scriptName, spriteFileName)
{
	_eType = eComponentType::MONSTER;

	_moveTime = 0.0f;
	_moveRange = 5;

	//Stat Info
	{
		_attackPoint = 10;
		_damage = 0;
		_maxHp = 50;
		_hp = _maxHp - 40;

		_attackRange = 2;
	}
}

Monster::~Monster()
{
}

void Monster::InitState()
{
	ReplaceState(eStateType::IDLE, new IdleState());
	ReplaceState(eStateType::DEFENSE, new DefenseState());
	ReplaceState(eStateType::DEAD, new DeadState());
}

void Monster::UpdateCharacter()
{

}

void Monster::UpdateText()
{
	WCHAR state[100];
	ZeroMemory(state, sizeof(state));

	switch (_state->GetState())
	{
	case eStateType::ATTACK:
		wsprintf(state, L"ATTACK");
		break;
	case eStateType::DEAD:
		wsprintf(state, L"DEAD");
		break;
	case eStateType::DEFENSE:
		wsprintf(state, L"DEFENSE");
		break;
	case eStateType::IDLE:
		wsprintf(state, L"IDLE");
		break;
	case eStateType::MOVE:
		wsprintf(state, L"MOVE");
		break;
	case eStateType::PATHFINDING:
		wsprintf(state, L"PATHFINDING");
		break;
	case eStateType::SELECT_TARGET:
		wsprintf(state, L"WAITING SELECT");
		break;
	}

	WCHAR text[255];
	wsprintf(text, L"HP: %d\n", _hp);
	_font->SetText(text);
}