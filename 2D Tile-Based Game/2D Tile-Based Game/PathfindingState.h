#pragma once
#include <queue>
#include "State.h"
#include "TileCell.h"

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

	struct compare
	{
		bool operator()(TileCell* a, TileCell* b)
		{
			return a->GetDistanceFromStart() > b->GetDistanceFromStart();
		}
	};

private:
	//std::queue<TileCell*> _pathfindingTileQueue;
	std::priority_queue<TileCell*, std::vector<TileCell*>, compare> _pathfindingTileQueue;
	TileCell* _targetTileCell;
	TileCell* _reverseTileCell;
	eUpdateState _updateState;

public:
	void UpdatePathfinding();
	void UpdateBuildPath();
};