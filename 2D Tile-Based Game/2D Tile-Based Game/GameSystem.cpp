#include <stdio.h>
#include <string>

#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Stage.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int mouseX;
	int mouseY;

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		GameSystem::GetInstance().MouseDown(mouseX, mouseY);
		return 0;
	case WM_LBUTTONUP:
		GameSystem::GetInstance().MouseUp();
		return 0;
	case WM_RBUTTONDOWN:
		GameSystem::GetInstance().RightMouseDown();
		return 0;
	case WM_RBUTTONUP:
		GameSystem::GetInstance().RightMouseUp();
		return 0;

	case WM_KEYDOWN:
		if (VK_ESCAPE == wParam)
		{
			ComponentSystem::GetInstance().RemoveAllComponents();
			DestroyWindow(hWnd);
		}
		GameSystem::GetInstance().KeyDown(wParam);
		return 0;
	case WM_KEYUP:
		GameSystem::GetInstance().KeyUp(wParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

GameSystem* GameSystem::_instance = NULL;


GameSystem::GameSystem()
{
	//_isEnable4xMSAA = false;
	_isFullScreen = false;
	_clientWidth = 1600;
	_clientHeight = 900;
	_frameDuration = 0.0f;
}

GameSystem::~GameSystem()
{
	delete _stage;

	RELEASE_COM(_sprite);
	RELEASE_COM(_device3d);
}

GameSystem& GameSystem::GetInstance()
{
	if (NULL == _instance)
		_instance = new GameSystem();
	return *_instance;
}

bool GameSystem::InitSystem(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"Base";

	if (!RegisterClass(&wc))
	{
		return false;
	}
	
	DWORD style;
	if (_isFullScreen)
	{
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}
	else
	{
		style = WS_OVERLAPPEDWINDOW;
	}
	_hMainWnd = CreateWindow(
		L"Base",
		L"타이틀",
		style,
		CW_USEDEFAULT,			//x
		CW_USEDEFAULT,			//y
		_clientWidth,			//Width
		_clientHeight,			//Height
		0,
		0,
		hInstance,
		0
	);

	if (NULL == _hMainWnd)
	{
		MessageBox(0, L"FAIL TO CREATE WINDOW", L"ERROR", MB_OK);
		return false;
	}

	if (false == _isFullScreen)
	{
		RECT clientRect;
		GetClientRect(_hMainWnd, &clientRect);

		int addWidth = _clientWidth - clientRect.right;
		int addHeight = _clientHeight - clientRect.bottom;
		int finalWidth = _clientWidth + addWidth;
		int finalHeight = _clientHeight + addHeight;

		MoveWindow(_hMainWnd, 0, 0, finalWidth, finalHeight, TRUE);
	}

	ShowWindow(_hMainWnd, nCmdShow);
	UpdateWindow(_hMainWnd);

	if (false == InitDirect3D())
		return false;

	InitInput();

	//Stage
	_stage = new Stage();
	_stage->Init(L"Map01", eStage::TOWN);

	return true;
}

int	GameSystem::Update()
{
	MSG msg = { 0 };

	_gameTimer.Reset();
	_frameDuration = 0.0f;

	while (WM_QUIT != msg.message)
	{
		//메세지 내용이 있으면 들어옴
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			//윈도우가 처리
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			_gameTimer.Update();
			float deltaTime = _gameTimer.GetDeltaTime();
			//wchar_t timeCheck[256];
			//swprintf(timeCheck, L"DeltaTime: %f\n", deltaTime);
			//OutputDebugString(timeCheck);

			_frameDuration += _gameTimer.GetDeltaTime();

			//게임 업데이트
			//들어온 메세지들을 큐에 쌓아놓고 처리
			//컴포넌트들이 공평하게 동작
			ComponentSystem::GetInstance().Update(deltaTime);

			_stage->Update(deltaTime);

			//60 Frame Update
			float secPerFrame = 1.0f / 60.0f;
			if (secPerFrame <= _frameDuration)
			{
				//wchar_t timeCheck[256];
				//swprintf(timeCheck, L"FrameDuration: %f\n", _frameDuration);
				//OutputDebugString(timeCheck);

				_frameDuration = 0.0f;

				//DX9
				_device3d->Clear(0,
					NULL,
					D3DCLEAR_TARGET,				//클리어 타겟(후면버퍼)
					D3DCOLOR_XRGB(0, 0, 0),			//색상
					0.0f,							//깊이
					0
				);

				//Begin과 End 사이에 출력작업
				_device3d->BeginScene();

				_sprite->Begin(D3DXSPRITE_ALPHABLEND);

				{
					_stage->Render();
				}

				_sprite->End();

				_device3d->EndScene();

				//출력 직전에 Device Lost 체크
				CheckDeviceLost();

				_device3d->Present(NULL, NULL, NULL, NULL);

				//TEST STAGE LOAD
				{
					if (IsKeyDown(VK_F1))
					{
						ComponentSystem::GetInstance().ClearMessageQueue();
						delete _stage;
						_stage = new Stage();
						_stage->Init(L"", eStage::TOWN);
					}

					if (IsKeyDown(VK_F2))
					{
						ComponentSystem::GetInstance().ClearMessageQueue();
						delete _stage;
						_stage = new Stage();
						_stage->Init(L"", eStage::DUNGEON);
					}
				}
			}
		}
	}

	return (int)msg.wParam;
}

