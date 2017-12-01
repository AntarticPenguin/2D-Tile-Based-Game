#pragma once
#include "TileObject.h"

class LifeTileObject : public TileObject
{
public:
	LifeTileObject(int tileX, int tileY, std::wstring name, Sprite* sprite);
	~LifeTileObject();

public:
	void Update(float deltaTime);
};