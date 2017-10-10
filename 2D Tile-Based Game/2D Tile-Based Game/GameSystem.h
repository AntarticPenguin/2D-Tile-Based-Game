#pragma once
#include <Windows.h>
//#include <d3dx11.h>
#include <d3dx9.h>

#include "GameTimer.h"

#define RELEASE_COM(x) { if(x){x->Release(); x = NULL;} }
#define SAFE_DELETE(x) { if(x){delete x; x = NULL;} }

class GameSystem
{
private:
	static GameSystem* _instance;

public:
	static GameSystem& GetInstance();
	
	//Windows
private:
	HWND _hMainWnd;
	bool _isFullScreen;
	int _clientWidth;
	int _clientHeight;

public:
	bool InitSystem(HINSTANCE hInstance, int nCmdShow);
	int	Update();

	//Direct3D
private:
	//Ver DX11
	/*
	ID3D11Device* _d3dDevice;
	ID3D11DeviceContext* _d3dDeviceContext;
	UINT _4xMsaaQuality;					//하드웨어가 지원하는 최대 품질
	bool _isEnable4xMSAA;

	IDXGISwapChain* _swapChain;

	ID3D11RenderTargetView* _renderTargetView;

	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilView* _depthStencilView;

	D3D11_VIEWPORT _screenViewport;
	*/

	//Ver DX9
	LPDIRECT3DDEVICE9 _device3d;

	//Sprite & Texture
private:
	LPD3DXSPRITE _sprite;
	D3DXIMAGE_INFO _textureInfo;
	IDirect3DTexture9* _texture;
	RECT _srcTextureRect;
	D3DCOLOR _textureColor;

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