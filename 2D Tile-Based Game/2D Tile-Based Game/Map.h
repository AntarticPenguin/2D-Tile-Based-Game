#pragma once
#include <Windows.h>
#include <vector>


//#define MAP_HEIGHT 16
//#define MAP_WIDTH 16

class Sprite;

class Map
{
private:
	//Sprite* _testTileMap[MAP_HEIGHT][MAP_WIDTH];
	std::vector<std::vector<Sprite*>> _tileMap;
	int _mapWidth;
	int _mapHeight;

	float _startX;
	float _startY;
	float _deltaX;
	float _deltaY;

	std::vector<Sprite*> _spriteList;

public:
	Map(LPCWSTR fileName);
	~Map();

public:
	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();
	void Scroll(float deltaX, float deltaY);
};