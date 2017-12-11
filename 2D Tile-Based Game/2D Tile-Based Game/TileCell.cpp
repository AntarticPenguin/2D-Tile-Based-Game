#include "TileCell.h"
#include "Sprite.h"
#include "Component.h"

TileCell::TileCell(int tileX, int tileY)
{
	_componentList.clear();
	_tileX = tileX;
	_tileY = tileY;
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

	if (isRender)
	{
		_renderList.push_back(component);
	}
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
	{
		(*itr)->MoveDeltaPosition(deltaX, deltaY);
	}
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
		{
			collisionList.push_back((*itr));
		}
	}

	if (0 == collisionList.size())
		return true;
	return false;

}

std::list<Component*> TileCell::GetComponentList()
{
	return _componentList;
}

void TileCell::InitPathfinding()
{
	_isPathfindingMark = false;
	_prevPathfindingCell = NULL;
}

bool TileCell::IsPathfindingMark()
{
	return _isPathfindingMark;
}

void TileCell::PathFinded()
{
	_isPathfindingMark = true;
}

TileCell* TileCell::GetPrevPathfindingNode()
{
	return _prevPathfindingCell;
}

void TileCell::SetPrevPathfindingNode(TileCell* tileCell)
{
	_prevPathfindingCell = tileCell;
}

