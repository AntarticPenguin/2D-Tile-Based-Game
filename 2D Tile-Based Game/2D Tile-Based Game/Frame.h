#pragma once
#include <d3dx9.h>

class Frame
{
private:
	LPD3DXSPRITE _sprite;
	
	IDirect3DTexture9* _texture;
	RECT _srcTextureRect;
	D3DCOLOR _textureColor;

	int _width;
	int _height;

public:
	Frame();
	~Frame();

public:
	void Init(LPDIRECT3DDEVICE9 device3d, LPD3DXSPRITE sprite, IDirect3DTexture9* texture, int x, int y, int width, int height);
	void Deinit();
	void Render();
	void Reset(LPDIRECT3DDEVICE9 device3d, LPD3DXSPRITE sprite);
	void Release();
};