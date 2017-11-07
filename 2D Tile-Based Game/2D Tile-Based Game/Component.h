#pragma once
#include <Windows.h>
#include <string>

class Component
{
protected:
	LPCWSTR _name;
	bool _canMove;

	int _tileX;
	int _tileY;

public:
	Component()	{ _canMove = false; }
	Component(LPCWSTR name);
	virtual ~Component();

public:
	virtual void Init() = 0;
	virtual void Deinit() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
	virtual void Reset() = 0;

public:
	virtual void SetPosition(float posX, float posY) {}
	virtual void MoveDeltaPosition(float deltaX, float deltaY) {}

	void SetCanMove(bool canMove) { _canMove = canMove; }
	bool CanMove() { return _canMove; }
	int GetTileX() { return _tileX; }
	int GetTileY() { return _tileY; }

	//Message
public:
	virtual void ReceiveMessage(Component* sender, std::wstring message);
};