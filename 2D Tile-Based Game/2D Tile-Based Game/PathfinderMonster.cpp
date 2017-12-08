#include "PathfinderMonster.h"
#include "IdleState.h"

PathfinderMonster::PathfinderMonster(std::wstring name, std::wstring scriptName, std::wstring spriteFileName) :
	Monster(name, scriptName, spriteFileName)
{
}

PathfinderMonster::~PathfinderMonster()
{
}

void PathfinderMonster::UpdateAI(float deltaTime)
{

}

void PathfinderMonster::InitState()
{
	Monster::InitState();
	ReplaceState(eStateType::ET_ATTACK, new IdleState());
}