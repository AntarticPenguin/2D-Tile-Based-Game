#include "LifeNPC.h"

LifeNPC::LifeNPC(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName) :
	Character(name, scriptName, spriteFileName)
{
	_eType = eComponentType::CT_NPC;
}

LifeNPC::~LifeNPC()
{

}

void LifeNPC::UpdateAI(float deltaTime)
{

}