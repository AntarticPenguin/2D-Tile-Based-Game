#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player(std::wstring name, std::wstring scriptName, std::wstring spriteFileName);
	~Player();

public:
	void UpdateAI(float deltaTime);
	Component* Collision(std::list<Component*>& collisionList);
};