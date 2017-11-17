#pragma once
#include <vector>
#include <d3dx9.h>

class Frame;
class Texture;

class Sprite
{
	//DirectX
private:
	LPDIRECT3DDEVICE9 _device3d;
	LPD3DXSPRITE _sprite;

	Texture* _srcTexture;
	LPCWSTR _textureFileName;
	LPCWSTR _scriptFileName;

private:
	std::vector<Frame*> _frameList;
	int _curFrame;
	float _frameTime;
	float _x;
	float _y;

	float _rotate;

public:
	Sprite(LPCWSTR textureFileName, LPCWSTR scriptFileName, float rotate = 0.0f);
	~Sprite();

public:
	void Init();
	void Init(int srcX, int srcY, int width, int height, float delay);
	void Deinit();

	void Update(float deltaTime);
	void Render();

	void Reset();
	void Release();

	void SetPosition(float x, float y);
};