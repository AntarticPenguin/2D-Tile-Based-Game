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
	UISystem::GetInstance().TurnOffBattleMenu();
	character->ChangeState(eStateType::ET_SELECT_TARGET);
}