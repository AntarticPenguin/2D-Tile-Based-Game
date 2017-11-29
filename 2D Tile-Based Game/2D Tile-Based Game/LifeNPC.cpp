#include "LifeNPC.h"

LifeNPC::LifeNPC(std::wstring name, std::wstring scriptName, std::wstring spriteFileName) :
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