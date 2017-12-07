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
	return NULL;
}