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

	//��ã�� �˰��� ����
	if (0 != _pathfindingTileQueue.size())
	{
		//ù��° ��带 ������ �˻�
		TileCell* tileCell = _pathfindingTileQueue.front();
		_pathfindingTileQueue.pop();

		if (false == tileCell->IsPathfindingMark())
		{
			tileCell->PathFinded();

			/*WCHAR msg[256];
			wsprintf(msg, L"X: %d, Y: %d, targetX: %d, targetY: %d\n", tileCell->GetTileX(), tileCell->GetTileY()
			,_targetTileCell->GetTileX(), _targetTileCell->GetTileY());
			OutputDebugString(msg);*/

			//��ǥ Ÿ���̸� ����
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
				_nextState = eStateType::ET_IDLE;
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

				if ( (true == map->CanMoveTileMap(nextTilePos) && false == nextTileCell->IsPathfindingMark()) ||
					(nextTileCell->GetTileX() == _targetTileCell->GetTileX() && nextTileCell->GetTileY() == _targetTileCell->GetTileY()) )
				{
					if (NULL == nextTileCell->GetPrevPathfindingCell())
					{
						nextTileCell->SetPrevPathfindingCell(tileCell);
						_pathfindingTileQueue.push(nextTileCell);

						/*if (����, �÷��̾�� ����)
						{
							�������� �ʴ� npc �����ؼ� �ش� Ÿ�ϼ����ٰ� ��ġ
								������ ���� ������ �ٶ󺸰� �Ѵ�.
						}*/
						if ( 
							!(nextTileCell->GetTileX() == _targetTileCell->GetTileX() && nextTileCell->GetTileY() == _targetTileCell->GetTileY())
							&&
							!(nextTileCell->GetTileX() == _character->GetTileX() && nextTileCell->GetTileY() == _character->GetTileY())
							)
						{
							GameSystem::GetInstance().GetStage()->CreatePathfindNPC(nextTileCell);
						}
					}
				}
			}
		}
	}
}

void PathfindingState::Start()
{
	State::Start();
	_curState = eStateType::ET_PATHFINDING;

	_targetTileCell = _character->GetTargetCell();

	//��� Ÿ�ϼ� ��ã�� �Ӽ� �ʱ�ȭ
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
	
	while(false == _pathfindingTileQueue.empty())
		_pathfindingTileQueue.pop();
}