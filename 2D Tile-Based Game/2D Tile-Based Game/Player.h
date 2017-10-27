#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player(LPCWSTR name);
	~Player();
};