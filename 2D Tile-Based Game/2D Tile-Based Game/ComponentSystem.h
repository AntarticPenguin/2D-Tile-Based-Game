#pragma once
#include <Windows.h>
#include <string>
#include <map>

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

	//Message
public:
	void SendMessageToComponent(Component* sender, Component* receiver, std::wstring message);

private:
	ComponentSystem();

public:
	~ComponentSystem();
};
