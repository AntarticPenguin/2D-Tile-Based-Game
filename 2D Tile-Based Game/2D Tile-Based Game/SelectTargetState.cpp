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

}

void SelectTargetState::Update(float deltaTime)
{
	State::Update(deltaTime);
	
	int mouseX = GameSystem::GetInstance().GetMouseX();
	int mouseY = GameSystem::GetInstance().GetMouseY();

	Map* map = GameSystem::GetInstance().GetStage()->GetMap();
	_mouseOverCell = map->FindTileCellWithMousePosition(mouseX, mouseY);

	if (NULL != _mouseOverCell)
	{
		if (_character->CheckAttackRange(_mouseOverCell))
		{
			D3DCOLOR color = D3DCOLOR_ARGB(100, 255, 0, 0);
			_mouseOverCell->TurnOnColorTile(color);
		}

		//������
		//Pathfinding�� �̿��� ������ �����ְ� ����
		//Pathfinding�� ������ �����ִ� �ڵ�� ĳ������ moveRange�� üũ�ϰ� ��ĥ
		//		moveRange ��� �׳� Range�� ����.
		//FindPath�Լ� ����
		//CheckAttackRange ����� CheckMoveRange�� CheckRange�� �ٲܰ�
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

	Pathfinding* pathfinder = new Pathfinding(_character);
	pathfinder->Init();
	pathfinder->FindPath(ePathMode::VIEW_RANGE, _character->GetAttackRange());
}

void SelectTargetState::Stop()
{
	State::Stop();
}