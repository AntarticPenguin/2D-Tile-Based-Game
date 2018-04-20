#include "GameSystem.h"
#include "Texture.h"

Texture::Texture()
{
	
}

Texture::~Texture()
{
	
}

void Texture::Init(std::wstring fileName)
{
	//Texture
	//파일로 이미지 너비와 높이를 가져온다.
	HRESULT hr = D3DXGetImageInfoFromFile(fileName.c_str(), &_textureInfo);
	if (FAILED(hr))
	{
		MessageBox(0, L"TextureInfo 획득 실패", L"ERROR", MB_OK);
		return;
	}

	//텍스쳐 생성
	hr = D3DXCreateTextureFromFileEx(
		GameSystem::GetInstance().GetDevice(),
		fileName.c_str(),
		_textureInfo.Width,
		_textureInfo.Height,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_ARGB(255, 10, 10, 10),		//컬러키
		&_textureInfo,
		NULL,
		&_texture
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"Texture 생성 실패", L"ERROR", MB_OK);
		return;
	}
	
}

void Texture::Deinit()
{
	RELEASE_COM(_texture);
}

void Texture::Release()
{
	RELEASE_COM(_texture);
}

IDirect3DTexture9* Texture::GetTextureDX()
{
	return _texture;
}