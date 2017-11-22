#include <stdio.h>
#include <string>

#include "GameSystem.h"
#include "ComponentSystem.h"

#include "Map.h"
#include "Player.h"
#include "NPC.h"
#include "Monster.h"
#include "RecoveryItem.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
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
		L"Ÿ��Ʋ",
		style,
		CW_USEDEFAULT,			//x
		CW_USEDEFAULT,			//y
		CW_USEDEFAULT,			//Width
		CW_USEDEFAULT,			//Height
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
	
	_componentList.clear();

	Map* tileMap = new Map(L"tileMap");
	_componentList.push_back(tileMap);

	for (int i = 0; i < 10; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"recovery_Item_%d", i);
		RecoveryItem* item = new RecoveryItem(name, L"recovery_Item", L"recovery_Item");
		_componentList.push_back(item);
	}

	for (int i = 0; i < 20; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"npc_%d", i);
		NPC* npc = new NPC(name, L"npc", L"Npc");
		_componentList.push_back(npc);
	}

	for (int i = 0; i < 10; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"monster_%d", i);
		Monster* monster = new Monster(name, L"monster", L"monster");
		_componentList.push_back(monster);
	}

	Player* player = new Player(L"Player", L"Player", L"Player");	//(������Ʈ�̸�, ��ũ��Ʈ �̸�, ��������Ʈ�̸�)
	_componentList.push_back(player);

	for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
	{
		(*itr)->Init();
	}
	
	tileMap->InitViewer(player);

	return true;
}

int	GameSystem::Update()
{
	MSG msg = { 0 };

	_gameTimer.Reset();
	_frameDuration = 0.0f;

	while (WM_QUIT != msg.message)
	{
		//�޼��� ������ ������ ����
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			//�����찡 ó��
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

			//���� ������Ʈ
			//���� �޼������� ť�� �׾Ƴ��� ó��
			//������Ʈ���� �����ϰ� ����
			ComponentSystem::GetInstance().Update(deltaTime);


			for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
			{
				(*itr)->Update(deltaTime);
			}

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
					D3DCLEAR_TARGET,				//Ŭ���� Ÿ��(�ĸ����)
					D3DCOLOR_XRGB(0, 0, 0),			//����
					0.0f,							//����
					0
				);

				//Begin�� End ���̿� ����۾�
				_device3d->BeginScene();

				_sprite->Begin(D3DXSPRITE_ALPHABLEND);

				{
					for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
					{
						(*itr)->Render();
					}
				}

				_sprite->End();

				_device3d->EndScene();

				//��� ������ Device Lost üũ
				CheckDeviceLost();

				_device3d->Present(NULL, NULL, NULL, NULL);
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
		MessageBox(0, L"D3D9��ü�� ���� ���߽��ϴ�.", L"ERROR", MB_OK);
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
	
	D3DCAPS9 caps;		//�ϵ���� ���ɰ˻�
	HRESULT hr = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);	//�ϵ���� ������ ��ŭ �����ϴ°�
	if (FAILED(hr))
	{
		MessageBox(0, L"�ϵ���� ����üũ ����", L"ERROR", MB_OK);
		return false;
	}

	DWORD behavior;
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1,1))
	{
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;		//�ϵ���� ������ �������� ������ ����Ʈ����� ����
	}
	else
	{
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	hr = direct3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, _hMainWnd, behavior, &_d3dpp, &_device3d);
	if (FAILED(hr))
	{
		MessageBox(0, L"direct3d device ��������", L"ERROR", MB_OK);
		return false;
	}

	hr = D3DXCreateSprite(_device3d, &_sprite);
	if (FAILED(hr))
	{
		MessageBox(0, L"Sprite ���� ����", L"ERROR", MB_OK);
		return false;
	}

	return true;
}

void GameSystem::CheckDeviceLost()
{
	HRESULT hr = _device3d->TestCooperativeLevel();

	//��ȿ�� ���°� �ƴ϶��
	if (FAILED(hr))
	{
		if (D3DERR_DEVICELOST == hr)		//������ �Ұ����� ����: ���
		{
			Sleep(100);						//OS�� ���� ó���� ���� App�� ����
			return;
		}
		else if (D3DERR_DEVICENOTRESET == hr)	//������ ������ ����
		{
			//����
			for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
			{
				(*itr)->Release();
			}

			InitDirect3D();
			hr = _device3d->Reset(&_d3dpp);

			for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
			{
				(*itr)->Reset();
			}
		}
	}
}

void GameSystem::InitInput()
{
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