int GameSystem::GetClientWidth()
{
	return _clientWidth;
}

int GameSystem::GetClientHeight()
{
	return _clientHeight;
}

LPD3DXSPRITE GameSystem::GetSprite()
{
	return _sprite;
}

LPDIRECT3DDEVICE9 GameSystem::GetDevice()
{
	return _device3d;
}

bool GameSystem::InitDirect3D()
{
	//Ver DX9
	LPDIRECT3D9 direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == direct3d)
	{
		MessageBox(0, L"D3D9객체를 얻지 못했습니다.", L"ERROR", MB_OK);
		return false;
	}

	ZeroMemory(&_d3dpp, sizeof(_d3dpp));

	_d3dpp.BackBufferWidth = _clientWidth;
	_d3dpp.BackBufferHeight = _clientHeight;

	if (_isFullScreen)
	{
		_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	else
	{
		_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	}
	_d3dpp.BackBufferCount = 1;
	_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	_d3dpp.hDeviceWindow = _hMainWnd;
	_d3dpp.Windowed = (!_isFullScreen);
	_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	
	D3DCAPS9 caps;		//하드웨어 성능검사
	HRESULT hr = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);	//하드웨어 가속을 얼만큼 지원하는가
	if (FAILED(hr))
	{
		MessageBox(0, L"하드웨어 성능체크 실패", L"ERROR", MB_OK);
		return false;
	}

	DWORD behavior;
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1,1))
	{
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;		//하드웨어 가속을 지원하지 않으면 소프트웨어로 구동
	}
	else
	{
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	hr = direct3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, _hMainWnd, behavior, &_d3dpp, &_device3d);
	if (FAILED(hr))
	{
		MessageBox(0, L"direct3d device 생성실패", L"ERROR", MB_OK);
		return false;
	}

	hr = D3DXCreateSprite(_device3d, &_sprite);
	if (FAILED(hr))
	{
		MessageBox(0, L"Sprite 생성 실패", L"ERROR", MB_OK);
		return false;
	}

	return true;
}

void GameSystem::CheckDeviceLost()
{
	HRESULT hr = _device3d->TestCooperativeLevel();

	//유효한 상태가 아니라면
	if (FAILED(hr))
	{
		if (D3DERR_DEVICELOST == hr)		//복구가 불가능한 상태: 대기
		{
			Sleep(100);						//OS의 빠른 처리를 위해 App을 정지
			return;
		}
		else if (D3DERR_DEVICENOTRESET == hr)	//복구가 가능한 상태
		{
			//복구
			_stage->Release();

			InitDirect3D();
			hr = _device3d->Reset(&_d3dpp);

			_stage->Reset();
		}
	}
}

void GameSystem::InitInput()
{
	_isMouseDown = false;
	_isRightMouseDown = false;
	_mouseX = 0;
	_mouseY = 0;

	for (int i = 0; i < 256; i++)
		_eKeyState[i] = eKeyState::KEY_UP;
}

void GameSystem::KeyDown(unsigned int keyCode)
{
	_eKeyState[keyCode] = eKeyState::KEY_DOWN;
}

void GameSystem::KeyUp(unsigned int keyCode)
{
	_eKeyState[keyCode] = eKeyState::KEY_UP;
}

bool GameSystem::IsKeyDown(unsigned int keyCode)
{
	return (eKeyState::KEY_DOWN == _eKeyState[keyCode]);
}

void GameSystem::MouseDown(int mouseX, int mouseY)
{
	_isMouseDown = true;

	_mouseX = mouseX;
	_mouseY = mouseY;
}

void GameSystem::MouseUp()
{
	_isMouseDown = false;
}

Stage* GameSystem::GetStage()
{
	return _stage;
}

bool GameSystem::IsMouseDown()
{
	return _isMouseDown;
}

void GameSystem::RightMouseDown()
{
	_isRightMouseDown = true;
}

void GameSystem::RightMouseUp()
{
	_isRightMouseDown = false;
}

bool GameSystem::IsRightMouseDown()
{
	return _isRightMouseDown;
}

int GameSystem::GetMouseX()
{
	return _mouseX;
}

int GameSystem::GetMouseY()
{
	return _mouseY;
}