#include "LifeStageParts.h"

#include "LifeNPC.h"
#include "Stage.h"
#include "Player.h"
#include "LifePlayer.h"
#include "Map.h"

LifeStageParts::LifeStageParts(Stage* stage) :
	StageParts(stage)
{
	
}

LifeStageParts::~LifeStageParts()
{

}

void LifeStageParts::CreateComponents(std::wstring mapName)
{
	StageParts::CreateComponents(mapName);

	for (int i = 0; i < 150; i++)
	{
		CreateLifeNPC(L"npc", L"Npc");
	}

	Player* player = new LifePlayer(L"Player", L"Player", L"Player");	//(������Ʈ�̸�, ��ũ��Ʈ �̸�, ��������Ʈ�̸�)
	_stage->AddStageComponent(player);
	
	_map->InitViewer(player);
}