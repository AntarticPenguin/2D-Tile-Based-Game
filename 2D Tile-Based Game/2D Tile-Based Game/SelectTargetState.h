#pragma once
#include "State.h"

class Pathfinding;

class SelectTargetState : public State
{
private:
	Pathfinding* _pathfinder;

public:
	SelectTargetState();
	~SelectTargetState();

public:
	void Init(Character* character);
	void Update(float deltaTime);

	void Start();
	void Stop();
};