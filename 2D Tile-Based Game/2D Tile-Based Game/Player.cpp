#include "Player.h"

#include "ComponentSystem.h"
#include "GameSystem.h"
#include "UISystem.h"

#include "Stage.h"
#include "Skill.h"
#include "Map.h"

#include "AttackState.h"
#include "DefenseState.h"
#include "DeadState.h"
#include "IdleState.h"
#include "PathfindingState.h"
#include "PathfindingMoveState.h"
#include "SelectTargetState.h"

Player::Player(std::wstring name, std::wstring scriptName, std::wstring spriteFileName) :
	Character(name, scriptName, spriteFileName)
{
	_eType = eComponentType::PLAYER;

	_moveTime = 0.0f;
	_moveRange = 10;

	_tileX = 32;
	_tileY = 15;

	//Stat Info
	{
		_attackPoint = 5;
		_attackedPoint = 0;
		_maxHp = 50;
		_hp = _maxHp;

		_attackRange = 4;
	}
}

Player::~Player()
{

}

void Player::InitState()
{
	ReplaceState(eStateType::IDLE, new IdleState());
	ReplaceState(eStateType::MOVE, new PathfindingMoveState());
	ReplaceState(eStateType::PATHFINDING, new PathfindingState());
	
	ReplaceState(eStateType::SELECT_TARGET, new SelectTargetState());
	ReplaceState(eStateType::ATTACK, new AttackState());
	ReplaceState(eStateType::DEFENSE, new DefenseState());
	ReplaceState(eStateType::DEAD, new DeadState());
}

void Player::InitSkill()
{
	{
		Skill* skill = new Skill(L"파이어볼", L"skill_fireball.png");
		_skillList.push_back(skill);
	}
	{
		Skill* skill = new Skill(L"아이스볼트", L"skill_icebolt.png");
		_skillList.push_back(skill);
	}
	{
		Skill* skill = new Skill(L"썬더", L"skill_thunder.png");
		_skillList.push_back(skill);
	}
	{
		Skill* skill = new Skill(L"윈드스톰", L"skill_windstorm.png");
		_skillList.push_back(skill);
	}
	{
		Skill* skill = new Skill(L"수면", L"skill_sleep.png");
		_skillList.push_back(skill);
	}
	{
		Skill* skill = new Skill(L"체인라이트닝", L"skill_chainlightning.png");
		_skillList.push_back(skill);
	}
	{
		Skill* skill = new Skill(L"체인라이트닝2", L"skill_chainlightning.png");
		_skillList.push_back(skill);
	}
}

void Player::UpdateCharacter()
{
	// 마우스 키처리
	if (GameSystem::GetInstance().IsMouseDown())
	{
		int mouseX = GameSystem::GetInstance().GetMouseX();
		int mouseY = GameSystem::GetInstance().GetMouseY();

		//Check UI
		if (true == UISystem::GetInstance().CheckUIClick(mouseX, mouseY))
			return;

		TileCell* targetTileCell = GameSystem::GetInstance().GetStage()->GetMap()
			->FindTileCellWithMousePosition(mouseX, mouseY);

		if (NULL == targetTileCell)
			return;

		if (eStage::TOWN == GameSystem::GetInstance().GetStage()->GetStageInfo())
		{
			SetTargetTileCell(targetTileCell);
			ChangeState(eStateType::PATHFINDING);
		}
		else if (eStage::DUNGEON == GameSystem::GetInstance().GetStage()->GetStageInfo())
		{
			if (false == UISystem::GetInstance().IsMenuOn() && IsClickCharacter(targetTileCell))
			{
				UISystem::GetInstance().SetActiveMenu(eMenuType::BATTLE);
				UISystem::GetInstance().TurnOnMenu();
			}
			else
				SetTargetTileCell(targetTileCell);
		}
	}
}

Component* Player::Collision(std::list<Component*>& collisionList)
{
	for (std::list<Component*>::iterator itr = collisionList.begin(); itr != collisionList.end(); itr++)
	{
		Component* com = (*itr);
		if (eComponentType::MONSTER == com->GetType())
		{
			return com;
		}
	}
	return NULL;
}