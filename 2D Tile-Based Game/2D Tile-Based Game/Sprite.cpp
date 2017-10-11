#include "GameSystem.h"
#include "Sprite.h"

Sprite::Sprite()
{

}

Sprite::~Sprite()
{

}

void Sprite::Init(LPDIRECT3DDEVICE9 device3d, LPD3DXSPRITE sprite)
{
	//Texture
	{
		//파일로 이미지 너비와 높이를 가져온다.
		HRESULT hr = D3DXGetImageInfoFromFile(L"character_sprite.png", &_textureInfo);
		if (FAILED(hr))
		{
			MessageBox(0, L"TextureInfo 획득 실패", L"ERROR", MB_OK);
			return false;
		}

	//텍스쳐 생성
	hr = D3DXCreateTextureFromFileEx(
		_device3d,
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
		return false;
	}

	_srcTextureRect.left = 0;
	_srcTextureRect.top = 0;
	_srcTextureRect.right = _textureInfo.Width;
	_srcTextureRect.bottom = _textureInfo.Height;

	_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	}
}

void Sprite::Deinit()
{
	RELEASE_COM(_texture);
}

void Sprite::Render()
{
	{
		//Sprite 출력 전 모양(위치, 크기, 회전) 조정
		D3DXVECTOR2  spriteCenter = D3DXVECTOR2((float)_textureInfo.Width / 2.0f, (float)_textureInfo.Height / 2.0f);	//행렬계산용 Sprite 중심좌표
		D3DXVECTOR2 translate = D3DXVECTOR2((float)_clientWidth / 2.0f - (float)_textureInfo.Width / 2.0f,
			(float)_clientHeight / 2.0f - (float)_textureInfo.Height / 2.0f);		//위치
		D3DXVECTOR2 scaling = D3DXVECTOR2(1.0f, 1.0f);	//크기

		D3DXMATRIX matrix;
		D3DXMatrixTransformation2D(
			&matrix,
			NULL,
			0.0f,
			&scaling,
			&spriteCenter,
			0.0f,
			&translate
		);

		_sprite->SetTransform(&matrix);
		_sprite->Draw(_texture, &_srcTextureRect, NULL, NULL, _textureColor);
	}
}

void Sprite::Reset(LPDIRECT3DDEVICE9 device3d, LPD3DXSPRITE sprite)
{
	Init(device3d, sprite);
}

void Sprite::Release()
{
	RELEASE_COM(_texture);
}