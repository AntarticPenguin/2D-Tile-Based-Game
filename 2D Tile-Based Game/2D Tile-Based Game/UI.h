#pragma once
#include <d3dx9.h>
#include <string>

class Sprite;
class Character;

class UI
{
	//FOR DEBUG
private:
	std::wstring _name;

private:
	Sprite* _sprite;
	int _width;
	int _height;

	float _posX;
	float _posY;

	RECT _collider;

public:
	UI(std::wstring UIName, std::wstring fileName, int width, int height);
	~UI();

public:
	void Render();

public:
	void SetPosition(float posX, float posY);
	bool CheckCollision(int mouseX, int mouseY);
	virtual void Action(Character* character);
};