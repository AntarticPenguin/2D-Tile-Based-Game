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
	Monster* monster = new PathfinderMonster(L"pathfindermonster", L"Monster", L"Monster");
	_stage->AddStageComponent(monster);
	//monster->InitTilePosition(1, 1);

	//1개의 플레이어 생성( 추후 길찾기 전용 플레이어로 바꿀 예정)
	Player* player = new PathfinderPlayer(L"Player", L"Player", L"Player");
	_stage->AddStageComponent(player);
	//player->InitTilePosition(23, 23);

	TileCell* targetCell = _stage->GetMap()->GetTileCell(monster->GetTileX(), monster->GetTileY());
	player->SetTargetTileCell(targetCell);

	//뷰어를 맵 중앙으로 세팅(타일오브젝트로)
	int midTileX = _stage->GetMap()->GetWidth() / 2;
	int midTileY = _stage->GetMap()->GetHeight() / 2;

	std::list<Component*> comList = _stage->GetMap()->GetTileComponentList(midTileX, midTileY);
	_map->InitViewer(comList.front());
}