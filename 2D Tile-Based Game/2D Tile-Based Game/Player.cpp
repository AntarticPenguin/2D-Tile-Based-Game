#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Stage.h"
#include "Map.h"
#include "Player.h"

#include "State.h"
#include "DeadState.h"
#include "CounterAttackState.h"
#include "RecoveryState.h"
#include "DefenseState.h"
#include "AttackState.h"
#include "MoveState.h"
#include "IdleState.h"

Player::Player(std::wstring name, std::wstring scriptName, std::wstring spriteFileName) :
	Character(name, scriptName, spriteFileName)
{
	_moveTime = 0.1f;
	_tileX = 32;
	_tileY = 15;
	_eType = eComponentType::CT_PLAYER;

	//Stat Info
	{
		_attackPoint = 10;
		_attackedPoint = 0;
		_maxHp = 50;
		_hp = _maxHp - 40;

		_recoveryStat = 5;

		_attackCooltimeDuration = 0.0f;
		_attackCooltime = 1.0f;				//attackSpeed

		_recoveryCooltimeDuration = 0.0f;
		_recoveryCooltime = 3.0f;
	}
}

Player::~Player()
{

}

void Player::InitState()
{
	ReplaceState(eStateType::ET_IDLE, new IdleState());
	ReplaceState(eStateType::ET_MOVE, new MoveState());
	ReplaceState(eStateType::ET_ATTACK, new AttackState());
	ReplaceState(eStateType::ET_DEFENSE, new DefenseState());
	ReplaceState(eStateType::ET_COUNTERATTACK, new CounterAttackState());
	ReplaceState(eStateType::ET_RECOVERY, new RecoveryState());
	ReplaceState(eStateType::ET_DEAD, new DeadState());
}

void Player::UpdateAI(float deltaTime)
{
	eDirection direction = eDirection::NONE;
	if (GameSystem::GetInstance().IsKeyDown(VK_UP))
	{
		direction = eDirection::UP;
	}
	if (GameSystem::GetInstance().IsKeyDown(VK_DOWN))
	{
		direction = eDirection::DOWN;
	}
	if (GameSystem::GetInstance().IsKeyDown(VK_LEFT))
	{
		direction = eDirection::LEFT;
	}
	if (GameSystem::GetInstance().IsKeyDown(VK_RIGHT))
	{
		direction = eDirection::RIGHT;
	}

	//������ �Ա� �� ����
	if (GameSystem::GetInstance().IsKeyDown(VK_SPACE))
	{
		Map* map = GameSystem::GetInstance().GetStage()->GetMap();

		std::list<Component*> componentList = map->GetTileComponentList(_tileX, _tileY);
		for (std::list<Component*>::iterator itr = componentList.begin(); itr != componentList.end(); itr++)
		{
			Component* component = (*itr);
			if (eComponentType::CT_ITEM == component->GetType() ||
				eComponentType::CT_ITEM_WEAPON == component->GetType())
			{
				sComponentMsgParam msgParam;
				msgParam.sender = (Component*)this;
				msgParam.receiver = component;
				msgParam.message = L"Use";
				ComponentSystem::GetInstance().SendMessageToComponent(msgParam);
			}
		}
	}

	if (eDirection::NONE != direction)
	{
		_curDirection = direction;
		_state->NextState(eStateType::ET_MOVE);
	}
}

Component* Player::Collision(std::list<Component*>& collisionList)
{
	for (std::list<Component*>::iterator itr = collisionList.begin(); itr != collisionList.end(); itr++)
	{
		Component* com = (*itr);
		if (eComponentType::CT_MONSTER == com->GetType())
		{
			return com;
		}
	}
	return NULL;
}