#pragma once
#include "Component.h"

class Sprite;

class Sword : public Component
{
private:
	Sprite* _sprite;
	std::wstring _spriteFileName;
	std::wstring _scriptFileName;

	float _posX;
	float _posY;

	int _attackPoint;

public:
	Sword(std::wstring name, std::wstring scriptName, std::wstring spriteFileName);
	~Sword();

public:
	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void SetPosition(float posX, float posY);
	void MoveDeltaPosition(float deltaX, float deltaY);

	int GetEquipStat();

	//Message
public:
	void ReceiveMessage(const sComponentMsgParam& msgParam);
};