#include "SkillUI.h"
#include "UISystem.h"

SkillUI::SkillUI(std::wstring UIName, std::wstring fileName, int width, int height) :
	UI(UIName, fileName, width, height)
{

}

SkillUI::~SkillUI()
{

}

void SkillUI::Action(Character* character)
{
	UISystem::GetInstance().TurnOffMenu();
}