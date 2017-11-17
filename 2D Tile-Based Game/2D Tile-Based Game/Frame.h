#pragma once
#include <d3dx9.h>

class Texture;

class Frame
{
private:
	LPD3DXSPRITE _sprite;
	
	Texture* _texture;
	RECT _srcTextureRect;
	D3DCOLOR _textureColor;

	int _width;
	int _height;
	float _rotate;

	float _frameDelay;

	float _x;
	float _y;

public:
	Frame();
	~Frame();

public:
	void Init(Texture* texture, int x, int y, int width, int height, float rotate, float frameDelay);
	void Deinit();
	void Render();
	void Reset();
	void Release();

	float GetFrameDelay();
	void SetPosition(float x, float y);
};