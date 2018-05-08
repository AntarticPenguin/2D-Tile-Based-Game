#include "Monster.h"

#include "IdleState.h"
#include "DefenseState.h"
#include "DeadState.h"

#include "Font.h"

Monster::Monster(std::wstring name, std::wstring scriptName, std::wstring spriteFileName) :
	Character(name, scriptName, spriteFileName)
{
	_eType = eComponentType::CT_MONSTER;

	_moveTime = 0.0f;
	_moveRange = 5;

	//Stat Info
	{
		_attackPoint = 10;
		_attackedPoint = 0;
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
	ReplaceState(eStateType::ET_IDLE, new IdleState());
	ReplaceState(eStateType::ET_DEFENSE, new DefenseState());
	ReplaceState(eStateType::ET_DEAD, new DeadState());
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
	case eStateType::ET_ATTACK:
		wsprintf(state, L"ATTACK");
		break;
	case eStateType::ET_DEAD:
		wsprintf(state, L"DEAD");
		break;
	case eStateType::ET_DEFENSE:
		wsprintf(state, L"DEFENSE");
		break;
	case eStateType::ET_IDLE:
		wsprintf(state, L"IDLE");
		break;
	case eStateType::ET_MOVE:
		wsprintf(state, L"MOVE");
		break;
	case eStateType::ET_PATHFINDING:
		wsprintf(state, L"PATHFINDING");
		break;
	case eStateType::ET_SELECT_TARGET:
		wsprintf(state, L"WAITING SELECT");
		break;
	}

	WCHAR text[255];
	wsprintf(text, L"HP: %d\n", _hp);
	_font->SetText(text);
}