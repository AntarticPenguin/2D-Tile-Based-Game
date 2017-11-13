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

void ComponentSystem::Update(float deltaTime)
{
	ProcessMessageQueue();
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
	//맵은 트리구조. 반복자를 이용해 삭제
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
		1. 자신을 기준으로, 검사할 타일 범위 결정
		2. 범위 내 타일을 검사하면서 NPC, PLAYER가 있으면 "찾았음"
		3. 없으면 평소 움직임
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
					//죽은대상이면 검사 제외
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

void ComponentSystem::SendMessageToComponent(const sComponentMsgParam& msgParam)
{
	_msgQueue.push(msgParam);
}

void ComponentSystem::ProcessMessageQueue()
{
	while (0 < _msgQueue.size())
	{
		sComponentMsgParam msgParam = _msgQueue.front();
		_msgQueue.pop();
		msgParam.receiver->ReceiveMessage(msgParam);
	}
}

ComponentSystem::ComponentSystem()
{
	_componentMap.clear();
}

ComponentSystem::~ComponentSystem()
{

}