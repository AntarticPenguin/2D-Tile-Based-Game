#pragma once
#include "State.h"

class Character;

class RecoveryState : public State
{
public:
	RecoveryState();
	~RecoveryState();

public:
	void Init(Character* character);
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void Start();
	void Stop();
};