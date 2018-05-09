#pragma once
#include <vector>

enum class eUIType
{
	NONE,
	MOVE,
	ATTACK,
	MAGIC,
};

enum class eMenuType
{
	NONE,
	BATTLE,
	SKILL,
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

	//Menu
private:
	std::vector<UI*> _battleMenu;
	std::vector<UI*> _skillMenu;
	bool _IsBattle;

	//test
	std::vector<UI*>* _activeMenu;
	bool _menuOn;

public:
	void InitBattleMenu();
	void InitSkillMenu();
	void TurnOnMenu();
	void TurnOffMenu();
	bool IsMenuOn();

	void SetActiveMenu(eMenuType menuType);

	//Constructor & Destroyer
private:
	UISystem();

public:
	~UISystem();
};