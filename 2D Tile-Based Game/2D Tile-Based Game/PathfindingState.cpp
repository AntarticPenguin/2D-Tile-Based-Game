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
		_pathfinder->FindPath(ePathMode::VIEW_RANGE, _character->GetMoveRange());	//ĳ���� �ִ� �̵� �Ÿ��� ���� Ÿ�� ������ �����ش�.
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

	//������ Ÿ�� ������ Ŭ���� ������ ���
	TileCell* targetCell = _character->GetTargetCell();
	if ((NULL != targetCell)
		&& !(targetCell->GetTileX() == _character->GetTileX() && targetCell->GetTileY() == _character->GetTileY())
		)
	{
		_pathfinder->Reset();
		_pathfinder->FindPath(ePathMode::FIND_PATH, eFindMethod::ASTAR);	//Ÿ���� Ŭ���Ǹ� ��θ� Ž�� : Astar
		_pathfinder->BuildPath();											//��θ� �����Ͽ� �����δ�.

		_character->ClearColorTile();
		UISystem::GetInstance().TurnOffBattleMenu();
	}
}