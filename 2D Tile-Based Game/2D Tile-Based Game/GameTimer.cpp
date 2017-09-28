#include <Windows.h>

#include "GameTimer.h"

GameTimer::GameTimer() :
	_secondPerCount(0.0), _deltaTime(-1.0), _oldCount(0)
{
	__int64 countPerSec;		//윈도우에서 초당 몇count
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
	QueryPerformanceCounter((LARGE_INTEGER*)&curCount);		//윈도우에게 현재 몇Count를 세고있는지 요청
	_deltaTime = (curCount - _oldCount) * _secondPerCount;	//1count당 시간을 곱해줘 시간을 구해준다.

	_oldCount = curCount;
}

float GameTimer::GetDeltaTime()
{
	return (float)_deltaTime;
}