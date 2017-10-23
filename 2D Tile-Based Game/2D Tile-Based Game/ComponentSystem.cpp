#include <stdio.h>
#include "ComponentSystem.h"

ComponentSystem* ComponentSystem::_instance = NULL;

ComponentSystem& ComponentSystem::GetInstance()
{
	if (NULL == _instance)
		_instance = new ComponentSystem();
	return *_instance;
}

void ComponentSystem::AddComponent(LPCWSTR name, Component* component)
{

}

void ComponentSystem::RemoveAllComponents()
{

}

ComponentSystem::ComponentSystem()
{

}

ComponentSystem::~ComponentSystem()
{

}