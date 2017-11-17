#pragma once
#include "State.h"

class Character;

class DefenseState : public State
{
public:
	DefenseState();
	~DefenseState();

public:
	void Init(Character* character);
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void Start();
	void Stop();
};