#include "ComponentSystem.h"
#include "ComponentMessage.h"
#include "GameSystem.h"
#include "Stage.h"

#include "Map.h"
#include "Sprite.h"
#include "Font.h"

#include "DeadState.h"
#include "CounterAttackState.h"
#include "RecoveryState.h"
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

	//Stat Info
	{
		_attackPoint = 10;
		_attackedPoint = 0;
		_maxHp = 20;
		_hp = _maxHp;

		_recoveryStat = 5;

		_attackCooltimeDuration = 0.0f;
		_attackCooltime = 1.0f;				//attackSpeed

		_recoveryCooltimeDuration = 0.0f;
		_recoveryCooltime = 3.0f;
	}

	_counterTarget = NULL;
}

Character::~Character()
{

}

void Character::Init()
{
	{
		Map* map = GameSystem::GetInstance().GetStage()->GetMap();

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
		State* state = new RecoveryState();
		state->Init(this);
		_stateMap[eStateType::ET_RECOVERY] = state;
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
		//UpdateText();
	}
}

void Character::Init(int tileX, int tileY)
{
	{
		Map* map = GameSystem::GetInstance().GetStage()->GetMap();

		bool canMove = false;
		_tileX = tileX;
		_tileY = tileY;

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
		State* state = new RecoveryState();
		state->Init(this);
		_stateMap[eStateType::ET_RECOVERY] = state;
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
		//UpdateText();
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
	if (false == _isLive)
		return;
	UpdateAttackCooltime(deltaTime);
	UpdateRecoveryCooltime(deltaTime);
	_state->Update(deltaTime);

	//UpdateText();
}

void Character::Render()
{
	if (false == _isLive)
		return;

	_state->Render();

	_font->SetPosition(_x, _y);
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

		_state->NextState(eStateType::ET_DEAD);
	}
}

void Character::Equip(Component* weapon)
{
	_equipList.push_back(weapon);
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

	Map* map = GameSystem::GetInstance().GetStage()->GetMap();
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
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();
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

void Character::UpdateRecoveryCooltime(float deltaTime)
{
	if (_recoveryCooltimeDuration < _recoveryCooltime)
	{
		_recoveryCooltimeDuration += deltaTime;
	}
	else
	{
		_recoveryCooltimeDuration = _recoveryCooltime;
	}
}

bool Character::IsHpFull()
{
	if (_maxHp == _hp)
		return true;
	return false;
}

bool Character::IsRecoveryCoolTime()
{
	if (_recoveryCooltime <= _recoveryCooltimeDuration)
		return true;
	return false;
}

void Character::RecoveryHP()
{
	_hp += _recoveryStat;

	if (_maxHp < _hp)
		_hp = _maxHp;
}

void Character::RecoveryHP(int hp)
{
	_hp += hp;

	if (_maxHp < _hp)
		_hp = _maxHp;
}

void Character::ResetRecoveryCooltime()
{
	_recoveryCooltimeDuration = 0.0f;
}

void Character::UpdateText()
{
	int coolTime = (int)(_attackCooltimeDuration * 1000.0f);
	int RecoveryTime = (int)(_recoveryCooltimeDuration * 1000.0f);

	WCHAR state[100];
	ZeroMemory(state, sizeof(state));

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
	case eStateType::ET_RECOVERY:
		wsprintf(state, L"RECOVERY");
		break;
	}


	WCHAR text[255];
	wsprintf(text, L"HP:%d\nCool: %d\n", _hp, coolTime);
	//wsprintf(text, L"HP:%d\nCool: %d\nState:%s\nRecovery:%d\nATK:%d", _hp, coolTime, state, RecoveryTime, _attackPoint);

	_font->SetText(text);
}