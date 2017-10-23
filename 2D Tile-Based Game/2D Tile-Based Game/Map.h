#pragma once
#include <Windows.h>
#include <vector>

#include "Component.h"

class Sprite;

class Map : public Component
{
private:
	std::vector<std::vector<Sprite*>> _tileMap;
	int _mapWidth;
	int _mapHeight;

	float _startX;
	float _startY;
	float _deltaX;
	float _deltaY;

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
};