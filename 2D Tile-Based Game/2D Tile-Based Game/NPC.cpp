#include "NPC.h"
#include "ComponentSystem.h"
#include "Map.h"

NPC::NPC(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName) :
	Character(name, scriptName, spriteFileName)
{
	_eType = CT_NPC;
	
	int speed = (rand() % 1500) + 300;
	_moveTime = (float)speed / 1000.0f;
}

NPC::~NPC()
{

}

void NPC::UpdateAI(float deltaTime)
{
	if (false == _isMoving)
	{
		/*
		1. �ڽ��� ��������, �˻��� Ÿ�� ���� ����
		2. ���� �� Ÿ���� �˻��ϸ鼭 NPC, PLAYER�� ������ "ã����"
		3. ������ ��� ������
		*/
		Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");
		int range = 2;
		int minTileX = _tileX - range;
		int maxTileX = _tileX + range;
		int minTileY = _tileY - range;
		int maxTileY = _tileY + range;

		if (minTileX < 0)
			minTileX = 0;
		if (map->GetWidth() <= maxTileX)
			maxTileX = map->GetWidth() - 1;
		if (minTileY < 0)
			minTileY = 0;
		if (map->GetHeight() <= maxTileY)
			maxTileY = map->GetHeight() - 1;

		Component* findEnemy = NULL;
		//������ ���� �ִ��� Ȯ��
		for (int y = minTileY; y <= maxTileY; y++)
		{
			for (int x = minTileX; x <= maxTileX; x++)
			{
				std::list<Component*> componentList;
				if (false == map->GetTileCollisionList(x, y, componentList))
				{
					for (std::list<Component*>::iterator itr = componentList.begin(); itr != componentList.end(); itr++)
					{
						Component* component = (*itr);
						if (component->GetType() == eComponentType::CT_MONSTER)
						{
							findEnemy = component;
							break;
						}
					}
				}
			}
		}

		//���� ������
		if (NULL != findEnemy)
		{
			//�߰� ���� ����
			eDirection direction = eDirection::NONE;

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
				direction = (eDirection)findDirection;
				MoveStart(direction);
			}
		}
		else
		{
			//������ ��ҿ�����
			Character::UpdateAI(deltaTime);
		}
	}
}