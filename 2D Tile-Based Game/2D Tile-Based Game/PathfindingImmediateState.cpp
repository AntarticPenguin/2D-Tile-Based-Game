#include "PathfindingImmediateState.h"

PathfindingImmediateState::PathfindingImmediateState()
{

}

PathfindingImmediateState::~PathfindingImmediateState()
{

}

void PathfindingImmediateState::Start()
{
	PathfindingState::Start();

	//���Ž��
	while (0 != _pathfindingTileQueue.size())
	{
		if (eUpdateState::BUILD_PATH == _updateState)
			break;
		UpdatePathfinding();
	}
	
	//��ε���
	while (NULL != _reverseTileCell)
	{
		UpdateBuildPath();
	}

}