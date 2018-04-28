#include "Pathfinding.h"

#include "GameSystem.h"
#include "GlobalTile.h"
#include "Stage.h"
#include "Character.h"
#include "Map.h"
#include "TileCell.h"

Pathfinding::Pathfinding(Character* character)
{
	_character = character;
	_reverseTileCell = NULL;
}

Pathfinding::~Pathfinding()
{
	
}

void Pathfinding::Init()
{
	_map = GameSystem::GetInstance().GetStage()->GetMap();
}

void Pathfinding::FindPath(ePathMode mode, int range, eFindMethod method)
{
	TileCell* targetTileCell = _character->GetTargetCell();
	TileCell* startTileCell = _map->GetTileCell(_character->GetTileX(), _character->GetTileY());

	sPathCommand newCommand;
	newCommand.heuristic = 0.0f;
	newCommand.tileCell = startTileCell;
	_pathfindingTileQueue.push(newCommand);

	while (0 != _pathfindingTileQueue.size())
	{
		//ù��° ��带 ������ �˻�
		sPathCommand command = _pathfindingTileQueue.top();
		_pathfindingTileQueue.pop();

		if (false == command.tileCell->IsPathfindingMark())
		{
			command.tileCell->PathFinded();

			//��ǥ Ÿ���̸� ����
			if ( (ePathMode::FIND_PATH == mode)
				&& (command.tileCell->GetTileX() == targetTileCell->GetTileX() 
					&& command.tileCell->GetTileY() == targetTileCell->GetTileY()) )
			{
				_reverseTileCell = targetTileCell;
				return;
			}

			for (int direction = 0; direction < eDirection::NONE; direction++)
			{
				TilePosition curTilePos = { command.tileCell->GetTileX(), command.tileCell->GetTileY() };
				TilePosition nextTilePos = GetNextTilePosition(curTilePos, (eDirection)direction);

				TileCell* nextTileCell = _map->GetTileCell(nextTilePos);

				if (CheckPreCondition(mode, nextTilePos, nextTileCell, targetTileCell))
				{
					float distanceFromStart = command.tileCell->GetDistanceFromStart() + command.tileCell->GetDistanceWeight();	//�Ÿ��켱
					float heuristic = CalcHeuristic(method, distanceFromStart, command.tileCell, nextTileCell, targetTileCell);

					if (ePathMode::VIEW_RANGE == mode && (range < distanceFromStart))
						return;

					if (NULL == nextTileCell->GetPrevPathfindingCell())
					{
						nextTileCell->SetDistanceFromStart(distanceFromStart);	//�Ÿ��켱
						nextTileCell->SetHeuristic(heuristic);
						nextTileCell->SetPrevPathfindingCell(command.tileCell);

						sPathCommand newCommand;
						newCommand.heuristic = heuristic;
						newCommand.tileCell = nextTileCell;
						_pathfindingTileQueue.push(newCommand);

						if(ePathMode::VIEW_RANGE == mode)	//�˻������� �׷��ش�.
						{
							if ( !(nextTileCell->GetTileX() == _character->GetTileX()
								&& nextTileCell->GetTileY() == _character->GetTileY()) )
							{
								nextTileCell->TurnOnColorTile(D3DCOLOR_ARGB(100, 0, 0, 255));
								_character->PushColorTileCell(nextTileCell);
							}
						}
					}	
					else
					{
						if (distanceFromStart < nextTileCell->GetDistanceFromStart())
						{
							//�ٽ� �˻�(ť�� ����) : heuristic ���� �ٽ� ����ؼ� ��
							nextTileCell->SetDistanceFromStart(distanceFromStart);
							nextTileCell->SetPrevPathfindingCell(command.tileCell);

							sPathCommand newCommand;
							newCommand.heuristic = CalcHeuristic(method, distanceFromStart, command.tileCell, nextTileCell, targetTileCell);
							newCommand.tileCell = nextTileCell;
							_pathfindingTileQueue.push(newCommand);
						}
					}
				}
			}
		}
	}
}

void Pathfinding::BuildPath()
{
	while (NULL != _reverseTileCell)
	{
		_character->PushPathTileCell(_reverseTileCell);
		_reverseTileCell = _reverseTileCell->GetPrevPathfindingCell();
	}

	_character->ChangeState(eStateType::ET_MOVE);
}

