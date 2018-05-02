#pragma once
#include <vector>

enum class eUIType
{
	NONE,
	MOVE,
	ATTACK,
	MAGIC,
};

class UI;
class Character;

class UISystem
{
private:
	static UISystem* _instance;
	Character* _character;
	eUIType	_clickedUI;

public:
	void Render();

public:
	static UISystem& GetInstance();
	bool CheckUIClick(int mouseX, int mouseY);
	eUIType GetClickedUI();
	void SetClickedUI(eUIType type);
	void SetActor(Character* character);

	//Battle UI
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