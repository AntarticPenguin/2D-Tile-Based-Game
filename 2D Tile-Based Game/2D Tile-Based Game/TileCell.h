#pragma once
#include <list>

class Component;
class Sprite;

class TileCell
{
private:
	float _posX;
	float _posY;

	std::list<Component*> _componentList;
	std::list<Component*> _renderList;

public:
	TileCell();
	~TileCell();

public:
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void SetPosition(float posX, float posY);

	float GetPositionX();
	float GetPositionY();

	void AddComponent(Component* component, bool isRender);
	void RemoveComponent(Component* component);
	void MoveDeltaPosition(float deltaX, float deltaY);

	bool CanMove();
	bool GetCollisionList(std::list<Component*>& collisionList);
};