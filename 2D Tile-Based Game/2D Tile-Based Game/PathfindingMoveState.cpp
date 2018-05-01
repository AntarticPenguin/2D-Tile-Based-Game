#include <list>
#include <vector>

#include "PathfindingMoveState.h"

#include "ComponentSystem.h"
#include "GameSystem.h"

#include "Character.h"
#include "Map.h"
#include "Stage.h"
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
		_pathTileCellStack.pop();	//자기위치는 제외
}

void PathfindingMoveState::Stop()
{
	State::Stop();

	if (eStage::DUNGEON == GameSystem::GetInstance().GetStage()->GetStageInfo())
		_character->DecreaseBehaviorPoint(_character->GetMovePoint());

	_character->ClearPathTileCellStack();
}