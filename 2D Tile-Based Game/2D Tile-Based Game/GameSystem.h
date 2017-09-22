#pragma once
#include <Windows.h>
#include "d3dx11.h"

class GameSystem
{
private:
	static GameSystem* _instance;

public:
	static GameSystem& GetInstance();
	
	//Windows
public:
	bool InitSystem(HINSTANCE hInstance, int nCmdShow);
	int	Update();

	//Direct3D
private:
	ID3D11Device* _d3dDevice;
	ID3D11DeviceContext* _d3dDeviceContext;
	UINT _4xMsaaQuality;					//�ϵ��� �����ϴ� �ִ� ǰ��
	bool _isEnable4xMSAA;

public:
	bool InitDirect3D();

	//Initializer & Destroyer
private:
	GameSystem();

public:
	~GameSystem();
};