#pragma once
#include <Windows.h>
#include <string>

enum eComponentType
{
	CT_PLAYER,
	CT_MONSTER,
	CT_NPC,
	CT_TILE_OBJECT,
	CT_NONE,
};

class Component
{
protected:
	LPCWSTR _name;
	bool _canMove;

	int _tileX;
	int _tileY;

	float _moveDistancePerTimeX;
	float _moveDistancePerTimeY;

	eComponentType _eType;

public:
	Component()
	{	_canMove = false;
		_moveDistancePerTimeX = 0;
		_moveDistancePerTimeY = 0;
		_eType = eComponentType::CT_NONE;
	}
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

	float GetMoveDeltaX() { return _moveDistancePerTimeX; }
	float GetMoveDeltaY() { return _moveDistancePerTimeY; }

	eComponentType GetType() { return _eType; }

	//Message
public:
	virtual void ReceiveMessage(Component* sender, std::wstring message);
};