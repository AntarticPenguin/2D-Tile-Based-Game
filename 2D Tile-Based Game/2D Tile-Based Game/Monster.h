#pragma once
#include "Character.h"

class Monster : public Character
{
public:
	Monster(LPCWSTR name, LPCWSTR spriteFileName);
	~Monster();
};