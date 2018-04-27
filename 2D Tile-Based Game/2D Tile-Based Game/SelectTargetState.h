#pragma once
#include "State.h"

class SelectTargetState : public State
{
public:
	SelectTargetState();
	~SelectTargetState();

public:
	void Init(Character* character);
	void Update(float deltaTime);

	void Start();
	void Stop();
};