bool Pathfinding::CheckPreCondition(ePathMode mode, TilePosition nextTilePos, TileCell* nextTileCell, TileCell* targetTileCell)
{
	bool condition = false;

	//�� �� �ִ� Ÿ���̰� �湮���� ���� Ÿ���ΰ�?
	/*
		if ((true == _map->CanMoveTileMap(nextTilePos) && false == nextTileCell->IsPathfindingMark())
			|| (nextTileCell->GetTileX() == targetTileCell->GetTileX() && nextTileCell->GetTileY() == targetTileCell->GetTileY()))
	*/
	if ((true == _map->CanMoveTileMap(nextTilePos) && false == nextTileCell->IsPathfindingMark()))
		condition = true;

	if (ePathMode::VIEW_RANGE == mode)
		return condition;
	else if (ePathMode::FIND_PATH == mode)
	{
		//�ش� Ÿ���� ��ǥ Ÿ���ΰ�?
		if ((nextTileCell->GetTileX() == targetTileCell->GetTileX() && nextTileCell->GetTileY() == targetTileCell->GetTileY()))
			condition = true;
	}
	return condition;
}

void Pathfinding::Reset()
{
	int height = _map->GetHeight();
	int width = _map->GetWidth();

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			TileCell* tileCell = _map->GetTileCell(x, y);
			tileCell->InitPathfinding();
		}
	}

	while (false == _pathfindingTileQueue.empty())
		_pathfindingTileQueue.pop();
}

float Pathfinding::CalcHeuristic(eFindMethod eMethod, float distanceFromStart, TileCell* tileCell, TileCell* nextTileCell, TileCell* targetTileCell)
{
	switch (eMethod)
	{
	case eFindMethod::DISTANCE:
		return distanceFromStart;
	case eFindMethod::SIMPLE:
		return CalcSimpleHeuristic(tileCell, nextTileCell, targetTileCell);
	case eFindMethod::COMPLEX:
		return CalcComplexHeuristic(nextTileCell, targetTileCell);
	case eFindMethod::ASTAR:
		return CalcAStarHeuristic(distanceFromStart, nextTileCell, targetTileCell);
	}
}

float Pathfinding::CalcSimpleHeuristic(TileCell* tileCell, TileCell* nextTileCell, TileCell* targetTileCell)
{
	float heuristic = 0.0f;

	int diffFromCurrent = 0;
	int diffFromNext = 0;

	//x�� �߰��� �� ����
	{
		diffFromCurrent = tileCell->GetTileX() - targetTileCell->GetTileX();
		if (diffFromCurrent < 0)
			diffFromCurrent = diffFromCurrent * (-1);

		diffFromNext = nextTileCell->GetTileX() - targetTileCell->GetTileX();
		if (diffFromNext < 0)
			diffFromNext = diffFromNext * (-1);

		if (diffFromNext < diffFromCurrent)		//���� Ÿ���� ���� Ÿ�Ϻ��� ��ǥŸ�Ͽ� ����� ��
		{
			heuristic -= 1.0f;
		}
		else if (diffFromNext > diffFromCurrent)
		{
			heuristic += 1.0f;
		}
	}

	//y�� �߰��� �� ���� ����
	{
		diffFromCurrent = tileCell->GetTileY() - targetTileCell->GetTileY();
		if (diffFromCurrent < 0)
			diffFromCurrent = diffFromCurrent * (-1);

		diffFromNext = nextTileCell->GetTileY() - targetTileCell->GetTileY();
		if (diffFromNext < 0)
			diffFromNext = diffFromNext * (-1);

		if (diffFromNext < diffFromCurrent)		//���� Ÿ���� ���� Ÿ�Ϻ��� ��ǥŸ�Ͽ� ����� ��
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

float Pathfinding::CalcComplexHeuristic(TileCell* nextTileCell, TileCell* targetTileCell)
{
	int distanceW = nextTileCell->GetTileX() - targetTileCell->GetTileX();
	int distanceH = nextTileCell->GetTileY() - targetTileCell->GetTileY();

	distanceW = distanceW * distanceW;
	distanceH = distanceH * distanceH;

	return (float)sqrt((double)distanceW + (double)distanceH);
}

float Pathfinding::CalcAStarHeuristic(float distanceFromStart, TileCell* nextTileCell, TileCell* targetTileCell)
{
	return distanceFromStart + CalcComplexHeuristic(nextTileCell, targetTileCell);
}