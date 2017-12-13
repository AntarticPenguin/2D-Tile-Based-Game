#include "Stage.h"
#include "ComponentSystem.h"

#include "Map.h"
#include "TileCell.h"
#include "LifeNPC.h"

#include "StageParts.h"
#include "LifeStageParts.h"
#include "DefaultStageParts.h"
#include "PathfinderStageParts.h"

Stage::Stage()
{
	_partsLoader = NULL;
	_partsMap.clear();
}

Stage::~Stage()
{
	ComponentSystem::GetInstance().RemoveAllComponents();
	
	for (std::map<std::wstring, StageParts*>::iterator itr = _partsMap.begin();
		itr != _partsMap.end(); itr++)
	{
		if (NULL != itr->second)
			delete itr->second;
	}
	_partsMap.clear();
}


void Stage::Init(std::wstring mapName)
{
	_partsMap[L"default"] = new DefaultStageParts(this);
	_partsMap[L"Map3"] = new LifeStageParts(this);
	_partsMap[L"Map4"] = new PathfinderStageParts(this);

	_componentList.clear();

	_partsLoader = GetStageParts(mapName);
	_partsLoader->CreateComponents(mapName);
}

void Stage::Update(float deltaTime)
{
	for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
	{
		(*itr)->Update(deltaTime);
	}

	UpdateBaseComponentList();
	UpdateRemoveComponentList();
}

void Stage::Render()
{
	for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
	{
		(*itr)->Render();
	}
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

void Stage::CreateLifeNPC(Component* component)
{
	//해당위치에 컴포넌트를 생성
	component->GetTileX();
	component->GetTileY();
	_createBaseComponentList.push_back(component);
}

void Stage::CreatePathfindNPC(TileCell* tileCell)
{
	LifeNPC* npc = (LifeNPC*)(_partsLoader->CreateLifeNPC(L"npc", L"Red_NPC"));
	npc->InitTilePosition(tileCell->GetTileX(), tileCell->GetTileY());

	_componentList.remove(npc);
	tileCell->AddComponent(npc, true);

	if (tileCell->GetTileX() < tileCell->GetPrevPathfindingCell()->GetTileX())
		npc->SetDirection(eDirection::RIGHT);
	else if(tileCell->GetTileX() > tileCell->GetPrevPathfindingCell()->GetTileX())
		npc->SetDirection(eDirection::LEFT);
	else if (tileCell->GetTileY() < tileCell->GetPrevPathfindingCell()->GetTileY())
		npc->SetDirection(eDirection::DOWN);
	else if (tileCell->GetTileY() > tileCell->GetPrevPathfindingCell()->GetTileY())
		npc->SetDirection(eDirection::UP);
}

void Stage::CreatePathfindMark(TileCell* tileCell)
{
	LifeNPC* npc = (LifeNPC*)(_partsLoader->CreateLifeNPC(L"npc", L"Zombie_NPC"));
	npc->InitTilePosition(tileCell->GetTileX(), tileCell->GetTileY());

	_componentList.remove(npc);
	tileCell->AddComponent(npc, true);

	if (NULL != tileCell->GetPrevPathfindingCell())
	{
		if (tileCell->GetTileX() < tileCell->GetPrevPathfindingCell()->GetTileX())
			npc->SetDirection(eDirection::LEFT);
		else if (tileCell->GetTileX() > tileCell->GetPrevPathfindingCell()->GetTileX())
			npc->SetDirection(eDirection::RIGHT);
		else if (tileCell->GetTileY() < tileCell->GetPrevPathfindingCell()->GetTileY())
			npc->SetDirection(eDirection::UP);
		else if (tileCell->GetTileY() > tileCell->GetPrevPathfindingCell()->GetTileY())
			npc->SetDirection(eDirection::DOWN);
	}
}

void Stage::DestroyLifeNPC(int tileX, int tileY, Component* tileCharacter)
{
	_map->ResetTileComponent(tileX, tileY, tileCharacter);

	_componentList.remove(tileCharacter);
	ComponentSystem::GetInstance().RemoveComponent(tileCharacter);
}

void Stage::CheckDestroyLifeNPC(Component* tileCharacter)
{
	_removeComponentList.push_back(tileCharacter);
}

void Stage::UpdateBaseComponentList()
{
	for (std::list<Component*>::iterator itr = _createBaseComponentList.begin();
		itr != _createBaseComponentList.end();	itr++)
	{
		Component* baseCom = (*itr);

		LifeNPC* npc = (LifeNPC*)(_partsLoader->CreateLifeNPC(L"npc", L"Npc"));
		//npc->Init();

		npc->InitTilePosition(baseCom->GetTileX(), baseCom->GetTileY());
	}

	_createBaseComponentList.clear();
}

void Stage::UpdateRemoveComponentList()
{
	for (std::list<Component*>::iterator itr = _removeComponentList.begin();
		itr != _removeComponentList.end();	itr++)
	{
		Component* component = (*itr);
		DestroyLifeNPC(component->GetTileX(), component->GetTileY(), component);
	}
	_removeComponentList.clear();
}

void Stage::AddStageComponent(Component* component)
{
	component->Init();
	_componentList.push_back(component);
}

StageParts* Stage::GetStageParts(std::wstring mapName)
{
	std::map<std::wstring, StageParts*>::iterator itr = _partsMap.find(mapName);
	if (itr != _partsMap.end())
		return _partsMap[mapName];
	else
		return _partsMap[L"default"];
}