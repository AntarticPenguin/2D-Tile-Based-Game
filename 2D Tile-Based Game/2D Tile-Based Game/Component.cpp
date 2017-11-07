#include "ComponentSystem.h"
#include "Component.h"

Component::Component(LPCWSTR name)
{
	_name = name;
	_canMove = false;
	ComponentSystem::GetInstance().AddComponent(_name, this);
}

Component::~Component()
{

}

void Component::ReceiveMessage(Component* sender, std::wstring message)
{
	if (L"Collision" == message)
	{

	}
}