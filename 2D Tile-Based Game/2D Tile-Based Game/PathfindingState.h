#pragma once
#include <queue>
#include "State.h"

class TileCell;

class PathfindingState : public State
{
public:
	PathfindingState();
	~PathfindingState();

public:
	void Init(Character* character);
	void Update(float deltaTime);

	void Start();
	void Stop();

	//pathfinding
public:
	enum eUpdateState
	{
		PATHFINDING,
		BUILD_PATH,
	};
private:
	std::queue<TileCell*> _pathfindingTileQueue;
	TileCell* _targetTileCell;
	TileCell* _reverseTileCell;
	eUpdateState _updateState;

public:
	void UpdatePathfinding();
	void UpdateBuildPath();
};