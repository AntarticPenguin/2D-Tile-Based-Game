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

	player = new LifePlayer(L"Player", L"Player", L"Player");	//(컴포넌트이름, 스크립트 이름, 스프라이트이름)
	_stage->AddStageComponent(player);
}