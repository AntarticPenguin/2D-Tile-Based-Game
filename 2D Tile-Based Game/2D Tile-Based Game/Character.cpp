#include "ComponentSystem.h"
#include "Map.h"
#include "Character.h"
#include "Sprite.h"

Character::Character(LPCWSTR name) : Component(name)
{
	_sprite = NULL;
}

Character::~Character()
{

}

void Character::Init()
{
	WCHAR textureFileName[256];
	WCHAR scriptFileName[256];
	wsprintf(textureFileName, L"%s.png", _name);
	wsprintf(scriptFileName, L"%s.json", _name);

	_sprite = new Sprite(textureFileName, scriptFileName);
	_sprite->Init();

	{
		Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"Map");
		int tileX = 2;
		int tileY = 2;
		_x = map->GetPositionX(tileX, tileY);
		_y = map->GetPositionY(tileX, tileY);
		map->SetTileComponent(tileX, tileY, this);
	}
}

void Character::Deinit()
{
	if (NULL != _sprite)
	{
		_sprite->Deinit();
		delete _sprite;
		_sprite = NULL;
	}
}

void Character::Update(float deltaTime)
{
	_sprite->Update(deltaTime);
}

void Character::Render()
{
	_sprite->SetPosition(_x, _y);
	_sprite->Render();
}

void Character::Release()
{
	_sprite->Release();
}

void Character::Reset()
{
	_sprite->Reset();
}
