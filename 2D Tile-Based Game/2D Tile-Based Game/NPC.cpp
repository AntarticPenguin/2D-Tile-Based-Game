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
		1. 자신을 기준으로, 검사할 타일 범위 결정
		2. 범위 내 타일을 검사하면서 NPC, PLAYER가 있으면 "찾았음"
		3. 없으면 평소 움직임
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
		//범위에 적이 있는지 확인
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

		//적이 있으면
		if (NULL != findEnemy)
		{
			//추격 방향 설정
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
			//없으면 평소움직임
			Character::UpdateAI(deltaTime);
		}
	}
}