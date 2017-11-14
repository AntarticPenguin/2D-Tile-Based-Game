#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Map.h"
#include "Player.h"

#include "MoveState.h"

Player::Player(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName) :
	Character(name, scriptName, spriteFileName)
{
	_moveTime = 0.2f;
	_tileX = 32;
	_tileY = 15;
	_eType = eComponentType::CT_PLAYER;
}

Player::~Player()
{

}

void Player::UpdateAI(float deltaTime)
{
	if (false == _isLive)
		return;

	if (false == _state->IsMoving())
	{
		eDirection direction = eDirection::NONE;
		if (GameSystem::GetInstance().IsKeyDown(VK_UP))
		{
			//MoveStart(eDirection::UP);
			direction = eDirection::UP;
		}
		if (GameSystem::GetInstance().IsKeyDown(VK_DOWN))
		{
			//MoveStart(eDirection::DOWN);
			direction = eDirection::DOWN;
		}
		if (GameSystem::GetInstance().IsKeyDown(VK_LEFT))
		{
			//MoveStart(eDirection::LEFT);
			direction = eDirection::LEFT;
		}
		if (GameSystem::GetInstance().IsKeyDown(VK_RIGHT))
		{
			//MoveStart(eDirection::RIGHT);
			direction = eDirection::RIGHT;
		}

		if (eDirection::NONE != direction)
		{
			_curDirection = direction;
			//MoveStart();
			_state->Start();
		}
	}
}