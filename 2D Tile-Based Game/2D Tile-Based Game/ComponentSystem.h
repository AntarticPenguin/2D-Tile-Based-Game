#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include <queue>
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
	void Update(float deltaTime);
	void AddComponent(std::wstring name, Component* component);
	void RemoveAllComponents();
	void RemoveComponent(Component* component);
	Component* FindComponent(std::wstring name);
	Component* FindComponentInRange(Component* mapComponent, Component* center, int range, std::vector<eComponentType> compareTypeList);

	//Message
private:
	std::queue<sComponentMsgParam> _msgQueue;		//ť�� �⺻������ Reference�� ó��

public:
	void ComponentSystem::SendMessageToComponent(const sComponentMsgParam& msgParam);
	void ProcessMessageQueue();
	void ClearMessageQueue();

private:
	ComponentSystem();

public:
	~ComponentSystem();
};
