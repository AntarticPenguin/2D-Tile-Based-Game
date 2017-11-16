#include "Monster.h"
#include "ComponentSystem.h"
#include "Map.h"

#include "MoveState.h"

Monster::Monster(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName) :
	Character(name, scriptName, spriteFileName)
{
	_eType = eComponentType::CT_MONSTER;

	int speed = (rand() % 1500) + 200;
	_moveTime = (float)speed / 1000.0f;
}

Monster::~Monster()
{

}

void Monster::UpdateAI(float deltaTime)
{

	std::vector<eComponentType> compareTypeList;
	compareTypeList.push_back(eComponentType::CT_NPC);
	compareTypeList.push_back(eComponentType::CT_PLAYER);
	Component* findEnemy = ComponentSystem::GetInstance().FindComponentInRange(this, 4, compareTypeList);

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
			ChangeState(eStateType::ET_MOVE);
		}
	}
	else
	{
		//없으면 평소움직임
		Character::UpdateAI(deltaTime);
	}
}

void Monster::Collision(std::list<Component*>& collisionList)
{
	for (std::list<Component*>::iterator itr = collisionList.begin(); itr != collisionList.end(); itr++)
	{
		Component* com = (*itr);
		if (eComponentType::CT_NPC == com->GetType() || eComponentType::CT_PLAYER == com->GetType())
		{
			/*
			sComponentMsgParam msgParam;
			msgParam.sender = this;
			msgParam.receiver = com;
			msgParam.message = L"Attack";
			msgParam.attackPoint = _attackPoint;
			ComponentSystem::GetInstance().SendMessageToComponent(msgParam);
			*/
			_target = com;
			ChangeState(eStateType::ET_ATTACK);
			return;
		}
	}
	ChangeState(eStateType::ET_IDLE);
}