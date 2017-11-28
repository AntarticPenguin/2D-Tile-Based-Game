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

Stage::Stage()
{

}

Stage::~Stage()
{
	ComponentSystem::GetInstance().RemoveAllComponents();
}


void Stage::Init(std::wstring mapName)
{
	_componentList.clear();

	_map = new Map(mapName.c_str());
	_componentList.push_back(_map);

	
	if (L"Map3" == mapName)
	{
		_lifeNPCCount = 0;
		for (int i = 0; i < 300; i++)
		{
			WCHAR name[256];
			wsprintf(name, L"life_npc_%d", _lifeNPCCount);
			_lifeNPCCount++;

			LifeNPC* npc = new LifeNPC(name, L"npc", L"Npc");
			_componentList.push_back(npc);
		}
	}
	else
	{
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
		for (int i = 0; i < 0; i++)
		{
			WCHAR name[256];
			wsprintf(name, L"monster_%d", i);
			Monster* monster = new Monster(name, L"monster", L"monster");
			_componentList.push_back(monster);
		}
	}

	Player* player = new Player(L"Player", L"Player", L"Player");	//(컴포넌트이름, 스크립트 이름, 스프라이트이름)
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

void Stage::CreateLifeNPC(int tileX, int tileY)
{
	WCHAR name[256];
	wsprintf(name, L"life_npc_%d", _lifeNPCCount);
	_lifeNPCCount++;

	LifeNPC* npc = new LifeNPC(name, L"npc", L"Npc");
	npc->Init(tileX, tileY);
	_componentList.push_back(npc);
}

void Stage::DestroyLifeNPC(int tileX, int tileY, Component* tileCharacter)
{
	_map->ResetTileComponent(tileX, tileY, tileCharacter);
	tileCharacter->SetCanMove(true);
	tileCharacter->SetLive(false);
}