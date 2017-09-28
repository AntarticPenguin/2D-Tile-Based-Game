#pragma once

class GameTimer
{
private:
	__int64 _oldCount;
	double _deltaTime;
	double _secondPerCount;

public:
	GameTimer();
	~GameTimer();

public:
	void Reset();
	void Update();
	float GetDeltaTime();
};