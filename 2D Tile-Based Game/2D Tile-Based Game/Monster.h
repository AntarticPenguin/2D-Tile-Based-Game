#pragma once
#include "Character.h"

class Monster : public Character
{
public:
	Monster(std::wstring name, std::wstring scriptName, std::wstring spriteFileName);
	~Monster();

public:
	void InitState();
	void UpdateCharacter();
};