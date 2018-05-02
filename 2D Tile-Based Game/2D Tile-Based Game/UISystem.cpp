#include <stdio.h>

#include "UISystem.h"
#include "UI.h"

#include "AttackUI.h"
#include "MoveUI.h"
#include "MagicUI.h"

#include "Character.h"
#include "Sprite.h"

UISystem* UISystem::_instance = NULL;

UISystem::UISystem()
{
	_character = NULL;
	_IsBattle = false;
	_clickedUI = eUIType::NONE;
}

UISystem::~UISystem()
{
}

void UISystem::Render()
{
	for (int i = 0; i < _battleMenuList.size(); i++)
	{
		if(_IsBattle)
			_battleMenuList[i]->Render();
	}
		
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
		if (true == _IsBattle && true == _battleMenuList[i]->CheckCollision(mouseX, mouseY))
		{
			_battleMenuList[i]->Action(_character);
			return true;
		}
	}
	return false;
}

eUIType UISystem::GetClickedUI()
{
	return _clickedUI;
}

void UISystem::SetClickedUI(eUIType type)
{
	_clickedUI = type;
}

void UISystem::SetActor(Character* character)
{
	_character = character;
}

void UISystem::InitBattleMenu()
{
	int width = 32;
	int height = 32;
	{
		UI* button = new MoveUI(L"�̵�", L"menu_MOVE.png", width, height);
		button->SetPosition(_character->GetX() - 32.0f, _character->GetY() - 32.0f);
		_battleMenuList.push_back(button);
	}
	{
		UI* button = new UI(L"������", L"menu_ITEM.png", width, height);
		button->SetPosition(_character->GetX(), _character->GetY() - 32.0f);
		_battleMenuList.push_back(button);
	}
	{
		UI* button = new AttackUI(L"����", L"menu_ATTACK.png", width, height);
		button->SetPosition(_character->GetX() + 32.0f, _character->GetY() - 32.0f);
		_battleMenuList.push_back(button);
	}
	{
		UI* button = new MagicUI(L"����", L"menu_MAGIC.png", width, height);
		button->SetPosition(_character->GetX() - 32.0f, _character->GetY());
		_battleMenuList.push_back(button);
	}
	{
		UI* button = new UI(L"�޽�", L"menu_REST.png", width, height);
		button->SetPosition(_character->GetX() - 32.0f, _character->GetY() + 32.0f);
		_battleMenuList.push_back(button);
	}
}

void UISystem::TurnOnBattleMenu()
{
	_IsBattle = true;
}

void UISystem::TurnOffBattleMenu()
{
	_IsBattle = false;
}

bool UISystem::IsBattleMenuOn()
{
	return _IsBattle;
}