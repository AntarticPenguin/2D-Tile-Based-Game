#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include <vector>
#include <queue>

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
	void Update(float deltaTime);
	void AddComponent(std::wstring name, Component* component);
	void RemoveAllComponents();
	Component* FindComponent(std::wstring name);
	Component* FindComponentInRange(Component* center, int range, std::vector<eComponentType> compareTypeList);

	//Message
private:
	std::queue<sComponentMsgParam> _msgQueue;		//ť�� �⺻������ Reference�� ó��

public:
	void ComponentSystem::SendMessageToComponent(const sComponentMsgParam& msgParam);
	void ProcessMessageQueue();

private:
	ComponentSystem();

public:
	~ComponentSystem();
};
