#pragma once
#include <string>
#include <list>
#include <map>

class Component;
class Map;
class StageLoader;
class TileCell;

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
	void SetMap(Map* map);

	//Stage Loader
private:
	std::map<std::wstring, StageLoader*> _stageMap;
	StageLoader* _stageLoader;

public:
	void AddStageComponent(Component* component);
	StageLoader* GetStageLoader(std::wstring mapName);
};