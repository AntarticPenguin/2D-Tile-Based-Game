#pragma once
#include <Windows.h>


#define MAP_HEIGHT 100
#define MAP_WIDTH 100

class Sprite;

class Map
{
private:
	Sprite* _testTileMap[MAP_HEIGHT][MAP_WIDTH];
	float _startX;
	float _startY;
	float _deltaX;
	float _deltaY;

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