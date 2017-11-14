#pragma once

class Character;

class MoveState
{
private:
	Character* _character;

	float _movingDuration;
	bool _isMoving;

public:
	MoveState();
	~MoveState();

public:
	void Init(Character* character);
	void Update(float deltaTime);
	void UpdateMove(float deltaTime);

	bool IsMoving();
	float GetMovingDuration();
	void Start();
	void Stop();
	void SetMoving(bool isMoving);
};