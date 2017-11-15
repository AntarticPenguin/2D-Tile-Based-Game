#pragma once
#include "State.h"

class Character;

class MoveState : public State
{
private:
	float _movingDuration;

public:
	MoveState();
	~MoveState();

public:
	void Init(Character* character);
	void Update(float deltaTime);

	void Start();
	void Stop();
};