#include "Character.h"

#include "ComponentSystem.h"
#include "ComponentMessage.h"
#include "GameSystem.h"

#include "Map.h"
#include "Stage.h"
#include "TileCell.h"

#include "AttackState.h"
#include "DefenseState.h"
#include "DeadState.h"
#include "IdleState.h"
#include "PathfindingMoveState.h"

#include "Font.h"

Character::Character(std::wstring name, std::wstring scriptName, std::wstring spriteFileName) :
	Component(name), _x(0.0f), _y(0.0f)
{
	_state = NULL;

	_spriteFileName = spriteFileName;
	_scriptFileName = scriptName;

	_moveTime = 1.0f;
	_moveRange = 6;

	_tileX = 10;
	_tileY = 10;

	//Stat Info
	{
		_behaviorPoint = 10;
		_behaviorDuration = 0.0f;
		_attackPoint = 10;
		_attackedPoint = 0;
		_attackRange = 1;

		_movePoint = 2;

		_canBattle = false;

		_maxHp = 20;
		_hp = _maxHp;
	}

	_targetTileCell = NULL;

	_targets.clear();
}

Character::~Character()
{

}

void Character::Init()
{
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();

	bool canMove = false;
	int tileX;
	int tileY;

	while (true)
	{
		tileX = rand() % (map->GetWidth() - 1) + 1;
		tileY = rand() % (map->GetHeight() - 1) + 1;

		canMove = map->CanMoveTileMap(tileX, tileY);

		if (true == canMove)
			break;
	}
	Init(tileX, tileY);
}

void Character::Init(int tileX, int tileY)
{
	{
		Map* map = GameSystem::GetInstance().GetStage()->GetMap();

		_tileX = tileX;
		_tileY = tileY;

		_x = map->GetPositionX(_tileX, _tileY);
		_y = map->GetPositionY(_tileX, _tileY);
		map->SetTileComponent(_tileX, _tileY, this, false);
	}

	InitMove();
	InitState();
	ChangeState(eStateType::ET_IDLE);

	//Font
	{
		D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 0);
		_font = new Font(L"Arial", 20, color);

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
	UpdateCharacter();
	_state->Update(deltaTime);

	UpdateText();
}

void Character::Render()
{
	_state->Render();

	_font->SetPosition(_x, _y - 20);
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

void Character::UpdateCharacter()
{

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


void Character::InitTilePosition(int tileX, int tileY)
{
	{
		Map* map = GameSystem::GetInstance().GetStage()->GetMap();

		map->ResetTileComponent(_tileX, _tileY, this);

		_tileX = tileX;
		_tileY = tileY;

		_x = map->GetPositionX(_tileX, _tileY);
		_y = map->GetPositionY(_tileX, _tileY);
		map->SetTileComponent(_tileX, _tileY, this, false);
	}
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


void Character::InitMove()
{
	_isMoving = false;
	_curDirection = eDirection::DOWN;
}

void Character::InitState()
{
	ReplaceState(eStateType::ET_IDLE, new IdleState());
	ReplaceState(eStateType::ET_MOVE, new PathfindingMoveState());
	ReplaceState(eStateType::ET_ATTACK, new AttackState());
	ReplaceState(eStateType::ET_DEFENSE, new DefenseState());
	ReplaceState(eStateType::ET_DEAD, new DeadState());
}

void Character::ReplaceState(eStateType changeType, State* replaceState)
{
	std::map<eStateType, State*>::iterator itr = _stateMap.find(changeType);
	if (itr != _stateMap.end())
	{
		delete itr->second;
		_stateMap.erase(changeType);
	}

	State* state = replaceState;
	state->Init(this);
	_stateMap[changeType] = state;
}

void Character::ChangeState(eStateType stateType)
{
	if (NULL != _state)
		_state->Stop();

	_state = _stateMap[stateType];
	_state->Start();
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

void Character::DecreaseBehaviorPoint(int point)
{
	_behaviorPoint -= point;

	if (_behaviorPoint <= 0)
	{
		_behaviorPoint = 0;
	}
}

void Character::ChargeBehavior(float deltaTime)
{
	if (10 <= _behaviorPoint)
	{
		_behaviorPoint = 10;
		_canBattle = true;
		return;
	}

	if (_behaviorDuration < 1.0f)
	{
		_behaviorDuration += deltaTime;
	}
	else
	{
		_behaviorPoint++;
		_behaviorDuration = 0.0f;
	}
}

int Character::GetBehaviorPoint()
{
	return _behaviorPoint;
}

int Character::GetMovePoint()
{
	return _movePoint;
}

void Character::SetCanBattle(bool canBattle)
{
	_canBattle = canBattle;
}

bool Character::CanBattle()
{
	return _canBattle;
}

void Character::MoveStart(int newTileX, int newTileY)
{
	_isMoving = true;

	Map* map = GameSystem::GetInstance().GetStage()->GetMap();
	map->ResetTileComponent(_tileX, _tileY, this);
	_tileX = newTileX;
	_tileY = newTileY;

	map->SetTileComponent(_tileX, _tileY, this, false);
}

void Character::MoveStop()
{
	_isMoving = false;
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

void Character::SetDirection(eDirection direction)
{
	_curDirection = direction;
}

float Character::GetMoveTime()
{
	return _moveTime;
}

bool Character::IsMoving()
{
	return _isMoving;
}

int Character::GetMoveRange()
{
	return _moveRange;
}

void Character::ReceiveMessage(const sComponentMsgParam& msgParam)
{
	if (L"Attack" == msgParam.message)
	{
		_attackedPoint = msgParam.attackPoint;
		_state->NextState(eStateType::ET_DEFENSE);
	}
}

std::vector<Component*>& Character::GetTargets()
{
	return _targets;
}

void Character::AddTarget(Component* target)
{
	_targets.push_back(target);
}

void Character::ResetTargets()
{
	_targets.clear();
}

int Character::GetAttackPoint()
{
	return _attackPoint;
}

int Character::GetAttackRange()
{
	return _attackRange;
}

void Character::RecoveryHP(int hp)
{
	_hp += hp;

	if (_maxHp < _hp)
		_hp = _maxHp;
}

void Character::UpdateText()
{
	//int coolTime = (int)(_attackCooltimeDuration * 1000.0f);

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
	//wsprintf(text, L"BehaviorPoint: %d\n HP: %d\n State: %s\n Battle: %d\n", _behaviorPoint, _hp, state, _canBattle);
	wsprintf(text, L"BehaviorPoint: %d\n State: %s\n HP: %d\n", _behaviorPoint, state, _hp);
	_font->SetText(text);
}

TileCell* Character::GetTargetCell()
{
	return _targetTileCell;
}

void Character::SetTargetTileCell(TileCell* tileCell)
{
	_targetTileCell = tileCell;
}

std::stack<TileCell*> Character::GetPathTileCellStack()
{
	return _pathTileCellStack;
}

void Character::PushPathTileCell(TileCell* tileCell)
{
	_pathTileCellStack.push(tileCell);
}

void Character::ClearPathTileCellStack()
{
	while (false == _pathTileCellStack.empty())
		_pathTileCellStack.pop();
}

bool Character::IsClickCharacter(TileCell* tileCell)
{
	if ((tileCell->GetTileX() == _tileX) && (tileCell->GetTileY() == _tileY))
		return true;
	return false;
}