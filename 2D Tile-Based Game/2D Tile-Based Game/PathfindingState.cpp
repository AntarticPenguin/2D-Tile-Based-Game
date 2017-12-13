#include "PathfindingState.h"

#include "GameSystem.h"
#include "GlobalTile.h"
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

	switch (_updateState)
	{
	case eUpdateState::PATHFINDING:
		UpdatePathfinding();
		break;
	case eUpdateState::BUILD_PATH:
		UpdateBuildPath();
		break;
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

	_updateState = eUpdateState::PATHFINDING;
}

void PathfindingState::Stop()
{
	State::Stop();
	
	while(false == _pathfindingTileQueue.empty())
		_pathfindingTileQueue.pop();
}

void PathfindingState::UpdatePathfinding()
{
	//길찾기 알고리즘 시작
	if (0 != _pathfindingTileQueue.size())
	{
		//첫번째 노드를 꺼내서 검사
		TileCell* tileCell = _pathfindingTileQueue.front();
		_pathfindingTileQueue.pop();

		if (false == tileCell->IsPathfindingMark())
		{
			tileCell->PathFinded();

			//목표 타겟이면 종료
			if (tileCell->GetTileX() == _targetTileCell->GetTileX() &&
				tileCell->GetTileY() == _targetTileCell->GetTileY())
			{
				std::list<Component*> comList = tileCell->GetComponentList();
				for (std::list<Component*>::iterator itr = comList.begin(); itr != comList.end(); itr++)
				{
					Component* component = (*itr);
					if (component->GetType() == eComponentType::CT_MONSTER)
					{
						if (tileCell->GetTileX() < tileCell->GetPrevPathfindingCell()->GetTileX())
							((Character*)component)->SetDirection(eDirection::RIGHT);
						else if (tileCell->GetTileX() > tileCell->GetPrevPathfindingCell()->GetTileX())
							((Character*)component)->SetDirection(eDirection::LEFT);
						else if (tileCell->GetTileY() < tileCell->GetPrevPathfindingCell()->GetTileY())
							((Character*)component)->SetDirection(eDirection::DOWN);
						else if (tileCell->GetTileY() > tileCell->GetPrevPathfindingCell()->GetTileY())
							((Character*)component)->SetDirection(eDirection::UP);
					}
				}

				OutputDebugString(L"Find Goal\n");
				_updateState = eUpdateState::BUILD_PATH;
				_reverseTileCell = _targetTileCell;
				return;
			}

			for (int direction = 0; direction < eDirection::NONE; direction++)
			{
				TilePosition curTilePos;
				curTilePos.x = tileCell->GetTileX();
				curTilePos.y = tileCell->GetTileY();
				TilePosition nextTilePos = GetNextTilePosition(curTilePos, (eDirection)direction);

				Map* map = GameSystem::GetInstance().GetStage()->GetMap();
				TileCell* nextTileCell = map->GetTileCell(nextTilePos);

				if ((true == map->CanMoveTileMap(nextTilePos) && false == nextTileCell->IsPathfindingMark()) ||
					(nextTileCell->GetTileX() == _targetTileCell->GetTileX() && nextTileCell->GetTileY() == _targetTileCell->GetTileY()))
				{
					if (NULL == nextTileCell->GetPrevPathfindingCell())
					{
						nextTileCell->SetPrevPathfindingCell(tileCell);
						_pathfindingTileQueue.push(nextTileCell);

						if (
							!(nextTileCell->GetTileX() == _targetTileCell->GetTileX() && nextTileCell->GetTileY() == _targetTileCell->GetTileY())
							&&
							!(nextTileCell->GetTileX() == _character->GetTileX() && nextTileCell->GetTileY() == _character->GetTileY())
							)
						{
							//검색범위를 그려준다.
							GameSystem::GetInstance().GetStage()->CreatePathfindNPC(nextTileCell);
						}
					}
				}
			}
		}
	}
	else
		_nextState = eStateType::ET_IDLE;
}

void PathfindingState::UpdateBuildPath()
{
	if (NULL != _reverseTileCell)
	{
		if (_reverseTileCell->GetTileX() != _targetTileCell->GetTileX() ||
			_reverseTileCell->GetTileY() != _targetTileCell->GetTileY())
		{
			//찾은 경로를 그려준다.
			GameSystem::GetInstance().GetStage()->CreatePathfindMark(_reverseTileCell);
			_character->PushPathTileCell(_reverseTileCell);
		}
		_reverseTileCell = _reverseTileCell->GetPrevPathfindingCell();
	}
	else
		_nextState = eStateType::ET_MOVE;
}