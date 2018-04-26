#include "PathfindingState.h"
#include "Pathfinding.h"
#include "GameSystem.h"
#include "UISystem.h"
#include "Stage.h"
#include "Character.h"
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
	_pathfinder = new Pathfinding(character);
	_pathfinder->Init();
}

void PathfindingState::Start()
{
	State::Start();

	_pathfinder->Reset();

	if (eStage::DUNGEON == GameSystem::GetInstance().GetStage()->GetStageInfo())
		_pathfinder->FindPath(ePathMode::VIEW_RANGE);	//캐릭터 최대 이동 거리에 따른 타일 범위를 보여준다.
}

void PathfindingState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStage::DUNGEON == GameSystem::GetInstance().GetStage()->GetStageInfo()
		&& GameSystem::GetInstance().IsRightMouseDown())
	{
		_character->ClearColorTile();
		_character->ChangeState(eStateType::ET_IDLE);
	}

	//보여준 타일 내에서 클릭할 때까지 대기
	TileCell* targetCell = _character->GetTargetCell();
	if ((NULL != targetCell)
		&& !(targetCell->GetTileX() == _character->GetTileX() && targetCell->GetTileY() == _character->GetTileY())
		)
	{
		_pathfinder->Reset();
		_pathfinder->FindPath(ePathMode::FIND_PATH, eFindMethod::ASTAR);	//타일이 클릭되면 경로를 탐색 : Astar
		_pathfinder->BuildPath();											//경로를 도출하여 움직인다.

		_character->ClearColorTile();
		UISystem::GetInstance().TurnOffBattleMenu();
	}
}