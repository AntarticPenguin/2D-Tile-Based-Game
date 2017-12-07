#include "PathfinderPlayer.h"
#include "IdleState.h"

PathfinderPlayer::PathfinderPlayer(std::wstring name, std::wstring scriptName, std::wstring spriteFileName) :
	Player(name, scriptName, spriteFileName)
{
}

PathfinderPlayer::~PathfinderPlayer()
{
}

void PathfinderPlayer::UpdateAI(float deltaTime)
{

}

void PathfinderPlayer::InitState()
{
	Player::InitState();
	ReplaceState(eStateType::ET_ATTACK, new IdleState());
}