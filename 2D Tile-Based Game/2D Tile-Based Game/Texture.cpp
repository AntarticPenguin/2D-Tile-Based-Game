#include "GameSystem.h"
#include "Texture.h"

Texture::Texture()
{

}

Texture::~Texture()
{

}

void Texture::Init(LPCWSTR fileName)
{
	//Texture
	//���Ϸ� �̹��� �ʺ�� ���̸� �����´�.
	HRESULT hr = D3DXGetImageInfoFromFile(fileName, &_textureInfo);
	if (FAILED(hr))
	{
		MessageBox(0, L"TextureInfo ȹ�� ����", L"ERROR", MB_OK);
		return;
	}

	//�ؽ��� ����
	hr = D3DXCreateTextureFromFileEx(
		GameSystem::GetInstance().GetDevice(),
		fileName,
		_textureInfo.Width,
		_textureInfo.Height,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_ARGB(255, 255, 255, 255),		//�÷�Ű
		&_textureInfo,
		NULL,
		&_texture
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"Texture ���� ����", L"ERROR", MB_OK);
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