#include "SelectTargetState.h"

#include "GameSystem.h"
#include "UISystem.h"

#include "Character.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Stage.h"
#include "TileCell.h"

SelectTargetState::SelectTargetState()
{
}

SelectTargetState::~SelectTargetState()
{
}

void SelectTargetState::Init(Character* character)
{
	State::Init(character);

	_pathfinder = new Pathfinding(_character);
	_pathfinder->Init();
}

void SelectTargetState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	_pathfinder->ColorMouseOverCell();

	if (GameSystem::GetInstance().IsRightMouseDown())
	{
		_character->SetTargetTileCell(NULL);
		_character->ClearColorTile();
		_character->ChangeState(eStateType::ET_IDLE);
	}

	_character->ChargeBehavior(deltaTime);
	
	SetNextStateByType();
}


void SelectTargetState::Start()
{
	State::Start();

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	_curState = eStateType::ET_SELECT_TARGET;

	int moveRange = GetViewRange();

	_pathfinder->Reset();
	_pathfinder->FindPath(ePathMode::VIEW_RANGE, moveRange);
}

void SelectTargetState::Stop()
{
	State::Stop();
}

int SelectTargetState::GetViewRange()
{
	eUIType rangeType = UISystem::GetInstance().GetClickedUI();
	switch (rangeType)
	{
	case eUIType::MOVE:
		return _character->GetMoveRange();
	case eUIType::ATTACK:
		return _character->GetAttackRange();
	}

	return 0;
}

void SelectTargetState::SetNextStateByType()
{
	eUIType type = UISystem::GetInstance().GetClickedUI();
	if (eUIType::MOVE == type)
	{
		TileCell* targetCell = _character->GetTargetCell();
		if ((NULL != targetCell)
			&& !(targetCell->GetTileX() == _character->GetTileX() && targetCell->GetTileY() == _character->GetTileY())
			)
		{
			_nextState = eStateType::ET_PATHFINDING;
		}
	}
}