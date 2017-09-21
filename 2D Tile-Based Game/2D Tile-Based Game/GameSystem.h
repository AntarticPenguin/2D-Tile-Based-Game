#pragma once
#include <Windows.h>

class GameSystem
{
private:
	static GameSystem* _instance;

public:
	static GameSystem& GetInstance();
	
public:
	bool InitSystem(HINSTANCE hInstance, int nCmdShow);
	int	Update();

private:
	GameSystem();

public:
	~GameSystem();
};