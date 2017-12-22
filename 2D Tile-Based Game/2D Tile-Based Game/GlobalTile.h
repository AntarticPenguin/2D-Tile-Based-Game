#pragma once

class Component;

enum eDirection
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE,
};

typedef struct TilePosition
{
	int x;
	int y;
}TilePosition;

TilePosition GetNextTilePosition(TilePosition curTilePos, eDirection direction);
eDirection GetDirection(TilePosition to, TilePosition from);
