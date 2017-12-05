#pragma once
#include "StageParts.h"

class LifeStageParts : public StageParts
{
private:
	int _lifeNpcCount;

public:
	LifeStageParts(Stage* stage);
	~LifeStageParts();

public:
	void CreateComponents(std::wstring mapName);
	Component* CreateLifeNPC(std::wstring scriptName, std::wstring spriteName);
};