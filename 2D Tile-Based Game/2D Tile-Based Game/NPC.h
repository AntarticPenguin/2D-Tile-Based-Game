#pragma once
#include "Character.h"

class NPC : public Character
{
public:
	NPC(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName);
	~NPC();

public:
	void UpdateAI(float deltaTime);

	//Message
public:
};