#pragma once
#include <d3dx9.h>

class Sprite
{
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