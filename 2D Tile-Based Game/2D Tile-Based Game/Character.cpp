#include "ComponentSystem.h"
#include "ComponentMessage.h"

#include "Map.h"
#include "Sprite.h"
#include "Font.h"

#include "DeadState.h"
#include "CounterAttackState.h"
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

	_attackCooltimeDuration = 0.0f;
	_attackCooltime = 1.0f;				//attackSpeed

	_counterTarget = NULL;
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
		State* state = new CounterAttackState();
		state->Init(this);
		_stateMap[eStateType::ET_COUNTERATTACK] = state;
	}
	{
		State* state = new DeadState();
		state->Init(this);
		_stateMap[eStateType::ET_DEAD] = state;
	}

	ChangeState(eStateType::ET_IDLE);

	//Font
	{
		D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 0, 0);
		_font = new Font(L"Arial", 15, color);

		_font->SetRect(100, 100, 400, 100);
		UpdateText();
	}
}

void Character::Deinit()
{
	delete _font;

	for (std::map<eStateType, State*>::iterator itr = _stateMap.begin(); itr != _stateMap.end(); itr++)
	{
		State* state = itr->second;
		delete state;
	}
	_stateMap.clear();
}

void Character::Update(float deltaTime)
{
	UpdateAttackCooltime(deltaTime);
	_state->Update(deltaTime);

	UpdateText();
}

void Character::Render()
{
	_state->Render();

	_font->SetPosition(_x - 200, _y - 50);
	_font->Render();
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

	if (_hp <= 0)
	{
		//DEAD
		_isLive = false;
		_hp = 0;
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
	_state->NextState(eStateType::ET_MOVE);
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

	//�ִϸ��̼� �̵� ����
	{
		map->SetTileComponent(_tileX, _tileY, this, false);

		//�̵��Ÿ� ���
		_targetX = map->GetPositionX(_tileX, _tileY);
		_targetY = map->GetPositionY(_tileX, _tileY);

		float distanceX = _targetX - _x;		//���밪 �ʿ����.
		float distanceY = _targetY - _y;

		//�����̵��ð�
		_moveDistancePerTimeX = distanceX / _moveTime;
		_moveDistancePerTimeY = distanceY / _moveTime;
	}
}

void Character::MoveStop()
{
	_isMoving = false;

	//�̵��� �����ϸ� Ÿ���� ��Ȯ�� ��ġ�� ������ �Ѵ�.
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
	/*
	for (std::list<Component*>::iterator itr = collisionList.begin(); itr != collisionList.end(); itr++)
	{
		sComponentMsgParam msgParam;
		msgParam.sender = this;
		msgParam.receiver = (*itr);
		msgParam.message = L"Collision";
		ComponentSystem::GetInstance().SendMessageToComponent(msgParam);
	}
	*/
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
		_counterTarget = msgParam.sender;
		_attackedPoint = msgParam.attackPoint;
		_state->NextState(eStateType::ET_DEFENSE);
	}
}

Component* Character::GetTarget()
{
	return _target;
}

Component* Character::GetCounterTarget()
{
	return _counterTarget;
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

void Character::UpdateAttackCooltime(float deltaTime)
{
	if (_attackCooltimeDuration < _attackCooltime)
	{
		_attackCooltimeDuration += deltaTime;
	}
	else
	{
		_attackCooltimeDuration = _attackCooltime;
	}
}

bool Character::IsAttackCooltime()
{
	if (_attackCooltime <= _attackCooltimeDuration)
		return true;
	return false;
}

void Character::ResetAttackCooltime()
{
	_attackCooltimeDuration = 0.0f;
}

void Character::UpdateText()
{
	int coolTime = (int)(_attackCooltimeDuration * 1000.0f);
	WCHAR state[100];

	switch (_state->GetState())
	{
	case eStateType::ET_ATTACK:
		wsprintf(state, L"ATTACK");
		break;
	case eStateType::ET_COUNTERATTACK:
		wsprintf(state, L"COUNTER");
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
	}

	WCHAR text[255];
	wsprintf(text, L"HP:%d\nCool: %d\nState:%s", _hp, coolTime, state);

	_font->SetText(text);
}