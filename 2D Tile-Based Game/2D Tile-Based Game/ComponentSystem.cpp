#include <stdio.h>
#include "ComponentSystem.h"
#include "Component.h"
#include "Map.h"
using namespace std;

ComponentSystem* ComponentSystem::_instance = NULL;

ComponentSystem& ComponentSystem::GetInstance()
{
	if (NULL == _instance)
		_instance = new ComponentSystem();
	return *_instance;
}

void ComponentSystem::AddComponent(wstring name, Component* component)
{
	if (NULL != component)
	{
		map<wstring, Component*>::iterator itr = _componentMap.find(name);
		if (itr == _componentMap.end())
		{
			_componentMap[name] = component;
		}
	}
}

void ComponentSystem::RemoveAllComponents()
{
	//���� Ʈ������. �ݺ��ڸ� �̿��� ����
	for (map<wstring, Component*>::iterator itr = _componentMap.begin();
		itr != _componentMap.end(); itr++)
	{
		delete itr->second;
	}
	_componentMap.clear();
}

Component* ComponentSystem::FindComponent(wstring name)
{
	map<wstring, Component*>::iterator itr = _componentMap.find(name);
	if (itr != _componentMap.end())
	{
		return itr->second;
	}

	return NULL;
}

Component* ComponentSystem::FindComponentInRange(Component* center, int range, vector<eComponentType> compareTypeList)
{
	/*
		1. �ڽ��� ��������, �˻��� Ÿ�� ���� ����
		2. ���� �� Ÿ���� �˻��ϸ鼭 NPC, PLAYER�� ������ "ã����"
		3. ������ ��� ������
	*/
	Map* map = (Map*)FindComponent(L"tileMap");
	int minTileX = center->GetTileX() - range;
	int maxTileX = center->GetTileX() + range;
	int minTileY = center->GetTileY() - range;
	int maxTileY = center->GetTileY() + range;

	if (minTileX < 0)
		minTileX = 0;
	if (map->GetWidth() <= maxTileX)
		maxTileX = map->GetWidth() - 1;
	if (minTileY < 0)
		minTileY = 0;
	if (map->GetHeight() <= maxTileY)
		maxTileY = map->GetHeight() - 1;

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
					//��������̸� �˻� ����
					if (component->IsLive())
					{
						for (int i = 0; i < compareTypeList.size(); i++)
						{
							if (compareTypeList[i] == component->GetType())
							{
								return component;
							}
						}
					}
				}
			}
		}
	}
	return NULL;
}

void ComponentSystem::SendMessageToComponent(std::wstring message, Component* receiver, const sComponentMsgParam& msgParam)
{
	//receiver->ReceiveMessage(sender, message);
	receiver->ReceiveMessage(message, msgParam);
}

ComponentSystem::ComponentSystem()
{
	_componentMap.clear();
}

ComponentSystem::~ComponentSystem()
{

}