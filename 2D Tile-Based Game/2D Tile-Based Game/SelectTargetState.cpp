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

	_pathfinder = new Pathfinding();
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
		_pathfinder->ClearColorTile();
		_character->ChangeState(eStateType::ET_IDLE);
	}

	_character->ChargeBehavior(deltaTime);
	
	SetNextStateByType();
}


void SelectTargetState::Start()
{
	State::Start();

	_curState = eStateType::ET_SELECT_TARGET;

	int range = GetViewRange();
	ePathMode mode = GetPathMode();

	_character->SetTargetTileCell(NULL);

	_pathfinder->Reset();
	_pathfinder->SetStartFromCharacter(_character);
	_pathfinder->SetRange(range);
	_pathfinder->FindPath(mode);
}

void SelectTargetState::Stop()
{
	State::Stop();
	_pathfinder->ClearColorTile();
	UISystem::GetInstance().TurnOffBattleMenu();
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
	case eUIType::MAGIC:
		return _character->GetAttackRange();
	default:
		return 0;
	}
}

ePathMode SelectTargetState::GetPathMode()
{
	eUIType rangeType = UISystem::GetInstance().GetClickedUI();
	switch (rangeType)
	{
	case eUIType::MOVE:
		return ePathMode::VIEW_MOVE_RANGE;
	case eUIType::ATTACK:
	case eUIType::MAGIC:
		return ePathMode::VIEW_ATTACK_RANGE;
	}
}

void SelectTargetState::SetNextStateByType()
{
	eUIType type = UISystem::GetInstance().GetClickedUI();
	TileCell* targetCell = _character->GetTargetCell();
	if (NULL == targetCell)
		return;

	if (eUIType::MOVE == type)
	{
		if (!(targetCell->GetTileX() == _character->GetTileX() 
			&& targetCell->GetTileY() == _character->GetTileY()))
		{
			if(_pathfinder->CheckRange(targetCell))
				_nextState = eStateType::ET_PATHFINDING;
		}
	}
	else if (eUIType::ATTACK == type)
	{
		std::list<Component*> components = targetCell->GetComponentList();
		std::list<Component*>::iterator itr;

		for (itr = components.begin(); itr != components.end(); itr++)
		{
			Component* com = (*itr);

			if (eComponentType::CT_MONSTER == (*itr)->GetType())
			{
				_character->SetTarget(com);
				_nextState = eStateType::ET_ATTACK;
			}
		}
	}
	else if (eUIType::MAGIC == type)
	{
		//int mouseX = GameSystem::GetInstance().GetMouseX();
		//int mouseY = GameSystem::GetInstance().GetMouseY();

		//Map* map = GameSystem::GetInstance().GetStage()->GetMap();

		//TileCell* overCell = map->FindTileCellWithMousePosition(mouseX, mouseY);

		//if (_pathfinder->CheckRange(overCell))
		//{
		//	Pathfinding* skillviewer = new Pathfinding();
		//	skillviewer->Reset();
		//	skillviewer->SetStartCell(overCell);
		//	skillviewer->SetRange(4);
		//	skillviewer->FindPath(ePathMode::VIEW_ATTACK_RANGE);
		//}
	}
}