#include "DefaultStageLoader.h"

#include "UISystem.h"
#include "Map.h"
#include "Stage.h"

#include "Monster.h"
#include "Player.h"


DefaultStageLoader::DefaultStageLoader(Stage* stage) :
	StageLoader(stage)
{

}

DefaultStageLoader::~DefaultStageLoader()
{

}

void DefaultStageLoader::CreateComponents(std::wstring mapName)
{
	StageLoader::CreateComponents(L"Map01");

	/*
	//Recovery Item
	for (int i = 0; i < 20; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"recovery_Item_%d", i);
		RecoveryItem* item = new RecoveryItem(name, L"recovery_Item", L"recovery_Item");
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
	*/

	//Monster
	{
		Monster* monster = new Monster(L"monster", L"monster", L"monster");
		_stage->AddStageComponent(monster);
	}
	
	Player* player = new Player(L"Player", L"Player", L"Player");	//(컴포넌트이름, 스크립트 이름, 스프라이트이름)
	_stage->AddStageComponent(player);

	UISystem::GetInstance().SetActor(player);
	_map->InitViewer(player);
}