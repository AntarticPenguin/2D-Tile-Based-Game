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