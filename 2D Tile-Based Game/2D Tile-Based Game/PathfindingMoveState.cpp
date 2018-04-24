#include <list>
#include <vector>

#include "PathfindingMoveState.h"
#include "Character.h"
#include "Map.h"
#include "Stage.h"
#include "ComponentSystem.h"
#include "GameSystem.h"
#include "TileCell.h"

PathfindingMoveState::PathfindingMoveState()
{

}

PathfindingMoveState::~PathfindingMoveState()
{

}

void PathfindingMoveState::Init(Character* character)
{
	State::Init(character);
	_movingDuration = 0.0f;
}

void PathfindingMoveState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	if (false == _character->IsLive())
		return;
	
	if (_character->GetMoveTime() <= _movingDuration)
	{
		_movingDuration = 0.0f;
		if (0 != _pathTileCellStack.size())
		{
			TileCell* tileCell = _pathTileCellStack.top();
			_pathTileCellStack.pop();

			//���� ����
			TilePosition to = { tileCell->GetTileX(), tileCell->GetTileY() };		//����
			TilePosition from = { _character->GetTileX(), _character->GetTileY() };	//����
			eDirection direction = GetDirection(to, from);
			if(eDirection::NONE != direction)
				_character->SetDirection(direction);
			/*
				1. ���ݹ��� ���� Ÿ��Ÿ�ϼ��� ���Դ��� üũ
					- ���� ->2������
					- �ִ�.
						->Ÿ�ϼ��� �ִ� ������Ʈ �˻�.
							-���ʹ�. ->����
							-�׿� -> 2������
				2. �����δ�.
			*/
			Map* map = GameSystem::GetInstance().GetStage()->GetMap();

			int range = _character->GetAttackRange();
			int minTileX = _character->GetTileX() - range;
			int maxTileX = _character->GetTileX() + range;
			int minTileY = _character->GetTileY() - range;
			int maxTileY = _character->GetTileY() + range;

			if (minTileX < 0)
				minTileX = 0;
			if (map->GetWidth() <= maxTileX)
				maxTileX = map->GetWidth() - 1;
			if (minTileY < 0)
				minTileY = 0;
			if (map->GetHeight() <= maxTileY)
				maxTileY = map->GetHeight() - 1;
			
			for (int y = minTileY; y <= maxTileY; y++)
			{
				for (int x = minTileX; x <= maxTileX; x++)
				{
					TileCell* checkTileCell = map->GetTileCell(x, y);
					if (checkTileCell == _character->GetTargetCell())
					{
						std::list<Component*> collisionList;
						bool canMove = checkTileCell->GetCollisionList(collisionList);
						if (false == canMove)
						{
							Component* target = _character->Collision(collisionList);
							if (NULL != target && _character->IsAttackCooltime())
							{
								_character->ResetAttackCooltime();
								_character->SetTarget(target);
								_nextState = eStateType::ET_ATTACK;
								return;
							}
						}
					}
				}
			}

			if (true == tileCell->CanMove())
			{
				_character->MoveStart(tileCell->GetTileX(), tileCell->GetTileY());
				_character->MoveStop();
			}
			else
			{
				_nextState = eStateType::ET_IDLE;
			}
		}
		else
		{
			_nextState = eStateType::ET_IDLE;
		}
	}
	else
	{
		_movingDuration += deltaTime;
	}
}

void PathfindingMoveState::Start()
{
	State::Start();
	_curState = eStateType::ET_MOVE;

	_movingDuration = 0.0f;

	_pathTileCellStack = _character->GetPathTileCellStack();

	if(0 < _pathTileCellStack.size())
		_pathTileCellStack.pop();	//�ڱ���ġ�� ����
}

void PathfindingMoveState::Stop()
{
	State::Stop();

	if (eStage::DUNGEON == GameSystem::GetInstance().GetStage()->GetStageInfo())
		_character->DecreaseBehaviorPoint(2);

	_character->ClearPathTileCellStack();
}