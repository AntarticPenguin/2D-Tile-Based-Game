#include "StageLoader.h"

#include "Stage.h"
#include "Map.h"

StageLoader::StageLoader(Stage* stage)
{
	_stage = stage;
	_lifeNpcCount = 0;
}

StageLoader::~StageLoader()
{

}

void StageLoader::CreateComponents(std::wstring mapName)
{
	_map = new Map(mapName.c_str());
	_stage->SetMap(_map);
	_stage->AddStageComponent(_map);
}