#include "ComponentSystem.h"
#include "Map.h"
#include "Character.h"
#include "Sprite.h"
#include "ComponentMessage.h"

Character::Character(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName) :
	Component(name), _x(0.0f), _y(0.0f)
{
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
	UpdateAI(deltaTime);
	UpdateMove(deltaTime);
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
	_movingDuration = 0.0f;
	_curDirection = eDirection::DOWN;
}

void Character::UpdateAI(float deltaTime)
{
	if (false == _isLive)
		return;

	if (false == _isMoving)
	{
		int direction = rand() % 4;
		MoveStart((eDirection)direction);
	}
}

void Character::UpdateMove(float deltaTime)
{
	if (false == _isLive)
		return;

	if (false == _isMoving)
		return;

	if (_moveTime <= _movingDuration)
	{
		_movingDuration = 0.0f;
		_isMoving = false;

		//이동후 도착하면 타일의 정확한 위치에 찍어줘야 한다.
		Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");
		_x = map->GetPositionX(_tileX, _tileY);
		_y = map->GetPositionY(_tileX, _tileY);

		_moveDistancePerTimeX = 0.0f;
		_moveDistancePerTimeY = 0.0f;
	}
	else
	{
		_movingDuration += deltaTime;

		float moveDistanceX = _moveDistancePerTimeX * deltaTime;
		float moveDistanceY = _moveDistancePerTimeY * deltaTime;
		_x += moveDistanceX;
		_y += moveDistanceY;
	}
}

void Character::MoveStart(eDirection direction)
{
	if (true == _isMoving)
		return;

	_curDirection = direction;

	Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");

	//충돌체크 : 타일 위의 컴포넌트를 리셋하기 전에 충돌 여부 체크
	int newTileX = _tileX;
	int newTileY = _tileY;
	switch (direction)
	{
	case eDirection::LEFT:	//left
		newTileX--;
		break;
	case eDirection::RIGHT: //right
		newTileX++;
		break;
	case eDirection::UP: //up
		newTileY--;
		break;
	case eDirection::DOWN: //down
		newTileY++;
		break;
	}

	std::list<Component*> collisionList;
	bool canMove = map->GetTileCollisionList(newTileX, newTileY, collisionList);
	if (false == canMove)
	{
	/*
		충돌된 컴포넌트들끼리 메세지 교환
		각 하위 클래스에서 재정의 : 충돌시 메세지
	*/
		Collision(collisionList);
		return;
	}

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

	_isMoving = true;
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