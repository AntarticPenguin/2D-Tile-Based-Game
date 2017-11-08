#include "GameSystem.h"
#include "time.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow)
{
	srand((unsigned)time(NULL));
	GameSystem::GetInstance().InitSystem(hInstance, nCmdShow);
	return GameSystem::GetInstance().Update();
}