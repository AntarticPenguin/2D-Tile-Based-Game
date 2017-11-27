#pragma once
#include "Character.h"

class LifeNPC : public Character
{
public:
	LifeNPC(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName);
	~LifeNPC();

public:
	void UpdateAI(float deltaTime);
};