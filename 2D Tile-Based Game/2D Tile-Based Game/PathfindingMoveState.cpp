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

			/*if (true == tileCell->CanMove())
			{
				_character->MoveStart(tileCell->GetTileX(), tileCell->GetTileY());
				_character->MoveStop();
			}
			else
			{
				std::list<Component*> collisionList;
				bool canMove = tileCell->GetCollisionList(collisionList);
				if (false == canMove)
				{
					Component* target = _character->Collision(collisionList);
					if (NULL != target && _character->IsAttackCooltime())
					{
						_character->ResetAttackCooltime();
						_character->SetTarget(target);
						_nextState = eStateType::ET_ATTACK;
					}
					else
					{
						_nextState = eStateType::ET_IDLE;
					}
				}
			}*/
			/*
				1. ���ݹ��� ���� Ÿ��Ÿ�ϼ��� ���Դ��� üũ
				2. Ÿ��Ÿ�ϼ��� �� �� �ִ� -> �׳� ����.
							  �� �� ���� -> Ÿ�ϼ��� �ִ� ������Ʈ ����Ʈ �����´�.
					3. ���ʹ� -> ���� state��
					   �� �� -> �����δ�.
			*/
			
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
	_character->ClearPathTileCellStack();
}