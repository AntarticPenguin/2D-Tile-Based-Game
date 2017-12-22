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

			//방향 세팅
			TilePosition to = { tileCell->GetTileX(), tileCell->GetTileY() };		//도착
			TilePosition from = { _character->GetTileX(), _character->GetTileY() };	//시작
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
				1. 공격범위 내에 타겟타일셀이 들어왔는지 체크
				2. 타겟타일셀이 갈 수 있다 -> 그냥 간다.
							  갈 수 없다 -> 타일셀에 있는 컴포넌트 리스트 가져온다.
					3. 몬스터다 -> 공격 state로
					   그 외 -> 움직인다.
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
		_pathTileCellStack.pop();	//자기위치는 제외
}

void PathfindingMoveState::Stop()
{
	State::Stop();
	_character->ClearPathTileCellStack();
}