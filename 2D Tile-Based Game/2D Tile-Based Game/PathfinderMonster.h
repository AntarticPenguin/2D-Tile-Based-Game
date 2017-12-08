#pragma once
#include "Monster.h"

class PathfinderMonster : public Monster
{
public:
	PathfinderMonster(std::wstring name, std::wstring scriptName, std::wstring spriteFileName);
	~PathfinderMonster();

	//AI
public:
	void UpdateAI(float deltaTime);

	//State
public:
	void InitState();
};