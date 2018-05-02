#include "PathfindingState.h"

#include "GameSystem.h"
#include "UISystem.h"

#include "Character.h"
#include "Pathfinding.h"
#include "Stage.h"
#include "TileCell.h"

PathfindingState::PathfindingState()
{
	
}

PathfindingState::~PathfindingState()
{

}

void PathfindingState::Init(Character* character)
{
	State::Init(character);
	_pathfinder = new Pathfinding();
	_pathfinder->Init();
	_pathfinder->Init(character);
}

void PathfindingState::Start()
{
	State::Start();

	_pathfinder->Reset();
	_pathfinder->FindPath(ePathMode::FIND_PATH, eFindMethod::ASTAR);	//타일이 클릭되면 경로를 탐색 : Astar
	_pathfinder->BuildPath();											//경로를 도출하여 움직인다.
}

void PathfindingState::Stop()
{
	_character->SetTargetTileCell(NULL);
	_pathfinder->ClearColorTile();
	UISystem::GetInstance().TurnOffBattleMenu();
}