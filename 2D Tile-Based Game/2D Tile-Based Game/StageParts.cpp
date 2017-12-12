#include "StageParts.h"

#include "Stage.h"
#include "Map.h"
#include "LifeNPC.h"

StageParts::StageParts(Stage* stage)
{
	_stage = stage;
	_lifeNpcCount = 0;
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
	WCHAR name[256];
	wsprintf(name, L"lifeNpc_%d", _lifeNpcCount);
	_lifeNpcCount++;

	Component* component = new LifeNPC(name, scriptName, spriteName);
	_stage->AddStageComponent(component);

	return component;
}