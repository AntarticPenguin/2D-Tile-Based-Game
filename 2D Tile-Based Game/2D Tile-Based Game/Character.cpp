#include "ComponentSystem.h"
#include "Map.h"
#include "Character.h"
#include "Sprite.h"

Character::Character(LPCWSTR name) :
	Component(name), _x(0.0f), _y(0.0f), _tileX(5), _tileY(5)
{
	_spriteList.clear();
	_moveTime = 1.0f;
}

Character::~Character()
{

}

void Character::Init()
{
	WCHAR textureFileName[256];
	WCHAR scriptFileName[256];

	{
		wsprintf(textureFileName, L"%s.png", _name);
		wsprintf(scriptFileName, L"%s_left.json", _name);
		Sprite* sprite = new Sprite(textureFileName, scriptFileName);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(textureFileName, L"%s.png", _name);
		wsprintf(scriptFileName, L"%s_right.json", _name);
		Sprite* sprite = new Sprite(textureFileName, scriptFileName);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(textureFileName, L"%s.png", _name);
		wsprintf(scriptFileName, L"%s_up.json", _name);
		Sprite* sprite = new Sprite(textureFileName, scriptFileName);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(textureFileName, L"%s.png", _name);
		wsprintf(scriptFileName, L"%s_down.json", _name);
		Sprite* sprite = new Sprite(textureFileName, scriptFileName);
		sprite->Init();
		_spriteList.push_back(sprite);
	}

	{
		Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");
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

void Character::InitMove()
{
	_isMoving = false;
	_movingDuration = 0.0f;
	_curDirection = eDirection::DOWN;
}

void Character::UpdateAI(float deltaTime)
{

}

void Character::UpdateMove(float deltaTime)
{
	if (false == _isMoving)
		return;

	if (_moveTime <= _movingDuration)
	{
		_movingDuration = 0.0f;
		_isMoving = false;

		//�̵��� �����ϸ� Ÿ���� ��Ȯ�� ��ġ�� ������ �Ѵ�.
		Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");
		_x = map->GetPositionX(_tileX, _tileY);
		_y = map->GetPositionY(_tileX, _tileY);
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

	//�浹üũ : Ÿ�� ���� ������Ʈ�� �����ϱ� ���� �浹 ���� üũ
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

	if (false == map->CanMoveTileMap(newTileX, newTileY))
		return;

	map->ResetTileComponent(_tileX, _tileY, this);
	_tileX = newTileX;
	_tileY = newTileY;

	/*
	switch (direction)
	{
	case eDirection::LEFT:	//left
		_tileX--;
		if (_tileX < 0)
			_tileX = 0;
		break;
	case eDirection::RIGHT: //right
		_tileX++;
		if (49 < _tileX)
			_tileX = 49;
		break;
	case eDirection::UP: //up
		_tileY--;
		if (_tileY < 0)
			_tileY = 0;
		break;
	case eDirection::DOWN: //down
		_tileY++;
		if (49 < _tileY)
			_tileY = 49;
		break;
	}
	*/

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

	_isMoving = true;
}