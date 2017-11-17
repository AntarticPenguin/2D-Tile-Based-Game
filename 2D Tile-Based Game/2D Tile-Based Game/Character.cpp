#include "ComponentSystem.h"
#include "ComponentMessage.h"

#include "Map.h"
#include "Sprite.h"

#include "DeadState.h"
#include "DefenseState.h"
#include "AttackState.h"
#include "MoveState.h"
#include "IdleState.h"
#include "Character.h"

Character::Character(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName) :
	Component(name), _x(0.0f), _y(0.0f)
{
	_state = NULL;

	_spriteFileName = spriteFileName;
	_scriptFileName = scriptName;

	_moveTime = 1.0f;

	_tileX = 10;
	_tileY = 10;

	_attackPoint = 10;
	_attackedPoint = 0;
	_hp = 20;
}

Character::~Character()
{

}

void Character::Init()
{
	{
		Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");

		bool canMove = false;
		while (true != canMove)
		{
			_tileX = rand() % (map->GetWidth() - 1) + 1;
			_tileY = rand() % (map->GetHeight() - 1) + 1;

			canMove = map->CanMoveTileMap(_tileX, _tileY);
		}

		_x = map->GetPositionX(_tileX, _tileY);
		_y = map->GetPositionY(_tileX, _tileY);
		map->SetTileComponent(_tileX, _tileY, this, false);
	}

	InitMove();

	{
		State* state = new IdleState();
		state->Init(this);
		_stateMap[eStateType::ET_IDLE] = state;
	}
	{
		State* state = new MoveState();
		state->Init(this);
		_stateMap[eStateType::ET_MOVE] = state;
	}
	{
		State* state = new AttackState();
		state->Init(this);
		_stateMap[eStateType::ET_ATTACK] = state;
	}
	{
		State* state = new DefenseState();
		state->Init(this);
		_stateMap[eStateType::ET_DEFENSE] = state;
	}
	{
		State* state = new DeadState();
		state->Init(this);
		_stateMap[eStateType::ET_DEAD] = state;
	}

	ChangeState(eStateType::ET_IDLE);
}

void Character::Deinit()
{
	for (std::map<eStateType, State*>::iterator itr = _stateMap.begin(); itr != _stateMap.end(); itr++)
	{
		State* state = itr->second;
		delete state;
	}
	_stateMap.clear();
}

void Character::Update(float deltaTime)
{
	_state->Update(deltaTime);
}

void Character::Render()
{
	_state->Render();
}

void Character::Release()
{
	_state->Release();
}

void Character::Reset()
{
	_state->Reset();
}

std::wstring Character::GetTextureFileName()
{
	return _spriteFileName;
}

std::wstring Character::GetScriptFileName()
{
	return _scriptFileName;
}

float Character::GetX()
{
	return _x;
}

float Character::GetY()
{
	return _y;
}

void Character::MoveDeltaPosition(float deltaX, float deltaY)
{
	_x += deltaX;
	_y += deltaY;
}

void Character::SetPosition(float posX, float posY)
{
	_x = posX;
	_y = posY;
}

int Character::GetAttackedPoint()
{
	return _attackedPoint;
}

void Character::DecreaseHP(int decreaseHP)
{
	_hp -= decreaseHP;

	if (_hp < 0)
	{
		//DEAD
		_isLive = false;
	}
}

void Character::InitMove()
{
	_isMoving = false;
	_curDirection = eDirection::DOWN;
}

void Character::UpdateAI(float deltaTime)
{
	_curDirection = (eDirection)(rand() % 4);
	ChangeState(eStateType::ET_MOVE);
}

void Character::ChangeState(eStateType stateType)
{
	if (NULL != _state)
	{
		_state->Stop();
	}

	_state = _stateMap[stateType];
	_state->Start();
}

void Character::MoveStart(int newTileX, int newTileY)
{
	_isMoving = true;

	Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");
	map->ResetTileComponent(_tileX, _tileY, this);
	_tileX = newTileX;
	_tileY = newTileY;

	//애니메이션 이동 보간
	{
		map->SetTileComponent(_tileX, _tileY, this, false);

		//이동거리 계산
		_targetX = map->GetPositionX(_tileX, _tileY);
		_targetY = map->GetPositionY(_tileX, _tileY);

		float distanceX = _targetX - _x;		//절대값 필요없음.
		float distanceY = _targetY - _y;

		//단위이동시간
		_moveDistancePerTimeX = distanceX / _moveTime;
		_moveDistancePerTimeY = distanceY / _moveTime;
	}
}

void Character::MoveStop()
{
	_isMoving = false;

	//이동후 도착하면 타일의 정확한 위치에 찍어줘야 한다.
	Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");
	_x = map->GetPositionX(_tileX, _tileY);
	_y = map->GetPositionY(_tileX, _tileY);

	_moveDistancePerTimeX = 0.0f;
	_moveDistancePerTimeY = 0.0f;
}

void Character::Moving(float deltaTime)
{
	float moveDistanceX = _moveDistancePerTimeX * deltaTime;
	float moveDistanceY = _moveDistancePerTimeY * deltaTime;
	_x += moveDistanceX;
	_y += moveDistanceY;
}

Component* Character::Collision(std::list<Component*>& collisionList)
{
	for (std::list<Component*>::iterator itr = collisionList.begin(); itr != collisionList.end(); itr++)
	{
		/*sComponentMsgParam msgParam;
		msgParam.sender = this;
		msgParam.receiver = (*itr);
		msgParam.message = L"Collision";
		ComponentSystem::GetInstance().SendMessageToComponent(msgParam);*/
	}
	return NULL;
}

eDirection Character::GetDirection()
{
	return _curDirection;
}

float Character::GetMoveTime()
{
	return _moveTime;
}

bool Character::IsMoving()
{
	return _isMoving;
}

void Character::ReceiveMessage(const sComponentMsgParam& msgParam)
{
	if (L"Attack" == msgParam.message)
	{
		/*
		int attackPoint = msgParam.attackPoint;
		_hp -= attackPoint;

		if (_hp < 0)
		{
			//DEAD
			_isLive = false;
			SetCanMove(true);

			//STOP
			_moveDistancePerTimeX = 0.0f;
			_moveDistancePerTimeY = 0.0f;
		}
		*/
		_attackedPoint = msgParam.attackPoint;
		ChangeState(eStateType::ET_DEFENSE);
	}
}

Component* Character::GetTarget()
{
	return _target;
}

void Character::SetTarget(Component* target)
{
	_target = target;
}

void Character::ResetTarget()
{
	_target = NULL;
}

int Character::GetAttackPoint()
{
	return _attackPoint;
}