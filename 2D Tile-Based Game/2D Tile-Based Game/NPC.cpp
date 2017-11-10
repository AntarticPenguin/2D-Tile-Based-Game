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
	if (false == _isLive)
		return;

	if (false == _isMoving)
	{
		Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");
		std::vector<eComponentType> compareTypeList;
		compareTypeList.push_back(eComponentType::CT_MONSTER);
		Component* findEnemy = ComponentSystem::GetInstance().FindComponentInRange(this, 2, compareTypeList);

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

void NPC::ReceiveMessage(std::wstring message, const sComponentMsgParam& msgParam)
{
	if (L"Attack" == message)
	{
		int attackPoint = msgParam.attackPoint;
		_hp -= attackPoint;

		if (_hp < 0)
		{
			//DEAD
			_isLive = false;
			SetCanMove(true);

			//STOP
			_moveDistancePerTimeX = 0.0f;
			_moveDistancePerTimeY = 0.0f;
		}
	}
}