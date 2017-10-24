#pragma once
#include <list>

class Component;
class Sprite;

class TileCell
{
private:
	Sprite* _sprite;
	
	float _posX;
	float _posY;

	std::list<Component*> _componentList;

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
	void SetSprite(Sprite* sprite);

	float GetPositionX();
	float GetPositionY();

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);
};