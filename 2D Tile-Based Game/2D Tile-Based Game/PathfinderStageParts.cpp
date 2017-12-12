#include <list>

#include "PathfinderStageParts.h"
#include "Stage.h"
#include "Map.h"
#include "Monster.h"
#include "LifeNPC.h"

#include "PathfinderPlayer.h"
#include "PathfinderMonster.h"

PathfinderStageParts::PathfinderStageParts(Stage* stage) :
	StageParts(stage)
{

}

PathfinderStageParts::~PathfinderStageParts()
{

}

void PathfinderStageParts::CreateComponents(std::wstring mapName)
{
	StageParts::CreateComponents(mapName);

	//1개의 몬스터 생성
	Component* monster = new PathfinderMonster(L"pathfindermonster", L"npc", L"zombie_npc");
	_stage->AddStageComponent(monster);

	//1개의 플레이어 생성( 추후 길찾기 전용 플레이어로 바꿀 예정)
	Player* player = new PathfinderPlayer(L"Player", L"Player", L"Player");
	_stage->AddStageComponent(player);

	TileCell* targetCell = _stage->GetMap()->GetTileCell(monster->GetTileX(), monster->GetTileY());
	player->SetTargetTileCell(targetCell);

	//뷰어를 맵 중앙으로 세팅(타일오브젝트로)
	int midTileX = _stage->GetMap()->GetWidth() / 2;
	int midTileY = _stage->GetMap()->GetHeight() / 2;

	std::list<Component*> comList = _stage->GetMap()->GetTileComponentList(midTileX, midTileY);
	_map->InitViewer(comList.front());
}