#include "MagicUI.h"

#include "UISystem.h"
#include "Character.h"

MagicUI::MagicUI(std::wstring UIName, std::wstring fileName, int width, int height) :
	UI(UIName, fileName, width, height)
{
}

MagicUI::~MagicUI()
{
}

void MagicUI::Action(Character* character)
{
	/*if (character->CanBattle())
	{
		UISystem::GetInstance().SetClickedUI(eUIType::MAGIC);
		UISystem::GetInstance().TurnOffMenu();
		character->ChangeState(eStateType::ET_SELECT_TARGET);
	}*/
	UISystem::GetInstance().SetActiveMenu(eMenuType::SKILL);
}