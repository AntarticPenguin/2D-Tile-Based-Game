#pragma once
#include <Windows.h>
#include <string>
#include <list>
#include <map>

#include "Component.h"

enum eDirection
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE,
};

enum eStateType
{
	ET_NONE,
	ET_IDLE,
	ET_MOVE,
	ET_ATTACK,
	ET_DEFENSE,
	ET_COUNTERATTACK,
	ET_RECOVERY,
	ET_DEAD,
};

class Sprite;
class State;
class Font;

class Character : public Component
{
private:
	std::wstring _spriteFileName;
	std::wstring _scriptFileName;

	float _x;
	float _y;

public:
	Character(LPCWSTR name, LPCWSTR scriptName, LPCWSTR spriteFileName);
	virtual ~Character();

public:
	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	std::wstring GetTextureFileName();
	std::wstring GetScriptFileName();
	float GetX();
	float GetY();

	//Transform
public:
	void MoveDeltaPosition(float deltaX, float deltaY);
	void SetPosition(float posX, float posY);

	//Character Info
protected:
	int _maxHp;
	int _hp;
	int _attackedPoint;
	int _recoveryStat;

	float _recoveryCooltimeDuration;
	float _recoveryCooltime;

public:
	int GetAttackedPoint();
	void DecreaseHP(int decreaseHP);

	//AI & State
private:
	std::map<eStateType, State*> _stateMap;

public:
	void InitMove();
	virtual void UpdateAI(float deltaTime);
	void ChangeState(eStateType stateType);

	//MOVE
protected:
	bool _isMoving;
	float _moveTime;
	State* _state;

	float _targetX;
	float _targetY;

	eDirection _curDirection;

public:
	void MoveStart(int newTileX, int newTileY);
	void MoveStop();
	void Moving(float deltaTime);

	virtual Component* Collision(std::list<Component*>& collisionList);

	eDirection GetDirection();
	float GetMoveTime();
	bool IsMoving();

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

public:
	Component* GetTarget();
	Component* GetCounterTarget();
	void SetTarget(Component* target);
	void ResetTarget();
	int GetAttackPoint();

	void UpdateAttackCooltime(float deltaTime);
	bool IsAttackCooltime();
	void ResetAttackCooltime();

	void UpdateRecoveryCooltime(float deltaTime);
	bool IsHpFull();
	bool IsRecoveryCoolTime();
	void RecoveryHP();
	void RecoveryHP(int hp);
	void ResetRecoveryCooltime();

	//UI(Font)
private:
	Font* _font;

public:
	void UpdateText();
};