#pragma once
#include <stack>
#include "State.h"
#include "GlobalTile.h"

class Character;
class TileCell;

class PathfindingMoveState : public State
{
private:
	float _movingDuration;
	std::stack<TileCell*> _pathTileCellStack;

public:
	PathfindingMoveState();
	~PathfindingMoveState();

public:
	void Init(Character* character);
	void Update(float deltaTime);

	void Start();
	void Stop();
};