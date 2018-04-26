#include "MoveUI.h"
#include "Character.h"
#include "UISystem.h"

MoveUI::MoveUI(std::wstring UIName, std::wstring fileName, int width, int height) :
	UI(UIName, fileName, width, height)
{

}

MoveUI::~MoveUI()
{
}

void MoveUI::Action(Character* character)
{
	if (10 == character->GetBehaviorPoint())
	{
		UISystem::GetInstance().TurnOffBattleMenu();
		character->ChangeState(eStateType::ET_PATHFINDING);
	}
		
}