#pragma once
#include <Windows.h>
#include <vector>

#include "Component.h"

class Sprite;

class Character : public Component
{
private:
	//Sprite* _sprite;
	std::vector<Sprite*> _spriteList;

	float _x;
	float _y;

	int _tileX;
	int _tileY;

public:
	Character(LPCWSTR name);
	~Character();

public:
	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	//Transform
public:
	void MoveDeltaPosition(float deltaX, float deltaY);

	//Move
public:
	enum eDirection
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
	};

protected:
	float _moveTime;
	float _movingDuration;
	bool _isMoving;

	float _targetX;
	float _targetY;

	float _moveDistancePerTimeX;
	float _moveDistancePerTimeY;

	eDirection _curDirection;

public:
	void InitMove();
	virtual void UpdateMove(float deltaTime);
	void MoveStart(eDirection direction);

	//AI
public:
	virtual void UpdateAI(float deltaTime);
};