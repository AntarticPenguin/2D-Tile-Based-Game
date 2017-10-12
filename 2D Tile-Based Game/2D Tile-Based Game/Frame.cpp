#include "GameSystem.h"
#include "Frame.h"

Frame::Frame()
{

}

Frame::~Frame()
{

}

void Frame::Init(LPDIRECT3DDEVICE9 device3d, LPD3DXSPRITE sprite, IDirect3DTexture9* texture, int x, int y, int width, int height)
{
	_sprite = sprite;
	_texture = texture;
	
	_width = width;
	_height = height;

	_srcTextureRect.left = x;
	_srcTextureRect.top = y;
	_srcTextureRect.right = _width;
	_srcTextureRect.bottom = _height;

	_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);
}

void Frame::Deinit()
{

}

void Frame::Render()
{

	//Sprite ��� �� ���(��ġ, ũ��, ȸ��) ����
	D3DXVECTOR2  spriteCenter = D3DXVECTOR2((float)_width / 2.0f, (float)_height / 2.0f);	//��İ��� Sprite �߽���ǥ
	D3DXVECTOR2 translate = D3DXVECTOR2((float)GameSystem::GetInstance().GetClientWidth() / 2.0f - (float)_width / 2.0f,
	(float)GameSystem::GetInstance().GetClientHeight() / 2.0f - (float)_height / 2.0f);		//��ġ
	D3DXVECTOR2 scaling = D3DXVECTOR2(1.0f, 1.0f);	//ũ��

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

void Frame::Reset(LPDIRECT3DDEVICE9 device3d, LPD3DXSPRITE sprite)
{

}

void Frame::Release()
{

}
