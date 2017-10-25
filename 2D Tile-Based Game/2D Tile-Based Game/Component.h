#pragma once
#include <Windows.h>
#include <string>

class Component
{
protected:
	LPCWSTR _name;

public:
	Component() {}
	Component(LPCWSTR name);
	virtual ~Component();

public:
	virtual void Init() = 0;
	virtual void Deinit() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
	virtual void Reset() = 0;
};