#include "LifePlayer.h"
#include "IdleState.h"

LifePlayer::LifePlayer(std::wstring name, std::wstring scriptName, std::wstring spriteFileName)
	: Player(name, scriptName, spriteFileName)
{

}

LifePlayer::~LifePlayer()
{

}

void LifePlayer::InitState()
{
	Player::InitState();
	ReplaceState(eStateType::ET_ATTACK, new IdleState());
}

