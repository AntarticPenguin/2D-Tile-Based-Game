#include <stdio.h>
#include "ComponentSystem.h"
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
	//맵은 트리구조. 반복자를 이용해 삭제
	for (map<wstring, Component*>::iterator itr = _componentMap.begin();
		itr != _componentMap.end(); itr++)
	{
		delete itr->second;
	}
	_componentMap.clear();
}

Component* ComponentSystem::FindComponent(std::wstring name)
{
	map<wstring, Component*>::iterator itr = _componentMap.find(name);
	if (itr != _componentMap.end())
	{
		return itr->second;
	}

	return NULL;
}

ComponentSystem::ComponentSystem()
{
	_componentMap.clear();
}

ComponentSystem::~ComponentSystem()
{

}