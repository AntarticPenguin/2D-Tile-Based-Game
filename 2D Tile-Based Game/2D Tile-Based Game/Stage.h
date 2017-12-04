#pragma once
#include <string>
#include <list>

class Component;
class Map;
class StageParts;

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
	//int _lifeNPCCount;
	std::list<Component*> _removeComponentList;
	std::list<Component*> _createBaseComponentList;

public:
	void CreateLifeNPC(Component* component);
	void DestroyLifeNPC(int tileX, int tileY, Component* tileCharacter);
	void CheckDestroyLifeNPC(Component* tileCharacter);
	void UpdateBaseComponentList();
	void UpdateRemoveComponentList();

	//ºÎÇ°
private:
	StageParts* _bp;

public:
	void AddStageComponent(Component* component);
};