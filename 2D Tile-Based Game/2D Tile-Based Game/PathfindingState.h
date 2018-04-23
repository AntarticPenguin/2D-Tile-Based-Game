#pragma once
#include "State.h"

class Pathfinding;

class PathfindingState : public State
{
private:
	Pathfinding* _pathfinder;

public:
	PathfindingState();
	~PathfindingState();

public:
	void Init(Character* character);
	void Update(float deltaTime);
	void Start();
};

