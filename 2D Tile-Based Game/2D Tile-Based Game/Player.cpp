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
		_damage = 0;
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
		Skill* skill = new Skill(L"���̾", L"skill_fireball.png", 3, 1, 3, 7);
		AddSkill(L"���̾", skill);
	}
	{
		Skill* skill = new Skill(L"���̽���Ʈ", L"skill_icebolt.png", 3, 1, 3, 7);
		AddSkill(L"���̽���Ʈ", skill);
	}
	{
		Skill* skill = new Skill(L"���", L"skill_thunder.png", 5, 1, 3, 8);
		AddSkill(L"���", skill);
	}
	{
		Skill* skill = new Skill(L"���彺��", L"skill_windstorm.png", 5, 2, 4, 8);
		AddSkill(L"���彺��", skill);
	}
	{
		Skill* skill = new Skill(L"����", L"skill_sleep.png", 4, 3, 4, 1);
		AddSkill(L"����", skill);
	}
	{
		Skill* skill = new Skill(L"ü�ζ���Ʈ��", L"skill_chainlightning.png", 6, 5, 5, 10);
		AddSkill(L"ü�ζ���Ʈ��", skill);
	}
	{
		Skill* skill = new Skill(L"ü�ζ���Ʈ��2", L"skill_chainlightning.png", 6, 5, 5, 10);
		AddSkill(L"ü�ζ���Ʈ��2", skill);
	}
}

void Player::UpdateCharacter()
{
	// ���콺 Űó��
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