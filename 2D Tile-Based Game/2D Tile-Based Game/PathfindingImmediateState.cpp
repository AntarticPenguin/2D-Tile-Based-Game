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

	//경로탐색
	while (0 != _pathfindingTileQueue.size())
	{
		if (eUpdateState::BUILD_PATH == _updateState)
			break;
		UpdatePathfinding();
	}
	
	//경로도출
	while (NULL != _reverseTileCell)
	{
		UpdateBuildPath();
	}

}