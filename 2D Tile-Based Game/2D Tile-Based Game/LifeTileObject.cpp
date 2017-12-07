#include "LifeTileObject.h"
#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Stage.h"
#include "Map.h"
#include "Player.h"

LifeTileObject::LifeTileObject(std::wstring name, Sprite* sprite, int tileX, int tileY) :
	TileObject(name, sprite, tileX, tileY)
{

}

LifeTileObject::~LifeTileObject()
{
	
}

void LifeTileObject::Update(float deltaTime)
{
	TileObject::Update(deltaTime);

	//�ֺ� 8ĭ �˻� - ĳ���Ͱ� ��� �ִ°�?(�ڱ� ĭ�� ������)
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

	//������ ĳ���Ͱ� ������ Count�߰�
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

	/*
		1. �ֺ��� ����ִ� ������ 2������ ������ ���� ���
		2. �ֺ��� 2, 3���� ������ �ִٸ� �������뿡 ��
		3. ��Ȯ�� 3���� �ִٸ� ��� ��ų� �׾��ִ� ������ ��Ȱ
		4. 3�� �ʰ��̸� ����
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
				GameSystem::GetInstance().GetStage()->CheckDestroyLifeNPC(tileCharacter);
				tileCharacter = NULL;
			}
		}
		break;
	}
}