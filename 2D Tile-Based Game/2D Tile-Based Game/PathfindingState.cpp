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
	_pathfinder->FindPath(ePathMode::VIEW_RANGE);	//ĳ���� �ִ� �̵� �Ÿ��� ���� Ÿ�� ������ �����ش�.
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

	//������ Ÿ�� ������ Ŭ���� ������ ���
	if (NULL != _character->GetTargetCell())
	{
		_pathfinder->Reset();
		_pathfinder->FindPath(ePathMode::FIND_PATH, eFindMethod::ASTAR);	//Ÿ���� Ŭ���Ǹ� ��θ� Ž�� : Astar
		_pathfinder->BuildPath();											//��θ� �����Ͽ� �����δ�.
		_character->TurnOffMenu();
	}
}