#pragma once
#include "State.h"

class Character;

class CounterAttackState : public State
{
public:
	CounterAttackState();
	~CounterAttackState();

public:
	void Init(Character* character);
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void Start();
	void Stop();
};