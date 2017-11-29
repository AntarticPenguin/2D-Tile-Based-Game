#pragma once
#include "Character.h"

class Monster : public Character
{
public:
	Monster(std::wstring name, std::wstring scriptName, std::wstring spriteFileName);
	~Monster();

	//AI
public:
	void UpdateAI(float deltaTime);
	Component* Collision(std::list<Component*>& collisionList);
};