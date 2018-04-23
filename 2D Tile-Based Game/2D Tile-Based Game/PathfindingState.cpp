#include "PathfindingState.h"
#include "Pathfinding.h"
#include "GameSystem.h"
#include "Character.h"

PathfindingState::PathfindingState()
{
	
}

PathfindingState::~PathfindingState()
{

}

void PathfindingState::Init(Character* character)
{
	State::Init(character);
	_pathfinder = new Pathfinding(character);
}

void PathfindingState::Start()
{
	State::Start();
	//캐릭터 최대 이동 거리에 따른 타일 범위를 보여준다.
	_pathfinder->Reset();
	_pathfinder->ViewMoveRange();

	//경로탐색
	/*
	while (0 != _pathfindingTileQueue.size())
	{
		if (eUpdateState::BUILD_PATH == _updateState)
			break;
		UpdatePathfinding();
	}
	*/
	
	//경로도출
	/*
	while (NULL != _reverseTileCell)
	{
		UpdateBuildPath();
	}
	*/
}

void PathfindingState::Update(float deltaTime)
{
	State::Update(deltaTime);

	//오른쪽 마우스를 누르면 다시 IDLEState로
	if (GameSystem::GetInstance().IsRightMouseDown())
	{
		if (_character->IsMenuUp())
		{
			_character->TurnOffMenu();
			_character->ChangeState(eStateType::ET_IDLE);
		}
	}

	//보여준 타일 내에서 클릭할 때까지 대기
	if (NULL != _character->GetTargetCell())
	{
		OutputDebugString(L"범위 내 타일을 클릭!\n");

		//타일이 클릭되면 경로를 탐색
		_pathfinder->UpdatePathfinding();

		//경로를 도출하여 움직인다.
		_pathfinder->UpdateBuildPath();

		_character->TurnOffMenu();
	}
}