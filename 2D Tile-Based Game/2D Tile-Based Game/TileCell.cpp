#include "TileCell.h"
#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"
#include "Component.h"
#include "TileObject.h"

#include "Sprite.h"

TileCell::TileCell(int tileX, int tileY)
{
	_componentList.clear();
	_tileX = tileX;
	_tileY = tileY;
	_distanceWeight = 0.0f;

	InitColorTile();
}

TileCell::~TileCell()
{

}

void TileCell::Deinit()
{
	
}

void TileCell::Update(float deltaTime)
{
	for (std::list<Component*>::iterator itr = _renderList.begin(); itr != _renderList.end(); itr++)
	{
		(*itr)->Update(deltaTime);
	}
}

void TileCell::Render()
{
	for (std::list<Component*>::iterator itr = _renderList.begin(); itr != _renderList.end(); itr++)
	{
		(*itr)->Render();
	}

	_colorSprite->SetPosition(_posX, _posY);
	_colorSprite->Render();
}

void TileCell::Release()
{
	
}

void TileCell::Reset()
{
	
}

void TileCell::SetPosition(float posX, float posY)
{
	_posX = posX;
	_posY = posY;

	for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
	{
		(*itr)->SetPosition(_posX, _posY);
	}
}

float TileCell::GetPositionX()
{
	return _posX;
}

float TileCell::GetPositionY()
{
	return _posY;
}

int TileCell::GetTileX()
{
	return _tileX;
}

int TileCell::GetTileY()
{
	return _tileY;
}

void TileCell::AddComponent(Component* component, bool isRender)
{
	_componentList.push_back(component);

	if (eComponentType::TILE_OBJECT == component->GetType())
		_distanceWeight = ((TileObject*)component)->GetDistanceWeight();

	if (isRender)
		_renderList.push_back(component);
}

void TileCell::RemoveComponent(Component* component)
{
	_componentList.remove(component);
	_renderList.remove(component);
}

void TileCell::MoveDeltaPosition(float deltaX, float deltaY)
{
	_posX += deltaX;
	_posY += deltaY;

	for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
		(*itr)->MoveDeltaPosition(deltaX, deltaY);
}

bool TileCell::CanMove()
{
	for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
	{
		if (false == (*itr)->CanMove())
			return false;
	}

	return true;
}

bool TileCell::GetCollisionList(std::list<Component*>& collisionList)
{
	collisionList.clear();

	for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
	{
		if (false == (*itr)->CanMove())
			collisionList.push_back((*itr));
	}

	if (0 == collisionList.size())
		return true;
	return false;
}

std::list<Component*>& TileCell::GetComponentList()
{
	return _componentList;
}

void TileCell::InitPathfinding()
{
	_isPathfindingMark = false;
	_prevPathfindingCell = NULL;
	_distanceFromStart = 0.0f;
	_heuristic = 0.0f;
}

bool TileCell::IsPathfindingMark()
{
	return _isPathfindingMark;
}

void TileCell::PathFinded()
{
	_isPathfindingMark = true;
}

TileCell* TileCell::GetPrevPathfindingCell()
{
	return _prevPathfindingCell;
}

void TileCell::SetPrevPathfindingCell(TileCell* tileCell)
{
	_prevPathfindingCell = tileCell;
}

float TileCell::GetDistanceFromStart()
{
	return _distanceFromStart;
}

void TileCell::SetDistanceFromStart(float distance)
{
	_distanceFromStart = distance;
}

float TileCell::GetDistanceWeight()
{
	return _distanceWeight;
}

float TileCell::GetHeuristic()
{
	return _heuristic;
}

void TileCell::SetHeuristic(float heuristic)
{
	_heuristic = heuristic;
}

void TileCell::InitColorTile()
{
	_colorSprite = new Sprite(L"ColorTile.png");
	_colorSprite->Init(0, 0, 32, 32, 1.0f);
	_colorSprite->SetColor(D3DCOLOR_ARGB(0, 0, 0, 0));
}

void TileCell::TurnOnColorTile(D3DCOLOR color)
{
	_baseColor = color;
	_colorSprite->SetColor(_baseColor);
}

void TileCell::TurnOffColorTile()
{
	_colorSprite->SetColor(D3DCOLOR_ARGB(0, 0, 0, 0));
}

void TileCell::SetMouseOver(bool mouseOver)
{
	if(true == mouseOver)
		_colorSprite->SetColor(D3DCOLOR_ARGB(100, 255, 0, 0));
	else
		_colorSprite->SetColor(_baseColor);
}