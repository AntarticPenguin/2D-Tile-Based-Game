#include "PathfindingState.h"

#include "GameSystem.h"
#include "Stage.h"
#include "Character.h"
#include "Map.h"
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
}

void PathfindingState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	//길찾기 알고리즘 시작
	if (0 != _pathfindingTileQueue.size())
	{
		TileCell* node = _pathfindingTileQueue.front();
		_pathfindingTileQueue.pop();

		if (false == node->IsPathfindingMark())
		{
			node->PathFinded();

			//Is Target?
			if (node->GetTileX() == _targetTileCell->GetTileX() &&
				node->GetTileY() == _targetTileCell->GetTileY())
			{
				_nextState = eStateType::ET_IDLE;
				return;
			}
		}
	}
}

void PathfindingState::Start()
{
	State::Start();
	_curState = eStateType::ET_PATHFINDING;

	_targetTileCell = _character->GetTargetCell();

	//모든 타일셀 길찾기 속성 초기화
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();
	int height = map->GetHeight();
	int width = map->GetWidth();

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			TileCell* tileCell = map->GetTileCell(x, y);
			tileCell->InitPathfinding();
		}
	}

	TileCell* startTileCell = map->GetTileCell(_character->GetTileX(), _character->GetTileY());
	_pathfindingTileQueue.push(startTileCell);
}

void PathfindingState::Stop()
{
	State::Stop();
}