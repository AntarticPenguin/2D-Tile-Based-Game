#include "Monster.h"
#include "ComponentSystem.h"
#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"

#include "MoveState.h"

Monster::Monster(std::wstring name, std::wstring scriptName, std::wstring spriteFileName) :
	Character(name, scriptName, spriteFileName)
{
	_eType = eComponentType::CT_MONSTER;

	int speed = (rand() % 1000) + 200;
	_moveTime = (float)speed / 1000.0f;

	//Stat Info
	{
		_attackPoint = 10;
		_attackedPoint = 0;
		_maxHp = 20;
		_hp = _maxHp;

		_recoveryStat = 1;

		_attackCooltimeDuration = 0.0f;
		_attackCooltime = 2.0f;				//attackSpeed

		_recoveryCooltimeDuration = 0.0f;
		_recoveryCooltime = 4.0f;
	}
}

Monster::~Monster()
{

}

void Monster::UpdateAI(float deltaTime)
{
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();

	std::vector<eComponentType> compareTypeList;
	compareTypeList.push_back(eComponentType::CT_NPC);
	compareTypeList.push_back(eComponentType::CT_PLAYER);
	Component* findEnemy = ComponentSystem::GetInstance().FindComponentInRange(map, this, 4, compareTypeList);

	//적이 있으면
	if (NULL != findEnemy)
	{
		//추격 방향 설정
		eDirection direction = eDirection::NONE;

		if (findEnemy->GetTileX() < _tileX)
			direction = eDirection::LEFT;
		else if(_tileX < findEnemy->GetTileX())
			direction = eDirection::RIGHT;
		else if (findEnemy->GetTileY() < _tileY)
			direction = eDirection::UP;
		else if (_tileY < findEnemy->GetTileY())
			direction = eDirection::DOWN;

		if (eDirection::NONE != direction)
		{
			_curDirection = direction;
			_state->NextState(eStateType::ET_MOVE);
		}
	}
	else
	{
		//없으면 평소움직임
		Character::UpdateAI(deltaTime);
	}
}

Component* Monster::Collision(std::list<Component*>& collisionList)
{
	for (std::list<Component*>::iterator itr = collisionList.begin(); itr != collisionList.end(); itr++)
	{
		Component* com = (*itr);
		if (eComponentType::CT_NPC == com->GetType() || eComponentType::CT_PLAYER == com->GetType())
		{
			return com;
		}
	}
	return NULL;
}