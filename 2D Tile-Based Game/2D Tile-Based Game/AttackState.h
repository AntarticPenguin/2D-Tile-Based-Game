#pragma once
#include "State.h"

enum class eAttackType;
class Character;

class AttackState : public State
{
public:
	AttackState();
	~AttackState();

public:
	void Init(Character* character);
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void Start();
	void Stop();

	int GetDamageByType(eAttackType type);
};