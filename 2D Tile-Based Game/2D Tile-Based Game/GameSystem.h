#pragma once
#include <Windows.h>

#include "d3dx11.h"
#include "GameTimer.h"

class GameSystem
{
private:
	static GameSystem* _instance;

public:
	static GameSystem& GetInstance();
	
	//Windows
private:
	HWND _hMainWnd;

public:
	bool InitSystem(HINSTANCE hInstance, int nCmdShow);
	int	Update();

	//Direct3D
private:
	ID3D11Device* _d3dDevice;
	ID3D11DeviceContext* _d3dDeviceContext;
	UINT _4xMsaaQuality;					//�ϵ��� �����ϴ� �ִ� ǰ��
	bool _isEnable4xMSAA;

	IDXGISwapChain* _swapChain;

	ID3D11RenderTargetView* _renderTargetView;

	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilView* _depthStencilView;

	D3D11_VIEWPORT _screenViewport;

public:
	bool InitDirect3D();

	//Game
private:
	GameTimer _gameTimer;
	float _frameDuration;

	//Initializer & Destroyer
private:
	GameSystem();

public:
	~GameSystem();
};