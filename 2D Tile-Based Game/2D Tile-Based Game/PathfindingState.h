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

	typedef struct _sPathCommand
	{
		float heuristic;
		TileCell* tileCell;
	}sPathCommand;

	struct compare
	{
		bool operator()(sPathCommand& a, sPathCommand& b)
		{
			return a.heuristic > b.heuristic;
		}
	};

private:
	//std::priority_queue<TileCell*, std::vector<TileCell*>, compare> _pathfindingTileQueue;
	std::priority_queue<sPathCommand, std::vector<sPathCommand>, compare> _pathfindingTileQueue;
	TileCell* _targetTileCell;
	TileCell* _reverseTileCell;
	eUpdateState _updateState;

public:
	void UpdatePathfinding();
	void UpdateBuildPath();
	float CalcSimpleHeuristic(TileCell* tileCell, TileCell* nextTileCell, TileCell*_targetTileCell);
	float CalcComplexHeuristic(TileCell* nextTileCell, TileCell* targetTileCell);
	float CalcAStarHeuristic(float distanceFromStart, TileCell* nextTileCell, TileCell* targetTileCell);
};