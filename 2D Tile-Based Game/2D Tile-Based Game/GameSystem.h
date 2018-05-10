#pragma once
#include <Windows.h>
#include <d3dx9.h>

#include "GameTimer.h"

#define RELEASE_COM(x) { if(x){x->Release(); x = NULL;} }
#define SAFE_DELETE(x) { if(x){delete x; x = NULL;} }

class Component;
class Stage;

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
	int GetClientWidth();
	int GetClientHeight();
	LPD3DXSPRITE GetSprite();
	LPDIRECT3DDEVICE9 GetDevice();

	//Direct3D
private:
	//Ver DX9
	LPDIRECT3DDEVICE9 _device3d;
	D3DPRESENT_PARAMETERS _d3dpp;
	LPD3DXSPRITE _sprite;

public:
	bool InitDirect3D();
	void CheckDeviceLost();

	//Game
private:
	GameTimer _gameTimer;
	float _frameDuration;

	//Input
public:
	enum class eKeyState
	{
		KEY_DOWN,
		KEY_UP,
	};


	//INPUT
private:
	enum class eMouseState
	{
		DOWN,
		HOLD,
		UP,
	};

private:
	eKeyState _eKeyState[256];

	//bool _isMouseDown;
	eMouseState _mouseState;
	eMouseState _rightMouseState;
	int _mouseX;
	int _mouseY;

public:
	void InitInput();

	void KeyDown(unsigned int keyCode);
	void KeyUp(unsigned int keyCode);
	bool IsKeyDown(unsigned int keyCode);

	void MouseDown(int mouseX, int mouseY);
	void MouseUp();
	bool IsMouseDown();

	void RightMouseDown();
	void RightMouseUp();
	bool IsRightMouseDown();

	void SetMousePosition(int mouseX, int mouseY);

	int GetMouseX();
	int GetMouseY();

	//Stage
private:
	Stage* _stage;

public:
	Stage* GetStage();

	//Constructor & Destroyer
private:
	GameSystem();

public:
	~GameSystem();
};