#pragma once
#include <Windows.h>

#include "Component.h"

class Sprite;

class Character : public Component
{
private:
	Sprite* _sprite;
	float _x;
	float _y;

public:
	Character(LPCWSTR name);
	~Character();

public:
	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();
};