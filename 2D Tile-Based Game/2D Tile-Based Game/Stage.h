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

	//lifegame
private:
	int _lifeNPCCount;
public:
	void CreateLifeNPC(int tileX, int tileY);
	void DestroyLifeNPC(int tileX, int tileY, Component* tileCharacter);
};