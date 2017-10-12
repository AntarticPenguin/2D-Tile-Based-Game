#include "GameSystem.h"
#include "Sprite.h"
#include "Frame.h"


Sprite::Sprite()
	:_curFrame(0)
{

}

Sprite::~Sprite()
{

}

void Sprite::Init(LPDIRECT3DDEVICE9 device3d, LPD3DXSPRITE sprite)
{
	//Texture
	//파일로 이미지 너비와 높이를 가져온다.
	HRESULT hr = D3DXGetImageInfoFromFile(L"character_sprite.png", &_textureInfo);
	if (FAILED(hr))
	{
		MessageBox(0, L"TextureInfo 획득 실패", L"ERROR", MB_OK);
		return;
	}

	//텍스쳐 생성
	hr = D3DXCreateTextureFromFileEx(
		device3d,
		L"character_sprite.png",
		_textureInfo.Width,
		_textureInfo.Height,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_ARGB(255, 255, 255, 255),		//컬러키
		&_textureInfo,
		NULL,
		&_texture
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"Texture 생성 실패", L"ERROR", MB_OK);
		return;
	}

	{
		Frame* frame = new Frame();
		frame->Init(device3d, sprite, _texture, 32 * 0, 0, 32, 32);
		_frameList.push_back(frame);
	}
	{
		Frame* frame = new Frame();
		frame->Init(device3d, sprite, _texture, 32 * 1, 0, 32, 32);
		_frameList.push_back(frame);
	}
	{
		Frame* frame = new Frame();
		frame->Init(device3d, sprite, _texture, 32 * 2, 0, 32, 32);
		_frameList.push_back(frame);
	}

	_curFrame = 0;
}

void Sprite::Deinit()
{
	for (std::vector<Frame*>::iterator iter = _frameList.begin(); iter != _frameList.end(); iter++)
	{
		Frame* frame = (*iter);
		frame->Deinit();
		delete frame;
	}
	_frameList.clear();

	RELEASE_COM(_texture);
}

void Sprite::Render()
{
	if (_curFrame < _frameList.size())
		_frameList[_curFrame]->Render();
}

void Sprite::Reset(LPDIRECT3DDEVICE9 device3d, LPD3DXSPRITE sprite)
{
	Init(device3d, sprite);
	for (std::vector<Frame*>::iterator iter = _frameList.begin(); iter != _frameList.end(); iter++)
	{
		Frame* frame = (*iter);
		frame->Reset(device3d, sprite);
	}
}

void Sprite::Release()
{
	for (std::vector<Frame*>::iterator iter = _frameList.begin(); iter != _frameList.end(); iter++)
	{
		Frame* frame = (*iter);
		frame->Release();
	}

	RELEASE_COM(_texture);
}