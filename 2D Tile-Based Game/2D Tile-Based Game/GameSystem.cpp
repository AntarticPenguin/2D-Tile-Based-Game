#include <stdio.h>
#include <string>

#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Map.h"
#include "Character.h"

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
		
		//Scroll test
		if (VK_LEFT == wParam)
		{
			GameSystem::GetInstance().MapScrollTest(-10.0f, 0.0f);
		}
		if (VK_RIGHT == wParam)
		{
			GameSystem::GetInstance().MapScrollTest(10.0f, 0.0f);
		}
		if (VK_UP == wParam)
		{
			GameSystem::GetInstance().MapScrollTest(0.0f, -10.0f);
		}
		if (VK_DOWN == wParam)
		{
			GameSystem::GetInstance().MapScrollTest(0.0f, 10.0f);
		}
		return 0;
	case WM_KEYUP:
		GameSystem::GetInstance().MapScrollTest(0.0f, 0.0f);
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
	_character = NULL;
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

	if (NULL != _tileMap)
	{
		_tileMap->Deinit();
		delete _tileMap;
		_tileMap = NULL;
	}

	if (NULL != _character)
	{
		_character->Deinit();
		delete _character;
		_character = NULL;
	}

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

	
	_tileMap = new Map(L"tileMap");
	_tileMap->Init();

	_character = new Character(L"testCharacter");
	_character->Init();

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

			_tileMap->Update(deltaTime);
			_character->Update(deltaTime);

			float secPerFrame = 1.0f / 60.0f;
			if (secPerFrame <= _frameDuration)
			{
				//wchar_t timeCheck[256];
				//swprintf(timeCheck, L"FrameDuration: %f\n", _frameDuration);
				//OutputDebugString(timeCheck);

				_frameDuration = 0.0f;

				//게임 업데이트

				//DX11
				/*
				float color[4];
				color[0] = 0.3f;	//RED
				color[1] = 0.3f;	//GREEN
				color[2] = 0.3f;	//BLUE
				color[3] = 1.0f;	//ALPHA

				_d3dDeviceContext->ClearRenderTargetView(_renderTargetView, color);			//후면버퍼(사용자 눈에 안보임)
				_d3dDeviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH || D3D11_CLEAR_STENCIL, 1.0f, 0);

				//게임관련 Draw
				_swapChain->Present(0, 0);		//PRESENT(전면으로 교체)
				*/

				//DX9
				_device3d->Clear(0,
					NULL,
					D3DCLEAR_TARGET,				//클리어 타겟(후면버퍼)
					D3DCOLOR_XRGB(0, 0, 0),	//색상
					0.0f,							//깊이
					0
				);

				//Begin과 End 사이에 출력작업
				_device3d->BeginScene();

				_sprite->Begin(D3DXSPRITE_ALPHABLEND);

				{
					_tileMap->Render();
					//_character->Render();
				}

				_sprite->End();

				_device3d->EndScene();

				//출력 직전에 Device Lost 체크
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
		0,								//비디오카드
		D3D_DRIVER_TYPE_HARDWARE,		//드라이버 타입
		0,								//소프트웨어 구동자
		D3D11_CREATE_DEVICE_DEBUG,		//Flag
		0,								//지원되는 버전 설정
		0,								//설정한 버전 개수
		D3D11_SDK_VERSION,				//SDK버전
		&_d3dDevice,					//Device 포인터
		&featureLevel,					//featureLevel
		&_d3dDeviceContext				//DeviceContext 포인터
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11Device 생성 실패", L"ERROR", MB_OK);
		return false;
	}

	//4xMSAA 지원여부 체크
	hr = _d3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_4xMsaaQuality);
	if (FAILED(hr))
	{
		MessageBox(0, L"DX11이 지원되지 않은 하드웨어", L"ERROR", MB_OK);
		return false;
	}

	DXGI_SWAP_CHAIN_DESC sd;

	//후면버퍼 세팅
	sd.BufferDesc.Width = _clientWidth;
	sd.BufferDesc.Height = _clientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;								//디스플레이이 갱신률
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//화면 Scanline 모드
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					//디스플레이 비율(16:9 etc..)

	//안티 앨리어싱 품질 세팅
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

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//버퍼의 용도를 서술하는 구조체
	sd.BufferCount = 1;									//교환사슬에서 이용할 후면버퍼의 개수(더블버퍼링은 1개)
	sd.OutputWindow = _hMainWnd;						//렌더링 결과를 표시할 창의 핸들
	sd.Windowed = true;									//윈도우모드 : 전체화면
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			//교환효과 지정(후면->전면 교체될때의 효과)
	sd.Flags = 0;										//추가 플래그

	//IDXGISwapChain(SwapChain 인스턴스) 인터페이스 생성
	IDXGISwapChain* dxgiDevice = NULL;
	hr = _d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);	//uuidof: 일종의 캐스팅
	if (FAILED(hr))
	{
		MessageBox(0, L"dxgiDevice를 얻지 못하였습니다.", L"ERROR", MB_OK);
		return false;
	}
	
	IDXGIAdapter* dxgiAdapter = NULL;
	hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	if (FAILED(hr))
	{
		MessageBox(0, L"dxgiAdapter를 얻지 못하였습니다.", L"ERROR", MB_OK);
		return false;
	}

	IDXGIFactory* dxgiFactory = NULL;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	if (FAILED(hr))
	{
		MessageBox(0, L"dxgiFactory를 얻지 못하였습니다.", L"ERROR", MB_OK);
		return false;
	}

	hr = dxgiFactory->CreateSwapChain(_d3dDevice, &sd, &_swapChain);
	if (FAILED(hr))
	{
		MessageBox(0, L"SwapChain 인터페이스 생성 실패", L"ERROR", MB_OK);
		return false;
	}

	//이전 3개(Device, Adapter, Factory) 인터페이스 해제
	dxgiDevice->Release();
	dxgiDevice = NULL;

	dxgiAdapter->Release();
	dxgiAdapter = NULL;

	dxgiFactory->Release();
	dxgiFactory = NULL;

	//SwapChain의 후면버퍼에 대한 RenderTargetView 생성
	// 1.후면버퍼의 인터페이스 획득
	ID3D11Texture2D* backBuffer;
	hr = _swapChain->GetBuffer(
		0,										//얻고자 하는 backBuffer의 인덱스
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer)
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"backBuffer 획득 실패", L"ERROR", MB_OK);
		return false;
	}

	//2.backBuffer를 사용할 뷰 생성
	hr = _d3dDevice->CreateRenderTargetView(
		backBuffer,
		NULL,
		&_renderTargetView		//backBuffer를 사용하는 RenderTargetView
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"RenderTargetView 생성 실패", L"ERROR", MB_OK);
		return false;
	}

	backBuffer->Release();
	backBuffer = NULL;

	//*****이 밑으로는 3D를 위한 작업 *****
	//깊이, 스텐실 버퍼와 그에 연결되는 뷰 생성(그림자 등 특수효과)

	//1.특성 세팅
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = _clientWidth;
	depthStencilDesc.Height = _clientHeight;							
	depthStencilDesc.MipLevels = 1;		//밉맵의 수준(보간 정도: (ex)줌인 아웃)
	depthStencilDesc.ArraySize = 1;		//배열에 들어갈 텍스쳐 갯수
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

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;				//어떻게 쓰일지 설정(default: GPU를 사용)
	depthStencilDesc.BindFlags =  D3D11_BIND_DEPTH_STENCIL;		//파이프라인에 어떻게 묶일지 설정
	depthStencilDesc.CPUAccessFlags = 0;						//CPU가 자원에 접근하는 방법
	depthStencilDesc.MiscFlags = 0;								//기타 플래그

	hr = _d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &_depthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"depthStencilBuffer: Texture2D 생성 실패", L"ERROR", MB_OK);
		return false;
	}

	//2.깊이, 스텐실 뷰 생성
	hr = _d3dDevice->CreateDepthStencilView(_depthStencilBuffer, 0, &_depthStencilView);
	if (FAILED(hr))
	{
		MessageBox(0, L"깊이 스텐실 뷰 생성 실패", L"ERROR", MB_OK);
		return false;
	}

	//파이프라인에 뷰를 바인딩
	_d3dDeviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);	//깊이 스텐실 뷰를 제외한 바인딩할 뷰 갯수

	//뷰포트 설정
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
			_tileMap->Release();
			//_character->Release();

			InitDirect3D();
			hr = _device3d->Reset(&_d3dpp);

			_tileMap->Reset();
			//_character->Reset();
		}
	}
}

void GameSystem::MapScrollTest(float deltaX, float deltaY)
{
	_tileMap->Scroll(deltaX, deltaY);
}