#pragma once
#include <Windows.h>
#include <string>
#include <list>
#include <map>
#include <stack>
#include <vector>

#include "Component.h"
#include "GlobalTile.h"

enum eStateType
{
	ET_NONE,
	ET_IDLE,
	ET_MOVE,
	ET_ATTACK,
	ET_DEFENSE,
	ET_COUNTERATTACK,
	ET_PATHFINDING,
	ET_DEAD,
};

class Sprite;
class State;
class Font;
class TileCell;

class Character : public Component
{
private:
	std::wstring _spriteFileName;
	std::wstring _scriptFileName;

	float _x;
	float _y;

public:
	Character(std::wstring name, std::wstring scriptName, std::wstring spriteFileName);
	virtual ~Character();

public:
	void Init();
	void Init(int tileX, int tileY);
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	virtual void UpdateCharacter();

	std::wstring GetTextureFileName();
	std::wstring GetScriptFileName();
	float GetX();
	float GetY();

	void InitTilePosition(int tileX, int tileY);

	//Transform
public:
	void MoveDeltaPosition(float deltaX, float deltaY);
	void SetPosition(float posX, float posY);

	//Character Info
protected:
	int _maxHp;
	int _hp;
	int _attackedPoint;

public:
	int GetAttackedPoint();
	void DecreaseHP(int decreaseHP);

	//Character Item
protected:
	std::list<Component*> _equipList;

public:
	void Equip(Component* weapon);

	//State
protected:
	std::map<eStateType, State*> _stateMap;
	State* _state;

public:
	void InitMove();
	virtual void InitState();
	void ReplaceState(eStateType changeType, State* replaceState);
	void ChangeState(eStateType stateType);

	//Move
protected:
	bool _isMoving;
	float _moveTime;
	int _moveRange;

	float _targetX;
	float _targetY;

	eDirection _curDirection;

public:
	void MoveStart(int newTileX, int newTileY);
	void MoveStop();

	virtual Component* Collision(std::list<Component*>& collisionList);

	eDirection GetDirection();
	void SetDirection(eDirection direction);
	float GetMoveTime();
	bool IsMoving();

	int GetMoveRange();

	//Message
public:
	void ReceiveMessage(const sComponentMsgParam& msgParam);

	//Attack
protected:
	Component* _target;
	Component* _counterTarget;	//나에게 공격을 날린 대상(카운터공격에 사용됨)
	int _attackPoint;
	float _attackCooltimeDuration;
	float _attackCooltime;
	int _attackRange;

public:
	Component* GetTarget();
	Component* GetCounterTarget();
	void SetTarget(Component* target);
	void ResetTarget();
	int GetAttackPoint();
	int GetAttackRange();

	void UpdateAttackCooltime(float deltaTime);
	bool IsAttackCooltime();
	void ResetAttackCooltime();

	void RecoveryHP(int hp);

	//UI(Font)
private:
	Font* _font;

public:
	void UpdateText();

	//Pathfinding
private:
	TileCell* _targetTileCell;
	std::stack<TileCell*> _pathTileCellStack;

public:
	TileCell* GetTargetCell();
	void SetTargetTileCell(TileCell* tileCell);

	std::stack<TileCell*> GetPathTileCellStack();
	void PushPathTileCell(TileCell* tileCell);
	void ClearPathTileCellStack();

	//In Dungeon
private:
	bool _turnOnMenu;
	std::vector<TileCell*> _colorTileList;

public:
	bool IsClickCharacter(TileCell* tileCell);
	bool IsMenuUp();
	void TurnOffMenu();
	void PushColorTileCell(TileCell* tileCell);
	bool CheckMoveRange(TileCell* tileCell);
};