#include "Stage.h"
#include "ComponentSystem.h"
#include "UISystem.h"

#include "Map.h"
#include "TileCell.h"

#include "StageLoader.h"
#include "DefaultStageLoader.h"

Stage::Stage()
{
	_stageLoader = NULL;
	_stageMap.clear();
}

Stage::~Stage()
{
	ComponentSystem::GetInstance().RemoveAllComponents();
	
	for (std::map<std::wstring, StageLoader*>::iterator itr = _stageMap.begin();
		itr != _stageMap.end(); itr++)
	{
		if (NULL != itr->second)
			delete itr->second;
	}
	_stageMap.clear();
}


void Stage::Init(std::wstring mapName, eStage stageInfo)
{
	_stageMap[L"Map01"] = new DefaultStageLoader(this);

	_componentList.clear();

	_stageLoader = GetStageLoader(mapName);
	_stageLoader->CreateComponents(mapName);

	_stageInfo = stageInfo;
}

void Stage::Update(float deltaTime)
{
	for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
	{
		(*itr)->Update(deltaTime);
	}
}

void Stage::Render()
{
	for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
	{
		(*itr)->Render();
	}

	UISystem::GetInstance().Render();
}

void Stage::Release()
{
	for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
	{
		(*itr)->Release();
	}
}

void Stage::Reset()
{
	for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
	{
		(*itr)->Reset();
	}
}

Map* Stage::GetMap()
{
	return _map;
}

void Stage::SetMap(Map* map)
{
	_map = map;
}

eStage Stage::GetStageInfo()
{
	return _stageInfo;
}

void Stage::AddStageComponent(Component* component)
{
	component->Init();
	_componentList.push_back(component);
}

StageLoader* Stage::GetStageLoader(std::wstring mapName)
{
	std::map<std::wstring, StageLoader*>::iterator itr = _stageMap.find(mapName);
	if (itr != _stageMap.end())
		return _stageMap[mapName];
	else
		return _stageMap[L"Map01"];
}