#include "Monster.h"

#include "IdleState.h"

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
}

void Monster::UpdateCharacter()
{

}