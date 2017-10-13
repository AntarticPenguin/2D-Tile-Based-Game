#include "GameSystem.h"
#include "Texture.h"
#include "Frame.h"

Frame::Frame() :
	_frameDelay(0.0f)
{

}

Frame::~Frame()
{

}

void Frame::Init(Texture* texture, int x, int y, int width, int height, float frameDelay)
{
	_sprite = GameSystem::GetInstance().GetSprite();
	_texture = texture;
	
	_width = width;
	_height = height;

	_srcTextureRect.left = x;
	_srcTextureRect.top = y;
	_srcTextureRect.right = _width + x;
	_srcTextureRect.bottom = _height + y;

	_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	_frameDelay = frameDelay;
}

void Frame::Deinit()
{

}

void Frame::Render()
{

	//Sprite 출력 전 모양(위치, 크기, 회전) 조정
	D3DXVECTOR2  spriteCenter = D3DXVECTOR2((float)_width / 2.0f, (float)_height / 2.0f);	//행렬계산용 Sprite 중심좌표
	D3DXVECTOR2 translate = D3DXVECTOR2((float)GameSystem::GetInstance().GetClientWidth() / 2.0f - (float)_width / 2.0f,
	(float)GameSystem::GetInstance().GetClientHeight() / 2.0f - (float)_height / 2.0f);		//위치
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
	_sprite->Draw(_texture->GetTextureDX(), &_srcTextureRect, NULL, NULL, _textureColor);
	
}

void Frame::Reset()
{

}

void Frame::Release()
{

}

float Frame::GetFrameDelay()
{
	return _frameDelay;
}