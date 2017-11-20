#include "NPC.h"
#include "ComponentSystem.h"
#include "Map.h"

#include "MoveState.h"

NPC::NPC(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName) :
	Character(name, scriptName, spriteFileName)
{
	_eType = eComponentType::CT_NPC;
	
	int speed = (rand() % 1500) + 300;
	_moveTime = (float)speed / 1000.0f;
}

NPC::~NPC()
{

}

void NPC::UpdateAI(float deltaTime)
{
	Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");
	std::vector<eComponentType> compareTypeList;
	compareTypeList.push_back(eComponentType::CT_MONSTER);
	Component* findEnemy = ComponentSystem::GetInstance().FindComponentInRange(this, 2, compareTypeList);

	//���� ������
	if (NULL != findEnemy)
	{
		//�߰� ���� ����
		int findDirection = rand() % 4;
		int newTileX = _tileX;
		int newTileY = _tileY;

		switch (findDirection)
		{
		case eDirection::LEFT:
			newTileX--;
			break;
		case eDirection::RIGHT:
			newTileX++;
			break;
		case eDirection::UP:
			newTileY--;
			break;
		case eDirection::DOWN:
			newTileY++;
			break;
		}

		if (map->CanMoveTileMap(newTileX, newTileY))
		{
			if (eDirection::NONE != findDirection)
			{
				_curDirection = (eDirection)findDirection;
				_state->NextState(eStateType::ET_MOVE);
			}
		}
	}
	else
	{
		//������ ��ҿ�����
		Character::UpdateAI(deltaTime);
	}
}