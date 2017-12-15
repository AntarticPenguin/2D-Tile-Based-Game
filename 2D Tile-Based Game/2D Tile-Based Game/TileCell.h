#pragma once
#include <list>

class Component;
class Sprite;

class TileCell
{
private:
	float _posX;
	float _posY;

	int _tileX;
	int _tileY;

	std::list<Component*> _componentList;
	std::list<Component*> _renderList;

public:
	TileCell(int tileX, int tileY);
	~TileCell();

public:
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void SetPosition(float posX, float posY);

	float GetPositionX();
	float GetPositionY();
	int GetTileX();
	int GetTileY();

	void AddComponent(Component* component, bool isRender);
	void RemoveComponent(Component* component);
	void MoveDeltaPosition(float deltaX, float deltaY);

	bool CanMove();
	bool GetCollisionList(std::list<Component*>& collisionList);
	std::list<Component*> GetComponentList();

	//FindPath
private:
	bool _isPathfindingMark;
	TileCell* _prevPathfindingCell;

	float _distanceWeight;			//타일속성(늪, 얼음 등..)에 따라 세팅된 고정된 값
	float _distanceFromStart;		//계산된 값을 저장하기 위한 변수
	float _heuristic;

public:
	void InitPathfinding();
	bool IsPathfindingMark();
	void PathFinded();
	TileCell* GetPrevPathfindingCell();
	void SetPrevPathfindingCell(TileCell* tileCell);

	float GetDistanceFromStart();
	void SetDistanceFromStart(float distance);

	float GetDistanceWeight();
	float GetHeuristic();
	void SetHeuristic(float heuristic);
};