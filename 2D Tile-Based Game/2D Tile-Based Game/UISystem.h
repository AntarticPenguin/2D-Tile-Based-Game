#pragma once
#include <vector>

class UI;

class UISystem
{
private:
	static UISystem* _instance;
	std::vector<UI*> _battleMenuList;

public:
	void Render();

public:
	static UISystem& GetInstance();
	bool CheckUIClick(int mouseX, int mouseY);

	//Init UI
public:
	void InitBattleMenu();

	//Constructor & Destroyer
private:
	UISystem();

public:
	~UISystem();
};