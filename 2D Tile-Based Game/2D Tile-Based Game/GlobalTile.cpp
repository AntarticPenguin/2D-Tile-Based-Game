#include "GlobalTile.h"

TilePosition GetNextTilePosition(TilePosition curTilePos, eDirection direction)
{
	TilePosition tilePosition = curTilePos;

	switch (direction)
	{
	case eDirection::LEFT:	//left
		tilePosition.x--;
		break;
	case eDirection::RIGHT: //right
		tilePosition.x++;
		break;
	case eDirection::UP: //up
		tilePosition.y--;
		break;
	case eDirection::DOWN: //down
		tilePosition.y++;
		break;
	}
	return tilePosition;
}

eDirection GetDirection(TilePosition to, TilePosition from)
{
	eDirection direction = eDirection::NONE;

	if (to.x < from.x)
		direction = eDirection::LEFT;
	else if (to.x > from.x)
		direction = eDirection::RIGHT;
	else if (to.y < from.y)
		direction = eDirection::UP;
	else if (to.y > from.y)
		direction = eDirection::DOWN;

	return direction;
}