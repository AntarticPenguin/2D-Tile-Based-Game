#pragma once
#include <Windows.h>
#include <vector>
#include <string>

#include "Component.h"

class Sprite;

class Character : public Component
{
private:
	std::vector<Sprite*> _spriteList;
	std::wstring _spriteFileName;
	std::wstring _scriptFileName;

	float _x;
	float _y;

public:
	Character(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName);
	virtual ~Character();

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
	void SetPosition(float posX, float posY);

	//Move
public:
	enum eDirection
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
		NONE,
	};

protected:
	float _moveTime;
	float _movingDuration;
	bool _isMoving;

	float _targetX;
	float _targetY;

	eDirection _curDirection;

public:
	void InitMove();
	virtual void UpdateMove(float deltaTime);
	void MoveStart(eDirection direction);

	//AI
public:
	virtual void UpdateAI(float deltaTime);
};