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
	_mouseOverCell = NULL;
	_prevOverCell = NULL;
	_turnOn = true;
}

SelectTargetState::~SelectTargetState()
{
}

void SelectTargetState::Init(Character* character)
{
	State::Init(character);

	_pathfinder = new Pathfinding();
	_skillViewer = new Pathfinding();
}

void SelectTargetState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	ColorMouseOver();
	CancelUI(GameSystem::GetInstance().IsRightMouseDown());
	_character->ChargeBehavior(deltaTime);
	SetNextStateByType();
}

void SelectTargetState::Start()
{
	State::Start();

	_curState = eStateType::SELECT_TARGET;

	_character->SetTargetTileCell(NULL);

	ShowBaseRange();		//선택한 기능의 기본 범위 Draw
	TurnOnMouseOver();
}

void SelectTargetState::Stop()
{
	State::Stop();
	_pathfinder->ClearColorTile();
	_skillViewer->ClearColorTile();
	UISystem::GetInstance().TurnOffMenu();
}

void SelectTargetState::ColorMouseOver()
{
	if (_turnOn)
		_pathfinder->ColorMouseOverCell();
}

void SelectTargetState::TurnOnMouseOver()
{
	_turnOn = true;
}

void SelectTargetState::TurnOffMouseOver()
{
	_turnOn = false;
}

void SelectTargetState::CancelUI(bool rightDown)
{
	if (rightDown)
	{
		_character->SetTargetTileCell(NULL);
		_pathfinder->ClearColorTile();
		_skillViewer->ClearColorTile();
		_character->ChangeState(eStateType::IDLE);
	}
}

void SelectTargetState::ShowBaseRange()
{
	int range = GetViewRange();
	ePathMode mode = GetPathMode();

	_pathfinder->Reset();
	_pathfinder->SetStartFromCharacter(_character);
	_pathfinder->SetRange(range);
	_pathfinder->FindPath(mode);
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

	switch (type)
	{
	case eUIType::MOVE:
		DoMoveFunction();
		break;
	case eUIType::ATTACK:
		DoAttackFunction();
		break;
	case eUIType::MAGIC:
		DoMagicFunction();
		TurnOffMouseOver();
		break;
	default:
		break;
	}
}

void SelectTargetState::DoMoveFunction()
{
	TileCell* targetCell = _character->GetTargetCell();
	if (NULL == targetCell)
		return;

	if (!(targetCell->GetTileX() == _character->GetTileX()
		&& targetCell->GetTileY() == _character->GetTileY()))
	{
		if (_pathfinder->CheckRange(targetCell))
			_nextState = eStateType::PATHFINDING;
	}
}

void SelectTargetState::DoAttackFunction()
{
	TileCell* targetCell = _character->GetTargetCell();
	if (NULL == targetCell)
		return;

	std::list<Component*> components = targetCell->GetComponentList();
	std::list<Component*>::iterator itr;

	for (itr = components.begin(); itr != components.end(); itr++)
	{
		Component* com = (*itr);

		if (eComponentType::MONSTER == (*itr)->GetType())
		{
			_character->AddTarget(com);
			_nextState = eStateType::ATTACK;
		}
	}
}

void SelectTargetState::DoMagicFunction()
{
	int mouseX = GameSystem::GetInstance().GetMouseX();
	int mouseY = GameSystem::GetInstance().GetMouseY();

	Map* map = GameSystem::GetInstance().GetStage()->GetMap();

	_mouseOverCell = map->FindTileCellWithMousePosition(mouseX, mouseY);

	if (_pathfinder->CheckRange(_mouseOverCell))
		ShowSkillRange();
	_prevOverCell = _mouseOverCell;

	if (NULL != _character->GetTargetCell())
	{
		bool _haveTargets = false;
		std::vector<TileCell*> tiles = _skillViewer->GetRangeTiles();

		for (int i = 0; i < tiles.size(); i++)
		{
			std::list<Component*> components = tiles[i]->GetComponentList();
			std::list<Component*>::iterator itr;

			for (itr = components.begin(); itr != components.end(); itr++)
			{
				Component* com = (*itr);

				if (eComponentType::MONSTER == (*itr)->GetType())
				{
					_character->AddTarget(com);
					_haveTargets = true;
				}
			}
		}
		if(_haveTargets)
			_nextState = eStateType::ATTACK;
	}

	_character->SetTargetTileCell(NULL);
}

void SelectTargetState::ShowSkillRange()
{
	if (_prevOverCell != _mouseOverCell)
	{
		_skillViewer->ClearColorTile();

		ShowBaseRange();
		_skillViewer->Reset();
		_skillViewer->SetStartCell(_mouseOverCell);
		_skillViewer->SetColor(D3DCOLOR_ARGB(100, 255, 255, 0));
		_skillViewer->SetRange(2);
		_skillViewer->FindPath(ePathMode::VIEW_ATTACK_RANGE);
	}
}