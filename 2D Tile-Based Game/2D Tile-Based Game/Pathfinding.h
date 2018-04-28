#pragma once
#include <queue>

class TileCell;
class Character;
class Map;
struct TilePosition;

enum ePathMode
{
	VIEW_RANGE,
	FIND_PATH,
};

enum eFindMethod
{
	DISTANCE,
	SIMPLE,
	COMPLEX,
	ASTAR,
};

class Pathfinding
{
	//pathfinding
public:
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

public:
	Pathfinding(Character* character);
	~Pathfinding();

private:
	Character * _character;
	TileCell* _reverseTileCell;
	Map* _map;
	std::priority_queue<sPathCommand, std::vector<sPathCommand>, compare> _pathfindingTileQueue;

public:
	void Init();
	void FindPath(ePathMode mode, int range, eFindMethod method = eFindMethod::DISTANCE);
	void BuildPath();
	bool CheckPreCondition(ePathMode mode, TilePosition nextTilePos, TileCell* nextTileCell, TileCell* targetTileCell);
	void Reset();

	float CalcHeuristic(eFindMethod eMethod, float distanceFromStart, TileCell* tileCell, TileCell* nextTileCell, TileCell* targetTileCell);
	float CalcSimpleHeuristic(TileCell* tileCell, TileCell* nextTileCell, TileCell*_targetTileCell);
	float CalcComplexHeuristic(TileCell* nextTileCell, TileCell* targetTileCell);
	float CalcAStarHeuristic(float distanceFromStart, TileCell* nextTileCell, TileCell* targetTileCell);
};
