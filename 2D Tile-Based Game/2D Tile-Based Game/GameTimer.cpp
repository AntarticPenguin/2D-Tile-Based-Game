#include <Windows.h>

#include "GameTimer.h"

GameTimer::GameTimer() :
	_secondPerCount(0.0), _deltaTime(-1.0), _oldCount(0)
{
	__int64 countPerSec;		//�����쿡�� �ʴ� ��count
	QueryPerformanceFrequency((LARGE_INTEGER*)&countPerSec);

	_secondPerCount = 1.0 / (double)countPerSec;
}

GameTimer::~GameTimer()
{

}

void GameTimer::Reset()
{
	__int64 curCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&curCount);
	_oldCount = curCount;
}

void GameTimer::Update()
{
	__int64 curCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&curCount);		//�����쿡�� ���� ��Count�� �����ִ��� ��û
	_deltaTime = (curCount - _oldCount) * _secondPerCount;	//1count�� �ð��� ������ �ð��� �����ش�.

	_oldCount = curCount;
}

float GameTimer::GetDeltaTime()
{
	return (float)_deltaTime;
}