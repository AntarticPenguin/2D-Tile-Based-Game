#include "PathfindingState.h"

PathfindingState::PathfindingState()
{

}

PathfindingState::~PathfindingState()
{

}

void PathfindingState::Start()
{
	Pathfinding::Start();

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