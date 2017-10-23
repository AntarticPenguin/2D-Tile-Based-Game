#pragma once
#include <Windows.h>

class Component;

class ComponentSystem
{
private:
	static ComponentSystem* _instance;

public:
	static ComponentSystem& GetInstance();
	void AddComponent(LPCWSTR name, Component* component);
	void RemoveAllComponents();

private:
	ComponentSystem();

public:
	~ComponentSystem();
};
