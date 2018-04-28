#include "SelectTargetState.h"
#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"
#include "TileCell.h"
#include "Pathfinding.h"
#include "Character.h"

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

	_pathfinder->ColorMouseOverCell();

	if (GameSystem::GetInstance().IsRightMouseDown())
	{
		_character->ClearColorTile();
		_character->ChangeState(eStateType::ET_IDLE);
	}
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

	_pathfinder->Reset();
	_pathfinder->FindPath(ePathMode::VIEW_RANGE, _character->GetAttackRange());
}

void SelectTargetState::Stop()
{
	State::Stop();
}