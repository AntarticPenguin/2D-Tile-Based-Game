#include "ComponentSystem.h"
#include "Map.h"
#include "Character.h"
#include "Sprite.h"

Character::Character(LPCWSTR name) :
	Component(name), _x(0.0f), _y(0.0f), _tileX(5), _tileY(5)
{
	_spriteList.clear();
	InitMove();
}

Character::~Character()
{

}

void Character::Init()
{
	WCHAR textureFileName[256];
	WCHAR scriptFileName[256];

	/*
	wsprintf(textureFileName, L"%s.png", _name);
	wsprintf(scriptFileName, L"%s.json", _name);

	_sprite = new Sprite(textureFileName, scriptFileName);
	_sprite->Init();
	*/
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
		map->SetTileComponent(_tileX, _tileY, this);
	}
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

void Character::InitMove()
{
	_isMoving = false;
	_moveTime = 0.5f;
	_movingDuration = 0.0f;
	_curDirection = eDirection::DOWN;
}

void Character::UpdateAI(float deltaTime)
{
	if (false == _isMoving)
	{
		int direction = rand() % 4;
		MoveStart((eDirection)direction);
	}
}

void Character::UpdateMove(float deltaTime)
{
	if (false == _isMoving)
		return;

	if (_moveTime <= _movingDuration)
	{
		_movingDuration = 0.0f;
		_isMoving = false;
	}
	else
	{
		_movingDuration += deltaTime;
	}
}

void Character::MoveStart(eDirection direction)
{
	if (true == _isMoving)
		return;

	_curDirection = direction;

	Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");
	map->ResetTileComponent(_tileX, _tileY, this);

	switch (direction)
	{
	case eDirection::LEFT:	//left
		_tileX--;
		if (_tileX < 0)
			_tileX = 0;
		break;
	case eDirection::RIGHT: //right
		_tileX++;
		if (45 < _tileX)
			_tileX = 45;
		break;
	case eDirection::UP: //up
		_tileY--;
		if (_tileY < 0)
			_tileY = 0;
		break;
	case eDirection::DOWN: //down
		_tileY++;
		if (45 < _tileY)
			_tileY = 45;
		break;
	}

	_x = map->GetPositionX(_tileX, _tileY);
	_y = map->GetPositionY(_tileX, _tileY);

	map->SetTileComponent(_tileX, _tileY, this);

	_isMoving = true;
}