#pragma once
#include <Windows.h>
#include <vector>

#include "Component.h"

class TileCell;
class Sprite;

class Map : public Component
{
private:
	std::vector<std::vector<TileCell*>> _tileMap;
	int _mapWidth;
	int _mapHeight;

	float _startX;
	float _startY;
	float _deltaX;
	float _deltaY;

	int _tileSize;

	std::vector<Sprite*> _spriteList;

public:
	Map(LPCWSTR name);
	~Map();

public:
	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();
	void Scroll(float deltaX, float deltaY);

	int GetPositionX(int tileX, int tileY);
	int GetPositionY(int tileX, int tileY);

	void SetTileComponent(int tileX, int tileY, Component* component);
	void ResetTileComponent(int tileX, int tileY, Component* component);
};