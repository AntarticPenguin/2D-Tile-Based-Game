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
	Monster* monster = new PathfinderMonster(L"pathfindermonster", L"Monster", L"Monster");
	_stage->AddStageComponent(monster);
	//monster->InitTilePosition(1, 1);

	//1���� �÷��̾� ����( ���� ��ã�� ���� �÷��̾�� �ٲ� ����)
	Player* player = new PathfinderPlayer(L"Player", L"Player", L"Player");
	_stage->AddStageComponent(player);
	//player->InitTilePosition(23, 23);

	//���콺 ������� ���� �ּ�
	/*
	TileCell* targetCell = _stage->GetMap()->GetTileCell(monster->GetTileX(), monster->GetTileY());
	player->SetTargetTileCell(targetCell);
	*/

	//�� �� �߾����� ����(Ÿ�Ͽ�����Ʈ��)
	int midTileX = _stage->GetMap()->GetWidth() / 2;
	int midTileY = _stage->GetMap()->GetHeight() / 2;

	std::list<Component*> comList = _stage->GetMap()->GetTileComponentList(midTileX, midTileY);
	_map->InitViewer(comList.front());
}