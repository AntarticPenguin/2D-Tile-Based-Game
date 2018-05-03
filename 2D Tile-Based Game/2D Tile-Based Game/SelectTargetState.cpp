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

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	_pathfinder->ColorMouseOverCell();
	CancelUI(GameSystem::GetInstance().IsRightMouseDown());
	_character->ChargeBehavior(deltaTime);
	SetNextStateByType();
}


void SelectTargetState::Start()
{
	State::Start();

	_curState = eStateType::ET_SELECT_TARGET;

	_character->SetTargetTileCell(NULL);

	ShowBaseRange();		//������ ����� �⺻ ���� Draw
}

void SelectTargetState::Stop()
{
	State::Stop();
	_pathfinder->ClearColorTile();
	UISystem::GetInstance().TurnOffBattleMenu();
}

void SelectTargetState::CancelUI(bool rightDown)
{
	if (rightDown)
	{
		_character->SetTargetTileCell(NULL);
		_pathfinder->ClearColorTile();
		_skillViewer->ClearColorTile();
		_character->ChangeState(eStateType::ET_IDLE);
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
			_nextState = eStateType::ET_PATHFINDING;
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

		if (eComponentType::CT_MONSTER == (*itr)->GetType())
		{
			_character->SetTarget(com);
			_nextState = eStateType::ET_ATTACK;
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
	{
		ShowBaseRange();

		_skillViewer->ColorMouseOverCell();

		_skillViewer->Reset();
		_skillViewer->SetStartCell(_mouseOverCell);
		_skillViewer->SetColor(D3DCOLOR_ARGB(100, 255, 255, 0));
		_skillViewer->SetRange(1);
		_skillViewer->FindPath(ePathMode::VIEW_ATTACK_RANGE);
	}
}