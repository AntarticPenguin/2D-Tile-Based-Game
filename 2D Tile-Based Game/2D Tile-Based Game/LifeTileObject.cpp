#include "LifeTileObject.h"
#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Stage.h"
#include "Map.h"
#include "Player.h"

LifeTileObject::LifeTileObject(int tileX, int tileY, std::wstring name, Sprite* sprite) :
	TileObject(name, sprite)
{
	_tileX = tileX;
	_tileY = tileY;
}

LifeTileObject::~LifeTileObject()
{
	
}

void LifeTileObject::Update(float deltaTime)
{
	TileObject::Update(deltaTime);

	//주변 8칸 검사 - 캐릭터가 몇마리 있는가?(자기 칸을 제외한)
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();
	int range = 1;
	
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

	int character_count = 0;
	Component* tileCharacter = NULL;

	//범위에 캐릭터가 있으면 Count추가
	for (int y = minTileY; y <= maxTileY; y++)
	{
		for (int x = minTileX; x <= maxTileX; x++)
		{
			/*if (x != _tileX || y != _tileY)
			{
				std::list<Component*> componentList;
				if (false == map->GetTileCollisionList(x, y, componentList))
				{
					for (std::list<Component*>::iterator itr = componentList.begin(); itr != componentList.end(); itr++)
					{
						Component* component = (*itr);
						if (component->IsLive())
						{
							switch (component->GetType())
							{
							case eComponentType::CT_NPC:
							case eComponentType::CT_PLAYER:
								character_count++;
								break;
							}
						}
					}
				}
			}
			else
			{
				std::list<Component*> componentList;
				if (false == map->GetTileCollisionList(x, y, componentList))
				{
					for (std::list<Component*>::iterator itr = componentList.begin(); itr != componentList.end(); itr++)
					{
						Component* component = (*itr);
						if (component->IsLive())
						{
							switch (component->GetType())
							{
							case eComponentType::CT_NPC:
							case eComponentType::CT_PLAYER:
								tileCharacter = component;
								break;
							}
						}
					}
				}
			}*/
			CheckLife(x, y, &character_count, tileCharacter);
		}
	}

	/*
		1. 주변에 살아있는 세포가 2개보다 적으면 세포 사망
		2. 주변에 2, 3개의 세포가 있다면 다음세대에 삼
		3. 정확히 3개가 있다면 계속 살거나 죽어있는 세포가 부활
		4. 3개 초과이면 죽음
	*/

	switch (character_count)
	{
	case 2:		//skip
		break;
	case 3:
		if (NULL == tileCharacter)
		{
			GameSystem::GetInstance().GetStage()->CreateLifeNPC(this);
		}
		break;
	default:	//dead
		if (NULL != tileCharacter)
		{
			if (eComponentType::CT_PLAYER != tileCharacter->GetType())
			{
				//GameSystem::GetInstance().GetStage()->DestroyLifeNPC(_tileX, _tileY, tileCharacter);
				GameSystem::GetInstance().GetStage()->CheckDestroyLifeNPC(tileCharacter);
				tileCharacter = NULL;
			}
		}
		break;
	}
}

void LifeTileObject::CheckLife(int x, int y, int* character_count, Component* tileCharacter)
{
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();

	std::list<Component*> componentList;
	if (false == map->GetTileCollisionList(x, y, componentList))
	{
		for (std::list<Component*>::iterator itr = componentList.begin(); itr != componentList.end(); itr++)
		{
			Component* component = (*itr);
			if (component->IsLive())
			{
				switch (component->GetType())
				{
				case eComponentType::CT_NPC:
				case eComponentType::CT_PLAYER:
					if (x != _tileX || y != _tileY)
						character_count++;
					else
						tileCharacter = component;
					break;
				}
			}
		}
	}
}