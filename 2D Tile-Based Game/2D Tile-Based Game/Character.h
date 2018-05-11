#pragma once
#include <Windows.h>
#include <string>
#include <list>
#include <map>
#include <stack>
#include <vector>

#include "Component.h"
#include "GlobalTile.h"

enum class eStateType
{
	NONE,
	IDLE,
	MOVE,
	SELECT_TARGET,
	ATTACK,
	DEFENSE,
	PATHFINDING,
	DEAD,
};

enum class eAttackType
{
	NORMAL,
	SKILL,
};

class State;
class Font;
class TileCell;
class Skill;

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

	//State
protected:
	std::map<eStateType, State*> _stateMap;
	State* _state;

public:
	void InitMove();
	virtual void InitState();
	void ReplaceState(eStateType changeType, State* replaceState);
	void ChangeState(eStateType stateType);

	//Character Info
protected:
	int _behaviorPoint;
	float _behaviorDuration;
	int _maxHp;
	int _hp;
	int _damage;
	int _movePoint;

	bool _canBattle;
	eAttackType _attackType;

public:
	int GetDamage();
	void DecreaseHP(int decreaseHP);

	void DecreaseBehaviorPoint(int point);
	void ChargeBehavior(float deltaTime);
	int GetBehaviorPoint();
	int GetMovePoint();

	void SetCanBattle(bool canBattle);
	bool CanBattle();
	
	eAttackType GetAttackType();
	void SetAttackType(eAttackType type);

	//Move
protected:
	bool _isMoving;
	float _moveTime;
	int _moveRange;
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
	std::vector<Component*> _targets;
	int _attackPoint;
	int _attackRange;

public:
	std::vector<Component*>& GetTargets();
	void AddTarget(Component* target);	//범위공격 타겟들
	void ResetTargets();
	int GetAttackPoint();
	int GetAttackRange();

	void RecoveryHP(int hp);

	//Skill
protected:
	std::map<std::wstring, Skill*> _skillList;
	std::wstring _selectedSkillName;

public:
	virtual void InitSkill();
	void AddSkill(std::wstring name, Skill* skill);
	std::map<std::wstring, Skill*>& GetSkillList();
	void SetSelectedSkill(std::wstring name);
	Skill* GetSelectedSkill();

	//UI(Font)
protected:
	Font* _font;

public:
	virtual void UpdateText();

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
public:
	bool IsClickCharacter(TileCell* tileCell);
};