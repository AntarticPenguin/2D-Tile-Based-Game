#include <stdio.h>

#include "UISystem.h"
#include "UI.h"

#include "AttackUI.h"
#include "MoveUI.h"
#include "MagicUI.h"
#include "SkillUI.h"

#include "Character.h"
#include "Sprite.h"
#include "Skill.h"

UISystem* UISystem::_instance = NULL;

UISystem::UISystem()
{
	_character = NULL;
	_clickedUI = eUIType::NONE;
	_menuOn = false;
	_activeMenu = &_battleMenu;
}

UISystem::~UISystem()
{
}

void UISystem::Render()
{
	for (size_t i = 0; i < _activeMenu->size(); i++)
	{
		if (_menuOn)
			(*_activeMenu)[i]->Render();
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
	for (size_t i = 0; i < _activeMenu->size(); i++)
	{
		if (true == _menuOn && true == (*_activeMenu)[i]->CheckCollision(mouseX, mouseY))
		{
			(*_activeMenu)[i]->Action(_character);
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
		UI* button = new MoveUI(L"이동", L"menu_MOVE.png", width, height);
		button->SetPosition(_character->GetX() - 32.0f, _character->GetY() - 32.0f);
		_battleMenu.push_back(button);
	}
	{
		UI* button = new UI(L"아이템", L"menu_ITEM.png", width, height);
		button->SetPosition(_character->GetX(), _character->GetY() - 32.0f);
		_battleMenu.push_back(button);
	}
	{
		UI* button = new AttackUI(L"공격", L"menu_ATTACK.png", width, height);
		button->SetPosition(_character->GetX() + 32.0f, _character->GetY() - 32.0f);
		_battleMenu.push_back(button);
	}
	{
		UI* button = new MagicUI(L"마법", L"menu_MAGIC.png", width, height);
		button->SetPosition(_character->GetX() - 32.0f, _character->GetY());
		_battleMenu.push_back(button);
	}
	{
		UI* button = new UI(L"휴식", L"menu_REST.png", width, height);
		button->SetPosition(_character->GetX() - 32.0f, _character->GetY() + 32.0f);
		_battleMenu.push_back(button);
	}
}

void UISystem::InitSkillMenu()
{
	int width = 32;
	int height = 32;

	std::map<std::wstring, Skill*> list = _character->GetSkillList();

	int rowCount = 0;
	int posX = -32;
	int posY = -32;

	auto itr = list.begin();
	for(; itr != list.end(); itr++)
	{
		if (0 == posX && 0 == posY)
		{
			posX += 32.0f;
			rowCount++;
		}

		std::wstring name = itr->first;
		std::wstring fileName = itr->second->GetFileName();

		UI* button = new SkillUI(name, fileName, width, height);

		button->SetPosition(_character->GetX() + posX, _character->GetY() + posY);
		_skillMenu.push_back(button);

		rowCount++;
		posX += 32.0f;
		if (0 == rowCount % 3)
		{
			posX = -32;
			posY += 32;
		}
	}
}

void UISystem::TurnOnMenu()
{
	_menuOn = true;
}

void UISystem::TurnOffMenu()
{
	_menuOn = false;
}

bool UISystem::IsMenuOn()
{
	return _menuOn;
}

void UISystem::SetActiveMenu(eMenuType menuType)
{
	switch (menuType)
	{
	case eMenuType::NONE:
		break;
	case eMenuType::BATTLE:
		_activeMenu = &_battleMenu;
		break;
	case eMenuType::SKILL:
		_activeMenu = &_skillMenu;
		break;
	default:
		break;
	}
}