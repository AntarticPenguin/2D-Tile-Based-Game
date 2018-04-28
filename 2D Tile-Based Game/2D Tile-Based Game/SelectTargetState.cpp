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

		//문제점
		//Pathfinding을 이용한 범위를 보여주고 싶음
		//Pathfinding의 범위를 보여주는 코드는 캐릭터의 moveRange를 체크하고 색칠
		//		moveRange 대신 그냥 Range를 받자.
		//FindPath함수 수정
		//CheckAttackRange 지우고 CheckMoveRange를 CheckRange로 바꿀것
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