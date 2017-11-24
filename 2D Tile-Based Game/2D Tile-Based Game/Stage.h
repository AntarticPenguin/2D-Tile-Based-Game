#pragma once
#include <string>
#include <list>

class Component;
class Map;

class Stage
{
private:
	std::list<Component*> _componentList;
	Map* _map;

public:
	Stage();
	~Stage();

public:
	void Init(std::wstring mapName);
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

public:
	Map* GetMap();
};