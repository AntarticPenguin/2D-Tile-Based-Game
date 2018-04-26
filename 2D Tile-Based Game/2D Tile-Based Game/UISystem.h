#pragma once
#include <vector>

class UI;
class Character;

class UISystem
{
private:
	static UISystem* _instance;
	Character* _character;

public:
	void Render();

public:
	static UISystem& GetInstance();
	bool CheckUIClick(int mouseX, int mouseY);
	void SetActor(Character* character);

	//UI
private:
	std::vector<UI*> _battleMenuList;
	bool _IsBattle;

public:
	void InitBattleMenu();
	void TurnOnBattleMenu();
	void TurnOffBattleMenu();
	bool IsBattleMenuOn();

	//Constructor & Destroyer
private:
	UISystem();

public:
	~UISystem();
};