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
	//ĳ���� �ִ� �̵� �Ÿ��� ���� Ÿ�� ������ �����ش�.
	_pathfinder->Reset();
	_pathfinder->ViewMoveRange();

	//���Ž��
	/*
	while (0 != _pathfindingTileQueue.size())
	{
		if (eUpdateState::BUILD_PATH == _updateState)
			break;
		UpdatePathfinding();
	}
	*/
	
	//��ε���
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

	//������ ���콺�� ������ �ٽ� IDLEState��
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
		OutputDebugString(L"���� �� Ÿ���� Ŭ��!\n");

		//Ÿ���� Ŭ���Ǹ� ��θ� Ž��
		_pathfinder->UpdatePathfinding();

		//��θ� �����Ͽ� �����δ�.
		_pathfinder->UpdateBuildPath();

		_character->TurnOffMenu();
	}
}