#include <stdio.h>
#include <string>

#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Map.h"
#include "Player.h"
#include "NPC.h"

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
	case WM_LBUTTONDOWN:
		//MessageBox(0, L"Hello World!!!!!!!!!!!", L"Hello World", MB_OK);
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

	_tileMap = NULL;
	_player = NULL;
	_npc = NULL;
}

GameSystem::~GameSystem()
{
	/*
	RELEASE_COM(_depthStencilView);
	RELEASE_COM(_depthStencilBuffer);
	RELEASE_COM(_renderTargetView);
	RELEASE_COM(_swapChain);
	RELEASE_COM(_d3dDeviceContext);
	RELEASE_COM(_d3dDevice);
	*/

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
		L"Å¸ÀÌÆ²",
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
	
	_tileMap = new Map(L"tileMap");
	_tileMap->Init();

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	Player* player = new Player(L"Player", L"Player");
	_componentList.push_back(player);
=======
	_player = new Player(L"npc");
	_player->SetCanMove(false);
>>>>>>> bcf162d76dc561bd9a88489097c6f51d5c4b566b
=======
	_player = new Player(L"npc");
	_player->SetCanMove(false);
	_player->Init();
>>>>>>> parent of 09a4751... 171103 ì¹´ë©”ë¼ì´ë™/ë¦¬íŒ©í† ë§
=======
	Player* player = new Player(L"Player", L"Player");
	_componentList.push_back(player);
>>>>>>> parent of 0aafdd7... Merge branch 'master'

	_npc = new NPC(L"npc");
	_npc->SetCanMove(false);
	_npc->Init();

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> parent of 0aafdd7... Merge branch 'master'
	Monster* monster = new Monster(L"Npc", L"character_sprite2");
	_componentList.push_back(monster);

	for (std::list<Component*>::iterator itr = _componentList.begin(); itr != _componentList.end(); itr++)
	{
		(*itr)->Init();
	}
	
	tileMap->InitViewer(player);
<<<<<<< HEAD
=======
	_tileMap->InitViewer(_player);
	_player->Init();
>>>>>>> bcf162d76dc561bd9a88489097c6f51d5c4b566b
=======
	_tileMap->InitViewer(_player);
>>>>>>> parent of 09a4751... 171103 ì¹´ë©”ë¼ì´ë™/ë¦¬íŒ©í† ë§
=======
>>>>>>> parent of 0aafdd7... Merge branch 'master'

	return true;
}

