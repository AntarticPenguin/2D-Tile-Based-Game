#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Map.h"
#include "Player.h"

Player::Player(LPCWSTR name, LPCWSTR spriteFileName) :
	Character(name, spriteFileName)
{
	_moveTime = 0.2f;
	_tileX = 1;
	_tileY = 1;
}

Player::~Player()
{

}

void Player::UpdateAI(float deltaTime)
{
	if (false == _isMoving)
	{
		if (GameSystem::GetInstance().IsKeyDown(VK_UP))
		{
			MoveStart(eDirection::UP);
		}
		if (GameSystem::GetInstance().IsKeyDown(VK_DOWN))
		{
			MoveStart(eDirection::DOWN);
		}
		if (GameSystem::GetInstance().IsKeyDown(VK_LEFT))
		{
			MoveStart(eDirection::LEFT);
		}
		if (GameSystem::GetInstance().IsKeyDown(VK_RIGHT))
		{
			MoveStart(eDirection::RIGHT);
		}
	}
}

void Player::UpdateMove(float deltaTime)
{
	if (false == _isMoving)
		return;

	if (_moveTime <= _movingDuration)
	{
		_movingDuration = 0.0f;
		_isMoving = false;

		//이동후 도착하면 타일의 정확한 위치에 찍어줘야 한다.
		Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");
		map->Scroll(0.0f, 0.0f);
	}
	else
	{
		_movingDuration += deltaTime;

		float moveDistanceX = _moveDistancePerTimeX * deltaTime;
		float moveDistanceY = _moveDistancePerTimeY * deltaTime;
		Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");
		map->Scroll(-moveDistanceX, -moveDistanceY);
	}
}

void Player::MoveDeltaPosition(float deltaX, float deltaY)
{

}