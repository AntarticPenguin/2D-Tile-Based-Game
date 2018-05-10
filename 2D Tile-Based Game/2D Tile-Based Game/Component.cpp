#include "ComponentSystem.h"
#include "Component.h"

Component::Component()
{
	_canMove = false;
	_eType = eComponentType::NONE;
	_isLive = true;
}

Component::Component(std::wstring name)
{
	_name = name;
	_canMove = false;
	_eType = eComponentType::NONE;
	_isLive = true;

	ComponentSystem::GetInstance().AddComponent(_name, this);
}

Component::~Component()
{

}

void Component::ReceiveMessage(const sComponentMsgParam& msgParam)
{
	if (L"Collision" == msgParam.message)
	{

	}
}