int	GameSystem::Update()
{
	MSG msg = { 0 };
	/*
	while (ret = GetMessage(&msg, 0, 0, 0))
	{
		if (-1 == ret)
		{
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	*/

	_gameTimer.Reset();
	_frameDuration = 0.0f;

	while (WM_QUIT != msg.message)
	{
		//¸Þ¼¼Áö ³»¿ëÀÌ ÀÖÀ¸¸é µé¾î¿È
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			//À©µµ¿ì°¡ Ã³¸®
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

			_tileMap->Update(deltaTime);
			_player->Update(deltaTime);
			_npc->Update(deltaTime);

			float secPerFrame = 1.0f / 60.0f;
			if (secPerFrame <= _frameDuration)
			{
				//wchar_t timeCheck[256];
				//swprintf(timeCheck, L"FrameDuration: %f\n", _frameDuration);
				//OutputDebugString(timeCheck);

				_frameDuration = 0.0f;

				//°ÔÀÓ ¾÷µ¥ÀÌÆ®

				//DX11
				/*
				float color[4];
				color[0] = 0.3f;	//RED
				color[1] = 0.3f;	//GREEN
				color[2] = 0.3f;	//BLUE
				color[3] = 1.0f;	//ALPHA

				_d3dDeviceContext->ClearRenderTargetView(_renderTargetView, color);			//ÈÄ¸é¹öÆÛ(»ç¿ëÀÚ ´«¿¡ ¾Èº¸ÀÓ)
				_d3dDeviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH || D3D11_CLEAR_STENCIL, 1.0f, 0);

				//°ÔÀÓ°ü·Ã Draw
				_swapChain->Present(0, 0);		//PRESENT(Àü¸éÀ¸·Î ±³Ã¼)
				*/

				//DX9
				_device3d->Clear(0,
					NULL,
					D3DCLEAR_TARGET,				//Å¬¸®¾î Å¸°Ù(ÈÄ¸é¹öÆÛ)
					D3DCOLOR_XRGB(0, 0, 0),	//»ö»ó
					0.0f,							//±íÀÌ
					0
				);

				//Begin°ú End »çÀÌ¿¡ Ãâ·ÂÀÛ¾÷
				_device3d->BeginScene();

				_sprite->Begin(D3DXSPRITE_ALPHABLEND);

				{
					_tileMap->Render();
					_player->Render();
					_npc->Render();
				}

				_sprite->End();

				_device3d->EndScene();

				//Ãâ·Â Á÷Àü¿¡ Device Lost Ã¼Å©
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
	//DX11
	/*
	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr = D3D11CreateDevice(
		0,								//ºñµð¿ÀÄ«µå
		D3D_DRIVER_TYPE_HARDWARE,		//µå¶óÀÌ¹ö Å¸ÀÔ
		0,								//¼ÒÇÁÆ®¿þ¾î ±¸µ¿ÀÚ
		D3D11_CREATE_DEVICE_DEBUG,		//Flag
		0,								//Áö¿øµÇ´Â ¹öÀü ¼³Á¤
		0,								//¼³Á¤ÇÑ ¹öÀü °³¼ö
		D3D11_SDK_VERSION,				//SDK¹öÀü
		&_d3dDevice,					//Device Æ÷ÀÎÅÍ
		&featureLevel,					//featureLevel
		&_d3dDeviceContext				//DeviceContext Æ÷ÀÎÅÍ
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11Device »ý¼º ½ÇÆÐ", L"ERROR", MB_OK);
		return false;
	}

	//4xMSAA Áö¿ø¿©ºÎ Ã¼Å©
	hr = _d3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_4xMsaaQuality);
	if (FAILED(hr))
	{
		MessageBox(0, L"DX11ÀÌ Áö¿øµÇÁö ¾ÊÀº ÇÏµå¿þ¾î", L"ERROR", MB_OK);
		return false;
	}

	DXGI_SWAP_CHAIN_DESC sd;

	//ÈÄ¸é¹öÆÛ ¼¼ÆÃ
	sd.BufferDesc.Width = _clientWidth;
	sd.BufferDesc.Height = _clientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;								//µð½ºÇÃ·¹ÀÌÀÌ °»½Å·ü
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//È­¸é Scanline ¸ðµå
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					//µð½ºÇÃ·¹ÀÌ ºñÀ²(16:9 etc..)

	//¾ÈÆ¼ ¾Ù¸®¾î½Ì Ç°Áú ¼¼ÆÃ
	if (_isEnable4xMSAA)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = _4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//¹öÆÛÀÇ ¿ëµµ¸¦ ¼­¼úÇÏ´Â ±¸Á¶Ã¼
	sd.BufferCount = 1;									//±³È¯»ç½½¿¡¼­ ÀÌ¿ëÇÒ ÈÄ¸é¹öÆÛÀÇ °³¼ö(´õºí¹öÆÛ¸µÀº 1°³)
	sd.OutputWindow = _hMainWnd;						//·»´õ¸µ °á°ú¸¦ Ç¥½ÃÇÒ Ã¢ÀÇ ÇÚµé
	sd.Windowed = true;									//À©µµ¿ì¸ðµå : ÀüÃ¼È­¸é
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			//±³È¯È¿°ú ÁöÁ¤(ÈÄ¸é->Àü¸é ±³Ã¼µÉ¶§ÀÇ È¿°ú)
	sd.Flags = 0;										//Ãß°¡ ÇÃ·¡±×

	//IDXGISwapChain(SwapChain ÀÎ½ºÅÏ½º) ÀÎÅÍÆäÀÌ½º »ý¼º
	IDXGISwapChain* dxgiDevice = NULL;
	hr = _d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);	//uuidof: ÀÏÁ¾ÀÇ Ä³½ºÆÃ
	if (FAILED(hr))
	{
		MessageBox(0, L"dxgiDevice¸¦ ¾òÁö ¸øÇÏ¿´½À´Ï´Ù.", L"ERROR", MB_OK);
		return false;
	}
	
	IDXGIAdapter* dxgiAdapter = NULL;
	hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	if (FAILED(hr))
	{
		MessageBox(0, L"dxgiAdapter¸¦ ¾òÁö ¸øÇÏ¿´½À´Ï´Ù.", L"ERROR", MB_OK);
		return false;
	}

	IDXGIFactory* dxgiFactory = NULL;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	if (FAILED(hr))
	{
		MessageBox(0, L"dxgiFactory¸¦ ¾òÁö ¸øÇÏ¿´½À´Ï´Ù.", L"ERROR", MB_OK);
		return false;
	}

	hr = dxgiFactory->CreateSwapChain(_d3dDevice, &sd, &_swapChain);
	if (FAILED(hr))
	{
		MessageBox(0, L"SwapChain ÀÎÅÍÆäÀÌ½º »ý¼º ½ÇÆÐ", L"ERROR", MB_OK);
		return false;
	}

	//ÀÌÀü 3°³(Device, Adapter, Factory) ÀÎÅÍÆäÀÌ½º ÇØÁ¦
	dxgiDevice->Release();
	dxgiDevice = NULL;

	dxgiAdapter->Release();
	dxgiAdapter = NULL;

	dxgiFactory->Release();
	dxgiFactory = NULL;

	//SwapChainÀÇ ÈÄ¸é¹öÆÛ¿¡ ´ëÇÑ RenderTargetView »ý¼º
	// 1.ÈÄ¸é¹öÆÛÀÇ ÀÎÅÍÆäÀÌ½º È¹µæ
	ID3D11Texture2D* backBuffer;
	hr = _swapChain->GetBuffer(
		0,										//¾ò°íÀÚ ÇÏ´Â backBufferÀÇ ÀÎµ¦½º
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer)
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"backBuffer È¹µæ ½ÇÆÐ", L"ERROR", MB_OK);
		return false;
	}

	//2.backBuffer¸¦ »ç¿ëÇÒ ºä »ý¼º
	hr = _d3dDevice->CreateRenderTargetView(
		backBuffer,
		NULL,
		&_renderTargetView		//backBuffer¸¦ »ç¿ëÇÏ´Â RenderTargetView
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"RenderTargetView »ý¼º ½ÇÆÐ", L"ERROR", MB_OK);
		return false;
	}

	backBuffer->Release();
	backBuffer = NULL;

	//*****ÀÌ ¹ØÀ¸·Î´Â 3D¸¦ À§ÇÑ ÀÛ¾÷ *****
	//±íÀÌ, ½ºÅÙ½Ç ¹öÆÛ¿Í ±×¿¡ ¿¬°áµÇ´Â ºä »ý¼º(±×¸²ÀÚ µî Æ¯¼öÈ¿°ú)

	//1.Æ¯¼º ¼¼ÆÃ
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = _clientWidth;
	depthStencilDesc.Height = _clientHeight;							
	depthStencilDesc.MipLevels = 1;		//¹Ó¸ÊÀÇ ¼öÁØ(º¸°£ Á¤µµ: (ex)ÁÜÀÎ ¾Æ¿ô)
	depthStencilDesc.ArraySize = 1;		//¹è¿­¿¡ µé¾î°¥ ÅØ½ºÃÄ °¹¼ö
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (_isEnable4xMSAA)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = _4xMsaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;				//¾î¶»°Ô ¾²ÀÏÁö ¼³Á¤(default: GPU¸¦ »ç¿ë)
	depthStencilDesc.BindFlags =  D3D11_BIND_DEPTH_STENCIL;		//ÆÄÀÌÇÁ¶óÀÎ¿¡ ¾î¶»°Ô ¹­ÀÏÁö ¼³Á¤
	depthStencilDesc.CPUAccessFlags = 0;						//CPU°¡ ÀÚ¿ø¿¡ Á¢±ÙÇÏ´Â ¹æ¹ý
	depthStencilDesc.MiscFlags = 0;								//±âÅ¸ ÇÃ·¡±×

	hr = _d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &_depthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"depthStencilBuffer: Texture2D »ý¼º ½ÇÆÐ", L"ERROR", MB_OK);
		return false;
	}

	//2.±íÀÌ, ½ºÅÙ½Ç ºä »ý¼º
	hr = _d3dDevice->CreateDepthStencilView(_depthStencilBuffer, 0, &_depthStencilView);
	if (FAILED(hr))
	{
		MessageBox(0, L"±íÀÌ ½ºÅÙ½Ç ºä »ý¼º ½ÇÆÐ", L"ERROR", MB_OK);
		return false;
	}

	//ÆÄÀÌÇÁ¶óÀÎ¿¡ ºä¸¦ ¹ÙÀÎµù
	_d3dDeviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);	//±íÀÌ ½ºÅÙ½Ç ºä¸¦ Á¦¿ÜÇÑ ¹ÙÀÎµùÇÒ ºä °¹¼ö

	//ºäÆ÷Æ® ¼³Á¤
	_screenViewport.TopLeftX = 0;
	_screenViewport.TopLeftY = 0;
	_screenViewport.Width = (float)_clientWidth;
	_screenViewport.Height = (float)_clientHeight;
	_screenViewport.MinDepth = 0.0f;
	_screenViewport.MaxDepth = 1.0f;

	_d3dDeviceContext->RSSetViewports(1, &_screenViewport);
	*/
	
	//Ver DX9
	LPDIRECT3D9 direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == direct3d)
	{
		MessageBox(0, L"D3D9°´Ã¼¸¦ ¾òÁö ¸øÇß½À´Ï´Ù.", L"ERROR", MB_OK);
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
	
	D3DCAPS9 caps;		//ÇÏµå¿þ¾î ¼º´É°Ë»ç
	HRESULT hr = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);	//ÇÏµå¿þ¾î °¡¼ÓÀ» ¾ó¸¸Å­ Áö¿øÇÏ´Â°¡
	if (FAILED(hr))
	{
		MessageBox(0, L"ÇÏµå¿þ¾î ¼º´ÉÃ¼Å© ½ÇÆÐ", L"ERROR", MB_OK);
		return false;
	}

	DWORD behavior;
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1,1))
	{
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;		//ÇÏµå¿þ¾î °¡¼ÓÀ» Áö¿øÇÏÁö ¾ÊÀ¸¸é ¼ÒÇÁÆ®¿þ¾î·Î ±¸µ¿
	}
	else
	{
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	hr = direct3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, _hMainWnd, behavior, &_d3dpp, &_device3d);
	if (FAILED(hr))
	{
		MessageBox(0, L"direct3d device »ý¼º½ÇÆÐ", L"ERROR", MB_OK);
		return false;
	}

	hr = D3DXCreateSprite(_device3d, &_sprite);
	if (FAILED(hr))
	{
		MessageBox(0, L"Sprite »ý¼º ½ÇÆÐ", L"ERROR", MB_OK);
		return false;
	}

	return true;
}

void GameSystem::CheckDeviceLost()
{
	HRESULT hr = _device3d->TestCooperativeLevel();

	//À¯È¿ÇÑ »óÅÂ°¡ ¾Æ´Ï¶ó¸é
	if (FAILED(hr))
	{
		if (D3DERR_DEVICELOST == hr)		//º¹±¸°¡ ºÒ°¡´ÉÇÑ »óÅÂ: ´ë±â
		{
			Sleep(100);						//OSÀÇ ºü¸¥ Ã³¸®¸¦ À§ÇØ AppÀ» Á¤Áö
			return;
		}
		else if (D3DERR_DEVICENOTRESET == hr)	//º¹±¸°¡ °¡´ÉÇÑ »óÅÂ
		{
			//º¹±¸
			_tileMap->Release();
			_player->Release();
			_npc->Release();

			InitDirect3D();
			hr = _device3d->Reset(&_d3dpp);

			_tileMap->Reset();
			_player->Reset();
			_npc->Reset();
		}
	}
}

void GameSystem::MapScrollTest(float deltaX, float deltaY)
{
	_tileMap->Scroll(deltaX, deltaY);
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