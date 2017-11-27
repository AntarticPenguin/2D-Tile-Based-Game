#include "LifeTileObject.h"
#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Stage.h"
#include "Map.h"
#include "Player.h"

LifeTileObject::LifeTileObject(LPCWSTR name, Sprite* sprite) :
	TileObject(name, sprite)
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
	Player* player = (Player*)ComponentSystem::GetInstance().FindComponent(L"Player");
	int range = 1;

	int count = 0;

	int minTileX = GetTileX() - range;
	int maxTileX = GetTileX() + range;
	int minTileY = GetTileY() - range;
	int maxTileY = GetTileY() + range;

	if (minTileX < 0)
		minTileX = 0;
	if (map->GetWidth() <= maxTileX)
		maxTileX = map->GetWidth() - 1;
	if (minTileY < 0)
		minTileY = 0;
	if (map->GetHeight() <= maxTileY)
		maxTileY = map->GetHeight() - 1;

	//������ ĳ���Ͱ� ������ Count�߰�
	for (int y = minTileY; y <= maxTileY; y++)
	{
		for (int x = minTileX; x <= maxTileX; x++)
		{
			std::list<Component*> componentList;
			if (false == map->GetTileCollisionList(x, y, componentList))
			{
				count++;
			}
		}
	}
}