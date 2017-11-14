#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <list>

#include "Component.h"

enum eDirection
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE,
};

class Sprite;
class MoveState;

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

protected:
	/*
	float _movingDuration;
	bool _isMoving;
	*/
	float _moveTime;
	MoveState* _state;

	float _targetX;
	float _targetY;

	eDirection _curDirection;

	int _hp;
	int _attackPoint;

	//AI & MOVE
public:
	void InitMove();
	virtual void UpdateAI(float deltaTime);
	void UpdateMove(float deltaTime);
	//void MoveStart(eDirection direction);
	void MoveStart(int newTileX, int newTileY);
	virtual void Collision(std::list<Component*>& collisionList);

	eDirection GetDirection();

	//Message
public:
	void ReceiveMessage(const sComponentMsgParam& msgParam);
};