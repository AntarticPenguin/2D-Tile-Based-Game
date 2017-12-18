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
		TileCell* tileCell = _pathfindingTileQueue.top();
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
				TilePosition curTilePos = { tileCell->GetTileX(), tileCell->GetTileY() };
				TilePosition nextTilePos = GetNextTilePosition(curTilePos, (eDirection)direction);

				Map* map = GameSystem::GetInstance().GetStage()->GetMap();
				TileCell* nextTileCell = map->GetTileCell(nextTilePos);

				if ((true == map->CanMoveTileMap(nextTilePos) && false == nextTileCell->IsPathfindingMark()) ||
					(nextTileCell->GetTileX() == _targetTileCell->GetTileX() && nextTileCell->GetTileY() == _targetTileCell->GetTileY()))
				{
					float distanceFromStart = tileCell->GetDistanceFromStart() + tileCell->GetDistanceWeight();	//거리우선
					//float heuristic = CalcSimpleHeuristic(tileCell, nextTileCell, _targetTileCell);
					//float heuristic = CalcComplexHeuristic(nextTileCell, _targetTileCell);
					float heuristic = CalcAStarHeuristic(distanceFromStart, nextTileCell, _targetTileCell);


					if (NULL == nextTileCell->GetPrevPathfindingCell())
					{
						nextTileCell->SetDistanceFromStart(distanceFromStart);	//거리우선
						nextTileCell->SetHeuristic(heuristic);
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
					//else
					//{
					//	if (distanceFromStart < nextTileCell->GetDistanceFromStart())
					//	{
					//		//다시 검사(큐에 삽입)
					//		nextTileCell->SetDistanceFromStart(distanceFromStart);
					//		nextTileCell->SetPrevPathfindingCell(tileCell);
					//		_pathfindingTileQueue.push(nextTileCell);
					//	}
					//}
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

float PathfindingState::CalcSimpleHeuristic(TileCell* tileCell, TileCell* nextTileCell, TileCell* targetTileCell)
{
	float heuristic = 0.0f;

	int diffFromCurrent = 0;
	int diffFromNext = 0;

	//x의 발견적 값 갱신
	{
		diffFromCurrent = tileCell->GetTileX() - targetTileCell->GetTileX();
		if (diffFromCurrent < 0)
			diffFromCurrent = diffFromCurrent * (-1);

		diffFromNext = nextTileCell->GetTileX() - targetTileCell->GetTileX();
		if (diffFromNext < 0)
			diffFromNext = diffFromNext * (-1);

		if (diffFromNext < diffFromCurrent)		//다음 타일이 현재 타일보다 목표타일에 가까울 때
		{
			heuristic -= 1.0f;
		}
		else if (diffFromNext > diffFromCurrent)
		{
			heuristic += 1.0f;
		}
	}

	//y의 발견적 값 누정 갱신
	{
		diffFromCurrent = tileCell->GetTileY() - targetTileCell->GetTileY();
		if (diffFromCurrent < 0)
			diffFromCurrent = diffFromCurrent * (-1);

		diffFromNext = nextTileCell->GetTileY() - targetTileCell->GetTileY();
		if (diffFromNext < 0)
			diffFromNext = diffFromNext * (-1);

		if (diffFromNext < diffFromCurrent)		//다음 타일이 현재 타일보다 목표타일에 가까울 때
		{
			heuristic -= 1.0f;
		}
		else if (diffFromNext > diffFromCurrent)
		{
			heuristic += 1.0f;
		}
	}

	//Add heuristic what you want
	if (tileCell->GetDistanceWeight() < nextTileCell->GetDistanceWeight())
		heuristic += 1.0f;
	else if (tileCell->GetDistanceWeight() > nextTileCell->GetDistanceWeight())
		heuristic -= 1.0f;

	return heuristic;
}

float PathfindingState::CalcComplexHeuristic(TileCell* nextTileCell, TileCell* targetTileCell)
{
	int distanceW = nextTileCell->GetTileX() - targetTileCell->GetTileX();
	int distanceH = nextTileCell->GetTileY() - targetTileCell->GetTileY();

	distanceW = distanceW * distanceW;
	distanceH = distanceH * distanceH;

	return (float)sqrt((double)distanceW + (double)distanceH);
}

float PathfindingState::CalcAStarHeuristic(float distanceFromStart, TileCell* nextTileCell, TileCell* targetTileCell)
{
	return distanceFromStart + CalcComplexHeuristic(nextTileCell, targetTileCell);
}