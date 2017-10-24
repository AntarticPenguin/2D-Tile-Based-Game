#include "ComponentSystem.h"
#include "Component.h"

Component::Component(std::wstring name)
{
	_name = name;
	ComponentSystem::GetInstance().AddComponent(_name, this);
}

Component::~Component()
{

}