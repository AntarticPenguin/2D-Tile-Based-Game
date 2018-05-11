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

	//Monster
	for (size_t i = 0; i < 30; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"monster_%d", i);
		Monster* monster = new Monster(name, L"monster", L"monster");
		_stage->AddStageComponent(monster);
	}
	
	Player* player = new Player(L"Player", L"Player", L"Player");	//(컴포넌트이름, 스크립트 이름, 스프라이트이름)
	_stage->AddStageComponent(player);

	UISystem::GetInstance().SetActor(player);
	_map->InitViewer(player);
}