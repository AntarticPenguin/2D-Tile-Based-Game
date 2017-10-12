#pragma once
#include <vector>
#include <d3dx9.h>

class Frame;

class Sprite
{
	//DirectX
private:
	LPDIRECT3DDEVICE9 _device3d;
	LPD3DXSPRITE _sprite;

	IDirect3DTexture9* _texture;
	D3DXIMAGE_INFO _textureInfo;

private:
	std::vector<Frame*> _frameList;
	int _curFrame;

public:
	Sprite();
	~Sprite();

public:
	void Init(LPDIRECT3DDEVICE9 device3d, LPD3DXSPRITE sprite);
	void Deinit();

	void Render();

	void Reset(LPDIRECT3DDEVICE9 device3d, LPD3DXSPRITE sprite);
	void Release();
};