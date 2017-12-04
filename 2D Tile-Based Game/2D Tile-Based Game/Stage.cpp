#include "Stage.h"
#include "ComponentSystem.h"

#include "Map.h"
#include "Player.h"
#include "NPC.h"
#include "Monster.h"
#include "RecoveryItem.h"
#include "PoisonItem.h"
#include "Sword.h"

#include "LifeNPC.h"
#include "LifePlayer.h"

#include "StageParts.h"

Stage::Stage()
{
	_bp = NULL;
}

Stage::~Stage()
{
	ComponentSystem::GetInstance().RemoveAllComponents();
	if (NULL != _bp)
	{
		delete _bp;
		_bp = NULL;
	}
}


void Stage::Init(std::wstring mapName)
{
	_componentList.clear();

	_map = new Map(mapName.c_str());
	_componentList.push_back(_map);

	Player* player = NULL;	

	//1단계 리팩토링
	/*
	if (L"Map3" == mapName)
	{
		_bp = new LifeStageParts(this);
		_bp->CreateComponents();
	}
	else
	{
		_bp = new DefaultStageParts(this);
		_bp->CreateComponents();

		//Recovery Item
		for (int i = 0; i < 20; i++)
		{
			WCHAR name[256];
			wsprintf(name, L"recovery_Item_%d", i);
			RecoveryItem* item = new RecoveryItem(name, L"recovery_Item", L"recovery_Item");
			_componentList.push_back(item);
		}

		//Poison Item
		for (int i = 0; i < 20; i++)
		{
			WCHAR name[256];
			wsprintf(name, L"poison_Item_%d", i);
			PoisonItem* item = new PoisonItem(name, L"recovery_Item", L"recovery_Item");
			_componentList.push_back(item);
		}

		//Sword Item
		for (int i = 0; i < 10; i++)
		{
			WCHAR name[256];
			wsprintf(name, L"sword_Item_%d", i);
			Sword* item = new Sword(name, L"sword_Item", L"sword_Item");
			_componentList.push_back(item);
		}

		//NPC
		for (int i = 0; i < 0; i++)
		{
			WCHAR name[256];
			wsprintf(name, L"npc_%d", i);
			NPC* npc = new NPC(name, L"npc", L"Npc");
			_componentList.push_back(npc);
		}

		//Monster
		for (int i = 0; i < 1; i++)
		{
			WCHAR name[256];
			wsprintf(name, L"monster_%d", i);
			Monster* monster = new Monster(name, L"monster", L"monster");
			_componentList.push_back(monster);
		}
		player = new Player(L"Player", L"Player", L"Player");
	}
	*/

	//2단계 리팩토링
	/*
	if (L"Map3" == mapName)
	{
		_bp = new LifeStageParts(this);
	}
	else
	{
		_bp = new DefaultStageParts(this);
	}
	*/

	//3단계 리팩토링
	if (find(mapName) == true)
		_bpMap[mapName]->CreateComponents();
	else
		_bpMap["default"]->CreateComponents();

	_bp->CreateComponents();
	_componentList.push_back(player);

	for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
	{
		(*itr)->Init();
	}

	_map->InitViewer(player);
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

void Stage::CreateLifeNPC(Component* component)
{
	//해당위치에 컴포넌트를 생성
	component->GetTileX();
	component->GetTileY();
	_createBaseComponentList.push_back(component);
}

void Stage::DestroyLifeNPC(int tileX, int tileY, Component* tileCharacter)
{
	_map->ResetTileComponent(tileX, tileY, tileCharacter);
	/*tileCharacter->SetCanMove(true);
	tileCharacter->SetLive(false);*/

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

		LifeNPC* npc = (LifeNPC*)(_bp->CreateLifeNPC(L"npc", L"Npc"));

		npc->Init(baseCom->GetTileX(), baseCom->GetTileY());
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
	_componentList.push_back(component);
}