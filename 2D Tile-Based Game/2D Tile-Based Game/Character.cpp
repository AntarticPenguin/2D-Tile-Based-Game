#include "ComponentSystem.h"
#include "ComponentMessage.h"

#include "Map.h"
#include "Sprite.h"

#include "MoveState.h"
#include "IdleState.h"
#include "Character.h"

Character::Character(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName) :
	Component(name), _x(0.0f), _y(0.0f)
{
	//_state = new MoveState();
	_state = new IdleState();

	_spriteFileName = spriteFileName;
	_scriptFileName = scriptName;

	_spriteList.clear();
	_moveTime = 1.0f;

	_tileX = 10;
	_tileY = 10;

	_attackPoint = 10;
	_hp = 20;
}

Character::~Character()
{
	delete _state;
}

void Character::Init()
{
	WCHAR textureFileName[256];
	wsprintf(textureFileName, L"%s.png", _spriteFileName.c_str());
	WCHAR scriptFileName[256];

	{
		wsprintf(scriptFileName, L"%s_left.json", _scriptFileName.c_str());
		Sprite* sprite = new Sprite(textureFileName, scriptFileName);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(scriptFileName, L"%s_right.json", _scriptFileName.c_str());
		Sprite* sprite = new Sprite(textureFileName, scriptFileName);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(scriptFileName, L"%s_up.json", _scriptFileName.c_str());
		Sprite* sprite = new Sprite(textureFileName, scriptFileName);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(scriptFileName, L"%s_down.json", _scriptFileName.c_str());
		Sprite* sprite = new Sprite(textureFileName, scriptFileName);
		sprite->Init();
		_spriteList.push_back(sprite);
	}

	{
		Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");

		bool canMove = false;
		while (true != canMove)
		{
			_tileX = rand() % (map->GetWidth() - 2) + 1;
			_tileY = rand() % (map->GetHeight() - 2) + 1;

			canMove = map->CanMoveTileMap(_tileX, _tileY);
		}

		_x = map->GetPositionX(_tileX, _tileY);
		_y = map->GetPositionY(_tileX, _tileY);
		map->SetTileComponent(_tileX, _tileY, this, false);
	}

	_state->Init(this);
	InitMove();
}

void Character::Deinit()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Deinit();
		delete _spriteList[i];
	}
	_spriteList.clear();
}

void Character::Update(float deltaTime)
{
	_spriteList[(int)_curDirection]->Update(deltaTime);
	_state->Update(deltaTime);
}

void Character::Render()
{
	_spriteList[(int)_curDirection]->SetPosition(_x, _y);
	_spriteList[(int)_curDirection]->Render();
}

void Character::Release()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Release();
	}
}

void Character::Reset()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Reset();
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

void Character::UpdateAI(float deltaTime)
{
	_curDirection = (eDirection)(rand() % 4);
	ChangeState(eStateType::ET_MOVE);
}

void Character::ChangeState(eStateType stateType)
{
	/*
	_state->Stop();
	delete _state;

	_state = new MoveState();
	_state->Init(this);
	_state->Start();
	*/
	
	/*_state가 NULL 이아니면 stop 후 제거
	
	switch문으로 타입을 검사해서 new

	다시 switch를 init 후 start 하기*/

	if(NULL != _state)
		delete _state;
	
	switch (stateType)
	{
	case eStateType::ET_IDLE:
		_state = new IdleState();
		break;
	case eStateType::ET_MOVE:
		_state = new MoveState();
		break;
	}

	_state->Init(this);
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

void Character::Collision(std::list<Component*>& collisionList)
{
	for (std::list<Component*>::iterator itr = collisionList.begin(); itr != collisionList.end(); itr++)
	{
		sComponentMsgParam msgParam;
		msgParam.sender = this;
		msgParam.receiver = (*itr);
		msgParam.message = L"Collision";
		ComponentSystem::GetInstance().SendMessageToComponent(msgParam);
	}
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
	}
}