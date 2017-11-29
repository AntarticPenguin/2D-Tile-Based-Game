#pragma once
#include "Character.h"

class NPC : public Character
{
public:
	NPC(std::wstring name, std::wstring scriptName, std::wstring spriteFileName);
	~NPC();

public:
	void UpdateAI(float deltaTime);

	//Message
public:
};