#pragma once

class Sprite;

class UISystem
{
private:
	static UISystem* _instance;
	Sprite* _moveButton;

public:
	void Render();

public:
	static UISystem& GetInstance();
	bool CheckUIClick(int mouseX, int mouseY);
	bool CheckCollision(int mouseX, int mouseY);

	//Constructor & Destroyer
private:
	UISystem();

public:
	~UISystem();
};