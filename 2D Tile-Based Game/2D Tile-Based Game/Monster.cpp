#include "Monster.h"

Monster::Monster(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName) :
	Character(name, scriptName, spriteFileName)
{

}

Monster::~Monster()
{

}

void Monster::UpdateAI(float deltaTime)
{
	if (false == _isMoving)
	{
		/*
			1. 자신을 기준으로, 검사할 타일 범위 결정
			2. 범위 내 타일을 검사하면서 NPC, PLAYER가 있으면 "찾았음"
			3. 없으면 평소 움직임
		*/

		Component* findEnemy = NULL;
		//범위에 적이 있는지 확인
		{
			component->GetType() == eComponentType::CT_NPC or eComponentType::CT_PLAYER
				findEnemy = component;
			break;
		}
		
		//적이 있으면
		if (NULL != findEnemy)
		{
			//추격 방향 설정
			eDirection direction = eDirection::NONE;
			MoveStart(direction);
		}
		else
		{
			//없으면 평소움직임
			Character::UpdateAI(deltaTime);
		}
	}
}