#pragma once
#include "Character.h"

class Monster : public Character
{
public:
	Monster(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName);
	~Monster();

	//AI
public:
	void UpdateAI(float deltaTime);
	void Collision(std::list<Component*>& collisionList);
};