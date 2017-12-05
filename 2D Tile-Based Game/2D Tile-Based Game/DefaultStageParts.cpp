#include "DefaultStageParts.h"

#include "Stage.h"
#include "Map.h"

#include "Player.h"
#include "RecoveryItem.h"
#include "PoisonItem.h"
#include "Sword.h"
#include "NPC.h"
#include "Monster.h"


DefaultStageParts::DefaultStageParts(Stage* stage) :
	StageParts(stage)
{

}

DefaultStageParts::~DefaultStageParts()
{

}

void DefaultStageParts::CreateComponents(std::wstring mapName)
{
	StageParts::CreateComponents(mapName);

	//Recovery Item
	for (int i = 0; i < 20; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"recovery_Item_%d", i);
		RecoveryItem* item = new RecoveryItem(name, L"recovery_Item", L"recovery_Item");
		_stage->AddStageComponent(item);
	}

	//Poison Item
	for (int i = 0; i < 20; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"poison_Item_%d", i);
		PoisonItem* item = new PoisonItem(name, L"recovery_Item", L"recovery_Item");
		_stage->AddStageComponent(item);
	}

	//Sword Item
	for (int i = 0; i < 10; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"sword_Item_%d", i);
		Sword* item = new Sword(name, L"sword_Item", L"sword_Item");
		_stage->AddStageComponent(item);
	}

	//NPC
	for (int i = 0; i < 20; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"npc_%d", i);
		NPC* npc = new NPC(name, L"npc", L"Npc");
		_stage->AddStageComponent(npc);
	}

	//Monster
	for (int i = 0; i < 20; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"monster_%d", i);
		Monster* monster = new Monster(name, L"monster", L"monster");
		_stage->AddStageComponent(monster);
	}
	
	Player* player = new Player(L"Player", L"Player", L"Player");	//(컴포넌트이름, 스크립트 이름, 스프라이트이름)
	_stage->AddStageComponent(player);
	
	_map->InitViewer(player);
}