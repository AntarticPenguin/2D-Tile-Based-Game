#pragma once
#include "Component.h"

class Sprite;

class TileObject : public Component
{
private:
	Sprite* _sprite;

	float _posX;
	float _posY;

public:
	TileObject(std::wstring name, Sprite* sprite, int tileX, int tileY);
	virtual ~TileObject();

public:
	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void SetPosition(float posX, float posY);
	void MoveDeltaPosition(float deltaX, float deltaY);

	//distance Weight
private:
	float _distanceWeight;

public:
	float GetDistanceWeight();
	void SetDistanceWeight(float distanceWeight);
};
