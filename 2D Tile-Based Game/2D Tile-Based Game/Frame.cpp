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

	//Sprite ��� �� ���(��ġ, ũ��, ȸ��) ����
	D3DXVECTOR2  spriteCenter = D3DXVECTOR2((float)_width / 2.0f, (float)_height / 2.0f);	//��İ��� Sprite �߽���ǥ
	D3DXVECTOR2 translate = D3DXVECTOR2(_x - (float)_width / 2.0f, _y - (float)_height / 2.0f);		//��ġ
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

void Frame::SetPosition(float x, float y)
{
	_x = x;
	_y = y;
}