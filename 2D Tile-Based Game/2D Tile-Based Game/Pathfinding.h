#pragma once
#include <queue>
#include "TileCell.h"

class TileCell;
class Character;

class Pathfinding
{
public:
	Pathfinding(Character* character);
	~Pathfinding();

public:
	void Init();
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

protected:
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

//test
private:
	Character* _character;

public:
	void Reset();
	void ViewMoveRange();
};
