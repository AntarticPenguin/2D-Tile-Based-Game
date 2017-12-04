#include "StageParts.h"
#include "LifeNPC.h"
#include "Stage.h"
#include "LifePlayer.h"

StageParts::StageParts(Stage* stage)
{
	_stage = stage;
	_lifeNPCCount = 0;
}

StageParts::~StageParts()
{

}

Component* StageParts::CreateLifeNPC(std::wstring scriptName, std::wstring spriteName)
{
	SetName(L"life_npc");
	_lifeNPCCount++;
	Component* component = new LifeNPC(_name.c_str(), scriptName, spriteName);
	_stage->AddStageComponent(component);

	return component;
}

void StageParts::SetName(std::wstring name)
{
	WCHAR tempName[256];
	wsprintf(tempName, L"%s_%d", name, _lifeNPCCount);
	_name = tempName;
}

void StageParts::CreateComponents(int count)
{
	for (int i = 0; i < count; i++)
	{
		CreateLifeNPC(L"npc", L"Npc");
	}

	player = new LifePlayer(L"Player", L"Player", L"Player");	//(������Ʈ�̸�, ��ũ��Ʈ �̸�, ��������Ʈ�̸�)
	_stage->AddStageComponent(player);
}