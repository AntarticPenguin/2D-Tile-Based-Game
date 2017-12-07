#pragma once
#include "Player.h"

class PathfinderPlayer : public Player
{
public:
	PathfinderPlayer(std::wstring name, std::wstring scriptName, std::wstring spriteFileName);
	~PathfinderPlayer();

	//AI
public:
	void UpdateAI(float deltaTime);

public:
	void InitState();
};
