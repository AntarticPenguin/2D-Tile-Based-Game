#pragma once
#include <vector>

enum eStateType;
class Character;
class Sprite;

class State
{
protected:
	Character* _character;
	std::vector<Sprite*> _spriteList;
	eStateType _nextState;
	eStateType _curState;

public:
	State();
	~State();

public:
	virtual void Init(Character* character);
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void Release();
	virtual void Reset();

	virtual void Start();
	virtual void Stop();

	virtual void CreateSprite();

	void NextState(eStateType stateType);
	eStateType GetState();
};