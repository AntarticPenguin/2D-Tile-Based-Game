#include "NPC.h"
#include "ComponentSystem.h"
#include "Map.h"

#include "MoveState.h"

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
	Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");
	std::vector<eComponentType> compareTypeList;
	compareTypeList.push_back(eComponentType::CT_MONSTER);
	Component* findEnemy = ComponentSystem::GetInstance().FindComponentInRange(this, 2, compareTypeList);

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
			if (eDirection::NONE != direction)
			{
				_curDirection = (eDirection)findDirection;
				ChangeState(eStateType::ET_MOVE);
			}
		}
	}
	else
	{
		//없으면 평소움직임
		Character::UpdateAI(deltaTime);
	}
}