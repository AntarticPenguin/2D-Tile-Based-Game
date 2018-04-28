#pragma once
#include "State.h"

class TileCell;

class SelectTargetState : public State
{
private:
	TileCell* _mouseOverCell;

public:
	SelectTargetState();
	~SelectTargetState();

public:
	void Init(Character* character);
	void Update(float deltaTime);

	void Start();
	void Stop();
};