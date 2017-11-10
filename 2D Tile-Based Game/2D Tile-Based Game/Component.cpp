#include "ComponentSystem.h"
#include "Component.h"

Component::Component(LPCWSTR name)
{
	_name = name;
	_canMove = false;
	_moveDistancePerTimeX = 0;
	_moveDistancePerTimeY = 0;
	_eType = eComponentType::CT_NONE;
	_isLive = true;

	ComponentSystem::GetInstance().AddComponent(_name, this);
}

Component::~Component()
{

}

void Component::ReceiveMessage(std::wstring message, const sComponentMsgParam& msgParam)
{
	if (L"Collision" == message)
	{

	}
}