#include <stdio.h>
#include "GameSystem.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (VK_ESCAPE == wParam)
			DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello World!!!!!!!!!!!", L"Hello World", MB_OK);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

GameSystem* GameSystem::_instance = NULL;

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

	HWND hWnd = CreateWindow(
		L"Base",
		L"타이틀",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,			//x
		CW_USEDEFAULT,			//y
		CW_USEDEFAULT,			//Width
		CW_USEDEFAULT,			//Height
		0,
		0,
		hInstance,
		0
	);

	if (NULL == hWnd)
	{
		MessageBox(0, L"FAIL TO CREATE WINDOW", L"ERROR", MB_OK);
		return false;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (false == InitDirect3D())
		return false;

	return true;
}

bool GameSystem::InitDirect3D()
{
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
	sd.BufferDesc.Width = 1280;
	sd.BufferDesc.Height = 800;
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
			//게임 처리

		}
	}

	return (int)msg.wParam;
}

GameSystem::GameSystem()
{
	_isEnable4xMSAA = false;
}
GameSystem::~GameSystem() { }