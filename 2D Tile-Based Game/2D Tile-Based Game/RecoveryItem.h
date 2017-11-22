#pragma once
#include "Component.h"

class Sprite;

class RecoveryItem : public Component
{
private:
	Sprite* _sprite;
	std::wstring _spriteFileName;
	std::wstring _scriptFileName;

	float _posX;
	float _posY;

public:
	RecoveryItem(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName);
	~RecoveryItem();

public:
	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void SetPosition(float posX, float posY);
	void MoveDeltaPosition(float deltaX, float deltaY);
};