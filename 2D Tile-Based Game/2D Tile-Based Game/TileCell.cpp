#include "TileCell.h"
#include "Sprite.h"
#include "Component.h"

TileCell::TileCell()
{
	_componentList.clear();
}

TileCell::~TileCell()
{

}

void TileCell::Deinit()
{
	
}

void TileCell::Update(float deltaTime)
{
	
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