#pragma once
#include <string>
#include <list>
#include <map>

class Component;
class Map;
class StageLoader;
class TileCell;

enum class eStage
{
	TOWN,
	DUNGEON,
};

class Stage
{
private:
	std::list<Component*> _componentList;
	Map* _map;
	eStage _stageInfo;

public:
	Stage();
	~Stage();

public:
	void Init(std::wstring mapName, eStage stageInfo);
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

public:
	Map* GetMap();
	void SetMap(Map* map);

	eStage GetStageInfo();

	//Stage Loader
private:
	std::map<std::wstring, StageLoader*> _stageMap;
	StageLoader* _stageLoader;

public:
	void AddStageComponent(Component* component);
	StageLoader* GetStageLoader(std::wstring mapName);
};