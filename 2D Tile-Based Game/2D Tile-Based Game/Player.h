#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player(LPCWSTR name);
	~Player();

public:
	void UpdateAI(float deltaTime);
	void UpdateMove(float deltaTime);
};