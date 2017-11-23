#include "ComponentSystem.h"
#include "RecoveryItem.h"
#include "Character.h"
#include "Map.h"
#include "Sprite.h"

RecoveryItem::RecoveryItem(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName)
	: Component(name), _posX(0.0f), _posY(0.0f)
{
	_eType = eComponentType::CT_ITEM;
	_sprite = NULL;
	_scriptFileName = scriptName;
	_spriteFileName = spriteFileName;
	_canMove = true;
}

RecoveryItem::~RecoveryItem()
{

}

void RecoveryItem::Init()
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

		_posX = map->GetPositionX(_tileX, _tileY);
		_posY = map->GetPositionY(_tileX, _tileY);
		map->SetTileComponent(_tileX, _tileY, this, false);
	}

	WCHAR textureFileName[256];
	wsprintf(textureFileName, L"%s.png", _spriteFileName.c_str());
	WCHAR scriptFileName[256];

	{
		wsprintf(scriptFileName, L"%s.json", _scriptFileName.c_str());
		_sprite = new Sprite(textureFileName, scriptFileName);
		_sprite->Init();
	}
}

void RecoveryItem::Deinit()
{
	_sprite->Deinit();
}

void RecoveryItem::Update(float deltaTime)
{
	if (false == _isLive)
		return;
	_sprite->Update(deltaTime);
}

void RecoveryItem::Render()
{
	if (false == _isLive)
		return;

	_sprite->SetPosition(_posX, _posY);
	_sprite->Render();
}

void RecoveryItem::Release()
{
	_sprite->Release();
}

void RecoveryItem::Reset()
{
	_sprite->Reset();
}

void RecoveryItem::SetPosition(float posX, float posY)
{
	_posX = posX;
	_posY = posY;
}

void RecoveryItem::MoveDeltaPosition(float deltaX, float deltaY)
{
	_posX += deltaX;
	_posY += deltaY;
}

void RecoveryItem::ReceiveMessage(const sComponentMsgParam& msgParam)
{
	if (L"Use" == msgParam.message)
	{
		Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");

		Component* sender = msgParam.sender;
		switch (sender->GetType())
		{
		case eComponentType::CT_NPC:
		case eComponentType::CT_MONSTER:
		case eComponentType::CT_PLAYER:
			((Character*)sender)->RecoveryHP(100);
			map->ResetTileComponent(_tileX, _tileY, this);
			_isLive = false;
			break;
		}
	}
}