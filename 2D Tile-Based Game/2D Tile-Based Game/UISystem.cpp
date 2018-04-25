#include <stdio.h>
#include "UISystem.h"
#include "UI.h"

#include "Sprite.h"

UISystem* UISystem::_instance = NULL;

UISystem::UISystem()
{
	InitBattleMenu();
}

UISystem::~UISystem()
{
}

void UISystem::Render()
{
	for (int i = 0; i < _battleMenuList.size(); i++)
		_battleMenuList[i]->Render();
}

UISystem& UISystem::GetInstance()
{
	if (NULL == _instance)
		_instance = new UISystem();
	return *_instance;
}

bool UISystem::CheckUIClick(int mouseX, int mouseY)
{
	for (int i = 0; i < _battleMenuList.size(); i++)
	{
		if (true == _battleMenuList[i]->CheckCollision(mouseX, mouseY))
			return true;
	}
	return false;
}

void UISystem::InitBattleMenu()
{
	int width = 32;
	int height = 32;
	{
		UI* button = new UI(L"이동", L"menu_MOVE.png", width, height);
		button->SetPosition(650, 500);
		_battleMenuList.push_back(button);
	}
	{
		UI* button = new UI(L"공격", L"menu_ATTACK.png", width, height);
		button->SetPosition(750, 500);
		_battleMenuList.push_back(button);
	}
}