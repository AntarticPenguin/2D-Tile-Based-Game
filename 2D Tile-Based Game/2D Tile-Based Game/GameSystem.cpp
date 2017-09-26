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


GameSystem::GameSystem()
{
	_isEnable4xMSAA = false;
}

GameSystem::~GameSystem()
{
	_depthStencilView->Release();
	_depthStencilView = NULL;

	_depthStencilBuffer->Release();
	_depthStencilBuffer = NULL;

	_renderTargetView->Release();
	_renderTargetView = NULL;

	_swapChain->Release();
	_swapChain = NULL;

	_d3dDeviceContext->Release();
	_d3dDeviceContext = NULL;

	_d3dDevice->Release();
	_d3dDevice = NULL;
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

	_hMainWnd = CreateWindow(
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

	if (NULL == _hMainWnd)
	{
		MessageBox(0, L"FAIL TO CREATE WINDOW", L"ERROR", MB_OK);
		return false;
	}

	ShowWindow(_hMainWnd, nCmdShow);
	UpdateWindow(_hMainWnd);

	if (false == InitDirect3D())
		return false;

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
			//게임 업데이트
			float color[4];
			color[0] = 1.0f;	//RED
			color[1] = 1.0f;	//GREEN
			color[2] = 0.0f;	//BLUE
			color[3] = 1.0f;	//ALPHA

			_d3dDeviceContext->ClearRenderTargetView(_renderTargetView, color);			//후면버퍼(사용자 눈에 안보임)
			_d3dDeviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH || D3D11_CLEAR_STENCIL, 1.0f, 0);

			//게임관련 Draw

			_swapChain->Present(0, 0);		//PRESENT(전면으로 교체)
		}
	}

	return (int)msg.wParam;
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
	depthStencilDesc.Width = 1280;
	depthStencilDesc.Height = 800;							
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
	_screenViewport.Width = 1280.0f;
	_screenViewport.Height = 800.0f;
	_screenViewport.MinDepth = 0.0f;
	_screenViewport.MaxDepth = 1.0f;

	_d3dDeviceContext->RSSetViewports(1, &_screenViewport);

	return true;
}