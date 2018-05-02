#pragma once
#include <queue>
#include <vector>

class TileCell;
class Character;
class Map;
struct TilePosition;

enum class ePathMode
{
	VIEW_MOVE_RANGE,
	VIEW_ATTACK_RANGE,
	FIND_PATH,
};

enum class eFindMethod
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
	Pathfinding();
	Pathfinding(Character* character);
	~Pathfinding();

private:
	Character * _character;
	TileCell* _reverseTileCell;
	Map* _map;
	int _range;
	std::vector<TileCell*> _colorTileList;
	std::priority_queue<sPathCommand, std::vector<sPathCommand>, compare> _pathfindingTileQueue;

	//test
	TileCell* _startTileCell;

public:
	void Init();
	void Init(Character* character);
	void Init(TileCell* tileCell);
	void SetRange(int range);
	void FindPath(ePathMode mode, eFindMethod method = eFindMethod::DISTANCE);
	void BuildPath();
	bool CheckPreCondition(ePathMode mode, TilePosition nextTilePos, TileCell* nextTileCell, TileCell* targetTileCell);
	void Reset();

	float CalcHeuristic(eFindMethod eMethod, float distanceFromStart, TileCell* tileCell, TileCell* nextTileCell, TileCell* targetTileCell);
	float CalcSimpleHeuristic(TileCell* tileCell, TileCell* nextTileCell, TileCell*_targetTileCell);
	float CalcComplexHeuristic(TileCell* nextTileCell, TileCell* targetTileCell);
	float CalcAStarHeuristic(float distanceFromStart, TileCell* nextTileCell, TileCell* targetTileCell);

private:
	TileCell* _prevOverCell;
	TileCell* _mouseOverCell;

public:
	bool CheckRange(TileCell* targetTileCell);
	void ClearColorTile();
	void ColorMouseOverCell();
};
