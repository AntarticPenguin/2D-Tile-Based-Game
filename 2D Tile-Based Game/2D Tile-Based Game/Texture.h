#pragma once
#include <d3dx9.h>
#include <string>

class Texture
{
private:
	IDirect3DTexture9* _texture;
	D3DXIMAGE_INFO _textureInfo;

public:
	Texture();
	~Texture();

public:
	void Init(std::wstring fileName);
	void Deinit();
	void Release();

	IDirect3DTexture9* GetTextureDX();
};