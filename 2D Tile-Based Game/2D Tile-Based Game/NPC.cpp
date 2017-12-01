#include "NPC.h"
#include "ComponentSystem.h"
#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"

#include "DeadState.h"
#include "CounterAttackState.h"
#include "RecoveryState.h"
#include "DefenseState.h"
#include "AttackState.h"
#include "MoveState.h"
#include "IdleState.h"

NPC::NPC(std::wstring name, std::wstring scriptName, std::wstring spriteFileName) :
	Character(name, scriptName, spriteFileName)
{
	_eType = eComponentType::CT_NPC;
	
	int speed = (rand() % 1500) + 300;
	_moveTime = (float)speed / 1000.0f;

	//Stat Info
	{
		_attackPoint = 10;
		_attackedPoint = 0;
		_maxHp = 30;
		_hp = _maxHp;

		_recoveryStat = 5;

		_attackCooltimeDuration = 0.0f;
		_attackCooltime = 1.0f;				//attackSpeed

		_recoveryCooltimeDuration = 0.0f;
		_recoveryCooltime = 3.0f;
	}
}

NPC::~NPC()
{

}

void NPC::InitState()
{
	ReplaceState(eStateType::ET_IDLE, new IdleState());
	ReplaceState(eStateType::ET_MOVE, new MoveState());
	ReplaceState(eStateType::ET_ATTACK, new AttackState());
	ReplaceState(eStateType::ET_DEFENSE, new DefenseState());
	ReplaceState(eStateType::ET_COUNTERATTACK, new CounterAttackState());
	ReplaceState(eStateType::ET_RECOVERY, new RecoveryState());
	ReplaceState(eStateType::ET_DEAD, new DeadState());
}

void NPC::UpdateAI(float deltaTime)
{
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();

	std::vector<eComponentType> compareTypeList;
	compareTypeList.push_back(eComponentType::CT_MONSTER);
	Component* findEnemy = ComponentSystem::GetInstance().FindComponentInRange(map, this, 2, compareTypeList);

	//적이 있으면
	if (NULL != findEnemy)
	{
		//추격 방향 설정
		int findDirection = rand() % 4;
		int newTileX = _tileX;
		int newTileY = _tileY;

		switch (findDirection)
		{
		case eDirection::LEFT:
			newTileX--;
			break;
		case eDirection::RIGHT:
			newTileX++;
			break;
		case eDirection::UP:
			newTileY--;
			break;
		case eDirection::DOWN:
			newTileY++;
			break;
		}

		if (map->CanMoveTileMap(newTileX, newTileY))
		{
			if (eDirection::NONE != findDirection)
			{
				_curDirection = (eDirection)findDirection;
				_state->NextState(eStateType::ET_MOVE);
			}
		}
	}
	else
	{
		//없으면 평소움직임
		Character::UpdateAI(deltaTime);
	}
}