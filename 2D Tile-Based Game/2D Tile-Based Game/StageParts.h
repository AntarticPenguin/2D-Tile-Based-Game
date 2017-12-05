#pragma once
#include <string>

class Component;
class Stage;
class Map;

class StageParts
{
protected:
	Stage* _stage;
	Map* _map;

public:
	StageParts(Stage* stage);
	~StageParts();

public:
	virtual void CreateComponents(std::wstring mapName);
	virtual Component* CreateLifeNPC(std::wstring scriptName, std::wstring spriteName);
};