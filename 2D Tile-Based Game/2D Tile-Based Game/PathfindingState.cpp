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
	_pathfinder->FindPath(ePathMode::FIND_PATH, eFindMethod::ASTAR);	//Ÿ���� Ŭ���Ǹ� ��θ� Ž�� : Astar
	_pathfinder->BuildPath();											//��θ� �����Ͽ� �����δ�.
}

void PathfindingState::Stop()
{
	_character->SetTargetTileCell(NULL);
	_pathfinder->ClearColorTile();
	UISystem::GetInstance().TurnOffBattleMenu();
}