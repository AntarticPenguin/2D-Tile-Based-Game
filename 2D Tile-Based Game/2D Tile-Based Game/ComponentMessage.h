#pragma once
#include <string>

class Component;

typedef struct sComponentMsgParam
{
	Component* sender;
	Component* receiver;
	std::wstring message;
	int attackPoint;
}sComponentMsgParam;