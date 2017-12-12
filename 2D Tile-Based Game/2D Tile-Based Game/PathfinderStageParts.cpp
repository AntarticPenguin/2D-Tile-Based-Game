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

	//1���� ���� ����
	Component* monster = new PathfinderMonster(L"pathfindermonster", L"npc", L"zombie_npc");
	_stage->AddStageComponent(monster);

	//1���� �÷��̾� ����( ���� ��ã�� ���� �÷��̾�� �ٲ� ����)
	Player* player = new PathfinderPlayer(L"Player", L"Player", L"Player");
	_stage->AddStageComponent(player);

	TileCell* targetCell = _stage->GetMap()->GetTileCell(monster->GetTileX(), monster->GetTileY());
	player->SetTargetTileCell(targetCell);

	//�� �� �߾����� ����(Ÿ�Ͽ�����Ʈ��)
	int midTileX = _stage->GetMap()->GetWidth() / 2;
	int midTileY = _stage->GetMap()->GetHeight() / 2;

	std::list<Component*> comList = _stage->GetMap()->GetTileComponentList(midTileX, midTileY);
	_map->InitViewer(comList.front());
}