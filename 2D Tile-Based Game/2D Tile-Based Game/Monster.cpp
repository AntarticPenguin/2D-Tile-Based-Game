#include "Monster.h"
#include "ComponentSystem.h"
#include "Map.h"

Monster::Monster(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName) :
	Character(name, scriptName, spriteFileName)
{
	_eType = eComponentType::CT_MONSTER;

	int speed = (rand() % 100) + 200;
	_moveTime = (float)speed / 1000.0f;
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
		Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");
		int range = 6;
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
						if (component->GetType() == eComponentType::CT_NPC ||
							component->GetType() == eComponentType::CT_PLAYER)
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

			if (findEnemy->GetTileX() < _tileX)
				direction = eDirection::LEFT;
			else if(_tileX < findEnemy->GetTileX())
				direction = eDirection::RIGHT;
			else if (findEnemy->GetTileY() < _tileY)
				direction = eDirection::UP;
			else if (_tileY < findEnemy->GetTileY())
				direction = eDirection::DOWN;

			MoveStart(direction);
		}
		else
		{
			//없으면 평소움직임
			Character::UpdateAI(deltaTime);
		}
	}
}