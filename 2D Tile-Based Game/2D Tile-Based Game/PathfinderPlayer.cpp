#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"
#include "PathfinderPlayer.h"
#include "IdleState.h"
#include "PathfindingState.h"
#include "PathfindingMoveState.h"

PathfinderPlayer::PathfinderPlayer(std::wstring name, std::wstring scriptName, std::wstring spriteFileName) :
	Player(name, scriptName, spriteFileName)
{
}

PathfinderPlayer::~PathfinderPlayer()
{
}

void PathfinderPlayer::UpdateAI(float deltaTime)
{
	// 마우스 키처리
	if (GameSystem::GetInstance().IsMouseDown())
	{
		int mouseX = GameSystem::GetInstance().GetMouseX();
		int mouseY = GameSystem::GetInstance().GetMouseY();
		TileCell* targetTileCell = GameSystem::GetInstance().GetStage()->GetMap()
			->FindTileCellWithMousePosition(mouseX, mouseY);

		if (NULL != targetTileCell)
		{
			SetTargetTileCell(targetTileCell);
		}
	}
}

void PathfinderPlayer::InitState()
{
	Player::InitState();
	//ReplaceState(eStateType::ET_ATTACK, new IdleState());
	ReplaceState(eStateType::ET_MOVE, new PathfindingMoveState());
	ReplaceState(eStateType::ET_PATHFINDING, new PathfindingState());
}