#pragma once
#include <vector>

class Character;
class Sprite;

class State
{
protected:
	Character* _character;
	std::vector<Sprite*> _spriteList;

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
};