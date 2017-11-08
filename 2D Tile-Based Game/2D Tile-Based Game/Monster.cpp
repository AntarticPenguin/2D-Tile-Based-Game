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
			1. �ڽ��� ��������, �˻��� Ÿ�� ���� ����
			2. ���� �� Ÿ���� �˻��ϸ鼭 NPC, PLAYER�� ������ "ã����"
			3. ������ ��� ������
		*/

		Component* findEnemy = NULL;
		//������ ���� �ִ��� Ȯ��
		{
			component->GetType() == eComponentType::CT_NPC or eComponentType::CT_PLAYER
				findEnemy = component;
			break;
		}
		
		//���� ������
		if (NULL != findEnemy)
		{
			//�߰� ���� ����
			eDirection direction = eDirection::NONE;
			MoveStart(direction);
		}
		else
		{
			//������ ��ҿ�����
			Character::UpdateAI(deltaTime);
		}
	}
}