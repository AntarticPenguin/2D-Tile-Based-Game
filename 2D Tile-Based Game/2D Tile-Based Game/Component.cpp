#include "ComponentSystem.h"
#include "Component.h"

Component::Component(LPCWSTR name)
{
	_name = name;
	_canMove = false;
	_moveDistancePerTimeX = 0;
	_moveDistancePerTimeY = 0;
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