#pragma once
#include <Windows.h>
#include <string>
#include "ComponentMessage.h"

enum class eComponentType
{
	PLAYER,
	MONSTER,
	NPC,
	TILE_OBJECT,
	ITEM,
	ITEM_WEAPON,
	NONE,
};

class Component
{
protected:
	std::wstring _name;
	bool _canMove;

	int _tileX;
	int _tileY;

	eComponentType _eType;

	bool _isLive;

public:
	Component();
	Component(std::wstring name);
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

	eComponentType GetType() { return _eType; }
	bool IsLive(){ return _isLive; }
	void SetLive(bool isLive) { _isLive = isLive; }

	std::wstring GetName() { return _name; }

	//Message
public:
	virtual void ReceiveMessage(const sComponentMsgParam& msgParam);
};