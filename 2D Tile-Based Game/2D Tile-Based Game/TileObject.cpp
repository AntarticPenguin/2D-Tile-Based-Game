#include "TileObject.h"
#include "Sprite.h"

TileObject::TileObject(std::wstring name, Sprite* sprite)
	: Component(name), _posX(0.0f), _posY(0.0f)
{
	_sprite = sprite;
	_eType = eComponentType::CT_TILE_OBJECT;
}

TileObject::~TileObject()
{

}

void TileObject::Init()
{
	_sprite->Init();
}

void TileObject::Deinit()
{
	_sprite->Deinit();
}

void TileObject::Update(float deltaTime)
{
	_sprite->Update(deltaTime);
}

void TileObject::Render()
{
	_sprite->SetPosition(_posX, _posY);
	_sprite->Render();
}

void TileObject::Release()
{
	_sprite->Release();
}

void TileObject::Reset()
{
	_sprite->Reset();
}

void TileObject::SetPosition(float posX, float posY)
{
	_posX = posX;
	_posY = posY;
}

void TileObject::MoveDeltaPosition(float deltaX, float deltaY)
{
	_posX += deltaX;
	_posY += deltaY;
}