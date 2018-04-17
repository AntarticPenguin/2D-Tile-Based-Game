#pragma once
#include <string>

class Component;
class Stage;
class Map;

class StageLoader
{
protected:
	Stage* _stage;
	Map* _map;
	int _lifeNpcCount;

public:
	StageLoader(Stage* stage);
	~StageLoader();

public:
	virtual void CreateComponents(std::wstring mapName);
	//virtual Component* CreateLifeNPC(std::wstring scriptName, std::wstring spriteName);
};