#include "NPC.h"
#include "ComponentSystem.h"
#include "Map.h"

NPC::NPC(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName) :
	Character(name, scriptName, spriteFileName)
{

}

NPC::~NPC()
{

}