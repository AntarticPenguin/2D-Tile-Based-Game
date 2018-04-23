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
	_pathfinder->Init();
}

void PathfindingState::Start()
{
	State::Start();

	_pathfinder->Reset();
	_pathfinder->FindPath(ePathMode::VIEW_RANGE);	//캐릭터 최대 이동 거리에 따른 타일 범위를 보여준다.
}

void PathfindingState::Update(float deltaTime)
{
	State::Update(deltaTime);

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
		_pathfinder->Reset();
		_pathfinder->FindPath(ePathMode::FIND_PATH, eFindMethod::ASTAR);	//타일이 클릭되면 경로를 탐색 : Astar
		_pathfinder->BuildPath();											//경로를 도출하여 움직인다.
		_character->TurnOffMenu();
	}
}