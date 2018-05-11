#include "Pathfinding.h"

#include "GameSystem.h"
#include "GlobalTile.h"

#include "Character.h"
#include "Map.h"
#include "Stage.h"
#include "TileCell.h"

Pathfinding::Pathfinding()
{
	_reverseTileCell = NULL;
	_prevOverCell = NULL;
	_map = GameSystem::GetInstance().GetStage()->GetMap();
	_range = 0;
	_startTileCell = NULL;

	_color = D3DCOLOR_ARGB(100, 0, 0, 255);
	_colorTileList.clear();
}

Pathfinding::~Pathfinding()
{
	
}

void Pathfinding::SetRange(int range)
{
	_range = range;
}

void Pathfinding::SetStartFromCharacter(Character* character)
{
	_character = character;
	SetStartCell(_map->GetTileCell(_character->GetTileX(), _character->GetTileY()));
}

void Pathfinding::SetStartCell(TileCell* tileCell)
{
	_startTileCell = tileCell;
}

void Pathfinding::FindPath(ePathMode mode, eFindMethod method)
{
	bool isViewRange = (ePathMode::VIEW_MOVE_RANGE == mode || ePathMode::VIEW_ATTACK_RANGE == mode);
	if (isViewRange)
		DrawSearchTile(_startTileCell);

	_prevOverCell = NULL;

	TileCell* targetTileCell = NULL;
	if (ePathMode::FIND_PATH == mode)
		targetTileCell = _character->GetTargetCell();

	sPathCommand newCommand;
	newCommand.heuristic = 0.0f;
	newCommand.tileCell = _startTileCell;
	_pathfindingTileQueue.push(newCommand);

	while (0 != _pathfindingTileQueue.size())
	{
		//1.ù��° ��带 ������ �˻�
		sPathCommand command = _pathfindingTileQueue.top();
		_pathfindingTileQueue.pop();

		if (false == command.tileCell->IsPathfindingMark())
		{
			command.tileCell->PathFinded();

			//2.���� ��尡 ��ǥ Ÿ���̸� ����
			if ((ePathMode::FIND_PATH == mode)
				&& (command.tileCell->GetTileX() == targetTileCell->GetTileX()
					&& command.tileCell->GetTileY() == targetTileCell->GetTileY()))
			{
				_reverseTileCell = targetTileCell;
				return;
			}

			//3.������ ������ ���ǿ� ���� �˻� ��, ť�� ����
			for (int direction = 0; direction < (int)eDirection::NONE; direction++)
			{
				TilePosition curTilePos = { command.tileCell->GetTileX(), command.tileCell->GetTileY() };
				TilePosition nextTilePos = GetNextTilePosition(curTilePos, (eDirection)direction);

				TileCell* nextTileCell = _map->GetTileCell(nextTilePos);

				if (CheckPreCondition(mode, nextTilePos, nextTileCell, targetTileCell))
				{
					float distanceFromStart = command.tileCell->GetDistanceFromStart() + command.tileCell->GetDistanceWeight();
					float heuristic = CalcHeuristic(method, distanceFromStart, command.tileCell, nextTileCell, targetTileCell);

					if (isViewRange	&& (_range < distanceFromStart))  //�˻����� ����
						return;

					if (NULL == nextTileCell->GetPrevPathfindingCell())
					{
						nextTileCell->SetDistanceFromStart(distanceFromStart);
						nextTileCell->SetHeuristic(heuristic);
						nextTileCell->SetPrevPathfindingCell(command.tileCell);

						sPathCommand newCommand;
						newCommand.heuristic = heuristic;
						newCommand.tileCell = nextTileCell;
						_pathfindingTileQueue.push(newCommand);

						if (isViewRange)
							DrawSearchTile(nextTileCell);	//�˻������� ��ĥ
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

	_character->ChangeState(eStateType::MOVE);
}

bool Pathfinding::CheckPreCondition(ePathMode mode, TilePosition nextTilePos, TileCell* nextTileCell, TileCell* targetTileCell)
{
	bool condition = false;

	/*
	//�� �� �ִ� Ÿ���̰� �湮���� ���� Ÿ���ΰ�?
	if ((true == _map->CanMoveTileMap(nextTilePos) && false == nextTileCell->IsPathfindingMark())
		|| (nextTileCell->GetTileX() == targetTileCell->GetTileX() && nextTileCell->GetTileY() == targetTileCell->GetTileY()))
	*/

	//����,������ canMove�� false���� ������ �����־�� �Ѵ�. �� �湮���θ� üũ���ָ� �ȴ�.
	if (false == nextTileCell->IsPathfindingMark())
		condition = true;

	if (ePathMode::VIEW_MOVE_RANGE == mode || ePathMode::FIND_PATH == mode)
	{
		if (condition)
			condition = _map->CanMoveTileMap(nextTilePos);
	}

	if (ePathMode::VIEW_MOVE_RANGE == mode || ePathMode::VIEW_ATTACK_RANGE == mode)
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
	default:
		return 0;
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
			heuristic -= 1.0f;
		else if (diffFromNext > diffFromCurrent)
			heuristic += 1.0f;
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
			heuristic -= 1.0f;
		else if (diffFromNext > diffFromCurrent)
			heuristic += 1.0f;
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

bool Pathfinding::CheckRange(TileCell* targetTileCell)
{
	for (size_t i = 0; i < _colorTileList.size(); i++)
	{
		if (_colorTileList[i] == targetTileCell)
			return true;
	}
	return false;
}

void Pathfinding::SetColor(D3DCOLOR color)
{
	_color = color;
}

void Pathfinding::DrawSearchTile(TileCell* tileCell)
{
	tileCell->TurnOnColorTile(_color);
	_colorTileList.push_back(tileCell);
}

std::vector<TileCell*>& Pathfinding::GetRangeTiles()
{
	return _colorTileList;
}

void Pathfinding::ClearColorTile()
{
	for (size_t i = 0; i < _colorTileList.size(); i++)
		_colorTileList[i]->TurnOffColorTile();
	_colorTileList.clear();
}

void Pathfinding::ColorMouseOverCell()
{
	int mouseX = GameSystem::GetInstance().GetMouseX();
	int mouseY = GameSystem::GetInstance().GetMouseY();

	_map = GameSystem::GetInstance().GetStage()->GetMap();
	_mouseOverCell = _map->FindTileCellWithMousePosition(mouseX, mouseY);

	if (CheckRange(_mouseOverCell))
	{
		if (NULL != _prevOverCell && _prevOverCell != _mouseOverCell)
			_prevOverCell->SetMouseOver(false);

		_mouseOverCell->SetMouseOver(true);
		_prevOverCell = _mouseOverCell;
	}
}