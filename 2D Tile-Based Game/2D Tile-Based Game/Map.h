#pragma once
#include <Windows.h>
#include <vector>
#include <list>

#include "Component.h"

class TileCell;
class Sprite;

class Map : public Component
{
private:
	std::vector<std::vector<TileCell*>> _tileMap;
	int _mapWidth;
	int _mapHeight;

	float _startX;
	float _startY;
	float _deltaX;
	float _deltaY;

	int _tileSize;

	Component* _viewer;

	std::vector<Sprite*> _spriteList;

public:
	Map(std::wstring name);
	~Map();

public:
	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();
	void Scroll(float deltaX, float deltaY);

	int GetPositionX(int tileX, int tileY);
	int GetPositionY(int tileX, int tileY);
	int GetWidth();
	int GetHeight();

	TileCell* GetTileCell(int tileX, int tileY);

	void SetTileComponent(int tileX, int tileY, Component* component, bool isRender);
	void ResetTileComponent(int tileX, int tileY, Component* component);
	bool CanMoveTileMap(int tileX, int tileY);
	bool GetTileCollisionList(int tileX, int tileY, std::list<Component*>& collisionList); //&(������)�� null���� �Ѿ�� �� ����
	std::list<Component*> GetTileComponentList(int tileX, int tileY);

	void InitViewer(Component* component);
};