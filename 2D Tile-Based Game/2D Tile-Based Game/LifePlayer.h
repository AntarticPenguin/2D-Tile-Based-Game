#pragma once
#include "Player.h"

class LifePlayer : public Player
{
public:
	LifePlayer(std::wstring name, std::wstring scriptName, std::wstring spriteFileName);
	~LifePlayer();

public:
	void InitState();
};