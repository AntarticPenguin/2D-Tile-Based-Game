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
		L"Ÿ��Ʋ",
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
		//�޼��� ������ ������ ����
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			//�����찡 ó��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//���� ������Ʈ
			float color[4];
			color[0] = 1.0f;	//RED
			color[1] = 1.0f;	//GREEN
			color[2] = 0.0f;	//BLUE
			color[3] = 1.0f;	//ALPHA

			_d3dDeviceContext->ClearRenderTargetView(_renderTargetView, color);			//�ĸ����(����� ���� �Ⱥ���)
			_d3dDeviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH || D3D11_CLEAR_STENCIL, 1.0f, 0);

			//���Ӱ��� Draw

			_swapChain->Present(0, 0);		//PRESENT(�������� ��ü)
		}
	}

	return (int)msg.wParam;
}

bool GameSystem::InitDirect3D()
{
	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr = D3D11CreateDevice(
		0,								//����ī��
		D3D_DRIVER_TYPE_HARDWARE,		//����̹� Ÿ��
		0,								//����Ʈ���� ������
		D3D11_CREATE_DEVICE_DEBUG,		//Flag
		0,								//�����Ǵ� ���� ����
		0,								//������ ���� ����
		D3D11_SDK_VERSION,				//SDK����
		&_d3dDevice,					//Device ������
		&featureLevel,					//featureLevel
		&_d3dDeviceContext				//DeviceContext ������
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11Device ���� ����", L"ERROR", MB_OK);
		return false;
	}

	//4xMSAA �������� üũ
	hr = _d3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_4xMsaaQuality);
	if (FAILED(hr))
	{
		MessageBox(0, L"DX11�� �������� ���� �ϵ����", L"ERROR", MB_OK);
		return false;
	}

	DXGI_SWAP_CHAIN_DESC sd;

	//�ĸ���� ����
	sd.BufferDesc.Width = 1280;
	sd.BufferDesc.Height = 800;
	sd.BufferDesc.RefreshRate.Numerator = 60;								//���÷����� ���ŷ�
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//ȭ�� Scanline ���
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					//���÷��� ����(16:9 etc..)

	//��Ƽ �ٸ���� ǰ�� ����
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

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//������ �뵵�� �����ϴ� ����ü
	sd.BufferCount = 1;									//��ȯ�罽���� �̿��� �ĸ������ ����(������۸��� 1��)
	sd.OutputWindow = _hMainWnd;						//������ ����� ǥ���� â�� �ڵ�
	sd.Windowed = true;									//�������� : ��üȭ��
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			//��ȯȿ�� ����(�ĸ�->���� ��ü�ɶ��� ȿ��)
	sd.Flags = 0;										//�߰� �÷���

	//IDXGISwapChain(SwapChain �ν��Ͻ�) �������̽� ����
	IDXGISwapChain* dxgiDevice = NULL;
	hr = _d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);	//uuidof: ������ ĳ����
	if (FAILED(hr))
	{
		MessageBox(0, L"dxgiDevice�� ���� ���Ͽ����ϴ�.", L"ERROR", MB_OK);
		return false;
	}
	
	IDXGIAdapter* dxgiAdapter = NULL;
	hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	if (FAILED(hr))
	{
		MessageBox(0, L"dxgiAdapter�� ���� ���Ͽ����ϴ�.", L"ERROR", MB_OK);
		return false;
	}

	IDXGIFactory* dxgiFactory = NULL;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	if (FAILED(hr))
	{
		MessageBox(0, L"dxgiFactory�� ���� ���Ͽ����ϴ�.", L"ERROR", MB_OK);
		return false;
	}

	hr = dxgiFactory->CreateSwapChain(_d3dDevice, &sd, &_swapChain);
	if (FAILED(hr))
	{
		MessageBox(0, L"SwapChain �������̽� ���� ����", L"ERROR", MB_OK);
		return false;
	}

	//���� 3��(Device, Adapter, Factory) �������̽� ����
	dxgiDevice->Release();
	dxgiDevice = NULL;

	dxgiAdapter->Release();
	dxgiAdapter = NULL;

	dxgiFactory->Release();
	dxgiFactory = NULL;

	//SwapChain�� �ĸ���ۿ� ���� RenderTargetView ����
	// 1.�ĸ������ �������̽� ȹ��
	ID3D11Texture2D* backBuffer;
	hr = _swapChain->GetBuffer(
		0,										//����� �ϴ� backBuffer�� �ε���
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer)
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"backBuffer ȹ�� ����", L"ERROR", MB_OK);
		return false;
	}

	//2.backBuffer�� ����� �� ����
	hr = _d3dDevice->CreateRenderTargetView(
		backBuffer,
		NULL,
		&_renderTargetView		//backBuffer�� ����ϴ� RenderTargetView
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"RenderTargetView ���� ����", L"ERROR", MB_OK);
		return false;
	}

	backBuffer->Release();
	backBuffer = NULL;

	//*****�� �����δ� 3D�� ���� �۾� *****
	//����, ���ٽ� ���ۿ� �׿� ����Ǵ� �� ����(�׸��� �� Ư��ȿ��)

	//1.Ư�� ����
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = 1280;
	depthStencilDesc.Height = 800;							
	depthStencilDesc.MipLevels = 1;		//�Ӹ��� ����(���� ����: (ex)���� �ƿ�)
	depthStencilDesc.ArraySize = 1;		//�迭�� �� �ؽ��� ����
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

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;				//��� ������ ����(default: GPU�� ���)
	depthStencilDesc.BindFlags =  D3D11_BIND_DEPTH_STENCIL;		//���������ο� ��� ������ ����
	depthStencilDesc.CPUAccessFlags = 0;						//CPU�� �ڿ��� �����ϴ� ���
	depthStencilDesc.MiscFlags = 0;								//��Ÿ �÷���

	hr = _d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &_depthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"depthStencilBuffer: Texture2D ���� ����", L"ERROR", MB_OK);
		return false;
	}

	//2.����, ���ٽ� �� ����
	hr = _d3dDevice->CreateDepthStencilView(_depthStencilBuffer, 0, &_depthStencilView);
	if (FAILED(hr))
	{
		MessageBox(0, L"���� ���ٽ� �� ���� ����", L"ERROR", MB_OK);
		return false;
	}

	//���������ο� �並 ���ε�
	_d3dDeviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);	//���� ���ٽ� �並 ������ ���ε��� �� ����

	//����Ʈ ����
	_screenViewport.TopLeftX = 0;
	_screenViewport.TopLeftY = 0;
	_screenViewport.Width = 1280.0f;
	_screenViewport.Height = 800.0f;
	_screenViewport.MinDepth = 0.0f;
	_screenViewport.MaxDepth = 1.0f;

	_d3dDeviceContext->RSSetViewports(1, &_screenViewport);

	return true;
}