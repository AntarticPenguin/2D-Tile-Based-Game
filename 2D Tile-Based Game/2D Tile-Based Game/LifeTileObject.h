#pragma once
#include "TileObject.h"

class LifeTileObject : public TileObject
{
public:
	LifeTileObject(std::wstring name, Sprite* sprite, int tileX, int tileY);
	~LifeTileObject();

public:
	void Update(float deltaTime);
};