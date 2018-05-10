#include "AttackUI.h"

#include "UISystem.h"
#include "Character.h"

AttackUI::AttackUI(std::wstring UIName, std::wstring fileName, int width, int height) :
	UI(UIName, fileName, width, height)
{

}

AttackUI::~AttackUI()
{

}

void AttackUI::Action(Character* character)
{
	if (character->CanBattle())
	{
		UISystem::GetInstance().SetClickedUI(eUIType::ATTACK);
		UISystem::GetInstance().TurnOffMenu();
		character->ChangeState(eStateType::SELECT_TARGET);
	}
}