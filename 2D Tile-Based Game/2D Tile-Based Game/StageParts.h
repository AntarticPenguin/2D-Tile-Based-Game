#pragma once
#include <string>

class Component;
class Stage;

class StageParts
{
private:
	Stage* _stage;
	std::wstring _name;
	int _lifeNPCCount;

public:
	StageParts(Stage* stage);
	~StageParts();

public:
	Component* CreateLifeNPC(std::wstring scriptName, std::wstring spriteName);
	void SetName(std::wstring name);
	void CreateComponents(int count);
};