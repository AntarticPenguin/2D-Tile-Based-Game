#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include <vector>
#include "ComponentMessage.h"

enum eComponentType;
class Component;

class ComponentSystem
{
private:
	static ComponentSystem* _instance;

	//ComponentMap
private:
	std::map<std::wstring, Component*> _componentMap;

public:
	static ComponentSystem& GetInstance();
	void AddComponent(std::wstring name, Component* component);
	void RemoveAllComponents();
	Component* FindComponent(std::wstring name);
	Component* FindComponentInRange(Component* center, int range, std::vector<eComponentType> compareTypeList);

	//Message
public:
	void SendMessageToComponent(std::wstring message, Component* receiver, const sComponentMsgParam& msgParam);

private:
	ComponentSystem();

public:
	~ComponentSystem();
};
