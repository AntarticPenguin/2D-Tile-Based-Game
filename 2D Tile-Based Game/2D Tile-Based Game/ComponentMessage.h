#pragma once

class Component;

typedef struct sComponentMsgParam
{
	Component* sender;
	int attackPoint;
}sComponentMsgParam;