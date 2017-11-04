#pragma once
#include "Character.h"

class NPC : public Character
{
public:
	NPC(LPCWSTR name, LPCWSTR spriteFileName);
	~NPC();
};