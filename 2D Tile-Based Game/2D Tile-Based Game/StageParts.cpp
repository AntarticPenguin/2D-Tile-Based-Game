#include "StageParts.h"

#include "Stage.h"
#include "Map.h"

StageParts::StageParts(Stage* stage)
{
	_stage = stage;
}

StageParts::~StageParts()
{

}

void StageParts::CreateComponents(std::wstring mapName)
{
	_map = new Map(mapName.c_str());
	_stage->SetMap(_map);
	_stage->AddStageComponent(_map);
}

Component* StageParts::CreateLifeNPC(std::wstring scriptName, std::wstring spriteName)
{
//	SetName(L"life_npc");
//	_lifeNPCCount++;
//	Component* component = new LifeNPC(_name.c_str(), scriptName, spriteName);
//	_stage->AddStageComponent(component);
//
//	return component;
	return NULL;
}